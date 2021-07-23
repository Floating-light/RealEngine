#version 330 core
struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

in vec2 TexCoord;
in vec3 fragCoord;
in vec3 Normal;
uniform sampler2D Texture1;
uniform sampler2D Texture2;

uniform vec3 objectColor;
uniform vec3 eyePos;

uniform Material material;
uniform Light light;

out vec4 outColor;

void main()
{
	vec3 ambient = light.ambient * material.ambient;

	vec3 diffuse = light.diffuse * material.diffuse * max(dot(normalize(light.position - fragCoord) , normalize(Normal)), 0) ;

	vec3 specular = light.specular * material.specular * pow(max(dot(normalize(normalize(light.position - fragCoord) + normalize(eyePos - fragCoord)), normalize(Normal)), 0), material.shininess);

	vec3 result = (diffuse + ambient + specular) * objectColor;

	outColor = vec4(result, 1.0);
}