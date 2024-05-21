#version 450 core
#extension GL_ARB_bindless_texture : require
#define PI 3.14159265

precision highp float;

layout(location = 5) out vec4 Max;
layout(location = 7) out vec4 Col;
layout(location = 3) out vec4 rml;

uniform sampler2D glgeLitMap;

in vec4 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPos;

layout (std140, binding = 0) uniform glgeObjectData
{
    mat4 glgeModelMat;
    mat4 glgeRotMat;
    int glgeObjectUUID;
};

layout (std140, binding = 1) uniform glgeCameraData
{
    mat4 glgeCamMat;
    mat4 glgeProjMat;
    mat4 glgeCamTransfMat;
    mat4 glgeCamRotMat;
    vec3 glgeCameraPos;
    vec3 glgeCameraRot;
    float glgeNear;
    float glgeFar;
    float glgeFov;
};

layout (std140, binding = 2) uniform glgeMaterialData
{
    vec4 glgeColor;
    float glgeRoughness;
    float glgeMetalic;
    int glgeLit;
    int glgeAmbientMapActive;
    int glgeNormalMapActive;
    int glgeRoughnessMapActive;
    int glgeMetalicMapActive;
    uvec2 glgeAmbientMap;
    uvec2 glgeNormalMap;
    uvec2 glgeRoughnessMap;
    uvec2 glgeMetalicMap;
};

uniform bool glgePass;

float rough = 0.f;

int iteration = 0;

vec3 actualNormal = vec3(0,0,0);

float lightRadius = 0.1;
float a = 0.f;
float b = 0.f;

vec3 pos;

float metallic = 0.f;

#include <glgeLightEvaluationFunction>

layout (std140, binding=3) uniform glgeLightData
{
    int glgeActiveLights;
    glgeLight glgeLights[128];
};

void main()
{
    //read the color for the object
    vec4 col = glgeColor + color;
    col.rgb *= (1-int(glgeAmbientMapActive));
    col.rgb += texture(sampler2D(glgeAmbientMap), texCoord).rgb * int(glgeAmbientMapActive);
    //no luck getting arround this if statement. Alpha clipping is enabled by default
    if(col.w == 0.f){discard;}
    if (!glgePass && col.w!=1.f){discard;}
    if (glgePass && col.w==1.f){discard;}
    //read the roughness and again use multiplikation like for the albedo texture
    float rough = texture(sampler2D(glgeRoughnessMap), texCoord).r * float(int(glgeRoughnessMapActive));
    //add the default roughness, multiplied with the inverted scalar (like an else, but faster)
    rough += glgeRoughness * (1.f - float(int(glgeRoughnessMapActive)));
    //store the normal
    actualNormal = normal;
    //store the position
    pos = currentPos;
    //store the final color
    vec4 finColor = col;
    //check if the object is lit
    if (bool(glgeLit))
    {
        //clear the final color
        finColor.rgb = vec3(0);
        //construct the lighting data for this pixel
        glgePixel d = glgePixel(
            col.rgb,
            currentPos,
            normal,
            rough, metallic
        );

        //calculate the lighting for the object
        for (int i = 0; i < glgeActiveLights; i++)
        {
            vec3 light = glgeEvaluateLight(glgeLights[i], d);
            finColor.xyz += light;
        }
    }

    Col = finColor;
    Max = vec4(0,0,1,1);
    rml.rgb = vec3(rough, glgeMetalic, float(glgeLit));
}