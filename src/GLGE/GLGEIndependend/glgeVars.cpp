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

//store the index of the main window in the window array
unsigned int glgeMainWindowIndex = 0;

//store if GLEW was allready initalised
bool glgeInitalisedGLEW = false;

//store if GLGE should exit if the main window is closed
bool glgeExitOnMainWindowClose = true;

//store the conversion from SDL window index space to GLGE window index space
int glgeWindowIndexOffset = -1;

//store the GLGE include defaults
std::unordered_map<std::string, std::string> glgeIncludeDefaults;

//store whatever was types this tick
std::string glgeTypedThisTick = "";

//specefy if the current pass is the shadow pass
bool glgeShadowPass = false;

//store the index of the current shadow caster
int glgeShadowCasterIndex = 0;