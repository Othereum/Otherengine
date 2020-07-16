# Shader Programming

[한국어](../한국어/셰이더%20프로그래밍.md)

Otherengine's shader can be written in GLSL, and there are some useful default parameters:

## Vertex shader

```glsl
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

layout(location=0) in vec3 inPosition;
layout(location=1) in vec3 inNormal; // normalized
layout(location=2) in vec2 inTexCoord;
```

## Fragment shader

```glsl
#define MAX_LIGHTS 4

struct DirLight
{
    vec3 dir; // normalized
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
    vec3 dir; // normalized
    float radius;
    float inner; // inner cone angle cosine
    float outer; // outer cone angle cosine
};

uniform sampler2D uTexture;
uniform float uSpecular;
uniform vec3 uCamPos;
uniform vec3 uSkyLight;
uniform DirLight uDirLight;
uniform PointLight uPointLights[MAX_LIGHTS];
uniform SpotLight uSpotLights[MAX_LIGHTS];
uniform int uNumPointLights; // never exceed MAX_LIGHTS
uniform int uNumSpotLights; // never exceed MAX_LIGHTS
```

### Lighting limitations

Light type | Number Supported
-|-
Directional | 1
Point | 4
Spot | 4

Note that the maximum number of point lights and spotlights is a shader limit, and there is no limit to the number that can be placed in a scene. The renderer determines which light data to be passed to the shader.

