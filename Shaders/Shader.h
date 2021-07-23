#pragma once


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <array>

#include "glm/glm.hpp"
#include "utility/CoreType.h"

enum class EShaderType 
{
	None,
    #define	SHADERDEF(Name, VertShader, FragShader) Name,
    #include "Shaders/Shader.def" 
    #undef SHADERDEF
	MAX_ShaderNum
};

/**
* Shader progrm
*/
class Shader
{
public:
	// the program ID
	unsigned int ID;

	// constructor reads and builds the shader
	Shader(const std::string& vertexPath, const std::string& fragmentPath);

	~Shader();

	// use/activate the shader
	void use();

	// utility uniform functions
	void setBool(const std::string& name, bool value) const;

	void setInteger(const std::string& name, int value) const;

	void setFloat(const std::string& name, float value) const;
	//void setVec3(const std::string& name, const glm::vec3& value);
	void setVec3(const std::string& name, const Vector3D& value);
	void setVec4(const std::string& name, float x, float y, float z, float w);
	void setMat4(const std::string& name, const glm::mat4& ptr);

public:
	static std::shared_ptr<Shader> GetShader(EShaderType Type);
private:
	static std::array<std::shared_ptr<Shader>, static_cast<unsigned int>(EShaderType::MAX_ShaderNum)> ShaderPool;
};

