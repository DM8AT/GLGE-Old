#version 300 es

#define PI 3.14159265

precision highp float;

layout(location = 3) out vec4 Lit;
layout(location = 5) out vec4 Max;
layout(location = 7) out vec4 Col;

uniform sampler2D glgeLitMap;

in vec4 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPos;

uniform int glgeObjectUUID;
uniform vec2 glgeScreenResolution;
uniform vec4 glgeColor;
uniform float glgeRough;
uniform float glgeMetalic;
uniform bool glgeHasTexture;
uniform sampler2D glgeTexture;
uniform sampler2D glgeAmbientMap;
uniform sampler2D glgeNormalMap;
uniform sampler2D glgeRoughMap;
uniform samplerCube shadowMap;
uniform bool NormalMapIsActive;
uniform bool glgeLit;
uniform bool roughMapIsActive;
uniform vec3 cameraPos;
uniform float farPlane;
uniform bool glgePass;

float rough = 0.f;
vec3 n = vec3(0);

int iteration = 0;

vec3 actualNormal = vec3(0,0,0);

//light data
uniform vec3 glgeLightColor[128];
uniform vec3 glgeLightData[128];
uniform vec3 glgeLightPos[128];
uniform vec3 glgeLightDir[128];
uniform int glgeActiveLights;

float lightRadius = 0.1;
float a = 0.f;
float b = 0.f;

vec3 pos;
float roughness;

float metallic = 0.f;
vec3 glgeCameraPos;

#include <glgeLightEvaluationFunction>

void main()
{
    vec2 uv = gl_FragCoord.rg/glgeScreenResolution;

    vec4 col = glgeColor;
    col.rgb += color.rgb;
    col.w += clamp(color.w,0.f,1.f);
    if (glgeHasTexture)
    {
        vec4 texColor = texture(glgeTexture, texCoord);
        col.rgb += texColor.rgb;
        col.w *= clamp(texColor.w,0.f,1.f);
    }
    
    col.w = clamp(col.w, 0.f, 1.f);

    if (col.w == 0.f){discard;}
    if (!glgePass && col.w!=1.f){discard;}
    if (glgePass && col.w==1.f){discard;}

    glgeCameraPos = cameraPos;

    if (roughMapIsActive)
    {
        rough = texture(glgeRoughMap, texCoord).r;
    }
    else
    {
        rough = glgeRough;
    }
    n = normalize(normal);
    if (NormalMapIsActive)
    {
        n = normalize(normal + (texture(glgeNormalMap, texCoord).rgb));
    }

    if (glgeLit)
    {
        glgePixel d = glgePixel(
            col.rgb,
            currentPos,
            normalize(n),
            rough, metallic
        );

        col.rgb = vec3(0);
        
        for (int i = 0; i < glgeActiveLights; i++)
        {
            glgeLight l = glgeLight(glgeLightPos[i], glgeLightColor[i], glgeLightDir[i], int(glgeLightData[i].x), glgeLightData[i].y, gkgeLightData[i].z);
            col.xyz += glgeEvaluateLight(l, d);
        }
    }

    Col = col;
    Max = vec4(0,0,1,1);
    Lit.b = 1.f;

    gl_FragDepth = gl_FragCoord.z;
}