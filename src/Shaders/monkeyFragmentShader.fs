#version 450 core

precision highp float;

layout(location = 0) out vec4 Albedo;
layout(location = 1) out vec4 Normal;
layout(location = 2) out vec4 Position;
layout(location = 3) out vec4 Roughness;

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

uniform samplerCube shadowMap;

float exposure = 0.1;

uniform int usedTextures;

uniform bool NormalMapIsActive;
uniform bool RoughnessMapIsActive;

uniform vec3 cameraPos;
uniform float farPlane;

float rough;

vec3 actualNormal = vec3(0,0,0);

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

    actualNormal = n;

    //the final output color
    vec4 FragColor = vec4(0,0,0,0);

    Albedo = min(max(col, vec4(0,0,0,0)), vec4(1,1,1,1));
    Normal = vec4((actualNormal + vec3(1,1,1)) / vec3(2,2,2),1);
    Position = vec4(currentPos,1);
    Roughness = vec4(rough,0,0,1);

    gl_FragDepth = gl_FragCoord.z;
}