#version 330 core

#define PI 3.14159265

precision highp float;

in vec4 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPos;

//mesh data
uniform float roughness;

uniform sampler2D AmbientMap;
uniform sampler2D NormalMap;
uniform sampler2D RoughnessMap;
uniform sampler2D Texture;

uniform samplerCube shadowMap;

uniform int usedTextures;

uniform bool NormalMapIsActive;
uniform bool RoughnessMapIsActive;

float rough;

void main()
{
    vec4 col = texture(Texture, texCoord);

    if (usedTextures > 1)
    {
        col = vec4(vec3(col * texture(AmbientMap, texCoord).r), col.w);
    }

    col += color;

    if (RoughnessMapIsActive)
    {
        rough = texture(RoughnessMap, texCoord).r;
    }
    else
    {
        rough = roughness;
    }

    //calculate the normal
    vec3 n = normalize(normal);

    if (NormalMapIsActive)
    {
        n = normalize(normal + (texture(NormalMap, texCoord).rgb * 2.f - 1.f));
    }

    vec4 FragColor = vec4(0);

    gl_FragData[0] = col;
    gl_FragData[1] = vec4(n,1);
    gl_FragData[2] = vec4(currentPos, 1);
    //watch out: the b chanell of the roughness map says if the object sould be lit or unlit: 1 is lit; 0 is unlit
    gl_FragData[3] = vec4(rough,0,1,1);

    gl_FragDepth = gl_FragCoord.z;
}