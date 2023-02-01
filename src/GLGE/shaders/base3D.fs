#version 300 es

precision mediump float;
out vec4 FragColor;

in vec4 fColor;
in vec2 fTexCoord;

void main()
{
    FragColor = fColor;
}