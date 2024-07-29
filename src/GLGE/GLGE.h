/**
 * @file GLGE.h
 * @author DM8AT
 * @brief A simple 3D game development engine
 * 
 * This is an module designed to make 3D game developement easyer. It is build on top of glew and freeglut, so if you want to develope using GLGE, make shute
 * you have glew and freeglut installed. 
 * To make developement easyer, it is inspired by the structure of the Unity game engine, but for C++ and without an viewport to drag the objects around. 
 * 
 * DEPENDENCIS:
 * - Freeglut
 * - Glew
 * - OpenGL
 * - CML (Designed by me, it is allready included)
 * 
 * ARGUEMTS:
 * - GLEW: -lGLEW
 * - SDL: -lSDL2main -lSDL2
 * - OPENGL: -lGL
 * - CML: "{PathToSurcecode}\CML\*.cpp"
 * 
 * @version 0.1
 * @date 2022-12-17
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

//check if GLGE is allready included
#ifndef _GLGE_H_
//if not, say that it is now included
#define _GLGE_H_

//check if OpenGL or Vulkan should be used
#ifndef GLGE_USE_VULKAN
//if opengl should be used, include opengl
#include "GLGEOpenGL/openglGLGE.h"
#else
//else, include vulkan
#include "GLGEVulkan/vulkanGLGE.h"
#endif

#endif