/**
 * @file GLGEShaderCore.h
 * @author DM8AT
 * @brief in this file, the functions for the shader core of the library are defined. 
 * @version 0.1
 * @date 2023-02-18
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 */

//check for double include
#ifndef _GLGE_SHADER_CORE_H_
#define _GLGE_SHADER_CORE_H_

//check if OpenGL or Vulkan should be used
#ifndef GLGE_USE_VULKAN
//if opengl should be used, include opengl
#include "GLGEOpenGL/openglGLGEShaderCore.h"
#else
//else, there is currently no vulkan implementation
#error [GLGE ERROR] the vulkan implementation is currently not there
#endif

#endif