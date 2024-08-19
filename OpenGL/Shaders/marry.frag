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

in VS_OUT{
	vec3 FragPos; // the world position of this fragment.
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace; // the light space world cooridinate of this fragment
} fs_in;

uniform vec3 eyePos;
uniform sampler2D emissionV;
uniform sampler2D shadowmap;

uniform Material material;

uniform PointLight pointlight;

out vec4 outColor;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace);

void main()
{
	vec3 norm = normalize(fs_in.Normal);
	vec3 viewDir = normalize(eyePos - fs_in.FragPos);

	vec3 result = CalcPointLight(pointlight, norm, fs_in.FragPos, viewDir);

//	outColor = vec4(result, 1.0);
//	outColor = vec4(vec3(texture(shadowmap, TexCoord).r),1.0f);
	outColor = texture(material.texture_diffuse1, fs_in.TexCoords);
}
// return 1 if this fragment in the shadow, else return 0;
float ShadowCalculation(vec4 fragPosLightSpace)
{
	// 由于light space 用的正交投影, 不会用到w组件(所以这里的/w操作没有意义). 但对于透视投影则必须这样做.
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; // light space [-1,1]
	// depth map 坐标在[0,1]^2,
	projCoords = projCoords * 0.5 + 0.5;

	float currentDepth = projCoords.z;

	float delta = 0.002;
	// 采样得到最近的z值
	float closestDepth = currentDepth > texture(shadowmap, vec2(projCoords.x - delta, projCoords.y - delta)).r ? 1.0f : 0.0f;
	closestDepth += currentDepth > texture(shadowmap, vec2(projCoords.x - delta, projCoords.y)).r ? 1.0f : 0.0f;
	closestDepth += currentDepth > texture(shadowmap, vec2(projCoords.x - delta, projCoords.y + delta)).r ? 1.0f : 0.0f;
	closestDepth += currentDepth > texture(shadowmap, vec2(projCoords.x , projCoords.y - delta)).r ? 1.0f : 0.0f;
	closestDepth = currentDepth > texture(shadowmap, projCoords.xy).r ? 1.0f : 0.0f;
	closestDepth += currentDepth > texture(shadowmap, vec2(projCoords.x , projCoords.y + delta)).r ? 1.0f : 0.0f;
	closestDepth += currentDepth > texture(shadowmap, vec2(projCoords.x + delta , projCoords.y - delta)).r ? 1.0f : 0.0f;
	closestDepth += currentDepth > texture(shadowmap, vec2(projCoords.x + delta , projCoords.y )).r ? 1.0f : 0.0f;
	closestDepth += currentDepth > texture(shadowmap, vec2(projCoords.x + delta , projCoords.y + delta)).r ? 1.0f : 0.0f;
	float shadow = closestDepth / 9.0f;

	return shadow;
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

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));

	vec3 diffuse = light.diffuse * vec3(texture(material.texture_diffuse1, fs_in.TexCoords)) * max(dot(normal, lightDir), 0.0f);

	vec3 specular = light.specular * vec3(texture(material.texture_specular1, fs_in.TexCoords)) * pow(max(dot(normalize(lightDir+viewDir), normal), 0.0f), material.shininess);

	float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

	return (ambient + (1.0f-shadow)*(diffuse + specular)) * attenuation;
}