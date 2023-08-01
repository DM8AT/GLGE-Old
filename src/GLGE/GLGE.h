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
 * - FREEGLUT: -lglut
 * - OPENGL: -lGL
 * - CML: {PathToSurcecode}\CML\*.cpp
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
//say that the thinks can only be defined once
#pragma once
//include glew
#include <GL/glew.h>
//include CML
#include "CML/CML.h"

//include the public defines
#include "GLGEDefines.hpp"
#include "GLGEKlasses.hpp"
//include the math functions
#include "GLGEMath.h"

///////////
//DEFINES//
///////////

//the screen space in pixels, from -halfWidth to halfWidth on x and from -halfHeight to halfHeight on y
#define GLGE_SCREEN_SPACE 0
//relative window coordinates, from 0 to 1 on both axis
#define GLGE_WINDOW_SPACE 1

//Arrow pointing up and to the right
#define GLGE_CURSOR_STYLE_RIGHT_ARROW 0x0000
//Arrow pointing up and to the left
#define GLGE_CURSOR_STYLE_LEFT_ARROW 0x0001
//Pointing hand
#define GLGE_CURSOR_STYLE_INFO 0x0002
//Skull & cross bones
#define GLGE_CURSOR_STYLE_DESTROY 0x0003
//Question mark
#define GLGE_CURSOR_STYLE_HELP 0x0004
//Arrows rotating in a circle
#define GLGE_CURSOR_STYLE_CYCLE 0x0005
//Spray can
#define GLGE_CURSOR_STYLE_SPRAY 0x0006
//Wrist watch
#define GLGE_CURSOR_STYLE_WAIT 0x0007
//Insertion point cursor for text
#define GLGE_CURSOR_STYLE_TEXT 0x0008
//Simple cross-hair
#define GLGE_CURSOR_STYLE_CROSSHAIR 0x0009
//Bi-directional pointing up & down
#define GLGE_CURSOR_STYLE_UP_DOWN 0x000A
//Bi-directional pointing left & right
#define GLGE_CURSOR_STYLE_LEFT_RIGHT 0x000B
//Arrow pointing to top side
#define GLGE_CURSOR_STYLE_TOP_SIDE 0x000C
//Arrow pointing to bottom side
#define GLGE_CURSOR_STYLE_BOTTOM_SIDE 0x000D
//Arrow pointing to left side
#define GLGE_CURSOR_STYLE_LEFT_SIDE 0x000E
//Arrow pointing to right side
#define GLGE_CURSOR_STYLE_RIGHT_SIDE 0x000F
//Arrow pointing to top-left corner
#define GLGE_CURSOR_STYLE_TOP_LEFT_CORNER 0x0010
//Arrow pointing to top-right corner
#define GLGE_CURSOR_STYLE_TOP_RIGHT_CORNER 0x0011
//Arrow pointing to bottom-left corner
#define GLGE_CURSOR_STYLE_BOTTOM_RIGHT_CORNER 0x0012
//Arrow pointing to bottom-right corner
#define GLGE_CURSOR_STYLE_BOTTOM_LEFT_CORNER 0x0013
//Full-screen cross-hair cursor (if possible, otherwise GLUT_CURSOR_CROSSHAIR)
#define GLGE_CURSOR_STYLE_FULL_CROSSHAIR 0x0066
//Invisible cursor
#define GLGE_CURSOR_STYLE_NONE 0x0065
//The default curser image
#define GLGE_CURSOR_STYLE_DEFAULT 0x0064

/////////////
//FUNCTIONS//
/////////////

/**
 * @brief initalise the GLGE library, execute at start of script
 * 
 * The arguments are the input to the main function. It initalises Freeglut. 
 * 
 * @param argc number of input arguments
 * @param argv input arguemnts
 */
void glgeInit(int argc, char** argv);

/**
 * @brief create a window using GLGE
 * 
 * this function creates a window with gl context. It is requierd to initalise GLGE befor calling this function. This function will also initalise GLEW. 
 * 
 * @param name the name of the window
 * @param size the size of the window in pixels
 * @param pos the position of the window in pixels DEFAULT: 0,0
 */
