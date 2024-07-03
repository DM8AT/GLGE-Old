/**
 * @file GLGEDefines.h
 * @author DM8AT
 * @brief this file stores all public defines for GLGE
 * @version 0.1
 * @date 2022-12-23
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

//check for double include
#ifndef _GLGE_DEFINES_OGL_H_
#define _GLGE_DEFINES_OGL_H_

//if this is included, std::strings are needed
#include <string>

/**
 * a fragment shader that basicaly dose nothing
 */
#define GLGE_EMPTY_FRAGMENT_SHADER std::string("#version 450 core\nprecision mediump float;out vec4 FragColor;in vec4 color;void main(){FragColor = color;}")

/**
 * a vertex shader that doese nothing
 */
#define GLGE_EMPTY_VERTEX_SHADER std::string("#version 450 core\nlayout (location = 0) in vec2 inPos;layout (location = 1) in vec2 inTexCoord;out vec2 texCoords;void main(){gl_Position = vec4(inPos.x, inPos.y, 0, 1);texCoords = inTexCoord;}")

//the default 3D vertex shader, it is bound by default to any 3D object
#define GLGE_DEFAULT_3D_VERTEX std::string("#version 450 core\nlayout (location = 0) in vec3 pos;layout (location = 1) in vec4 vColor;layout (location = 2) in vec2 vTexcoord;layout (location = 3) in vec3 vNormal;layout (location = 4) in vec3 vTangent;\n#include <glgeObject>\n\n#include <glgeCamera>\nlayout (std140, binding = 4) buffer glgeModelMatrices{mat4 glgeModelMatrix[];};uniform mat4 glgeLightSpaceMat[16];uniform int glgeActiveShadowCasters;out vec4 color;out vec2 texCoord;out vec3 normal;out vec3 fragPos;out vec3 vPos;flat out int activeShadowCasters;layout(location = 0) out vec4 lightPos[16];void main(){color = vColor;texCoord = vTexcoord;fragPos = (vec4(pos, 1) * glgeModelMat).xyz;normal = normalize(vec4(vNormal, 1) * glgeRotMat).xyz;gl_Position = vec4(fragPos,1)*glgeCamMat;vPos = pos;glgeModelMatrix[glgeObjectUUID] = glgeModelMat;}")
//the default 3D fragment shader, it is bound by default to any 3D object
#define GLGE_DEFAULT_3D_FRAGMENT std::string("#version 450 core\nprecision highp float;layout(location = 0) out vec4 Albedo;layout(location = 1) out vec4 Normal;layout(location = 2) out vec4 Position;layout(location = 3) out vec4 Roughness;layout(location = 5) out vec4 DepthAndAlpha;\n#include <glgeObject>\n#include <glgeCamera>\nin vec4 color;in vec2 texCoord;in vec3 normal;in vec3 tangent;in vec3 bitangent;in vec3 fragPos;in vec3 vPos;\n#include <glgeMaterial>\nstruct FragmentData{vec4 color;vec3 pos;vec3 normal;vec2 uv;};FragmentData parallaxMapping(mat3 TBN){FragmentData f = FragmentData(color, fragPos, normal, texCoord);if (!bool(glgeDisplacementMapActive)) { return f; }vec3 viewDir = normalize((glgeCameraPos*TBN) - (fragPos*TBN));float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));float layerDepth = 1.f / numLayers;float currentLayerDepth = 0.0;vec2 P = viewDir.xy * dispStrength; vec2 deltaTexCoords = P * layerDepth;vec2 currentTexCoords = f.uv;float currentDepthMapValue = texture(glgeDisplacementMap, currentTexCoords).r;while(currentLayerDepth < currentDepthMapValue){currentTexCoords -= deltaTexCoords;currentDepthMapValue = texture(glgeDisplacementMap, currentTexCoords).r;  currentLayerDepth += layerDepth;}float depthStep = -layerDepth / 2.f;for (int i = 0; i < binarySteps; i++){currentTexCoords -= P * depthStep;currentLayerDepth += depthStep;currentDepthMapValue = texture(glgeDisplacementMap, currentTexCoords).r;depthStep /= 2.f * sign(currentLayerDepth - currentDepthMapValue);}f.pos += currentLayerDepth*f.normal;f.uv = currentTexCoords;if (f.uv.x > 1.0 || f.uv.y > 1.0 || f.uv.x < 0.0 || f.uv.y < 0.0) { discard; }return f;}void main(){vec3 Q1 = dFdx(fragPos);vec3 Q2 = dFdy(fragPos);vec2 st1 = dFdx(texCoord);vec2 st2 = dFdy(texCoord);vec3 tangent = normalize(Q1*st2.t - Q2*st1.t);vec3 bitangent = normalize(-Q1*st2.s + Q2*st1.s);mat3 TBN = mat3(tangent,bitangent,normal);FragmentData frag = parallaxMapping(TBN);vec4 col = glgeColor + frag.color;col.rgb *= (1-int(glgeAmbientMapActive));col.rgb += texture(glgeAmbientMap, frag.uv).rgb * int(glgeAmbientMapActive);if(col.w < 0.5){discard;}Q1 = dFdx(frag.pos);Q2 = dFdy(frag.pos);st1 = dFdx(frag.uv);st2 = dFdy(frag.uv);tangent = normalize(Q1*st2.t - Q2*st1.t);bitangent = normalize(-Q1*st2.s + Q2*st1.s);vec3 n = mix(vec3(0,0,1),normalize(texture(glgeNormalMap,frag.uv).rgb * 2.f - 1.f),float(glgeNormalMapActive));n = normalize(TBN * n);float rough = texture(glgeRoughnessMap, frag.uv).r * float(int(glgeRoughnessMapActive));rough += glgeRoughness * (1.f - float(int(glgeRoughnessMapActive)));Albedo = vec4(col.rgb, vPos.x);Normal = vec4(n, vPos.y);Position = vec4(frag.pos, vPos.z);Roughness = vec4(rough,glgeMetalic,int(glgeLit),1);DepthAndAlpha = vec4(gl_FragCoord.z,glgeObjectUUID,0,1);}")

