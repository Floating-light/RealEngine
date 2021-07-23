#pragma once 
#include <vector>
#include <string>
#include <memory>

#include "../Shaders/Shader.h"
#include "Mesh.h"
#include "Material.h"

#include "assimp/scene.h"

class RModel
{
public:
	RModel(const std::string& path, EShaderType MyShader = EShaderType::Default)
	{
		loadModel(path, MyShader);
	}
	void Draw(Shader& shader);
	void Draw(std::shared_ptr<Material>& mat);
	void Draw();
	// model data 
	std::vector<RMesh> meshes;
private:

	std::string directory;

	void loadModel(const std::string& path, EShaderType ShaderType = EShaderType::Default);

	void processNode(aiNode* node, const aiScene* scene);

	/**
	* Construct a RMesh, and read all data(Vertex(Position, normal, texture cooridinate), indices, texture) from mesh.
	*/
	RMesh processMesh(aiMesh* mesh, const aiScene* scene);

	/**
	* Load all the texture of type in material.
	* @param mat the material 
	* @param type the texture type to load in material.
	* @param typeName custom name of the texture type.
	*/
	std::vector<BTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	std::shared_ptr<Material> material;

	EShaderType MyShaderType;
};