/**
 * @file glgeVars.hpp
 * @author DM8AT
 * @brief this file stores all behind-the-sceens variables used by GLGE. DO NOT CHANGE OUTSIDE OF GLGE!
 * @version 0.1
 * @date 2022-12-23
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

//check for double include
#ifndef _GLGE_VARS_H_
#define _GLGE_VARS_H_

//include needed dependencys
#include "GLGEKlasses.hpp"
//include the lighting core
#include "GLGELightingCore.h"
//include the shader core
#include "GLGEShaderCore.h"
//include the window core
#include "GLGEWindow.h"
//include the 3D core
#include "GLGE3Dcore.h"
//include 4D vectors
#include "CML/CMLVec4.h"
//include the vector library
#include <vector>
#include <ctime>

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

///////////
//CLASSES//
///////////

////////////////////
//Private Constans//
////////////////////

extern const char* glgePresets[];

/////////////////////
//Private Variables//
/////////////////////

//store the display mode
extern SDL_DisplayMode glgeMainDisplay;

//store the current window index
extern int glgeCurrentWindowIndex;

//store the amount of active glge windows
extern unsigned int glgeActiveWindows;

//should error be outputed?
extern bool glgeErrorOutput;

//should glge throw an error and crash if an normal error occures
extern bool glgeExitOnError;

//should warnings be outputed?
extern bool glgeWarningOutput;

//is an main window created?
extern bool glgeHasMainWindow;

//the maximal frames per second the window should run on
extern int glgeMaxFPS;

//the current frames per second the window is running on
extern int glgeCurrentFPS;

//the time sinse the last update
extern float glgeDeltaTime;

//the time the last tick started at
extern float glgeTickTime;

//store the name for the move matrix
extern char* glgeCamMatrix;

//store all pressed keys
extern Keys glgePressedKeys;

//store the keys that where pressed for one frame
extern Keys glgeKeysThisTick;

//store the keys that where relesd for one frame
extern Keys glgeKeysRelesdThisTick;

//store the mouse information
extern Mouse glgeMouse;

//store the active color buffers
extern GLenum glgeUsedColorBuffers[];

//store the length of the unsigned buffer array
extern unsigned int glgeLenUsedColorBuffers;

//store all the color buffers
extern GLenum glgeAllUsedColorBuffers[];

//store the length of all used color buffers
extern unsigned int glgeLenAllUsedColorBuffers;

//store the light world position
extern GLuint glgeLightWorldPosUniform;

//store how to sample images
extern GLenum glgeInterpolationMode;

//store the opperating system the application is compiled for
extern int glgeOperatingSystem;

//store if the current draw pass is a transparent or an opaque pass
extern bool glgeTransparentOpaquePass;

//store how many objects have been created
extern unsigned int glgeObjectUUID;

//store if the GLGE window has a border
extern bool glgeHasWindowBorder;

//say if glge should automaticaly close on window close events
extern bool glgeWindowForceOpen;

//say if the window should be closed by glge
extern bool glgeOPCloseWindow;

//say if the window is always renderd on top
extern bool glgeWindowAlwaysOnTop;

//store how bright the window is renderd
extern float glgeWindowBrightness;

//store the window flags GLGE is using
extern SDL_WindowFlags glgeWindowFlags;

//store if the window is maximised
extern bool glgeWindowMaximised;

//store if the window is shown
extern bool glgeWindowIsShown;

//store if the mouse is grabbed in the window
extern bool glgeMouseGrabMode;

//store the window vector
extern std::vector<GLGEWindow*> glgeWindows;

//store the index of the main window in the window array
extern unsigned int glgeMainWindowIndex;

//store if GLEW was allready initalised
extern bool glgeInitalisedGLEW;

//store if GLGE should exit if the main window is closed
extern bool glgeExitOnMainWindowClose;

#endif