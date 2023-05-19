#version 300 es

precision highp float;

layout (location = 0) in vec3 aPos;

uniform mat4 model;

out vec4 FragPos;

void main()
{
    fragPos = vec4(0,0,0,0);
    gl_Position = model * vec4(aPos, 1.0);
}