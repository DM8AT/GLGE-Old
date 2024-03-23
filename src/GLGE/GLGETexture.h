/**
 * @file glgeTexture.h
 * @author DM8AT
 * @brief a simple class to handle textures
 * @version 0.1
 * @date 2024-02-27
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */


#ifndef _GLGE_TEXTURE_H_
#define _GLGE_TEXTURE_H_

//check if OpenGL or Vulkan should be used
#ifndef GLGE_USE_VULKAN
//if opengl should be used, include opengl
#include "GLGEOpenGL/openglGLGETexture.hpp"
#else
//else, there is currently no vulkan implementation
#error [GLGE ERROR] the vulkan implementation is currently not there
#endif


#endif