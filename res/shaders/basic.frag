#version 330 core

out vec4 FragColor;

uniform vec3 light_pos;
uniform vec3 light_color;

in vec3 position;
in vec3 normal;
flat in vec4 color;

void main()
{
	float ambientStrength = 0.05;
	vec3 ambient = ambientStrength * light_color;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light_pos - position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light_color;

	vec4 result = vec4(ambient + diffuse, 1.0) * color;
	//FragColor = result;
	FragColor = color;
}
