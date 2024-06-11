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

/**
 * @brief store the allowed preset strings
 */
const char* glgePresets[] = {
                             "CUBE"
                            };

/////////////////////
//Private Variables//
/////////////////////

/**
 * @brief store the display mode
 */
SDL_DisplayMode glgeMainDisplay;

/**
 * @brief store the current window index
 */
int glgeCurrentWindowIndex = 0;

/**
 * @brief store the amount of active glge windows
 */
unsigned int glgeActiveWindows = 0;

/**
 * @brief should error be outputed?
 */
bool glgeErrorOutput = true;

/**
 * @brief should glge throw an error and crash if an normal error occures
 */
bool glgeExitOnError = true;

/**
 * @brief should warnings be outputed?
 */
bool glgeWarningOutput = true;

/**
 * @brief is an main window created?
 */
bool glgeHasMainWindow = false;

/**
 * @brief the maximal frames per second the window should run on
 */
int glgeMaxFPS = 60;

/**
 * @brief the current frames per second the window is running on
 */
int glgeCurrentFPS = 0;

/**
 * @brief the time sinse the last update
 */
float glgeDeltaTime = 0.f;

/**
 * @brief the time the last tick started at
 */
float glgeTickTime = 0.f;

/**
 * @brief store the name for the camera matrix
 */
char* glgeCamMatrix = (char*)"camMat";

/**
 * @brief store the keys
 */
Keys glgePressedKeys = Keys();

/**
 * @brief store the keys that where pressed for one frame
 */
Keys glgeKeysThisTick = Keys();

/**
 * @brief store the keys that where relesd for one frame
 */
Keys glgeKeysRelesdThisTick = Keys();

/**
 * @brief store the mouse information
 */
Mouse glgeMouse = Mouse();

/**
 * @brief store the opperating system the application is compiled for
 */
int glgeOperatingSystem = -1;

/**
 * @brief store if the current draw pass is a transparent or an opaque pass
 */
bool glgeTransparentOpaquePass = false;

/**
 * @brief store how many objects have been created
 */
unsigned int glgeObjectUUID = 1;

/**
 * @brief store the window flags GLGE is using
 */
SDL_WindowFlags glgeWindowFlags = SDL_WindowFlags(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

/**
 * @brief store the window id
 */
unsigned int glgeWindowID = 0;

/**
 * @brief store the index of the main window in the window array
 */
unsigned int glgeMainWindowIndex = 0;

/**
 * @brief store if GLEW was allready initalised
 */
bool glgeInitalisedGLEW = false;

/**
 * @brief store if GLGE should exit if the main window is closed
 */
bool glgeExitOnMainWindowClose = true;

/**
 * @brief store the conversion from SDL window index space to GLGE window index space
 */
int glgeWindowIndexOffset = -1;

/**
 * @brief store the GLGE include defaults
 */
std::unordered_map<std::string, std::string> glgeIncludeDefaults;

/**
 * @brief store whatever was types this tick
 */
std::string glgeTypedThisTick = "";

/**
 * @brief specefy if the current pass is the shadow pass
 */
bool glgeShadowPass = false;

/**
 * @brief store the index of the current shadow caster
 */
int glgeShadowCasterIndex = 0;