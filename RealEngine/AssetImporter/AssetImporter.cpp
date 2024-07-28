#include "AssetImporter.h"

#include <filesystem>
#include <cassert>
#include <array>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "Core.h"
#include "PrimitiveInfo.h"
#include "ModelData.h"

static void ProcessMesh(const aiScene* scene, const unsigned int Index,std::shared_ptr<RPrimitiveObject> OutPrimit)
{
    //if(Index < scene->mNumMeshes)
    //{
    //    aiMesh* Mesh = scene->mMeshes[Index];
    //    const size_t InitialSize = OutPrimit->mVertexData.size();
    //    OutPrimit->mVertexData.resize(InitialSize + Mesh->mNumVertices);
    //    for(int i = 0; i < Mesh->mNumVertices; ++i)
    //    {
    //        RVertex& Vert = OutPrimit->mVertexData[InitialSize + i];
    //        static_assert(sizeof(Vector3D) == sizeof(aiVector3D));
    //        std::memcpy((void*)&Vert.Vertex,(void*)&Mesh->mVertices[i],sizeof(aiVector3D));
    //        std::memcpy((void*)&Vert.Normal, (void*)&Mesh->mNormals[i], sizeof(aiVector3D));
    //        assert(Mesh->mNumUVComponents[0] == 2 && Mesh->mTextureCoords[0]);
    //        std::memcpy((void*)&Vert.UV, (void*)&Mesh->mTextureCoords[0][i], sizeof(Vector2D));
    //    }
    //    const size_t IndicesBegin = OutPrimit->mIndicesData.size();
    //    OutPrimit->mIndicesData.resize(IndicesBegin + 3 * Mesh->mNumFaces);
    //    for(int i = 0;i < Mesh->mNumFaces; ++i)
    //    {
    //        assert(Mesh->mFaces[i].mNumIndices == 3);
    //        std::memcpy((void*)&OutPrimit->mIndicesData[IndicesBegin+i*3], (void*)Mesh->mFaces[i].mIndices, 3*sizeof(uint32_t));
    //    }
    //    OutPrimit->mName = Mesh->mName.C_Str();

    //    RLOG(Info, "Name : {}, MeshIndex : {}, Num vertices : {}, Num Faces : {}",
    //        Mesh->mName.C_Str(), Index, Mesh->mNumVertices, Mesh->mNumFaces);
    //}
}

static void ProcessNode(const aiScene* scene, const aiNode* Node,std::shared_ptr<RPrimitiveObject> OutPrimit)
{
    if(!Node)
        return ;
    RLOG(Info, "mChildren : {}, Node name: {}, Mesh num: {}", 
        Node->mNumChildren, Node->mName.C_Str(), Node->mNumMeshes);
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
            RLOG(Info, "Key : {}", Node->mMetaData->mKeys[i].C_Str());
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
        RLOG(Error, "Load file failed : {} ", nFilePath );
    }
    else
    {
        ProcessNode(scene, scene->mRootNode, Primitive);
        RLOG(Debug, "Total mesh number {}", scene->mNumMeshes);
    }
    return Primitive;
}
void PrintaiTransform(const aiMatrix4x4& Trans)
{
    aiVector3D scale;
    aiVector3D rotation;
    aiVector3D position;
    Trans.Decompose(scale, rotation, position);
    RLOG(Info, "scale: [{}, {}, {}]", scale.x, scale.y, scale.z);
    RLOG(Info, "rotation: [{}, {}, {}]", rotation.x, rotation.y, rotation.z);
    RLOG(Info, "position: [{}, {}, {}]", position.x, position.y, position.z); 
}
void ProcessaiNode(aiNode* InNode)
{
    RLOG(Info, "NODE: {}, MeshNum: {}", InNode->mName.C_Str(), InNode->mNumMeshes);
    PrintaiTransform(InNode->mTransformation);
    for (size_t i = 0; i < InNode->mNumMeshes; ++i)
    {
        RLOG(Info, "{}. -> Mesh: {}",i, InNode->mMeshes[i]);
    }
    for (size_t i = 0; i < InNode->mNumChildren; i++)
    {
        ProcessaiNode(InNode->mChildren[i]);
    }
}
std::unordered_map<aiPropertyTypeInfo, std::string> matPropMap = 
{   {aiPTI_Float, "float"},
    {aiPTI_Double, "double"}, 
    {aiPTI_String, "string"},
    {aiPTI_Integer, "Integer"},
    {aiPTI_Buffer, "Buffer"} };
