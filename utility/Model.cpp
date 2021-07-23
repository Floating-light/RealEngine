#include "Model.h"

#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include "Utility.h"
#include "CoreType.h"
void RModel::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw(shader);
	}
}

void RModel::Draw(std::shared_ptr<Material>& mat)
{
	for (size_t i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw(mat);
	}
}

void RModel::Draw()
{
	for (size_t i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw(material);
	}
}

void RModel::loadModel(const std::string& path, EShaderType ShaderType )
{
	MyShaderType = ShaderType;
	Assimp::Importer import;
	/*
	* aiProcess_Triangulate 不是三角形的面先转换成三角面
	* aiProcess_FlipUVs texture 的y方向反向, 从上往下和从下往上的区别
	* aiProcess_GenNormals 没有法线就生成法线
	* aiProcess_SplitLargeMeshes 把大的Mesh 分得更小的, (当单次rendering有顶点数量的限制时)
	* aiProcess_OptimizeMeshes 把小的Mesh合成一个大的, 可以减少Draw call
	*/
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate );

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);


	if (ShaderType != EShaderType::None)
	{
		material = std::make_shared<Material>(ShaderType);
	}

	//for (auto& mesh : meshes)
	//{
	//	if (mesh.textures.empty())
	//	{
	//		for (size_t i = 0; i < textures_loaded.size(); ++i)
	//		{
	//			if (textures_loaded[i].type == "texture_diffuse")
	//			{
	//				mesh.textures.push_back(textures_loaded[i]);
	//				break;
	//			}
	//		}
	//	}
	//}
}

void RModel::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// then do the same for each of its children
	for (size_t i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}
}

RMesh RModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<BVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<BTexture> textures;

	// position, normal, texture coordinate
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		BVertex vertex;
		vertex.Position = Vector3D(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal= Vector3D(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		
		// texture coordinate maybe multiple, but only get the first one for general texture coords.
		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoords = Vector2D(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			if (mesh->mTextureCoords[1])
			{
				Log(__FUNCTION__, "Warning", "There have multiple texture coordinates array");
			}
		}
		else
		{
			Log(__FUNCTION__, "Warning", "Can not find a Texture coords array");
			vertex.TexCoords = Vector2D(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	// vertex indices buffer
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// proccess material, material all in a array of scene.
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		
		std::vector<BTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<BTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//scene->mMaterials

	}
	return RMesh(vertices, indices, textures, MyShaderType);
}

std::vector<BTexture> RModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<BTexture> textures;
	// Load all texture of type in this aiMaterial 
	for (size_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;

		for(size_t j = 0; j < textures_loaded.size(); ++j)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			BTexture tex;
			tex.id = Utility::TextureFromFile(str.C_Str(), directory);
			tex.type = typeName;
			tex.path = str.C_Str();

			textures.push_back(tex);
			textures_loaded.push_back(tex);
		}
	}
	return textures;
}

