#include "AssetImporter.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <filesystem>
#include <cassert>

#include "Core.h"
#include "PrimitiveInfo.h"

static void ProcessMesh(const aiScene* scene, const unsigned int Index,std::shared_ptr<RPrimitiveObject> OutPrimit)
{
    if(Index < scene->mNumMeshes)
    {
        aiMesh* Mesh = scene->mMeshes[Index];
        const size_t InitialSize = OutPrimit->VertexData.size();
        OutPrimit->VertexData.resize(InitialSize + Mesh->mNumVertices);
        for(int i = 0; i < Mesh->mNumVertices; ++i)
        {
            RVertex& Vert = OutPrimit->VertexData[InitialSize + i];
            static_assert(sizeof(Vector3D) == sizeof(aiVector3D));
            std::memcpy((void*)&Vert.Vertex,(void*)&Mesh->mVertices[i],sizeof(aiVector3D));
            std::memcpy((void*)&Vert.Normal, (void*)&Mesh->mNormals[i], sizeof(aiVector3D));
            assert(Mesh->mNumUVComponents[0] == 2 && Mesh->mTextureCoords[0]);
            std::memcpy((void*)&Vert.UV, (void*)&Mesh->mTextureCoords[0][i], sizeof(Vector2D));
        }
        const size_t IndicesBegin = OutPrimit->Indices.size();
        OutPrimit->Indices.resize(IndicesBegin + 3 * Mesh->mNumFaces);
        for(int i = 0;i < Mesh->mNumFaces; ++i)
        {
            assert(Mesh->mFaces[i].mNumIndices == 3);
            std::memcpy((void*)&OutPrimit->Indices[IndicesBegin+i*3], (void*)Mesh->mFaces[i].mIndices, 3*sizeof(uint32_t));
        }
        OutPrimit->mName = Mesh->mName.C_Str();
        
        RLOG(INFO) << "Name : " << Mesh->mName.C_Str() 
        << ", Mesh Index : " << Index
        << ", Num vertices : " << Mesh->mNumVertices
        << ", Num Faces : " << Mesh->mNumFaces;
    }
}

static void ProcessNode(const aiScene* scene, const aiNode* Node,std::shared_ptr<RPrimitiveObject> OutPrimit)
{
    if(!Node)
        return ;
    RLOG(INFO) << "mChildren : " << Node->mNumChildren
    << " ,Node name : " << Node->mName.C_Str()
    << " ,Mesh Num : " << Node->mNumMeshes;
    uint32_t TotalNumVert = 0, TotalNumInd = 0;

    for(int i = 0 ; i < Node->mNumMeshes; ++i)
    {
        TotalNumVert += scene->mMeshes[Node->mMeshes[i]]->mNumVertices;
        TotalNumInd += scene->mMeshes[Node->mMeshes[i]]->mNumFaces * 3;
        ProcessMesh(scene, Node->mMeshes[i], OutPrimit); 
        // Node->mTransformation;
    }
    if(Node->mMetaData)
    {
        for(int i = 0 ; i< Node->mMetaData->mNumProperties; ++i)
        {
            RLOG(INFO) << "Key : " << Node->mMetaData->mKeys[i].C_Str();
        }
    }
    for(int i = 0; i<Node->mNumChildren;++i)
    {
        ProcessNode(scene, Node->mChildren[i], OutPrimit);
    }
}

std::shared_ptr<RPrimitiveObject> RAssetImporter::ImportModel(const std::wstring& InFile)
{
    Assimp::Importer importer;
    const std::string nFilePath = std::filesystem::path(InFile).string();

    const aiScene* scene = importer.ReadFile(nFilePath, 0);

    std::shared_ptr<RPrimitiveObject> Primitive = std::make_shared<RPrimitiveObject>();
    if(scene == nullptr)
    {
        RLOG(ERROR) << "Load file failed : " << nFilePath ;
    }
    else
    {
        scene->mRootNode->mChildren;
        ProcessNode(scene, scene->mRootNode, Primitive);
        RLOG(INFO) << "Total mesh number " << scene->mMeshes;

    }
    return Primitive;
}
