#version 300 es

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTexCoord;

out vec2 texCoords;

void main()
{
    gl_Position = vec4(inPos.x, inPos.y, 0, 1);
    texCoords = inTexCoord;
}