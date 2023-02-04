#version 300 es

layout (location = 0) in vec2 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texcoord;
uniform mat3 moveMat;

out vec4 fColor;
out vec2 fTexCoord;

void main()
{
    fColor = color;
    fTexCoord = texcoord;
    vec4 memPos = vec4(vec3(pos,1)*moveMat ,1.0);
    memPos.z = 1.f;
    gl_Position = memPos;
}