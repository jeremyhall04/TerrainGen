#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec4 inColor;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 modelMatrix = mat4(1.0);

out vec3 position;
out vec3 normal;
flat out vec4 color;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPos, 1.0);
	position = vec3(modelMatrix * vec4(inPos, 1.0));
	normal = inNormal;
	color = inColor;	
}
