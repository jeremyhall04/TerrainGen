#version 330 core

layout (location = 0) in vec3 position_in;
layout (location = 1) in vec2 uv_in;

out vec2 uv;

uniform mat4 projection;
uniform mat4 view = mat4(1.0);
uniform mat4 model = mat4(1.0);

void main()
{
	uv = uv_in;
	gl_Position = projection * view * model * vec4(position_in, 1.0);
}
