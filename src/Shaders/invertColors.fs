#version 300 es

precision mediump float;

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D glgeMainImage;
uniform float strength;

void main()
{
    vec4 color = texture(glgeMainImage, texCoords);

    vec3 invert = (vec3(1,1,1)-vec3(color.x,color.y,color.z));
    float s = clamp(strength, 0.f, 1.f);
    FragColor = vec4((invert*(1.f-s))+(color.rgb*s),1);
}