//the default 2D vertex shader, it is boud automaticaly to every 2D shape
#define GLGE_DEFAULT_2D_VERTEX std::string("#version 450 core\nlayout (location = 0) in vec2 pos;layout (location = 1) in vec4 color;layout (location = 2) in vec2 texcoord;uniform mat3 camMat;out vec4 fColor;out vec2 fTexCoord;void main(){fColor = color;fTexCoord = texcoord;vec4 memPos = vec4(vec3(pos,1)*camMat ,1.0);memPos.z = 1.f;gl_Position = memPos;}")
//the default 2D fragment shader, bound by default to all 2D shapes
#define GLGE_DEFAULT_2D_FRAGMENT std::string("#version 450 core\nprecision mediump float;out vec4 FragColor;in vec4 fColor;in vec2 fTexCoord;uniform sampler2D Texture;void main(){vec4 col = texture(Texture, fTexCoord);col.rgb += fColor.rgb;col.w *= fColor.w;FragColor = col;}")

//the default post processing vertex shader, changes nothing and pases the texture coordinates
#define GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER std::string("#version 450 core\nlayout (location = 0) in vec2 inPos;layout (location = 1) in vec2 inTexCoord;out vec2 texCoords;void main(){gl_Position = vec4(inPos.x, inPos.y, 0, 1);texCoords = inTexCoord;}")
//the default post processing fragment shader, just draws the processed data to the screen
#define GLGE_DEFAULT_POST_PROCESSING_FRAGMENT_SHADER std::string("#version 450 core\nprecision highp float;uniform sampler2D glgeMainImage;uniform sampler2D glgeRoughnessMap;in vec4 color;in vec2 texCoords;out vec4 FragColor;const float gamma = 2.2f;const float exposure = 1.0f;void main(){vec3 col = texture(glgeMainImage, texCoords).rgb;vec3 mapped = col;if (int(texture(glgeRoughnessMap, texCoords).b) == 1){mapped = vec3(1.0) - exp(-mapped * exposure);mapped = pow(mapped, vec3(1.0 / gamma));}FragColor = vec4(mapped, 1.f);}")

//the default shader to draw an image to the screen
#define GLGE_DEFAULT_IMAGE_FRAGMENT_SHADER std::string("#version 450 core\nprecision mediump float;uniform sampler2D image;in vec2 texCoords;layout (location = 0) out vec4 FragColor;void main(){FragColor = texture(image, texCoords);}")

//define the default lighting shader
#define GLGE_DEFAULT_LIGHTING_SHADER std::string("#version 450 core\n#define PI 3.14159265\nprecision highp float;layout(location = 4) out vec4 FragColor;layout(location = 6) out vec4 LightData;in vec2 texCoords;uniform sampler2D glgeAlbedoMap;uniform sampler2D glgeNormalMap;uniform sampler2D glgePositionMap;uniform sampler2D glgeRoughnessMap;\n#include <glgeCamera>\n#include <glgeLightEvaluationFunction>\n#include <glgeLights>\nvoid main(){vec4 aMap = texture(glgeAlbedoMap,   texCoords);vec4 nMap = texture(glgeNormalMap,   texCoords);vec4 pMap = texture(glgePositionMap, texCoords);vec3 vPos = vec3(aMap.w,nMap.w,pMap.w);vec3 tmp = texture(glgeRoughnessMap, texCoords).rgb;if ((glgeActiveLights == 0) || (tmp.z == 0.f)){FragColor = vec4(aMap.rgb, 1);return;}glgePixel d = glgePixel(aMap.rgb,pMap.rgb,normalize(nMap.rgb),tmp.x, tmp.y);FragColor = vec4(0,0,0,1);for (int i = 0; i < glgeActiveLights; i++){vec3 light = glgeEvaluateLight(glgeLights[i], d);FragColor.xyz += light;}}")

