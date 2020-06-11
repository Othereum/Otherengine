#version 330

struct DirLight
{
	vec3 dir;
	vec3 color;
};

uniform sampler2D uTexture;
uniform float uSpecular;
uniform vec3 uCamPos;
uniform vec3 uSkyLight;
uniform DirLight uDirLight;

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