void glgeCreateWindow(const char* name, vec2 size, vec2 pos = vec2(0,0));

/**
 * @brief create a window using GLGE
 * 
 * this function creates a window with gl context. It is requierd to initalise GLGE befor calling this function. This function will also initalise GLEW. 
 * 
 * @param name the name of the window
 * @param width the width of the window in pixels
 * @param height the height of the window in pixels
 * @param pos the position of the window in pixels DEFAULT: 0,0
 */
void glgeCreateWindow(const char* name, int width, int height, vec2 pos = vec2(0,0));

/**
 * @brief set the window title
 * 
 * @param title the new window title
 */
void glgeSetWindowTitle(const char* title);

/**
 * @brief say if the GLGE errors should be outputed or not
 * 
 * this function enables or disables the error output of GLGE. For a final release it is better to disable all the errors, as they may create cunfusion, but for
 * developement it is better to eneble them to debug the project. 
 * 
 * @param ErrorOutput should the errors be outputed in the console
 */
void glgeSetErrorOutput(bool ErrorOutput);

/**
 * @brief get if GLGE is outputing errors
 * 
 * this function gets the inter parameter of GLGE to see if it can print errors. 
 * 
 * @return TRUE: GLGE will print errrors to the console |
 * @return FALSE: GLGE will not print errors to the console
 */
bool glgeGetErrorOutput();

/**
 * @brief say if GLGE should print warnings to the console
 * 
 * for final builds, it is good to not print warnings, but for developement, printing warnings is better
 * 
 * @param WarningOutput should the warnings be outputed?
 */
void glgeSetWarningOutput(bool WarningOutput);

/**
 * @brief get if GLGE is printig warnings
 * 
 * @return true GLGE print any occuring warnings to the console |
 * @return false GLGE will not print warnings
 */
bool glgeGetWarningOutput();

/**
 * @brief start the mainloop of GLGE
 * 
 * it activates the mainloop function of glut
 */
void glgeRunMainLoop();

/**
 * @brief set the clear color for OpenGL
 * 
 * set the color with wich the color buffer should be cleared, basically the background color. Alpha channel is ignored in this function. 
 * 
 * @param r the amount of red in the color
 * @param g the amount of green in the color
 * @param b the amount of blue in the color
 * @param normalise if the colors should be normalised, if the colors are between 0 and 1 it should be set to false DEFAULT: false
 */
void glgeSetClearColor(float r, float g, float b, bool normalise = false);

/**
 * @brief set the clear color for OpenGL
 * 
 * set the color with wich the color buffer should be cleared, basically the background color. Alpha channel is ignored in this function. 
 * 
 * @param color the new clear color
 * @param normalise if the colors should be normalised, if the colors are between 0 and 1 it should be set to false DEFAULT: false
 */
void glgeSetClearColor(vec3 color, bool normalise = false);

/**
 * @brief set the clear color for OpenGL
 * 
 * set the color with wich the color buffer should be cleared, basically the background color. Alpha channel is ignored in this function. 
 * 
 * @param color the new clear color
 * @param normalise if the colors should be normalised, if the colors are between 0 and 1 it should be set to false DEFAULT: false
 */
void glgeSetClearColor(vec4 color, bool normalise = false);

/**
 * @brief input a function that should be called every time the window refreshes
 * 
 * the function is called betwean the clearing and refreshing of the window, this dosn't need to be done manualy
 * 
 * @param func a function pointer to the function to call
 */
void glgeBindDisplayFunc(void (*func)());

/**
 * @brief if an display function is bound, debind it
 * 
 * this function deactivates any bound display function, so no display function is going to be additionaly used
 */
void glgeClearDisplayFunc();

/**
 * @brief bind the main loop for the aplication
 * 
 * this function binds a function that is going to call on every update. it is essential the main loop of the application
 * 
 * @param func a function pointer for the main loop
 */
void glgeBindMainFunc(void (*func)());

/**
 * @brief if an main loop function is bound, clear it
 * 
 * this function clears a bound main loop function and disables all calls for it
 */