//define the Vertex shader for the Skybox
#define GLGE_DEFAULT_VERTEX_SKYBOX std::string("#version 450 core\nlayout (location = 0) in vec3 pos;uniform mat4 glgeProject;uniform mat4 glgeRot;out vec4 color;out vec3 texCoords;void main(){texCoords = pos;vec4 p = vec4(pos,1)*(glgeRot*glgeProject);gl_Position = p.xyww;}")

//define the fragment shader for the Skybox
#define GLGE_DEFAULT_FRAGMENT_SKYBOX std::string("#version 450 core\nprecision mediump float;uniform samplerCube glgeSkybox;in vec3 texCoords;layout(location = 0) out vec4 Albedo;layout(location = 1) out vec4 Normal;layout(location = 2) out vec4 Position;layout(location = 3) out vec4 Roughness;layout(location = 6) out vec4 Lit;void main(){Albedo = texture(glgeSkybox, texCoords);Normal = vec4(0,0,0,1);Position = vec4(0,0,0,1);Roughness = vec4(0,0,0,1);Lit = Albedo;gl_FragDepth = -1.f;}")

//define the default transparent shader
#define GLGE_DEFAULT_TRANSPARENT_SHADER std::string("#version 450 core\n#define PI 3.14159265\nprecision highp float;layout(location = 5) out vec4 Max;layout(location = 7) out vec4 Col;layout(location = 3) out vec4 rml;uniform sampler2D glgeLitMap;in vec4 color;in vec2 texCoord;in vec3 normal;in vec3 fragPos;\n#include <glgeObject>\n#include <glgeCamera>\n#include <glgeMaterial>\nuniform bool glgePass;float rough = 0.f;int iteration = 0;vec3 actualNormal = vec3(0,0,0);float lightRadius = 0.1;float a = 0.f;float b = 0.f;vec3 pos;float metallic = 0.f;#include <glgeLightEvaluationFunction>\n#include <glgeLights>\nvoid main(){vec4 col = glgeColor + color;col.rgb *= (1-int(glgeAmbientMapActive));col.rgb += texture(glgeAmbientMap, texCoord).rgb * int(glgeAmbientMapActive);if(col.w == 0.f){discard;}if (!glgePass && col.w!=1.f){discard;}if (glgePass && col.w==1.f){discard;}float rough = texture(glgeRoughnessMap, texCoord).r * float(int(glgeRoughnessMapActive));rough += glgeRoughness * (1.f - float(int(glgeRoughnessMapActive)));actualNormal = normal;pos = fragPos;vec4 finColor = col;if (bool(glgeLit)){finColor.rgb = vec3(0);glgePixel d = glgePixel(col.rgb,fragPos,normal,rough, metallic);for (int i = 0; i < glgeActiveLights; i++){vec3 light = glgeEvaluateLight(glgeLights[i], d);finColor.xyz += light;}}Col = finColor;Max = vec4(0,0,1,1);rml.rgb = vec3(rough, glgeMetalic, float(glgeLit));}")

//define the default transparent combine shader
#define GLGE_DEFAULT_TRANSPARENT_COMBINE_SHADER std::string("#version 450 core\nprecision mediump float;layout(location = 4) out vec4 FragColor;uniform sampler2D glgeTranspAccumTexture;uniform sampler2D glgeTranspCountTexture;void main(){vec4 accum = texelFetch(glgeTranspAccumTexture, ivec2(gl_FragCoord.xy), 0);float n = max(1.0, texelFetch(glgeTranspCountTexture, ivec2(gl_FragCoord.xy), 0).b);FragColor = vec4(accum.rgb / max(accum.a, 0.0001), pow(max(0.0, 1.0 - accum.a / n), n));}")

//define the default vertex shader for particles
#define GLGE_DEFAULT_3D_PARTICLE_VERTEX_SHADER std::string("#version 450 core\nlayout (location = 0) in vec3 pos;layout (location = 1) in vec4 vColor;layout (location = 2) in vec2 vTexcoord;layout (location = 3) in vec3 vNormal;layout (location = 4) in vec3 vTangent;\n#include <glgeObject>\n\n#include <glgeCamera>\nout vec4 color;out vec2 texCoord;out vec3 normal;out vec3 fragPos;out vec3 vPos;\n#include <glgeParticles>\nvoid main(){    BuffParticle own = glgeParticles[gl_InstanceID];    mat4 mMat = own.modelMat * glgeModelMat;    color = vColor;    texCoord = vTexcoord;    fragPos = (vec4(pos, 1) * mMat).xyz;    normal = normalize(vec4(vNormal, 1) * glgeRotMat * own.rotMat).xyz;    gl_Position = vec4(fragPos,1)*glgeCamMat;    vPos = pos;}")

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

//define the OS identifier

//define the identifier for an undifined and unsuported OS
#define GLGE_OS_OTHER 0
//define the identifier for Linux
#define GLGE_OS_LINUX 1
//define the identifier for Windows
#define GLGE_OS_WINDOWS 2

#endif