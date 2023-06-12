#version 300 es

precision highp float;

uniform sampler2D glgeMainImage;

in vec4 color;
in vec2 texCoords;

out vec4 FragColor;

void main()
{
    FragColor = texture(glgeMainImage, texCoords);
}