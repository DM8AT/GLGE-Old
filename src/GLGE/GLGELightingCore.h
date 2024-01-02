/**
 * @file GLGELightingCore.h
 * @author DM8AT
 * @brief the lighting functionality for GLGE
 * @version 0.1
 * @date 2023-02-26
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_LIGHTING_CORE_H_
#define _GLGE_LIGHTING_CORE_H_

//check if OpenGL or Vulkan should be used
#ifndef GLGE_USE_VULKAN
//if opengl should be used, include opengl
#include "GLGEOpenGL/openglGLGELightingCore.h"
#else
//else, there is currently no vulkan implementation
#error [GLGE ERROR] the vulkan implementation is currently not there
#endif


#endif