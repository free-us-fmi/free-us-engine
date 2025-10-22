#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in mat4 aModel;

out S_OUT {
	vec2 texCoord;
	vec3 normal;
	vec3 position;
} vs_out;

vec2 texCoord;
vec3 normal;
vec3 position;

uniform mat4 projection_view;

out vec4 light_space_pos;
void main()
{
	texCoord = aTexCoord;
	position = vec3(aModel * vec4(aPos, 1.f));
	gl_Position = projection_view * vec4(position, 1.0);
	normal = mat3(transpose(inverse(aModel))) * aNormal;

	vs_out.texCoord = texCoord;
	vs_out.normal = normal;
	vs_out.position = position;

}
