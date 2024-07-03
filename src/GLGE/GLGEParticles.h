/**
 * @file GLGEParticles.h
 * @author DM8AT
 * @brief in this file, the functions for the particel system are included
 * @version 0.1
 * @date 2024-06-29
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 */

//check for double include
#ifndef _GLGE_PARTICEL_SYSTEM_H_
#define _GLGE_PARTICEL_SYSTEM_H_

//check if OpenGL or Vulkan should be used
#ifndef GLGE_USE_VULKAN
//if opengl should be used, include opengl
#include "GLGEOpenGL/openglGLGEParticles.h"
#else
//else, there is currently no vulkan implementation
#error [GLGE ERROR] the vulkan implementation is currently not there
#endif

#endif