void glgeClearMainFunc();

/**
 * @brief this function sets the maximal frames per second the window can update
 * 
 * this function sets the limit for the times the window can update in one second. The end update amount can varry, even if it is limited. It can be a bit
 * bigger or smaler, depending on the strength of the computer. 
 * 
 * @param FPS the maximal frames per second
 */
void glgeSetMaxFPS(int FPS);

/**
 * @brief get the current frames per second of the window
 * 
 * this function returns the current frames per second the application window is running on. The frames are calculated only for this tick, it is no
 * value calculated over a longer time, so it can varry hardly from tick to tick
 * 
 * @return int the current frames per second
 */
int glgeGetCurrentFPS();

/**
 * @brief get the maximal frames per second the window is running on
 * 
 * this function returns the maximal frames per second the window is allowed to run on. But practically, the maximal frames per second can varry
 * by a bit. 
 * 
 * @return int the maximal FPS for the window function
 */
int glgeGetMaxFPS();

/**
 * @brief get if GLGE has a main function bound in the moment
 * 
 * @return true GLGE has and will use an additional main function |
 * @return false GLGE has no additional main function bound
 */
bool glgeHasMainFunc();

/**
 * @brief get if GLGE has an additional display function bound
 * 
 * @return true GLGE has an additional display function bound |
 * @return false GLGE has no additional display function bound
 */
bool glgeHasDisplayFunc();

/**
 * @brief check if GLGE has an window active
 * 
 * @return true GLGE has a window to draw on |
 * @return false GLGE has no window created
 */
bool glgeHasWindow();

/**
 * @brief set the name of the move matrix used by the shader programs
 * 
 * @param moveMatrix the new name for the move matrix
 */
void glgeSetMoveMatrixName(const char* moveMatrix);

/**
 * @brief get the name of the move matrix variable for the shaders
 * 
 * @return char* the name of the move matrix, default name: "moveMat"
 */
char* glgeGetMoveMatrixName();

/**
 * @brief get the window aspect of the GLGE window
 * 
 * @return float the aspect of the window: height/width
 */
float glgeGetWindowAspect();

/**
 * @brief get the current size of the GLGE window
 * 
 * @return vec2 the size of the window | WIDTH: x, HEIGHT: y
 */
vec2 glgeGetWindowSize();

/**
 * @brief get all momentanly pressed keys
 * 
 * @return Keys a list of all pressed keys
 */
Keys glgeGetKeys();

/**
 * @brief get all keys that where pressed this tick
 * 
 * @return Keys a list of all keys that where pressed this tick
 */
Keys glgeGetToggledKeys();

/**
 * @brief get the time since last frame
 * 
 * @return float delta time
 */
float glgeGetDeltaTime();

/**
 * @brief Get a uniform variable in a shader
 * 
 * @param shaderProgram the program to get the variable from
 * @param name the name of the variable
 */
GLint glgeGetUniformVar(GLuint shaderProgram, const char* name);

/**
 * @brief add a new shader object
 * 
 * @param shaderProgram the program it should be added to
 * @param shadertext the text for the shader code
 * @param shaderType the type of the shader
 */
void glgeAddShader(GLuint shaderProgram, const char* shadertext, GLenum shaderType);

/**
 * @brief this function compiles the inputed two files into shaders
 * 
 * @param vertex the file for the vertex shader
 * @param fragment the file for the fragment shader
 */
GLuint glgeCompileShader(const char* vertex, const char* fragment);

/**
 * @brief this file loads the two inputed std::vectors of strings as shader
 * 
 * @param fileDataVertex the data of the vertex shader
 * @param fileDataFragment the data of the fragment shader
 * @return GLuint the compiled shader
 */
GLuint glgeCompileShader(std::string fileDataVertex, std::string fileDataFragment);

/**
 * @brief load an texture from an file
 * 
 * @param file the file to read the data from
 * @param storeSize an optional place to store the size of the Picture
 * @return GLint the compiled OpenGL texture
 */
GLuint glgeTextureFromFile(const char* file, vec2* storeSize = NULL);

