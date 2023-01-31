#version 300 es

precision mediump float;
out vec4 FragColor;

in vec4 fColor;
in vec2 fTexCoord;

uniform sampler2D Texture;

void main()
{
    FragColor = texture(Texture, fTexCoord) + fColor;
}