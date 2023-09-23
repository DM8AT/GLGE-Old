#version 300 es

precision mediump float;

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D glgeMainImage;
uniform vec2 glgeWindowSize;

float kernel[9] = float[]
(
    1.f, 1.f, 1.f,
    1.f,-8.f, 1.f,
    1.f, 1.f, 1.f
);

void main()
{
    float offset_x = 1.0f / glgeWindowSize.x;
    float offset_y = 1.0f / glgeWindowSize.y;
    
    vec2 offsets[9] = vec2[]
    (
        vec2(-offset_x,  offset_y), vec2( 0.0f,    offset_y), vec2( offset_x,  offset_y),
        vec2(-offset_x,  0.0f),     vec2( 0.0f,    0.0f),     vec2( offset_x,  0.0f),
        vec2(-offset_x, -offset_y), vec2( 0.0f,   -offset_y), vec2( offset_x, -offset_y) 
    );
    vec3 color = vec3(0);
    for(int i = 0; i < 9; i++)
    {
        color += vec3(texture(glgeMainImage, texCoords.st + offsets[i])) * kernel[i];
    }
    FragColor = vec4(color, 1.0f);
}