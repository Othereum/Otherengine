#version 330

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

layout(location=0) in vec3 inPosition;
layout(location=1) in vec3 inNormal;
layout(location=2) in vec2 inTexCoord;

out vec2 fragTexCoord;
out vec3 fragWorldPos;
out vec3 fragWorldNormal;

void main()
{
    vec4 wpos = vec4(inPosition, 1) * uWorldTransform;
    gl_Position = wpos * uViewProj;
    
    fragTexCoord = inTexCoord;
    fragWorldPos = wpos.xyz;
    fragWorldNormal = (vec4(inNormal, 0) * uWorldTransform).xyz;
}
