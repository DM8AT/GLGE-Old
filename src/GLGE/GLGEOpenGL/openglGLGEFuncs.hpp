/**
 * @file openglGLGEFuncs.hpp
 * @author DM8AT
 * @brief declare the behind-the-scenes functions of GLGE
 * @version 0.1
 * @date 2024-01-02
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */


#ifndef _GLGE_FUNCS_OGL_H_
#define _GLGE_FUNCS_OGL_H_
//include needed defines
#include <string>

//include the CML functions
#include "../CML/CMLVec2.h"
//check if glew is allready included
#ifndef _GLGE_GLEW_
//say that glew is now included
#define _GLGE_GLEW_
//include glew
#include <GL/glew.h>
//close the if for glew
#endif

//include SDL2
#ifdef _WIN32
#include <SDL/SDL.h> /* Windows-specific SDL2 library */
#include <SDL/SDL_opengl.h>
#include <SDL2/SDL_misc.h>
#else
#include <SDL2/SDL.h> /* macOS- and GNU/Linux-specific */
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_misc.h>
#endif

//include the shaders
#include "openglGLGEShaderCore.h"
//include windows
#include "openglGLGEWindow.h"
//include the rest of the default funcs
#include "../GLGEInternal/glgeInternalFuncs.h"

/**
 * @brief Create a Window
 * 
 * behind the sceens function for GLGE
 * 
 * @param n the name of the window
 * @param s the size of the window
 * @param p the position of the window
 */
GLGEWindow* createWindow(const char* n, vec2 s, vec2 p);

/**
 * @brief convert an error code from OpenGL into an error string
 * 
 * @param error the error code
 * @return char* a string created from the error index
 */
char* getGLErrorString(unsigned int error);

/**
 * @brief Get a uniform variable in a shader
 * 
 * @param shaderProgram the program to get the variable from
 * @param name the name of the variable
 */
GLint getUniformVar(unsigned int shaderProgram, const char* name);

/**
 * @brief add a new shader object
 * 
 * @param shaderProgram the program it should be added to
 * @param shadertext the text for the shader code
 * @param shaderType the type of the shader
 */
void addShader(unsigned int shaderProgram, const char* shadertext, GLenum shaderType);

/**
 * @brief load a file to an SDL_Surface
 * 
 * @param file the path to the file to load
 * @return SDL_Surface* a pointer to an SDL_Surface
 */
SDL_Surface* loadImage(const char* file);

#endif