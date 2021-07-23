#include <iostream>
#include <algorithm>

#include <glad/glad.h>

#include "Shaders/Shader.h"
#include "utility/Material.h"

Material::Material(EShaderType ShaderType, const std::vector<BTexture>& InTexParam)
{
	MyShader = Shader::GetShader(ShaderType);
	initTextureParams(InTexParam);

	assert(MyShader);
}

void Material::Use()
{
	if (!MyShader)
	{
		std::cout << "ERROR: Shader is nullptr, " << __FUNCTION__ << std::endl;
		return;
	}

	MyShader->use();

	for (auto& ScaleParam : ScalarParameters)
	{
		MyShader->setFloat(ScaleParam.ParameterName, ScaleParam.ParameterValue);
	}

	for (auto& VecParam : VectorParameters)
	{
		MyShader->setVec3(VecParam.ParameterName, VecParam.ParameterValue);
	}

	for (size_t i = 0; i < TextureParameters.size(); ++i)
	{
		// active texture unit 
		glActiveTexture(GL_TEXTURE0 + i);
		// bind this texture id to the actived texture unit above
		glBindTexture(GL_TEXTURE_2D, TextureParameters[i].ParameterValue.id);

		// set the texture unit id(0 -- 15 ) for texture parameter name in shader 
		MyShader->setInteger(TextureParameters[i].ParameterName, i);

		// that's say : texture id ---> texture unit number --> texture name in shader(bind a integer) 
	}

	for (auto& Mat4Param : Materix4Parameters)
	{
		MyShader->setMat4(Mat4Param.ParameterName, Mat4Param.ParameterValue);
	}
}

void Material::setOrAddScalarParameter(const std::string& paramName, float value)
{
	auto resItr = std::find_if(ScalarParameters.begin(), ScalarParameters.end(),
		[&](const RScalarParameterValue& Param)
		{
			return Param.ParameterName == paramName;
		});
	if (resItr != ScalarParameters.end())
	{
		resItr->ParameterValue = value;
	}
	else
	{
		RScalarParameterValue NewValue = { paramName, value };
		ScalarParameters.emplace_back(std::move(NewValue));
	}
}
void Material::setOrAddVectorParameterValue(const std::string& paramName, Vector3D value)
{
	auto resItr = std::find_if(VectorParameters.begin(), VectorParameters.end(),
		[&](const RVectorParameterValue& Param)
		{
			return Param.ParameterName == paramName;
		});
	if (resItr != VectorParameters.end())
	{
		resItr->ParameterValue = value;
	}
	else
	{
		RVectorParameterValue NewValue = { paramName, value };
		VectorParameters.emplace_back(std::move(NewValue));
	}
}

void Material::setOrAddTextureParameterValue(const std::string& paramName, BTexture value)
{
	auto resItr = std::find_if(TextureParameters.begin(), TextureParameters.end(), 
		[&](const RTextureParameterValue& Param) 
		{
			return Param.ParameterName == paramName;
		});
	if (resItr != TextureParameters.end())
	{
		resItr->ParameterValue = value;
	}
	else
	{
		RTextureParameterValue NewValue = { paramName, value };
		TextureParameters.emplace_back(std::move(NewValue));
	}
}

void Material::setOrAddMatrix4ParameterValue(const std::string& paramName, const Matrix4& value)
{
	auto resItr = std::find_if(Materix4Parameters.begin(), Materix4Parameters.end(),
		[&](const RMatrix4ParameterValue& Param)
		{
			return Param.ParameterName == paramName;
		});
	if (resItr != Materix4Parameters.end())
	{
		resItr->ParameterValue = value;
	}
	else
	{
		RMatrix4ParameterValue NewValue = { paramName, value };
		Materix4Parameters.emplace_back(std::move(NewValue));
	}
}


void Material::initTextureParams(const std::vector<BTexture>& InTexture)
{
	int diffuseNr = 0, specularNr = 0;
	for (unsigned int i = 0; i < InTexture.size(); ++i)
	{
		std::string number;
		std::string name = InTexture[i].type;
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}
		TextureParameters.emplace_back(RTextureParameterValue{ ("material." + name + number), InTexture[i] });
	}
}
