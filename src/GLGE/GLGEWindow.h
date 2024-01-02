/**
 * @file GLGEWindow.h
 * @author DM8AT
 * @brief this file contains a class to create a subwindow in GLGE
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_WINDOW_H_
#define _GLGE_WINDOW_H_

//check if OpenGL or Vulkan should be used
#ifndef GLGE_USE_VULKAN
//if opengl should be used, include opengl
#include "GLGEOpenGL/openglGLGEWindow.h"
#else
//else, there is currently no vulkan implementation
#error [GLGE ERROR] the vulkan implementation is currently not there
#endif

#endif