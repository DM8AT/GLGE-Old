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
#include <string>
#include <iostream>
#include "glgeVars.hpp"

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

/**
 * a fragment shader that basicaly dose nothing
 */
#define GLGE_EMPTY_FRAGMENT_SHADER std::string("#version 300 es\nprecision mediump float;out vec4 FragColor;in vec4 color;void main(){FragColor = color;}")

/**
 * a vertex shader that doese nothing
 */
#define GLGE_EMPTY_VERTEX_SHADER std::string("#version 300 es\nlayout (location = 0) in vec2 inPos;layout (location = 1) in vec2 inTexCoord;out vec2 texCoords;void main(){gl_Position = vec4(inPos.x, inPos.y, 0, 1);texCoords = inTexCoord;}")

/**
 * @brief define the string that is shown in the throw error string
 * 
 */
#define GLGE_THROW_ERROR_STRING "[GLGE ERROR THROWN] See above for more information or enable error output"

/**
 * @brief throw an error using GLGE
 * 
 */
#define GLGE_THROW_ERROR(msg) if(glgeErrorOutput && !glgeExitOnError){std::cerr << (std::string("[GLGE ERROR] ") + std::string(msg)).c_str();} else if (glgeExitOnError){std::__throw_runtime_error((std::string("[GLGE ERROR] ") + std::string(msg)).c_str());}

#endif