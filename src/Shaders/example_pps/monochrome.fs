#version 450 core

precision highp float;

uniform sampler2D glgeMainImage;
uniform sampler2D glgeRoughnessMap;

in vec4 color;
in vec2 texCoords;

out vec4 FragColor;

const float gamma = 2.2f;
const float exposure = 1.0f;

const vec3 dark = vec3(44.f/255.f,45.f/255.f,28.f/255.f);
const vec3 bright = vec3(228.f/255.f,1,251.f/255.f);

void main()
{
    vec3 col = texture(glgeMainImage, texCoords).rgb;

    vec3 roughData = texture(glgeRoughnessMap, texCoords).rgb;

    vec3 mapped = bright;
    /*//apply tone mapping if the object is lit
    if (roughData.b == 1.f)
    {
        // tone mapping
        mapped = vec3(1.0) - exp(-mapped * exposure);
        // gamma correction
        mapped = pow(mapped, vec3(1.0 / gamma));
    }*/

    if (int(col.b) * 100 % 10 == 0) {
        mapped = dark;
    }

    FragColor = vec4(mapped, 1.f);
}