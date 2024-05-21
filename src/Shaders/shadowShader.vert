#version 450 core
layout (location = 0) in vec3 pos;

uniform mat4 glgeLightSpaceMat;

layout (std140, binding = 0) uniform glgeObjectData
{
    mat4 glgeModelMat;
    mat4 glgeRotMat;
    int glgeObjectUUID;
};

void main()
{
    gl_Position = vec4(pos, 1.0) * glgeModelMat * glgeLightSpaceMat;
}