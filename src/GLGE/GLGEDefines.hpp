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
#define GLGE_DEFAULT_LIGHTING_SHADER std::string("#version 300 es\n#define PI 3.14159265\nprecision highp float;out vec4 FragColor;in vec2 texCoords;uniform sampler2D glgeAlbedoMap;uniform sampler2D glgeNormalMap;uniform sampler2D glgePositionMap;uniform sampler2D glgeRoughnessMap;uniform vec3 glgeLightColor[255];uniform float glgeLightInt[255];uniform vec3 glgeLightPos[255];uniform int glgeActiveLights;uniform vec3 glgeCameraPos;uniform float glgeFarPlane;float ambient = 0.1;float gamma = 2.2f;float exposure = 1.f;int iteration = 0;float biasAngle = 0.0005;vec3 actualNormal = vec3(0,0,0);vec4 lightCol[] = vec4[](vec4(1,1,1,1));vec3 lightPos[] = vec3[](vec3(2,5,0));float lightInt[] = float[](250.f);float rough;float lightRadius = 0.1;float a = 0.f;float b = 0.f;vec4 color;vec3 normal;vec3 pos;float roughness;vec3 schlickFresnel(float vDotH){vec3 F0 = vec3(0.04f);vec3 ret = F0 + (1.f - F0) * pow(clamp(1.f - vDotH, 0.f, 1.f), 5.f);return ret;}float geomSmith(float dp){float k = (rough + 1.f) * (rough + 1.f) / 8.f;float denom = dp * (1.f - k) + k;return dp / denom;}float ggxDistribution(float nDotH){float alpha2 = rough * rough * rough * rough;float d = nDotH * nDotH * (alpha2 - 1.f) + 1.f;float ggxdistrib = alpha2 / (PI * d * d);return ggxdistrib;}vec4 calculatePBR(vec4 col){float shadow = 1.f;vec3 lightIntensity = glgeLightColor[iteration] * glgeLightInt[iteration];vec3 l = vec3(0.f);l = glgeLightPos[iteration] - pos;float lightToPixelDist = length(l);l = normalize(l);lightIntensity /= (lightToPixelDist * lightToPixelDist);vec3 n = normalize(normal);vec3 FragNormal = n;vec3 v = normalize((glgeCameraPos) - pos);vec3 h = normalize(v+l);float nDotH = max(dot(n, h), 0.f);float vDotH = max(dot(v, h), 0.f);float nDotL = max(dot(n, l), 0.f);float nDotV = max(dot(n, v), 0.f);vec3 F = schlickFresnel(vDotH);vec3 ks = F;vec3 kd = 1.f - ks;vec3 SpecBRDFnom = ggxDistribution(nDotH) * F * geomSmith(nDotL) * geomSmith(nDotV);float SpecBRDFdenom = 4.f * nDotV * nDotL + 0.0001;float SpecBRDF = float(SpecBRDFnom) / SpecBRDFdenom;vec3 fLambert = vec3(0.f);fLambert = vec3(col);vec3 DiffuseBRDF = kd * fLambert / PI;vec3 FinalColor = ((DiffuseBRDF + SpecBRDF) * lightIntensity * vec3(shadow)) * nDotL;return vec4(FinalColor, col.w);}\nvec4 calculateLightingPBR(vec4 col){vec4 totalLight;for (int i = 0; i < min(int(glgeActiveLights), 255); i++){iteration = i;totalLight += calculatePBR(col);totalLight = totalLight / (totalLight + vec4(1.f));}totalLight /= float(glgeActiveLights);vec4 finalLight = vec4(pow(vec3(totalLight), vec3(1.0/2.2)), col.w);return finalLight;}\nvoid main(){vec4 col = vec4(0,0,0,0);color = texture(glgeAlbedoMap, texCoords);normal = (texture(glgeNormalMap, texCoords).xyz - vec3(0.5,0.5,0.5)) * vec3(2,2,2);pos = texture(glgePositionMap, texCoords).xyz;roughness = texture(glgeRoughnessMap, texCoords).r;if (normal == vec3(-5,-5,-5) || (glgeActiveLights == 0)){FragColor = color;}else{FragColor = calculateLightingPBR(color);}FragColor = vec4(vec3(1.f) - exp(-vec3(FragColor) * exposure),FragColor.w);}")

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