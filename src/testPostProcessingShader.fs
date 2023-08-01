#version 300 es

precision highp float;

uniform sampler2D glgeMainImage;

in vec4 color;
in vec2 texCoords;

out vec4 FragColor;

const float gamma = 2.2f;
const float exposure = 1.0f;

void main()
{
    FragColor = texture(glgeMainImage, texCoords);

    if (FragColor.w == 0.f)
    {
        FragColor.rgb *= -0.25f;
        FragColor.w = 1.f;
    }
    else
    {
        vec3 col = texture(glgeMainImage, texCoords).rgb;

        // tone mapping
        vec3 mapped = vec3(1.0) - exp(-col * exposure);
        // gamma correction
        mapped = pow(mapped, vec3(1.0 / gamma));

        FragColor = vec4(mapped, 1.f);
    }
}