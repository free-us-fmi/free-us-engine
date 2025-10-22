#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in S_OUT {
	vec2 texCoord;
	vec3 normal;
	vec3 position;
} gs_in[];

out S_OUT {
	vec2 texCoord;
	vec3 normal;
	vec3 position;
} vs_out;

in float posx[], posz[];

void main()
{
	vec3 normal;
	if ( posz[0] > 3.14 )
		normal = normalize(cross(gs_in[2].position - gs_in[1].position, gs_in[0].position - gs_in[1].position)); 
	else
		normal = normalize(cross(gs_in[0].position - gs_in[1].position, gs_in[2].position - gs_in[1].position)); 

	vs_out.texCoord = gs_in[0].texCoord;
	vs_out.position = gs_in[0].position;
	vs_out.normal = normal;
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	vs_out.texCoord = gs_in[1].texCoord;
	vs_out.position = gs_in[1].position;
	vs_out.normal = normal;
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	vs_out.texCoord = gs_in[2].texCoord;
	vs_out.position = gs_in[2].position;
	vs_out.normal = normal;
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
	
}
