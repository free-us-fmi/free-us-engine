#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in mat4 aModel;

out vec2 texCoord;
out vec3 normal;
out vec3 position;

uniform mat4 projection_view;

void main()
{
	texCoord = aTexCoord;
	position = vec3(aModel * vec4(aPos, 1.f));
	gl_Position = projection_view * vec4(position, 1.0);
	normal = mat3(transpose(inverse(aModel))) * aNormal;
	
}
