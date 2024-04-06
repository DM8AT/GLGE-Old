/**
 * @file openglGLGEVars.cpp
 * @author DM8AT
 * @brief initalise the opengl dependand behind-the-scenes variables of GLGE
 * @version 0.1
 * @date 2024-01-02
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#include "openglGLGEVars.hpp"

//store the active color buffers
GLenum glgeUsedColorBuffers[] = {GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, 
                                 GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, 
                                 GL_COLOR_ATTACHMENT7};

//store the length of the unsigned buffer array
unsigned int glgeLenUsedColorBuffers = sizeof(glgeUsedColorBuffers) / sizeof(glgeUsedColorBuffers[0]);

//store all the color buffers
GLenum glgeAllUsedColorBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, 
                                    GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, 
                                    GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7};

//store the length of all used color buffers
unsigned int glgeLenAllUsedColorBuffers = sizeof(glgeAllUsedColorBuffers) / sizeof(glgeAllUsedColorBuffers[0]);

//store how to sample images
GLenum glgeInterpolationMode = GL_NEAREST;

//store the window vector
std::vector<Window*> glgeWindows = {};

//store the current texture unit
unsigned int glgeTextureUnit = 0;