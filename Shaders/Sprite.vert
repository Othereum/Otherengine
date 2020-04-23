#version 330

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

layout(location=0) in vec3 inPosition;
layout(location=1) in vec2 inTexCoord;

out vec2 fragTexCoord;

void main()
{
    vec4 pos = vec4(inPosition, 1.0);
    gl_Position = pos * uWorldTransform * uViewProj;
    fragTexCoord = inTexCoord;
}
