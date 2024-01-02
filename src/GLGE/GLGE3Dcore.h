/**
 * @file GLGE3Dcore.h
 * @author DM8AT
 * @brief this is the 3D core for GLGE. It is used to create 3D graphics and such things. DEPENDECYS: GLGE.h
 * @version 0.1
 * @date 2022-12-23
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

//check for double include 
#ifndef _GLGE_3D_CORE_H_
#define _GLGE_3D_CORE_H_

//check if OpenGL or Vulkan should be used
#ifndef GLGE_USE_VULKAN
//if opengl should be used, include opengl
#include "GLGEOpenGL/openglGLGE3Dcore.h"
#else
//else, there is currently no vulkan implementation
#error [GLGE ERROR] the vulkan implementation is currently not there
#endif

#endif
