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
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;



void main()
{	
	vec3 worldEye = normalize(iWorldPosition - camera.worldPosition);
	vec3 h = dot(iWorldNormal, light.direction) * iWorldNormal;
	vec3 pprime = 2 * h - light.direction;
	vec3 specular = light.color * pow(max(0, dot(worldEye, pprime)), material.specularSmoothness) * material.specular;

	vec4 texColor1 = texture(texture1, iTexCoord);
	vec4 texColor2 = texture(texture2, iTexCoord);
	vec4 texColor3 = texture(texture3, iTexCoord);


	vec4 color = vec4(0.0);

	// Changer en stone quand la normal a un certains angle ?

    if (iHeight > 6) {
        color = mix(texColor3, texColor2, smoothstep(6.0, 7.0, iHeight));
    } else if (iHeight > 0.5) {
        color = mix(texColor1, texColor3, smoothstep(0.5, 2.0, iHeight));
    } else {
        color = texColor1;
    }

	vec3 ambient = material.ambient * color.rgb;
	vec3 diffuse = max(0, -dot(iWorldNormal, light.direction)) * light.color * color.rgb;

	fragColor = vec4(ambient + diffuse + specular, 1.f) + color;

}