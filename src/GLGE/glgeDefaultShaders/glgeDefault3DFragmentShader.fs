#version 450 core
#extension GL_ARB_bindless_texture : enable

precision highp float;

layout(location = 0) out vec4 Albedo;
layout(location = 1) out vec4 Normal;
layout(location = 2) out vec4 Position;
layout(location = 3) out vec4 Roughness;
layout(location = 5) out vec4 DepthAndAlpha;

layout (std140, binding = 0) uniform glgeObjectData
{
    mat4 glgeModelMat;
    mat4 glgeRotMat;
    int glgeObjectUUID;
};

layout (std140, binding = 1) uniform glgeCameraData
{
    mat4 glgeCamMat;
    mat4 glgeProjMat;
    mat4 glgeCamTransfMat;
    mat4 glgeCamRotMat;
    vec3 glgeCameraPos;
    vec3 glgeCameraRot;
    float glgeNear;
    float glgeFar;
    float glgeFov;
};

in vec4 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPos;
in vec3 vPos;

layout (std140, binding = 2) uniform glgeMaterialData
{
    vec4 glgeColor;
    float glgeRoughness;
    float glgeMetalic;
    int glgeLit;
    int glgeAmbientMapActive;
    int glgeNormalMapActive;
    int glgeRoughnessMapActive;
    int glgeMetalicMapActive;
    uvec2 glgeAmbientMap;
    uvec2 glgeNormalMap;
    uvec2 glgeRoughnessMap;
    uvec2 glgeMetalicMap;
};

void main()
{
    //read the color for the object
    vec4 col = glgeColor + color;
    col.rgb *= (1-int(glgeAmbientMapActive));
    col.rgb += texture(sampler2D(glgeAmbientMap), texCoord).rgb * int(glgeAmbientMapActive);
    //no luck getting arround this if statement. Alpha clipping is enabled by default
    if(col.w < 0.5){discard;}
    //read the roughness and again use multiplikation like for the albedo texture
    float rough = texture(sampler2D(glgeRoughnessMap), texCoord).r * float(int(glgeRoughnessMapActive));
    //add the default roughness, multiplied with the inverted scalar (like an else, but faster)
    rough += glgeRoughness * (1.f - float(int(glgeRoughnessMapActive)));
    //store the color
    Albedo = vec4(col.rgb, vPos.x);
    //store the normal
    Normal = vec4(normal, vPos.y);
    //store the position data
    Position = vec4(currentPos, vPos.z);
    //store the roughness data
    Roughness = vec4(rough,glgeMetalic,int(glgeLit),1);
    //store the depth data and object uuid
    DepthAndAlpha = vec4(gl_FragCoord.z,glgeObjectUUID,0,1);
}