#version 450 core

precision highp float;

uniform sampler2D litImage;

in vec2 texCoords;

out vec4 FragColor;
float threshold = 1.f;

void main()
{
    vec4 col = texture(litImage, texCoords);

    float lit = float((col.r > threshold) || (col.g > threshold) || (col.b > threshold));

    FragColor = vec4(vec3(lit), 1);
}