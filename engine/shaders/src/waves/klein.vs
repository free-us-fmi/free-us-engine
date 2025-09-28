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
	float R = 4.f;

	texCoord = aTexCoord;
	
	float u = (aPos.x + 1.f) * 3.14159, v = (aPos.z + 1.f) * 3.14159;
	float cos_u = cos(u), sin_u = sin(u);
	float cos_v = cos(v), sin_v = sin(v);
	float cos_half_u = cos(u/2), sin_half_u = sin(u/2);
	float sin_2v = sin(2*v), cos_2v = cos(2*v);

	float alpha = (R + cos_half_u*sin_v - sin_half_u*sin_2v);

	position.x = alpha*cos_u;
	position.z = alpha*sin_u;
	position.y = (sin_half_u*sin_v + cos_half_u*sin_2v);
	
	float du_alpha = -0.5f * ( sin_half_u*sin_v + cos_half_u*sin_2v );
	float dv_alpha = cos_half_u*cos_v - 2*sin_half_u*cos_2v;

	float du_x = -sin_u*alpha + cos_u*du_alpha;
	float du_y = cos_u*alpha + sin_u*du_alpha;
	float du_z = 0.5 * ( cos_half_u*sin_v - sin_half_u*sin_2v );

	float dv_x = cos_u*dv_alpha;
	float dv_y = sin_u*dv_alpha;
	float dv_z = sin_half_u*cos_v + 2*cos_half_u*cos_2v;

	normal.x = du_y * dv_z - du_z * dv_y;
	normal.y = du_x * dv_z - du_z * dv_x;
	normal.z = du_x * dv_y - du_y * dv_x; 

	normal = normalize(normal);

	if ( normal.x < 0.f )
		normal.x = -normal.x;
	if ( normal.y < 0.f )
		normal.y = -normal.y;
	if ( normal.z < 0.f )
		normal.z = -normal.z;

	normal = normalize(normal);
	position = vec3(model * vec4(position, 1.f));
	gl_Position = projection_view * vec4(position, 1.0);
	normal = normalize(mat3(normal_model) * normal);

	vs_out.texCoord = texCoord;
	vs_out.normal = normal;
	vs_out.position = position;
}
