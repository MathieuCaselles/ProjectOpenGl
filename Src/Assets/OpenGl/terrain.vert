#version 430 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec4 vColor;
layout (location = 3) in vec2 vTexCoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec4 iColor;
out vec3 iWorldNormal;
out vec3 iWorldPosition;

out vec2 iTexCoord;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vPosition;
	iColor = vColor;
	iWorldNormal = mat3(ModelMatrix) * vNormal;
	iWorldPosition = (ModelMatrix * vPosition).xyz;

	iTexCoord = vTexCoord;
}