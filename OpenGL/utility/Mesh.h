#pragma once 
#include "CoreType.h"
#include <string>
#include <vector>
#include <memory>
#include "Material.h"
#include "Shaders/Shader.h"

struct BVertex
{
	Vector3D Position;
	Vector3D Normal;
	Vector2D TexCoords;
};

struct BTexture
{
	unsigned int id;
	std::string type;
	std::string path; // store path of texture to compare with other textures
};

extern std::vector<BTexture> textures_loaded;

class Material;

class RMesh
{
public:
	std::vector<BVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<BTexture> textures;

	// constructor 
	RMesh(std::vector<BVertex> vertices, std::vector<unsigned int> indices, 
		std::vector<BTexture> textures, EShaderType shaderType = EShaderType::Default);

	// Draw this mesh
	/**
	* Draw this mesh, Bind all textures to shader, 
	*/
	void Draw(Shader& shader);

	void Draw(std::shared_ptr<Material>& mat);

	std::shared_ptr<Material> GetMaterial() { return MyMaterial; }
	
private:
	// render data
	unsigned int VAO, VBO, EBO;

	std::shared_ptr<Material> MyMaterial;

	/**
	* Called in Constructor, Setup VAO, VBO, EBO
	*/
	void SetupMesh();
};