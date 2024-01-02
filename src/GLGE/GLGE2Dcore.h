/**
 * @file GLGE2Dcore.hpp
 * @author DM8AT
 * @brief the 2D core for GLGE, everything containing 2D games and GUI
 * @version 0.1
 * @date 2023-01-29
 * 
 * //////////////////////////////////////////////////////
 * // IMPORTANT: THIS FILE IS CURRENTLY NOT MAINTAINED //
 * //////////////////////////////////////////////////////
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 */

#ifndef _GLGE2DCORE_H_
#define _GLGE2DCORE_H_

//check if OpenGL or Vulkan should be used
#ifndef GLGE_USE_VULKAN
//if opengl should be used, include opengl
#include "GLGEOpenGL/openglGLGE2Dcore.h"
#else
//else, there is currently no vulkan implementation
#error [GLGE ERROR] the vulkan implementation is currently not there
#endif

#endif
