/**
 * @file glgeComputeShader.h
 * @author DM8AT
 * @brief compute shaders allow for fast and paralell execution of arbitary code on the GPU
 * @version 0.1
 * @date 2024-02-27
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */


#ifndef _GLGE_COMPUTE_SHADER_H_
#define _GLGE_COMPUTE_SHADER_H_

//check if OpenGL or Vulkan should be used
#ifndef GLGE_USE_VULKAN
//if opengl should be used, include opengl
#include "GLGEOpenGL/openglGLGEComputeShader.hpp"
#else
//else, there is currently no vulkan implementation
#error [GLGE ERROR] the vulkan implementation is currently not there
#endif


#endif