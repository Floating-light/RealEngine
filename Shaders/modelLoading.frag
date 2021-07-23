#version 330 core

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 spotPosition;
	vec3 spotDirection;
	float cutOff;
	float outCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};

in vec2 TexCoord;
in vec3 fragCoord;
in vec3 Normal;

uniform vec3 eyePos;
uniform sampler2D emissionV;

uniform Material material;

#define NR_POINT_LIGHTS 4

uniform DirLight dirLight;
uniform PointLight pointlight[NR_POINT_LIGHTS];
uniform SpotLight spotLight;


out vec4 outColor;

vec3 CalcDirLight(DirLight light, vec3 noemal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(eyePos - fragCoord);
//	vec3 result;
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	for(int i = 0; i < NR_POINT_LIGHTS; ++i)
	{
		result += CalcPointLight(pointlight[i], norm, fragCoord, viewDir);
	}

	result += CalcSpotLight(spotLight, norm, fragCoord, viewDir);

	outColor = vec4(result, 1.0);
//	outColor = texture(material.texture_diffuse1, TexCoord);
}


vec3 CalcDirLight(DirLight light, vec3 noemal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	// 光照方向越接近法线方向越亮
	float diff = max(dot(lightDir, noemal), 0.0);

	float spec = pow(max(dot(viewDir, reflect(-lightDir, noemal)), 0.0), material.shininess);
//	float spec = pow(max(dot(noemal, normalize(lightDir + noemal)), 0.0), material.shininess);
	vec3 amb = light.ambient * texture(material.texture_diffuse1, TexCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoord).rgb;
	vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoord).rgb;

	return amb + diffuse + specular;
}

//struct PointLight
//{
//	vec3 position;
//
//	float constant;
//	float linear;
//	float quadratic;
//
//	vec3 ambient;
//	vec3 diffuse;
//	vec3 specular;
//};
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float lightDistance = length(light.position - fragPos);
	float attenuation = 1.0f/(light.constant + light.linear*lightDistance + light.quadratic*lightDistance * lightDistance);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));

	vec3 diffuse = light.diffuse * vec3(texture(material.texture_diffuse1, TexCoord)) * max(dot(normal, lightDir), 0.0f);

	vec3 specular = light.specular * vec3(texture(material.texture_specular1, TexCoord)) * pow(max(dot(normalize(lightDir+viewDir), normal), 0.0f), material.shininess);

	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.spotPosition - fragPos);
	float theta = dot(lightDir, normalize(-light.spotDirection));
	float epsilon = light.cutOff - light.outCutOff;
	float intensity = clamp((theta - light.outCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));

	vec3 diffuse = light.diffuse * vec3(texture(material.texture_diffuse1, TexCoord)) * max(dot(normal, lightDir), 0.0f);

	vec3 specular = light.specular * vec3(texture(material.texture_specular1, TexCoord)) * pow(max(dot(normalize(lightDir+viewDir), normal), 0.0f), material.shininess);



	return  intensity*(ambient + diffuse + specular);
}