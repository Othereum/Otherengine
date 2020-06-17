#version 330

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
	float angle;
	float radius;
};

uniform sampler2D uTexture;
uniform float uSpecular;
uniform vec3 uCamPos;
uniform vec3 uSkyLight;
uniform DirLight uDirLight;
uniform PointLight uPointLights[4];
uniform SpotLight uSpotLights[4];
uniform uint uNumPointLights;
uniform uint uNumSpotLights;

in vec2 fragTexCoord;
in vec3 fragWorldPos;
in vec3 fragWorldNormal;

out vec4 outColor;

void main()
{
	vec3 normal = normalize(fragWorldNormal);
	vec3 toCam = normalize(uCamPos - fragWorldPos);
	vec3 reflected = reflect(uDirLight.dir, normal);

	float diffuse = max(0, -dot(normal, uDirLight.dir));
	float specular = uSpecular * max(0, dot(reflected, toCam));
	vec3 light = uSkyLight + uDirLight.color * (diffuse + specular);

	outColor = texture(uTexture, fragTexCoord) * vec4(light, 1);
}
