#version 450 core

layout(location = 0) in vec3 aPos;

out vec2 uv;
void main()
{
    uv = aPos.xy*0.5 +0.5;
	gl_Position = vec4(aPos.x, aPos.y, 0.f, 1.0f);
}
