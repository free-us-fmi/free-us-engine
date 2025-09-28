#version 450 core
out vec4 FragColor;

uniform sampler2D _texture;
uniform bool isLight;
uniform vec3 eyePos;

in S_OUT{
	vec2 texCoord;
	vec3 normal;
	vec3 position;
} s_in;

vec2 texCoord;
vec3 normal;
vec3 position;

vec3 light_color = vec3(1.0f, 1.f, 1.f);
float specular_strength = 1.0;

struct Material
{
    float diffuse;
    sampler2D ambient;
    sampler2D specular;
    int shininess;
};

uniform Material material;

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionalLight dirLight;

struct PointLight
{
    bool is_active;

    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
#define NUM_POINT_LIGHTS 10
uniform PointLight pointLights[NUM_POINT_LIGHTS];

struct SpotLight
{
    PointLight pointLight;
    vec3 direction;
    float cutOff;
    float outerCutOff;
};

uniform SpotLight spotLight;

vec3 GetLight(vec3 light_dir, vec3 light_ambient, vec3 light_diffuse, vec3 light_specular)
{
    light_dir = normalize(light_dir);

    vec3 tex = texture(material.ambient, texCoord).xyz;
    vec3 specTex = texture(material.specular, texCoord).xyz;

    vec3 ambient = tex * light_ambient;

    vec3 norm = normalize(normal);

    float diff = max(dot(norm, light_dir), 0.0f);
    vec3 diffuse = diff * light_diffuse * tex;

    vec3 eyeDir = normalize(eyePos - position);
    vec3 reflectDir = reflect(-light_dir, norm);
    float spec = pow(max(dot(reflectDir, eyeDir), 0.f), material.shininess);
    vec3 specular = spec * specTex * specular_strength * light_specular;

    vec3 result = ambient + diffuse + specular;

    return result;
}

vec3 GetDirectionalLight(in DirectionalLight dirLight)
{
    return GetLight(-dirLight.direction, dirLight.ambient, dirLight.diffuse, dirLight.specular);
}

vec3 GetPointLight(in PointLight pointLight)
{
    vec3 lightDir = pointLight.position - position;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);

    float attenuation = 1.0f / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);
    return GetLight(lightDir, pointLight.ambient, pointLight.diffuse, pointLight.specular) * attenuation;
}

vec3 GetSpotLight(in SpotLight spotLight)
{
    vec3 result = GetPointLight(spotLight.pointLight);

    vec3 lightDir = normalize(spotLight.pointLight.position - position);
    float angleCos = dot(spotLight.direction, -lightDir);

    if (angleCos > spotLight.cutOff)
        return result;
    else if (angleCos > spotLight.outerCutOff)
        return result * (angleCos - spotLight.outerCutOff) / (spotLight.cutOff - spotLight.outerCutOff);
    else return vec3(0.f, 0.f, 0.f);
}

void main()
{
	texCoord = s_in.texCoord;
	normal = s_in.normal;
	position = s_in.position;

	if (isLight)
	{
		FragColor = vec4(normal, 1.f);
		return;
	}

	vec3 _output = vec3(0.f);
	for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
	if (pointLights[i].is_active)
	    _output += GetPointLight(pointLights[i]);
	_output += GetDirectionalLight(dirLight);
	FragColor = vec4(_output, 1.f);
}
