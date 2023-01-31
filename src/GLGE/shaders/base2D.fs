#version 300 es

precision mediump float;
out vec4 FragColor;

in vec4 color;
in vec2 texCoord;

uniform sampler2D Texture;

void main()
{
    FragColor = texture(Texture, texCoord) + color;
}