#version 300 es

precision mediump float;

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;

void main()
{
    vec4 color = texture(screenTexture, texCoords);
    FragColor = vec4(vec3(1,1,1)-vec3(color.x,color.y,color.z),1);
}