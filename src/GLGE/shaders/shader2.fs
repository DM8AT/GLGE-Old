#version 300 es

precision mediump float;

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;

float dist(vec2 a, vec2 b)
{
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

void main()
{
    float d = dist(texCoords, vec2(0.5,0.5));
    FragColor = texture(screenTexture, texCoords) + vec4(d,0,0,0);
}