std::string GetMaterialIdentityName(aiMaterial* InMat)
{
    RCHECK(InMat);
    aiString Path;
    InMat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), Path); 
    return Path.C_Str();
}
std::vector<RMaterial> ProcessMaterial(const aiScene* InScene, std::filesystem::path InParentPath)
{
    std::vector<RMaterial> Mats{};
    for (int32_t i = 0; i < InScene->mNumMaterials; ++i)
    {
        aiMaterial* Mat = InScene->mMaterials[i]; 
        for (size_t j = 0; j < Mat->mNumProperties; ++j)
        {
            aiMaterialProperty* Prop = Mat->mProperties[j];
            RLOG(Info, "{}. Mat: {}, Prop: {}, Type: {}", 
                i,Mat->GetName().C_Str(), Prop->mKey.C_Str(), matPropMap[Prop->mType]); 
        }
        Mats.emplace_back((InParentPath / GetMaterialIdentityName(Mat)).string());
    }
    return std::move(Mats);
}
std::shared_ptr<RModelData> RAssetImporter::ImportModelNew(const std::string& InFile)
{
    const std::filesystem::path FilePath = std::filesystem::path(InFile);
    Assimp::Importer importer; 
    //const std::string nFilePath = std::filesystem::path(InFile).string(); 

    const aiScene* scene = importer.ReadFile(FilePath.string(),
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices |
        aiProcess_Triangulate |
        aiProcess_RemoveComponent |
        aiProcess_GenSmoothNormals |
        aiProcess_SplitLargeMeshes |
        aiProcess_ValidateDataStructure |
        aiProcess_ImproveCacheLocality | 
        aiProcess_RemoveRedundantMaterials |
        aiProcess_SortByPType |
        aiProcess_FindInvalidData |
        aiProcess_GenUVCoords |
        aiProcess_TransformUVCoords |
        aiProcess_OptimizeMeshes |
        aiProcess_OptimizeGraph); 

    if (scene == nullptr) 
    {
        RLOG(Error, "Load file failed : {} ", FilePath.string()); 
        return nullptr;
    }

    std::shared_ptr<RModelData> RetData(new RModelData());  
    ProcessaiNode(scene->mRootNode);
    
    RetData->SetMaterials(ProcessMaterial(scene,  FilePath.parent_path()));

    std::vector<uint8_t>& LocalGeometryData =RetData->GetGeometryData(); 
    std::vector<RMeshData>& LocalMeshesData = RetData->GetMeshesData();
    const uint32_t strides = sizeof(float) * (3 + 3 + 2);
    
    static_assert(std::is_same<ai_real, float>::value);
    Vector MaxVet((std::numeric_limits<float>::lowest)());
    Vector Minvet((std::numeric_limits<float>::max)()); 

    for (size_t i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh* Mesh = scene->mMeshes[i];  
        RMeshData& MeshData = LocalMeshesData.emplace_back();  
        MeshData.vbOffset = LocalGeometryData.size(); 
        MeshData.vbStride = strides; 

        {
            assert(Mesh->HasPositions());
            assert(Mesh->HasNormals());
            assert(Mesh->HasTextureCoords(0));

            assert(Mesh->HasFaces()); 

            //assert(Mesh->HasVertexColors(0));// No vertex color
            assert(Mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_TRIANGLE);
        }
        std::vector<uint8_t> CurMeshGeo;  
        CurMeshGeo.resize(strides*Mesh->mNumVertices);  
        uint8_t* pPosition = CurMeshGeo.data();  
        uint8_t* pNormal = pPosition + sizeof(float)*3; 
        uint8_t* pUV0 = pNormal + sizeof(float)*3; 

        for (size_t j = 0; j < Mesh->mNumVertices; ++j)
        {
            const aiVector3D& CurVert = Mesh->mVertices[j];
            const Vector MyVersion(CurVert.x, CurVert.y, CurVert.z);
            MaxVet = Vector::ComponentWiseMax(MaxVet, MyVersion);
            Minvet = Vector::ComponentWiseMin(MaxVet, MyVersion);

            memcpy(pPosition, &(Mesh->mVertices[j]), sizeof(aiVector3D));
            pPosition += strides;
            memcpy(pNormal, &(Mesh->mNormals[j]), sizeof(aiVector3D));
            pNormal += strides;
            memcpy(pUV0, &(Mesh->mTextureCoords[0][j]), sizeof(ai_real)*2);
            pUV0 += strides; 
        }
        LocalGeometryData.insert(LocalGeometryData.end(), CurMeshGeo.begin(), CurMeshGeo.end());
        MeshData.vbSize = CurMeshGeo.size();

        std::vector<uint8_t> CurIndexBuffer;
        CurIndexBuffer.resize(Mesh->mNumFaces * 3 * sizeof(uint32_t));
        MeshData.indexCount = Mesh->mNumFaces * 3;
        MeshData.ibOffset = LocalGeometryData.size();
        MeshData.ibSize = Mesh->mNumFaces * 3 * sizeof(uint32_t);
        MeshData.MaterialIndex = Mesh->mMaterialIndex;

        uint8_t* pIndexBuffer = CurIndexBuffer.data(); 
        for (size_t j = 0; j < Mesh->mNumFaces; ++j)
        {
            const aiFace& Face = Mesh->mFaces[j]; 
            assert(Face.mNumIndices == 3);
            memcpy(pIndexBuffer, Face.mIndices, sizeof(uint32_t) * 3);
            pIndexBuffer += sizeof(uint32_t) * 3; 
        }
        LocalGeometryData.insert(LocalGeometryData.end(), CurIndexBuffer.begin(), CurIndexBuffer.end()); 
        MeshData.Name = Mesh->mName.C_Str();
        RLOG(Info, "{}, V: {}, F: {}, T: {}, Mat: {}, Mat name: {}", 
            Mesh->mName.C_Str(), Mesh->mNumVertices, Mesh->mNumFaces, Mesh->mPrimitiveTypes, Mesh->mMaterialIndex, 
            GetMaterialIdentityName(scene->mMaterials[Mesh->mMaterialIndex]));
    }
    if (RetData)
    {
        RetData->PostLoad();
    }
    return RetData;
}
struct LocalVertex
{
    LocalVertex(const Vector3D& InPosition, const Vector3D& InNormal, const Vector2D& InUV)
        : Position(InPosition)
        , Normal(InNormal)
        , UV(InUV)
    {}
    Vector3D Position;
    Vector3D Normal;
    Vector2D UV;
};
std::shared_ptr<RModelData> RAssetImporter::MakeCube()
{
    //std::array<CubeVertex, 8> vertex =
    //{ 
    //    CubeVertex(Vector3D(0.5f, 0.5f,-0.5f)  , Vector3D(1.f,1.f,1.f), Vector2D()),
    //    CubeVertex(Vector3D(0.5f, -0.5f,-0.5f) , Vector3D(1.f,1.f,1.f), Vector2D()),
    //    CubeVertex(Vector3D(-0.5f, -0.5f,-0.5f), Vector3D(1.f,1.f,1.f), Vector2D()),
    //    CubeVertex(Vector3D(-0.5f, 0.5f,-0.5f) , Vector3D(1.f,1.f,1.f), Vector2D()),
    //    CubeVertex(Vector3D(0.5f, 0.5f,0.5f)  ,  Vector3D(1.f,1.f,1.f), Vector2D()),
    //    CubeVertex(Vector3D(0.5f, -0.5f,0.5f) ,  Vector3D(1.f,1.f,1.f), Vector2D()),
    //    CubeVertex(Vector3D(-0.5f, -0.5f,0.5f),  Vector3D(1.f,1.f,1.f), Vector2D()),
    //    CubeVertex(Vector3D(-0.5f, 0.5f,0.5f) ,  Vector3D(1.f,1.f,1.f), Vector2D())
    //};
    return std::shared_ptr<RModelData>();
}

