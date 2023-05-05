#version 430 core

out vec4 fragColor;

in vec4 iColor;
in vec3 iWorldNormal;
in vec3 iWorldPosition;

in vec2 iTexCoord;

in float iHeight;

struct Camera
{
	vec3 worldPosition;
};

struct Material
{
	float ambient;
	float diffuse;
	float specular;
	float specularSmoothness;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
};

uniform Material material;
uniform DirectionalLight light;
uniform Camera camera;
uniform sampler2D textureWater;

uniform float waterClearness;

void main()
{	
	vec3 worldEye = normalize(iWorldPosition - camera.worldPosition);
	vec3 h = dot(iWorldNormal, light.direction) * iWorldNormal;
	vec3 pprime = 2 * h - light.direction;
	vec3 specular = light.color * pow(max(0, dot(worldEye, pprime)), material.specularSmoothness) * material.specular;

	vec4 texColor1 = texture(textureWater, iTexCoord);
	texColor1.a = waterClearness;

	vec4 color = vec4(0.0);
	float angle = degrees(acos(dot(iWorldNormal, vec3(0, -1, 0))));

	color = texColor1;

	vec3 ambient = material.ambient * color.rgb;
	vec3 diffuse = max(0, -dot(iWorldNormal, light.direction)) * light.color * color.rgb;

	fragColor = vec4(ambient + diffuse + specular / 2, 1.f) + color;
	fragColor.a = waterClearness;
}