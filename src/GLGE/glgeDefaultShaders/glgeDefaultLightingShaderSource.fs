#version 300 es

#define PI 3.14159265

precision highp float;

layout(location = 4) out vec4 FragColor;
layout(location = 6) out vec4 LightData;
in vec2 texCoords;

uniform sampler2D glgeAlbedoMap;
uniform sampler2D glgeNormalMap;
uniform sampler2D glgePositionMap;
uniform sampler2D glgeRoughnessMap;

//light data
uniform vec3 glgeLightColor[128];
uniform vec4 glgeLightData[128];
uniform vec3 glgeLightPos[128];
uniform vec3 glgeLightDir[128];
uniform int glgeActiveLights;

//camera data
uniform vec3 glgeCameraPos;
uniform float glgeFarPlane;
uniform vec3 glgeCameraRot;

#include <glgeLightEvaluationFunction>

void main()
{
    vec3 tmp = texture(glgeRoughnessMap, texCoords).rgb;
    glgePixel d = glgePixel(
        texture(glgeAlbedoMap,   texCoords).rgb,
        texture(glgePositionMap, texCoords).rgb,
        normalize(texture(glgeNormalMap,   texCoords).rgb),
        tmp.x, tmp.y
    );

    if ((glgeActiveLights == 0) || (tmp.z == 0.f))
    {
        FragColor = vec4(d.color, 1);
        return;
    }

    FragColor = vec4(0,0,0,1);

    for (int i = 0; i < glgeActiveLights; i++)
    {
        glgeLight l = glgeLight(glgeLightPos[i], glgeLightColor[i], glgeLightDir[i], int(glgeLightData[i].x), glgeLightData[i].y, glgeLightData[i].z, glgeLightData[i].w);
        FragColor.xyz += glgeEvaluateLight(l, d);
    }
}