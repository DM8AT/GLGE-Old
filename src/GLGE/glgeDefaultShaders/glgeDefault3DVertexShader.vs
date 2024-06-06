#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec2 vTexcoord;
layout (location = 3) in vec3 vNormal;

#include <glgeObject>
#include <glgeCamera>

layout (std140, binding = 4) buffer glgeModelMatrices
{
    mat4 glgeModelMatrix[];
};

uniform mat4 glgeLightSpaceMat[16];
uniform int glgeActiveShadowCasters;

out vec4 color;
out vec2 texCoord;
out vec3 normal;
out vec3 currentPos;
out vec3 vPos;
flat out int activeShadowCasters;

layout(location = 0) out vec4 lightPos[16];

void main()
{
    color = vColor;
    texCoord = vTexcoord;
    normal = vNormal;
    currentPos = vec3(vec4(pos, 1)*glgeModelMat);
    gl_Position = vec4(pos, 1)*glgeModelMat*glgeCamMat;
    vPos = pos;
    glgeModelMatrix[glgeObjectUUID] = glgeModelMat;
}