std::shared_ptr<RModelData> RAssetImporter::MakePlane() 
{
    std::array<LocalVertex, 4> Vertex = {
        LocalVertex(Vector3D(1.5f, 1.5f,-1.5f)  , Vector3D(1.f,1.f,1.f), Vector2D(1,0)),
        LocalVertex(Vector3D(-1.5f, 1.5f,-1.5f)  , Vector3D(1.f,1.f,1.f), Vector2D(0,0)),
        LocalVertex(Vector3D(-1.5f, -1.5f,-1.5f)  , Vector3D(1.f,1.f,1.f), Vector2D(0,1)),
        LocalVertex(Vector3D(1.5f, -1.5f,-1.5f)  , Vector3D(1.f,1.f,1.f), Vector2D(1,1)),
    };
    std::array<uint32_t, 6> Index = {3,0,1,1,2,3};

    std::shared_ptr<RModelData> RetVal = std::shared_ptr<RModelData>(new RModelData());
    std::vector<uint8_t>& Data = RetVal->GetGeometryData(); 
    Data.resize(sizeof(LocalVertex) * 4 + sizeof(uint32_t)*6);
    memcpy(Data.data(), Vertex.data(), sizeof(LocalVertex) * 4);
    memcpy(Data.data() + sizeof(LocalVertex) * 4, Index.data(), sizeof(uint32_t) * 6);

    RMeshData meshData{};
    meshData.vbOffset = 0;
    meshData.vbSize = sizeof(LocalVertex) * 4;
    meshData.vbStride = sizeof(LocalVertex);
    meshData.ibOffset = meshData.vbSize;
    meshData.ibSize = sizeof(uint32_t) * 6;
    meshData.indexCount = 6;
    meshData.MaterialIndex = 0;
    meshData.xform = Matrix4::Identity;
    /*meshData.rotation = RQuat::*/
    meshData.scale = Vector3D::OneVector;

    meshData.Name = "TestPlane";
    RetVal->AddMeshData(meshData);
    RetVal->PostLoad();

    return RetVal;
}
