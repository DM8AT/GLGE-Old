#version 300 es

precision highp float;

uniform sampler2D image;
uniform vec2 screenSize;
uniform int sampleMult;

in vec2 texCoords;

out vec4 FragColor;

float kernel[9] = float[]
(
    1.f, 2.f, 1.f,
    2.f, 4.f, 2.f,
    1.f, 2.f, 1.f
);

void main()
{
    float offset_x = 1.0f / screenSize.x;
    float offset_y = 1.0f / screenSize.y;
    
    vec2 offsets[9] = vec2[]
    (
        vec2(-offset_x,  offset_y), vec2( 0.0f,    offset_y), vec2( offset_x,  offset_y),
        vec2(-offset_x,  0.0f),     vec2( 0.0f,    0.0f),     vec2( offset_x,  0.0f),
        vec2(-offset_x, -offset_y), vec2( 0.0f,   -offset_y), vec2( offset_x, -offset_y) 
    );
    vec3 color = vec3(0);
    for(int i = 0; i < 9; i++)
    {
        color += (1.f/16.f)*vec3(texture(image, ((texCoords*vec2(sampleMult)) + offsets[i]))) * kernel[i];
    }

    FragColor = vec4(color, 1.0f);
}