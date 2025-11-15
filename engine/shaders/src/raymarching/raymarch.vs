#version 450 core

layout(location = 0) in vec3 aPos;

out vec3 position;
void main()
{
    position = aPos;
	gl_Position = vec4(aPos.x, aPos.y, 0.f, 1.0f);
}