/**
 * @brief get the size of an texture from an file
 * 
 * @param texture the file for the texture
 * @return vec2 the size of the texture (width, height)
 */
vec2 glgeGetTextureSize(const char* texture);

/**
 * @brief get all mouse data from glge
 * 
 * @return Mouse the mouse data
 */
Mouse glgeGetMouse();

/**
 * @brief say that backface culling should be used
 */
void glgeEnableBackfaceCulling();

/**
 * @brief say that no backface culling should be used
 */
void glgeDisableBackfaceCulling();

/**
 * @brief switch the backface culling status
 */
void glgeSwapBackfaceCulling();

/**
 * @brief set the backwface culling status
 * 
 * @param status the status
 */
void glgeSetBackfaceCulling(bool status);

/**
 * @brief get the backface culling status
 * 
 * @return true backface culling is currently used | 
 * @return false no backface culling is used
 */
bool glgeGetBackfaceCullingStatus();

/**
 * @brief bind an shader that will controll how light sources effect the objects
 * 
 * @param lightingShaderFile the path to the post processing shader
 */
void glgeSetLightingShader(const char* lightingShaderFile);

/**
 * @brief bind an shader that will controll how light sources effect the objects
 * 
 * @param lightingShader the source code for the shader
 */
void glgeSetLightingShader(std::string lightingShader);

/**
 * @brief bind an shader that will controll how light sources effect the objects
 * 
 * @param shader the allready compiled OpenGL shader
 */
void glgeSetLightingShader(GLuint shader);

/**
 * @brief bind an shader that will controll how light sources effect the objects
 * 
 * @param shaderFile the path to the post processing shader
 */
void glgeSetPostProcessingShader(const char* shaderFile);

/**
 * @brief bind an shader that will controll how light sources effect the objects
 * 
 * @param shader the source code for the shader
 */
void glgeSetPostProsessingShader(std::string shader);

/**
 * @brief bind an shader that will controll how light sources effect the objects
 * 
 * @param shader the allready compiled OpenGL shader
 */
void glgeSetPostProsessingShader(GLuint shader);

/**
 * @brief Set the Wraping Mode for textures
 * 
 * @param mode the new wraping mode
 */
void glgeSetWrapingMode(unsigned int mode);

/**
 * @brief set the position of the mouse pointer
 * 
 * @param pointerPos the position of the pointer
 * @param space the current space of the position (GLGE_SCREEN_SPACE or GLGE_WINDOW_SPACE are valide)
 */
void glgeWarpPointer(vec2 pointerPos, unsigned int space = GLGE_SCREEN_SPACE);

/**
 * @brief set the position of the mouse pointer
 * 
 * @param x the x position of the pointer
 * @param y the y position of the pointer
 * @param space the current space of the position (GLGE_SCREEN_SPACE or GLGE_WINDOW_SPACE are valide)
 */
void glgeWarpPointer(float x, float y, unsigned int space = GLGE_SCREEN_SPACE);

/**
 * @brief set the style for the cursor
 * 
 * @param style the style defined in the defines, the default is the normal cursor
 */
void glgeSetCursor(int style=GLGE_CURSOR_STYLE_DEFAULT);

/**
 * @brief gets the size of the users screen
 * 
 * @return vec2 the size of the screen(x = width, y = height)
 */
vec2 glgeGetScreenSize();

/**
 * @brief get the current time since the engine started
 * 
 * @return float the current time since start
 */
float glgeGetCurrentElapsedTime();

/**
 * @brief get the albedo texture from the last frame
 * 
 * @return GLuint a pointer to the albedo texture on the GPU
 */
GLuint glgeGetMainAlbedoMap();

/**
 * @brief get the normal texture from the last frame
 * 
 * @return GLuint a pointer to the normal texture on the GPU
 */
GLuint glgeGetMainNormalMap();

/**
 * @brief get the position texture from the last frame
 * 
 * @return GLuint a position to the albedo texture on the GPU
 */
GLuint glgeGetMainPositionMap();

/**
 * @brief get the roughness texture from the last frame
 * 
 * @return GLuint a roughness to the albedo texture on the GPU
 */
