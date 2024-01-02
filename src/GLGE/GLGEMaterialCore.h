/**
 * @file GLGEMaterialCore.h
 * @author DM8AT
 * @brief this file specifies all the things that can be done with materials. 
 * 
 * Materials are what can be applied to 3D objects to make the look realistic. This is needed, because GLGE uses
 * PBR (Physically Based Rendering). 
 * This file will be included using GLGE3DCore.h
 * 
 * @version 0.1
 * @date 2023-02-23
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

//check for double includes
#ifndef _GLGE_MATERIAL_CORE_H_
#define _GLGE_MATERIAL_CORE_H_

//check if OpenGL or Vulkan should be used
#ifndef GLGE_USE_VULKAN
//if opengl should be used, include opengl
#include "GLGEOpenGL/openglGLGEMaterialCore.h"
#else
//else, there is currently no vulkan implementation
#error [GLGE ERROR] the vulkan implementation is currently not there
#endif

//end the file
#endif