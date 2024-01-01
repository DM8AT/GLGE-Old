/**
 * @file glgeVars.cpp
 * @author DM8AT
 * @brief In this file, all behind-the-scenes variables for GLGE are created
 * @version 0.1
 * @date 2023-02-09
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

#include "glgeVars.hpp"
#include "GLGEShaderCore.h"
#include <map>

///////////
//CLASSES//
///////////



////////////////////
//Private Constans//
////////////////////

const char* glgePresets[] = {
                             "CUBE"
                            };

/////////////////////
//Private Variables//
/////////////////////

//store the display mode
SDL_DisplayMode glgeMainDisplay;

//store the current window index
int glgeCurrentWindowIndex = 0;

//store the amount of active glge windows
unsigned int glgeActiveWindows = 0;

//should error be outputed?
bool glgeErrorOutput = true;

//should glge throw an error and crash if an normal error occures
bool glgeExitOnError = true;

//should warnings be outputed?
bool glgeWarningOutput = true;

//is an main window created?
bool glgeHasMainWindow = false;

//the maximal frames per second the window should run on
int glgeMaxFPS = 60;

//the current frames per second the window is running on
int glgeCurrentFPS = 0;

//the time sinse the last update
float glgeDeltaTime = 0.f;

//the time the last tick started at
float glgeTickTime = 0.f;

//store the name for the camera matrix
char* glgeCamMatrix = (char*)"camMat";

//store the keys
Keys glgePressedKeys = Keys();

//store the keys that where pressed for one frame
Keys glgeKeysThisTick = Keys();

//store the keys that where relesd for one frame
Keys glgeKeysRelesdThisTick = Keys();

//store the mouse information
Mouse glgeMouse = Mouse();

//store the backface culling mode
bool glgeUseCulling = false;

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

//store the opperating system the application is compiled for
int glgeOperatingSystem = -1;

//store if the current draw pass is a transparent or an opaque pass
bool glgeTransparentOpaquePass = false;

//store how many objects have been created
unsigned int glgeObjectUUID = 1;

//store the window flags GLGE is using
SDL_WindowFlags glgeWindowFlags = SDL_WindowFlags(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

//store the window id
unsigned int glgeWindowID = 0;

//store the window vector
std::vector<GLGEWindow*> glgeWindows = {};

//store the index of the main window in the window array
unsigned int glgeMainWindowIndex = 0;

//store if GLEW was allready initalised
bool glgeInitalisedGLEW = false;

//store if GLGE should exit if the main window is closed
bool glgeExitOnMainWindowClose = true;