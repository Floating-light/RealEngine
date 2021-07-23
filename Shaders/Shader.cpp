#include "Shader.h"
#include <glad/glad.h>
#include "glm/gtc/type_ptr.hpp"

static const std::string ShaderFloder = "./Shaders/";
std::array<std::shared_ptr<Shader>, static_cast<unsigned int>(EShaderType::MAX_ShaderNum)> Shader::ShaderPool{};

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	// 1. retrieve the vertex / fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		vShaderFile.open(ShaderFloder+vertexPath);
		fShaderFile.open(ShaderFloder+fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// close file handlers
		vShaderFile.close();
		fShaderFile.close();

		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders 
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);

	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);

	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shader programe
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	// glUniform
	// f --> float
	// i --> integer
	// ui --> unsigned int
	// 3f --> 3 个 floats
	// fv --> float vector/array
	// c 库, 不支持函数重载.
}

void Shader::setInteger(const std::string& name, int value) const 
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const 
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

//void Shader::setVec3(const std::string& name, const glm::vec3& value)
//{
//	glUniform3f(glGetUniformLocation(ID, name.c_str()),value.x, value.y, value.z);
//}

void Shader::setVec3(const std::string& name, const Vector3D& value)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
	// 在当前激活的shader program 中设置这个nuniform 变量的值
	// get uniform location does not require use.
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat4(const std::string& name, const glm::mat4& ptr)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.data()),1, GL_FALSE, glm::value_ptr(ptr));
}

std::shared_ptr<Shader> Shader::GetShader(EShaderType Type)
{
	const unsigned int Index = static_cast<unsigned int>(Type);
	assert(Type < EShaderType::MAX_ShaderNum);
	if (!ShaderPool[Index])
	{
#define SHADERDEF(Name, VertShader, FragShader) ShaderPool[static_cast<unsigned int>(EShaderType::Name)] = std::make_shared<Shader>(VertShader, FragShader);
#include "Shaders/Shader.def"
#undef SHADERDEF
	}
	return ShaderPool[Index];
}

