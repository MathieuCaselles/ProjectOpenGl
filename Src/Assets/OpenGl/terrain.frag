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
uniform sampler2D textureGrass;
uniform sampler2D textureSnow;
uniform sampler2D textureStone;
uniform sampler2D textureSand;


uniform float snowHeight;
uniform float stoneAngle;
uniform float waterHeight;

void main()
{	
	vec3 worldEye = normalize(iWorldPosition - camera.worldPosition);
	vec3 h = dot(iWorldNormal, light.direction) * iWorldNormal;
	vec3 pprime = 2 * h - light.direction;
	vec3 specular = light.color * pow(max(0, dot(worldEye, pprime)), material.specularSmoothness) * material.specular;

	vec4 texGrassColor = texture(textureGrass, iTexCoord);
	vec4 texSnowColor = texture(textureSnow, iTexCoord);
	vec4 texStoneColor = texture(textureStone, iTexCoord);
	vec4 texSandColor = texture(textureSand, iTexCoord);


	vec4 color = vec4(0.0);
	float angle = degrees(acos(dot(iWorldNormal, vec3(0, -1, 0))));

	if (angle > stoneAngle && iHeight < snowHeight && iHeight > waterHeight + 1) {
		color = texStoneColor;
	} else {
		if (iHeight > snowHeight && iHeight > waterHeight) {
			color = mix(texStoneColor, texSnowColor, smoothstep(snowHeight, snowHeight + 4, iHeight));
		} else if(iHeight < waterHeight + 1)
		{
			color = mix(texGrassColor, texSandColor, smoothstep(waterHeight + 1, waterHeight , iHeight));
			
		} else {
			color = texGrassColor;
		}
	}

	vec3 ambient = material.ambient * color.rgb;
	vec3 diffuse = max(0, -dot(iWorldNormal, light.direction)) * light.color * color.rgb;
	
	if(iHeight > waterHeight){
		
		fragColor = vec4(ambient + diffuse + specular / 2, 1.f) + color;

	}else{
		fragColor = vec4(ambient + diffuse, 1.f) + color;
	
	}
	
	

}