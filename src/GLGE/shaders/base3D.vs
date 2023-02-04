#version 300 es

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texcoord;
uniform mat4 moveMat;

out vec4 fColor;
out vec2 fTexCoord;

void main()
{
    fColor = color;
    fTexCoord = texcoord;
    gl_Position = vec4(pos ,1.0) * moveMat;
}