#version 300 es

precision highp float;

uniform sampler2D glgeMainImage;

in vec4 color;
in vec2 texCoords;

out vec4 FragColor;

float threshold = 0.3f;

bool calculateIfBloom(vec4 color)
{
    return (color.x < threshold) || (color.y < threshold) || (color.z < threshold);
}

void main()
{
    FragColor = texture(glgeMainImage, texCoords);
}