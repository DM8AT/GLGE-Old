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
#define GLGE_DEFAULT_3D_FRAGMENT std::string("#version 300 es\nprecision mediump float;out vec4 FragColor;in vec4 color;in vec2 texCoord;uniform sampler2D Texture;void main(){FragColor = texture(Texture, texCoord) + color;}")

//the default 2D vertex shader, it is boud automaticaly to every 2D shape
#define GLGE_DEFAULT_2D_VERTEX std::string("#version 300 es\nlayout (location = 0) in vec2 pos;layout (location = 1) in vec4 color;layout (location = 2) in vec2 texcoord;uniform mat3 camMat;out vec4 fColor;out vec2 fTexCoord;void main(){fColor = color;fTexCoord = texcoord;vec4 memPos = vec4(vec3(pos,1)*camMat ,1.0);memPos.z = 1.f;gl_Position = memPos;}")
//the default 2D fragment shader, bound by default to all 2D shapes
#define GLGE_DEFAULT_2D_FRAGMENT std::string("#version 300 es\nprecision mediump float;out vec4 FragColor;in vec4 fColor;in vec2 fTexCoord;uniform sampler2D Texture;void main(){FragColor = texture(Texture, fTexCoord) + fColor;}")

//the default post processing vertex shader, changes nothing and pases the texture coordinates
#define GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER std::string("#version 300 es\nlayout (location = 0) in vec2 inPos;layout (location = 1) in vec2 inTexCoord;out vec2 texCoords;void main(){gl_Position = vec4(inPos.x, inPos.y, 0, 1);texCoords = inTexCoord;}")
//the default post processing fragment shader, just draws the processed data to the screen
#define GLGE_DEFAULT_POST_PROCESSING_FRAGMENT_SHADER std::string("#version 300 es\nprecision mediump float;out vec4 FragColor;in vec2 texCoords;uniform sampler2D screenTexture;void main(){FragColor = texture(screenTexture, texCoords);}")

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