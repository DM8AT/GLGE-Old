#version 450 core

#define PI 3.14159265

precision highp float;

in vec4 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPos;

//mesh data
float emmission = 0.2f;
uniform float roughness;

uniform sampler2D AmbientMap;
uniform sampler2D NormalMap;
uniform sampler2D RoughnessMap;
uniform sampler2D Texture;

uniform int usedTextures;

uniform bool NormalMapIsActive;

float ambient = 0.1;
float exposure = 1.f;
float gamma = 2.2f;

vec3 actualNormal = vec3(0,0,0);

void calculateNormal()
{
    vec3 n = normalize(normal);

    if (NormalMapIsActive)
    {
        n = normalize(normal + (texture(NormalMap, texCoord).rgb * 2.f - 1.f));
    }

    actualNormal = n;
}

void main()
{
    vec4 col = texture(Texture, texCoord);

    if (usedTextures > 1)
    {
        col = vec4(vec3(col * texture(AmbientMap, texCoord).r), col.w);
    }

    col += color;

    //apply tonemapping
    col = vec4(vec3(1.f) - exp(-vec3(col) * exposure),col.w);

    vec4 FragColor = col;

    vec4 minColor = col * ambient;

    calculateNormal();

    gl_FragData[0] = vec4(actualNormal,1);
    gl_FragData[1] = min(max(FragColor, minColor), vec4(1,1,1,1));

    gl_FragDepth = gl_FragCoord.z;
}