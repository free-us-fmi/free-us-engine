#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out S_OUT {
	vec2 texCoord;
	vec3 normal;
	vec3 position;
} vs_out;

vec2 texCoord;
vec3 normal;
vec3 position;

uniform mat4 projection_view;
uniform mat4 model;
uniform mat4 normal_model;

uniform float time;

void main()
{
	texCoord = aTexCoord;

	position.x = aPos.x;
	position.y = aPos.y + sin(aPos.x * aPos.x * 10 + aPos.z * aPos.z * 10 + time) * 5;
	position.z = aPos.z;

	normal.x = -100 * aPos.x * cos(aPos.x * aPos.x * 10 + aPos.z * aPos.z * 10 + time);
	normal.z = -100 * aPos.z * cos(aPos.x * aPos.x * 10 + aPos.z * aPos.z * 10 + time);
	normal.y = 1.f;

	position = vec3(model * vec4(position, 1.f));
	gl_Position = projection_view * vec4(position, 1.0);
	normal = normalize(mat3(normal_model) * normal);

	vs_out.texCoord = texCoord;
	vs_out.normal = normal;
	vs_out.position = position;
}
