#version 300 es

precision highp float;

uniform sampler2D currImage;
uniform sampler2D bloomMap;

in vec2 texCoords;

out vec4 FragColor;

void main()
{
    vec3 col = texture(bloomMap, texCoords).rgb*vec3(5);
    col += (texture(currImage, texCoords).rgb);

    FragColor = vec4(col, 1);
}