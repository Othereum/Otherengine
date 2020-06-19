#version 330
#define MAX_LIGHTS 4

struct DirLight
{
	vec3 dir;
	vec3 color;
};

struct PointLight
{
	vec3 color;
	vec3 pos;
	float radius;
};

struct SpotLight
{
	vec3 color;
	vec3 pos;
	vec3 dir;
	float angleCos;
	float radius;
};

uniform sampler2D uTexture;
uniform float uSpecular;
uniform vec3 uCamPos;
uniform vec3 uSkyLight;
uniform DirLight uDirLight;
uniform PointLight uPointLights[MAX_LIGHTS];
uniform SpotLight uSpotLights[MAX_LIGHTS];
uniform int uNumPointLights;
uniform int uNumSpotLights;

in vec2 fragTexCoord;
in vec3 fragWorldPos;
in vec3 fragWorldNormal;

out vec4 outColor;

vec3 Phong(vec3 normal, vec3 toCam, vec3 lightDir, vec3 lightColor)
{
	float diffuse = max(-dot(normal, lightDir), 0);
	float specular = uSpecular * max(dot(reflect(lightDir, normal), toCam), 0);
	return lightColor * (diffuse + specular);
}

float FallOff(vec3 unnormLightDir, float lightRadius)
{
	float i = max(0, 1 - length(unnormLightDir) / lightRadius);
	return i * i;
}

void main()
{
	vec3 normal = normalize(fragWorldNormal);
	vec3 toCam = normalize(uCamPos - fragWorldPos);

	vec3 light = uSkyLight + Phong(normal, toCam, uDirLight.dir, uDirLight.color);

	for (int i=0; i<uNumPointLights; ++i)
	{
		vec3 lightDir = fragWorldPos - uPointLights[i].pos;
		float intensity = FallOff(lightDir, uPointLights[i].radius);
		light += Phong(normal, toCam, normalize(lightDir), uPointLights[i].color * intensity);
	}

	for (int i=0; i<uNumSpotLights; ++i)
	{
		vec3 lightDir = fragWorldPos - uSpotLights[i].pos;
		float intensity = FallOff(lightDir, uSpotLights[i].radius);
		lightDir = normalize(lightDir);

		float theta = dot(uSpotLights[i].dir, lightDir);
		intensity *= max(0, (theta - uSpotLights[i].angleCos) / (1 - uSpotLights[i].angleCos));

		light += Phong(normal, toCam, lightDir, uSpotLights[i].color * intensity);
	}

	outColor = texture(uTexture, fragTexCoord) * vec4(light, 1);
}
