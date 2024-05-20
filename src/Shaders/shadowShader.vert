#version 450 core
layout (location = 0) in vec3 pos;

uniform mat4 glgeLightSpaceMat;
uniform mat4 glgeModelMat;

void main()
{
    gl_Position = vec4(pos, 1.0) * glgeModelMat * glgeLightSpaceMat;
}