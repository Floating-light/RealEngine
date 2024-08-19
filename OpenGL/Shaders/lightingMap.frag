#version 330 core
struct Light
{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	vec3 spotPosition;
	vec3 spotDirection;
	float cutOff;
	float outerCutOff;
};
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

in vec2 TexCoord;
in vec3 fragCoord;
in vec3 Normal;

uniform vec3 eyePos;

uniform Material material;
uniform Light light;
uniform sampler2D emissionV;
out vec4 outColor;
void main()
{
	float theta = dot(normalize(light.spotPosition - fragCoord ), normalize(-light.spotDirection));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp( ( theta - light.outerCutOff ) / epsilon, 0.0, 1.0);

//	if(theta > light.cutOff)
//	{
		float dis = length(light.position - fragCoord);
		float attenuation = 1.0f/(light.constant + light.linear * dis + light.quadratic * dis * dis);
		vec3 position = normalize(-light.direction);
		// 不论eye的位置
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

		// eye的位置与光源越接近(夹角越小) 越亮
		vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoord)) * max(dot(normalize(light.position - fragCoord) , normalize(Normal)), 0) ;

		// eye 光源 fragemnt 的半程向量与法线越接近 
		// eye 在光源的相反的位置
		vec3 specular = light.specular *vec3(texture(material.specular, TexCoord)) * pow(max(dot(normalize(normalize(light.position - fragCoord) + normalize(eyePos - fragCoord)), normalize(Normal)), 0), material.shininess);

		vec3 result = (diffuse*intensity + ambient + specular*intensity) * attenuation + texture(emissionV, TexCoord).rgb;

		outColor = vec4(result, 1.0);
		//outColor = texture(material.specular, TexCoord);
//	}
//	else // 圆圈外
//	{
//		outColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoord)), 1.0);
//	}
}