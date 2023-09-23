#version 300 es

precision highp float;

uniform sampler2D glgeMainImage;
uniform sampler2D glgeRoughnessMap;

in vec4 color;
in vec2 texCoords;

out vec4 FragColor;

const float gamma = 2.2f;
const float exposure = 1.0f;

void main()
{
    vec3 col = texture(glgeMainImage, texCoords).rgb;

    vec3 mapped = col;
    //apply tone mapping if the object is lit
    if (int(texture(glgeRoughnessMap, texCoords).b) == 1)
    {
        // tone mapping
        mapped = vec3(1.0) - exp(-mapped * exposure);
        // gamma correction
        mapped = pow(mapped, vec3(1.0 / gamma));
    }

    FragColor = vec4(mapped, 1.f);
}