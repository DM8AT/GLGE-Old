#version 300 es

precision mediump float;

out vec4 FragColor;
in vec2 texCoords;

sampler2D screenTexture;
sampler2DMS normalTexture;

void main()
{
    vec3 normal = texelFetch(normalTexture, ivec2(gl_FragCoord.xy), 0).xyz;
    vec4 color = texture(screenTexture, texCoords);

    FragColor = color;//vec4(normal, 1.f);
}