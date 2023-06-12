/**
 * @file GLGEDefines.h
 * @author DM8AT
 * @brief this file stores all public defines for GLGE
 * @version 0.1
 * @date 2022-12-23
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

//check for double include
#ifndef _GLGE_DEFINES_H_
#define _GLGE_DEFINES_H_

//if this is included, std::strings are needed
#include <string>

//the default 3D vertex shader, it is bound by default to any 3D object
#define GLGE_DEFAULT_3D_VERTEX std::string("#version 300 es\nlayout (location = 0) in vec3 pos;layout (location = 1) in vec4 vColor;layout (location = 2) in vec2 vTexcoord;layout (location = 3) in vec3 vNormal;uniform mat4 camMat;uniform mat4 modelMat;out vec4 color;out vec2 texCoord;out vec3 normal;out vec3 currentPos;void main(){color = vColor;texCoord = vTexcoord;normal = vNormal;currentPos = vec3(vec4(pos, 1)*modelMat);gl_Position = vec4(pos, 1)*modelMat*camMat;}")
//the default 3D fragment shader, it is bound by default to any 3D object
#define GLGE_DEFAULT_3D_FRAGMENT std::string("#version 300 es\nprecision highp float;layout(location = 0) out vec4 Albedo;layout(location = 1) out vec4 Normal;layout(location = 2) out vec4 Position;layout(location = 3) out vec4 Roughness;in vec4 color;in vec2 texCoord;in vec3 normal;in vec3 currentPos;float emmission = 0.2f;uniform float roughness;uniform sampler2D AmbientMap;uniform sampler2D NormalMap;uniform sampler2D RoughnessMap;uniform sampler2D Texture;uniform samplerCube shadowMap;float exposure = 0.1;uniform int usedTextures;uniform bool NormalMapIsActive;uniform bool RoughnessMapIsActive;uniform vec3 cameraPos;uniform float farPlane;float rough;vec3 actualNormal = vec3(0,0,0);void main(){vec4 col = texture(Texture, texCoord);if (usedTextures > 1){col = vec4(vec3(col * texture(AmbientMap, texCoord).r), col.w);}col += color;if (RoughnessMapIsActive){rough = texture(RoughnessMap, texCoord).r;}else{rough = roughness;}vec3 n = normalize(normal);if (NormalMapIsActive){n = normalize(normal + (texture(NormalMap, texCoord).rgb * 2.f - 1.f));}actualNormal = n;vec4 FragColor = vec4(0,0,0,0);Albedo = min(max(col, vec4(0,0,0,0)), vec4(1,1,1,1));Normal = vec4((actualNormal + vec3(1,1,1)) / vec3(2,2,2),1);Position = vec4(currentPos,1);Roughness = vec4(rough,0,0,1);gl_FragDepth = gl_FragCoord.z;}")

//the default 2D vertex shader, it is boud automaticaly to every 2D shape
#define GLGE_DEFAULT_2D_VERTEX std::string("#version 300 es\nlayout (location = 0) in vec2 pos;layout (location = 1) in vec4 color;layout (location = 2) in vec2 texcoord;uniform mat3 camMat;out vec4 fColor;out vec2 fTexCoord;void main(){fColor = color;fTexCoord = texcoord;vec4 memPos = vec4(vec3(pos,1)*camMat ,1.0);memPos.z = 1.f;gl_Position = memPos;}")
//the default 2D fragment shader, bound by default to all 2D shapes
#define GLGE_DEFAULT_2D_FRAGMENT std::string("#version 300 es\nprecision mediump float;out vec4 FragColor;in vec4 fColor;in vec2 fTexCoord;uniform sampler2D Texture;void main(){FragColor = texture(Texture, fTexCoord) + fColor;}")

//the default post processing vertex shader, changes nothing and pases the texture coordinates
#define GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER std::string("#version 300 es\nlayout (location = 0) in vec2 inPos;layout (location = 1) in vec2 inTexCoord;out vec2 texCoords;void main(){gl_Position = vec4(inPos.x, inPos.y, 0, 1);texCoords = inTexCoord;}")
//the default post processing fragment shader, just draws the processed data to the screen
#define GLGE_DEFAULT_POST_PROCESSING_FRAGMENT_SHADER std::string("#version 300 es\nprecision mediump float;out vec4 FragColor;in vec2 texCoords;uniform sampler2D glgeMainImage;void main(){FragColor = texture(glgeMainImage, texCoords);}")

//define the default lighting shader
#define GLGE_DEFAULT_LIGHTING_SHADER std::string("#version 300 es\n#define PI 3.14159265\nprecision highp float;out vec4 FragColor;in vec2 texCoords;uniform sampler2D glgeAlbedoMap;uniform sampler2D glgeNormalMap;uniform sampler2D glgePositionMap;uniform sampler2D glgeRoughnessMap;uniform vec3 glgeLightColor[255];uniform float glgeLightInt[255];uniform vec3 glgeLightPos[255];uniform int glgeActiveLights;uniform vec3 glgeCameraPos;uniform float glgeFarPlane;float ambient = 0.1;float gamma = 2.2f;float exposure = 1.f;int iteration = 0;float biasAngle = 0.0005;vec3 actualNormal = vec3(0,0,0);float rough;float lightRadius = 0.1;float a = 0.f;float b = 0.f;vec4 color;vec3 normal;vec3 pos;float roughness;float metallic = 0.f;float DistributionGGX(vec3 N, vec3 H, float roughness){float a      = roughness*roughness;float a2     = a*a;float NdotH  = max(dot(N, H), 0.0);float NdotH2 = NdotH*NdotH;float num   = a2;float denom = (NdotH2 * (a2 - 1.0) + 1.0);denom = PI * denom * denom;return num / denom;}float GeometrySchlickGGX(float NdotV, float roughness){float r = (roughness + 1.0);float k = (r*r) / 8.0;float num   = NdotV;float denom = NdotV * (1.0 - k) + k;return num / denom;}float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){float NdotV = max(dot(N, V), 0.0);float NdotL = max(dot(N, L), 0.0);float ggx2  = GeometrySchlickGGX(NdotV, roughness);float ggx1  = GeometrySchlickGGX(NdotL, roughness);return ggx1 * ggx2;}vec3 fresnelSchlick(float cosTheta, vec3 F0){return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);}  vec3 calculateLightingPBR(vec3 col){vec3 N = normalize(normal);vec3 V = normalize(glgeCameraPos - pos);vec3 F0 = vec3(0.04); F0 = mix(F0, col, metallic);vec3 Lo = vec3(0.0);for(int i = 0; i < glgeActiveLights; ++i) {vec3 L = normalize(glgeLightPos[i] - pos);vec3 H = normalize(V + L);float distance    = length(glgeLightPos[i] - pos);float attenuation = 1.0 / (distance * distance);vec3 radiance     = glgeLightColor[i] * attenuation;        float NDF = DistributionGGX(N, H, roughness);        float G   = GeometrySmith(N, V, L, roughness);      vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       vec3 kS = F;vec3 kD = vec3(1.0) - kS;kD *= 1.0 - metallic;	  vec3 numerator = NDF * G * F;float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;vec3 specular = numerator / denominator;  float NdotL = max(dot(N, L), 0.0);Lo += (kD * col / PI + specular) * radiance * NdotL; }vec3 ambient = vec3(0.03) * col * ambient;vec3 color = ambient + Lo;color = color / (color + vec3(1.0));return (pow(color, vec3(1.0/2.2)));}vec4 calculateLighting(vec4 color, vec3 normal, vec3 pos, float roughness){vec3 col = color.rgb;vec3 lightDir = normalize(glgeLightPos[0] - pos);float diff = max(dot(normal.xyz, lightDir), 0.0);col *= diff;return vec4(col, color.w);}void main(){vec4 col = vec4(0,0,0,0);color = texture(glgeAlbedoMap, texCoords);normal = (texture(glgeNormalMap, texCoords).xyz - vec3(0.5,0.5,0.5)) * vec3(2,2,2);pos = texture(glgePositionMap, texCoords).xyz;roughness  = texture(glgeRoughnessMap, texCoords).r;if (normal == vec3(-5,-5,-5) || (glgeActiveLights == 0)){FragColor = color;}else{FragColor = vec4(calculateLightingPBR(color.rgb), color.w);FragColor = vec4(vec3(1.f) - exp(-vec3(FragColor) * exposure),FragColor.w);}}")

//define the wrap modes for the textures

//sample the closest pixels
#define GLGE_NEAREST 0x2600

//sample an interpolation of the clostest pixels
#define GLGE_LINEAR 0x2601

//a list of all keys

//the keycode for the a key in glge
#define GLGE_KEY_A "a"
//the keycode for the b key in glge
#define GLGE_KEY_B "b"
//the keycode for the c key in glge
#define GLGE_KEY_C "c"
//the keycode for the d key in glge
#define GLGE_KEY_D "d"
//the keycode for the e key in glge
#define GLGE_KEY_E "e"
//the keycode for the f key in glge
#define GLGE_KEY_F "f"
//the keycode for the g key in glge
#define GLGE_KEY_G "g"
//the keycode for the h key in glge
#define GLGE_KEY_H "h"
//the keycode for the i key in glge
#define GLGE_KEY_I "i"
//the keycode for the j key in glge
#define GLGE_KEY_J "j"
//the keycode for the k key in glge
#define GLGE_KEY_K "k"
//the keycode for the l key in glge
#define GLGE_KEY_L "l"
//the keycode for the m key in glge
#define GLGE_KEY_M "m"
//the keycode for the n key in glge
#define GLGE_KEY_n "n"
//the keycode for the o key in glge
#define GLGE_KEY_O "o"
//the keycode for the p key in glge
#define GLGE_KEY_P "p"
//the keycode for the q key in glge
#define GLGE_KEY_Q "q"
//the keycode for the r key in glge
#define GLGE_KEY_R "r"
//the keycode for the s key in glge
#define GLGE_KEY_S "s"
//the keycode for the t key in glge
#define GLGE_KEY_T "t"
//the keycode for the u key in glge
#define GLGE_KEY_U "u"
//the keycode for the v key in glge
#define GLGE_KEY_V "v"
//the keycode for the w key in glge
#define GLGE_KEY_W "w"
//the keycode for the x key in glge
#define GLGE_KEY_X "x"
//the keycode for the y key in glge
#define GLGE_KEY_Y "y"
//the keycode for the z key in glge
#define GLGE_KEY_Z "z"

#endif