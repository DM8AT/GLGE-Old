/**
 * @file GLGE.h
 * @author DM8AT
 * @brief A simple 3D game development engine
 * 
 * This is an module designed to make 3D game developement easyer. It is build on top of glew and freeglut, so if you want to develope using GLGE, make shute
 * you have glew and freeglut installed. 
 * To make developement easyer, it is inspired by the structure of the Unity game engine, but for C++ and without an viewport to drag the objects around. 
 * 
 * FEATURES:
 * - initalisation
 * - Window creation
 * - binding of own additional functions for mor flexebility in programming
 * - easy way to limit the FPS
 * 
 * PLANED:
 * - Objects (glgeObject)
 * - simple 2D features
 * - shaders
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
 * @copyright CC BY (see  https://creativecommons.org/licenses/by/4.0/)
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
#include "GLGE/CML/CML.h"

//include the public defines
#include "GLGE/GLGEDefines.hpp"
#include "GLGE/GLGEKlasses.hpp"

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
 * @brief get all momentanly pressed keys
 * 
 * @return Keys a list of all pressed keys
 */
Keys glgeGetKeys();

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

#endif