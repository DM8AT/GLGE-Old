/**
 * @file openglGLGEVars.hpp
 * @author DM8AT
 * @brief declare the OpenGL dependand behind-the-scenes variables of GLGE
 * @version 0.1
 * @date 2024-01-02
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_VARS_OGL_H_
#define _GLGE_VARS_OGL_H_
//include the rest of the variables
#include "../GLGEIndependend/glgeVars.hpp"
//include the window core
#include "openglGLGEWindow.h"
//include the 2D core
#include "openglGLGE2Dcore.h"
//include the 3D core
#include "openglGLGE3Dcore.h"
//include light sources
#include "openglGLGELightingCore.h"
//check if glew is allready included
#ifndef _GLGE_GLEW_
//say that glew is now included
#define _GLGE_GLEW_
//include glew
#include <GL/glew.h>
//close the if for glew
#endif
//include vector
#include <vector>

/**
 * @brief store the active color buffers
 */
extern GLenum glgeUsedColorBuffers[];

/**
 * @brief store the length of the unsigned buffer array
 */
extern unsigned int glgeLenUsedColorBuffers;

/**
 * @brief store all the color buffers
 */
extern GLenum glgeAllUsedColorBuffers[];

/**
 * @brief store the length of all used color buffers
 */
extern unsigned int glgeLenAllUsedColorBuffers;

/**
 * @brief store how to sample images
 */
extern GLenum glgeInterpolationMode;

/**
 * @brief store the window vector
 */
extern std::vector<Window*> glgeWindows;

/**
 * @brief store the current texture unit
 */
extern unsigned int glgeTextureUnit;

/**
 * @brief store the current light source that is casting shadows
 */
extern Light* glgeCurrentShadowCaster;

/**
 * @brief store the type of the current framebuffer
 */
extern unsigned int glgeCurrentFramebufferType;

#endif