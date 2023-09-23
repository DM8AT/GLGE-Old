#version 300 es

precision highp float;

uniform sampler2D image;
uniform vec2 screenSize;
uniform int sampleMult;

in vec2 texCoords;

out vec4 FragColor;

vec3 sampleAtPosition(vec2 position)
{
    vec3 col = 
        texture(image, (position+vec2(1.f/screenSize.x, 1.f/screenSize.y))).rgb   +
        texture(image, (position+vec2(1.f/screenSize.x, -1.f/screenSize.y))).rgb  +
        texture(image, (position+vec2(-1.f/screenSize.x, 1.f/screenSize.y))).rgb  +
        texture(image, (position+vec2(-1.f/screenSize.x, -1.f/screenSize.y))).rgb;
    col /= 4.f;
    return col;
}

vec3 sampleCurrentPos()
{
    vec3 c0 = sampleAtPosition((texCoords*vec2(sampleMult)));
    vec3 c1 = sampleAtPosition((texCoords*vec2(sampleMult)) + vec2(1.f/screenSize.x, 1.f/screenSize.y));
    vec3 c2 = sampleAtPosition((texCoords*vec2(sampleMult)) + vec2(1.f/screenSize.x, -1.f/screenSize.y));
    vec3 c3 = sampleAtPosition((texCoords*vec2(sampleMult)) + vec2(-1.f/screenSize.x, 1.f/screenSize.y));
    vec3 c4 = sampleAtPosition((texCoords*vec2(sampleMult)) + vec2(-1.f/screenSize.x, -1.f/screenSize.y));

    return ((c0*0.5f) + (c1*0.125f) + (c2*0.125f) + (c3*0.125f) + (c4*0.125f));
}

void main()
{
    FragColor = vec4(sampleCurrentPos(),1);
}