GLuint glgeGetMainRoughnessMap();

/**
 * @brief get a texture that always points to the last frame
 * 
 * @return GLuint the OpenGL texture
 */
GLuint glgeGetLastFrame();

/**
 * @brief say if GLGE should exit the application if an error occures
 * 
 * @param exitOnError say if the application should be closed
 */
void glgeSetExitOnError(bool exitOnError);

/**
 * @brief swap the current exit on error value
 */
void glgeToggleExitOnError();

/**
 * @brief get if glge will close if an error occures
 * 
 * @return true : glge will close if an error occures | 
 * @return false : glge won't clos if an error occures
 */
bool glgeGetExitOnError();

/**
 * @brief say if GLGE should down and then upsample the lighted image before the post-processing pass
 * 
 * @param shouldUse say if the sampling chain should be used
 */
void glgeUseDownUpSampeling(bool shouldUse);

/**
 * @brief swap the current state of the down-upsampeling chain use
 */
void glgeSwapDownUpSampeling();

/**
 * @brief say if GLGE is currently using the down and upsampeling chain before the post-processing pass
 * 
 * @return true : GLGE will down and upsample before the post-processing pass | 
 * @return false : GLGE won't down and upsample before the post-processing pass
 */
bool glgeGetDownUpSampeling();

/**
 * @brief set the function that glge calles after it has resized the window
 * int 1 = width, int 2 = height
 * @param func a functionpointer to call
 */
void glgeBindOnWindowResizeFunc(void (*func)(int, int));

/**
 * @brief set the fullscreen mode to an inputed boolean
 * 
 * @param isFullscreen true: the window will be a fullscreen window | false: the window will be normal
 */
void glgeSetFullscreenMode(bool isFullscreen);

/**
 * @brief set the current fullscreen mode to the inverse of the current fullscreen mode
 */
void glgeToggleFullscreen();

/**
 * @brief get if the window is currently in fullscreen mode
 * 
 * @return true : the window is currently in fullscreen mode | 
 * @return false : the window is not in fullscreen mode
 */
bool glgeIsFullscreen();

/**
 * @brief change the window size
 * 
 * @param width the new width for the window
 * @param height the new height for the window
 * @param force say if the window reshape should also be applied if the normal window reshape is disabled | default: false
 */
void glgeResizeWindow(int width, int height, bool force = false);

/**
 * @brief change the window size
 * 
 * @param size the new size on both axis
 * @param force say if the window reshape should also be applied if the normal window reshape is disabled | default: false
 */
void glgeResizeWindow(vec2 size, bool force = false);

/**
 * @brief set if the main window is resizable
 * 
 * @param resizable true: the window is resizable | false: the window can not be resized
 */
void glgeSetWindowResizable(bool resizable);

/**
 * @brief change if the window can be resized
 */
void glgeToggleWindowResizable();

/**
 * @brief get if the window can be resized
 * 
 * @return true : the window can be resized
 * @return false : the window can't be resized
 */
bool glgeIsWindowResizable();

/**
 * @brief set the window position
 * 
 * @param position the new window position
 * @param force say if the window positoining should also be applied if the normal window movement is disabled | default: false
 */
void glgeSetWindowPosition(vec2 position, bool force = false);

/**
 * @brief set the window positon
 * 
 * @param x the x position for the window
 * @param y the y position for the window
 * @param force say if the window positoining should also be applied if the normal window movement is disabled | default: false
 */
void glgeSetWindowPosition(int x, int y, bool force = false);

/**
 * @brief get the window position
 * 
 * @return vec2 the current window position
 */
vec2 glgeGetWindowPosition();

/**
 * @brief set if it is allowed to move the window
 * 
 * @param allowMovement say if the window movement is allowed
 */
void glgeSetWindowMoveable(bool allowMovement);

/**
 * @brief toggle if it is allowed to move the window
 */
void glgeToggleWindowMoveable();

/**
 * @brief get if it is allowed to move the window
 * 
 * @return true : it is allowed to move the window | 
 * @return false : it is not allowed to move the window
 */
bool glgeGetWindowMoveable();

#endif