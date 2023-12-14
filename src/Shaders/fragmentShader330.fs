#version 330 core

#define PI 3.14159265

precision highp float;

in vec4 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPos;

//mesh data
uniform int glgeObjectUUID;
uniform float glgeRoughness;

uniform sampler2D glgeAmbientMap;
uniform sampler2D glgeNormalMap;
uniform sampler2D glgeRoughnessMap;
uniform sampler2D glgeTexture;

uniform samplerCube shadowMap;

uniform int usedTextures;

uniform bool glgeNormalMapIsActive;
uniform bool glgeRoughnessMapIsActive;

float rough;

void main()
{
    vec4 col = texture(glgeTexture, texCoord);

    if (usedTextures > 1)
    {
        col = vec4(vec3(col * texture(glgeAmbientMap, texCoord).r), col.w);
    }

    col += color;

    if (glgeRoughnessMapIsActive)
    {
        rough = texture(glgeRoughnessMap, texCoord).r;
    }
    else
    {
        rough = glgeRoughness;
    }

    //calculate the normal
    vec3 n = normalize(normal);

    if (glgeNormalMapIsActive)
    {
        n = normalize(normal + (texture(glgeNormalMap, texCoord).rgb * 2.f - 1.f));
    }

    vec4 FragColor = vec4(0);

    gl_FragData[0] = col;
    gl_FragData[1] = vec4(n,1);
    gl_FragData[2] = vec4(currentPos, 1);
    //watch out: the b chanell of the roughness map says if the object sould be lit or unlit: 1 is lit; 0 is unlit
    gl_FragData[3] = vec4(rough,0,1,1);
    gl_FragData[5] = vec4(gl_FragCoord.z,glgeObjectUUID,0,1);

    gl_FragDepth = gl_FragCoord.z;
}