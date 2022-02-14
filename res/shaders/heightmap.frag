#version 330 core

out vec4 FragColor;

in vec3 position;
in vec2 uv;
in vec3 normal;

uniform sampler2D tex;
uniform vec3 light_pos;
uniform vec3 light_color;

void main()
{
	vec4 objColor = texture(tex, uv);
	float ambientStrength = 0.05;
	vec3 ambient = ambientStrength * light_color;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light_pos - position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light_color;

	FragColor = vec4(ambient + diffuse, 1.0) * objColor;

	//FragColor = texture(tex, uv);
}
