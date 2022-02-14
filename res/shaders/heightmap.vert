#version 330 core

layout (location = 0) in vec3 position_in;
layout (location = 1) in vec3 normal_in;
layout (location = 2) in vec2 uv_in;

out vec3 position;
out vec2 uv;
out vec3 normal;

uniform mat4 projection;
uniform mat4 view = mat4(1.0);
uniform mat4 model = mat4(1.0);

void main()
{
	position = position_in;
	uv = uv_in;
	normal = normal_in;
	gl_Position = projection * view * model * vec4(position_in, 1.0);
}
