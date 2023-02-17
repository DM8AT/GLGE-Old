/**
 * @file glgePrivDefines.h
 * @author DM8AT
 * @brief this file stores all private defines for GLGE
 * @version 0.1
 * @date 2022-12-23
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_PRIV_DEFINES_H_
#define _GLGE_PRIV_DEFINES_H_

/**
 * define PI to pi
 */
#define GLGE_PI 3.141592

/**
 * define a value to multyply with to get radians from degrees
 */
#define GLGE_TO_RADIANS 0.01745329

/**
 * define a value to multiply with to get degrees from radians
 */
#define GLGE_TO_DEGREES 57.295779513

#define GLGE_EMPTY_VERTEX_SHADER std::string("#version 300 es\nprecision mediump float;out vec4 FragColor;in vec2 texCoords;uniform sampler2D screenTexture;float dist(vec2 a, vec2 b){return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));}void main(){FragColor = texture(screenTexture, texCoords);}")

#endif