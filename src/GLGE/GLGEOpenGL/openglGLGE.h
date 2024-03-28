/**
 * @file openglGLGE.h
 * @author DM8AT
 * @brief declaer the main functions for bare bones aplications
 * @version 0.1
 * @date 2024-01-02
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_OGL_H_
#define _GLGE_OGL_H_

//include CML
#include "../CML/CML.h"

//include the public defines
#include "openglGLGEDefines.hpp"
#include "../GLGEIndependend/GLGEKlasses.hpp"
//include the math functions
#include "../GLGEMath.h"
//include the shader
#include "openglGLGEShaderCore.h"
//include the render targets
#include "openglGLGERenderTarget.h"
//include the store function
#include "../GLGEIndependend/GLGEScene.hpp"
//include a class for compacting data
#include "../GLGEIndependend/GLGEData.h"

///////////
//DEFINES//
///////////

//the screen space in pixels, from -halfWidth to halfWidth on x and from -halfHeight to halfHeight on y
#define GLGE_SCREEN_SPACE 0
//relative window coordinates, from 0 to 1 on both axis
#define GLGE_WINDOW_SPACE 1

//Wrist watch
#define GLGE_CURSOR_STYLE_WAIT 0x0002
//Simple cross-hair
#define GLGE_CURSOR_STYLE_CROSSHAIR 0x0003
//a hand to grab
#define GLGE_CURSOR_STYLE_HAND 0x000B
//an style like when editing text
#define GLGE_CURSOR_STYLE_TEXT 0x0001
//An cross
#define GLGE_CURSOR_STYLE_NO 0x000A
//the cursor is invisible
#define GLGE_CURSOR_STYLE_NONE 0x0BB
//The default curser image
#define GLGE_CURSOR_STYLE_DEFAULT 0x0000
//Four pointed arrow pointing north, south, east, and west
#define GLGE_CURSOR_STYLE_FOUR_ARROWS 0x0009
//Double arrow pointing northeast and southwest
#define GLGE_CURSOR_STYLE_NORTHEAST_SOUTHWEST 0x0006
//Double arrow pointing north and south
#define GLGE_CURSOR_STYLE_NORTH_SOUTH 0x0008
//Double arrow pointing northwest and southeast
#define GLGE_CURSOR_STYLE_NORTHWEST_SOUTHEAST 0x0005
//Double arrow pointing west and east
#define GLGE_CURSOR_STYLE_WEST_EAST 0x0007

//define the integer limit as unlimited FPS
#define GLGE_FPS_UNLIMITED 2147483647

//define the window flags

//say that the window should be created in high-DPI mode if supported
#define GLGE_WINDOW_FLAG_ALLOW_HIGHDPI 8192
//window should be treated as a popup menu
#define GLGE_WINDOW_FLAG_POPUP_MENU 524288
//window should not be added to the taskbar
#define GLGE_WINDOW_FLAG_SKIP_TASKBAR 65536
//window should be treated as a tooltip
#define GLGE_WINDOW_FLAG_TOOLTIP 262144
//window should be treated as a utility window
#define GLGE_WINDOW_FLAG_UTILITY 131072

/////////////
//FUNCTIONS//
/////////////

/**
 * @brief initalise the GLGE library, execute at start of script
 * 
 * The arguments are the input to the main function. It initalises SDL2. 
 * 
 */
void glgeInit();

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
 * @brief load a cube map as a skybox
 * 
 * @param top the top image for the skybox
 * @param bottom the bottom image for the skybox
 * @param left the left image for the skybox
 * @param right the right image for the skybox
 * @param front the front image for the skybox
 * @param back the back image for the skybox
 */
void glgeSetSkybox(const char* top, const char* bottom, const char* left, const char* right, const char* front, const char* back);

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
int glgeGetUniformVar(unsigned int shaderProgram, const char* name);

/**
 * @brief add a new shader object
 * 
 * @param shaderProgram the program it should be added to
 * @param shadertext the text for the shader code
 * @param shaderType the type of the shader
 */
void glgeAddShader(unsigned int shaderProgram, const char* shadertext, unsigned int shaderType);

/**
 * @brief this function compiles the inputed two files into shaders
 * 
 * @param vertex the file for the vertex shader
 * @param fragment the file for the fragment shader
 */
unsigned int glgeCompileShader(const char* vertex, const char* fragment);

/**
 * @brief this file loads the two inputed std::vectors of strings as shader
 * 
 * @param fileDataVertex the data of the vertex shader
 * @param fileDataFragment the data of the fragment shader
 * @return unsigned int the compiled shader
 */
unsigned int glgeCompileShader(std::string fileDataVertex, std::string fileDataFragment);

/**
 * @brief load an texture from an file
 * 
 * @param file the file to read the data from
 * @param storeSize an optional place to store the size of the Picture
 * @return GLint the compiled OpenGL texture
 */
unsigned int glgeTextureFromFile(const char* file, vec2* storeSize = NULL);

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
void glgeSetLightingShader(unsigned int shader);

/**
 * @brief bind an shader that will controll how light sources effect the objects
 * 
 * @param shaderFile the path to the post processing shader
 */
Shader* glgeSetPostProsessingShader(const char* shaderFile);

/**
 * @brief bind an shader that will controll how light sources effect the objects
 * 
 * @param shader the source code for the shader
 */
Shader* glgeSetPostProsessingShader(std::string shader);

/**
 * @brief bind an shader that will controll how light sources effect the objects
 * 
 * @param shader the allready compiled OpenGL shader
 */
Shader* glgeSetPostProsessingShader(unsigned int shader);

/**
 * @brief set the post-processing shader to an allready existing shader
 * 
 * @param shader the shader object
 * @param int the index in the post processing stack
 */
int glgeSetPostProsessingShader(Shader* shader);

/**
 * @brief get an post-processing shader by an index
 * 
 * @param index the index of the post-processing shader in the pipeline
 * @return Shader* a pointer to the shader
 */
Shader* glgeGetPostProcessingShader(int index);

/**
 * @brief get the index of an post-processing shader, -1 if it dosn't exist
 * 
 * @param shader the shader to find the index for
 * @return int the index in the pipeline
 */
int glgeGetIndexOfPostProcessingShader(Shader* shader);

/**
 * @brief delete an post processing shader
 * 
 * @param index the index of the shader to remove
 * @param del say if the shader sould be deletet from memory, default = false
 */
void glgeDeletePostProcessingShader(int index, bool del = false);

/**
 * @brief delete an post processing shader
 * 
 * @param shader a pointer to the shader, that sould be removed
 * @param del say if the shader sould be deletet from memory, default = false
 */
void glgeDeletePostProcessingShader(Shader* shader, bool del = false);

/**
 * @brief Set the Interpolation Mode for textures
 * 
 * @param mode the new Interpolation mode
 */
void glgeSetInterpolationMode(unsigned int mode);

/**
 * @brief set the position of the mouse pointer
 * 
 * @param pointerPos the position of the pointer
 * @param space the current space of the position (GLGE_SCREEN_SPACE or GLGE_WINDOW_SPACE are valide)
 */
void glgeWarpPointer(vec2 pointerPos, unsigned int space = GLGE_WINDOW_SPACE);

/**
 * @brief set the position of the mouse pointer
 * 
 * @param x the x position of the pointer
 * @param y the y position of the pointer
 * @param space the current space of the position (GLGE_SCREEN_SPACE or GLGE_WINDOW_SPACE are valide)
 */
void glgeWarpPointer(float x, float y, unsigned int space = GLGE_WINDOW_SPACE);

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
 * @return unsigned int a pointer to the albedo texture on the GPU
 */
unsigned int glgeGetMainAlbedoMap();

/**
 * @brief get the normal texture from the last frame
 * 
 * @return unsigned int a pointer to the normal texture on the GPU
 */
unsigned int glgeGetMainNormalMap();

/**
 * @brief get the position texture from the last frame
 * 
 * @return unsigned int a position to the albedo texture on the GPU
 */
unsigned int glgeGetMainPositionMap();

/**
 * @brief get the roughness texture from the last frame
 * 
 * @return unsigned int a roughness to the albedo texture on the GPU
 */
unsigned int glgeGetMainRoughnessMap();

/**
 * @brief get a texture that always points to the last frame
 * 
 * @return unsigned int the OpenGL texture
 */
unsigned int glgeGetLastFrame();

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

/**
 * @brief change the Icon of the application window
 * 
 * @param file the path for the new icon
 */
void glgeSetWindowIcon(const char* file);

/**
 * @brief get the current albedo buffer - stores the main color of all pixels on screen
 * 
 * @return unsigned int the main albedo buffer
 */
unsigned int glgeGetAlbedoBuffer();

/**
 * @brief get the current normal buffer - stores the main normal vector for all pixels on screen
 * 
 * @return unsigned int the main normal buffer
 */
unsigned int glgeGetNormalBuffer();

/**
 * @brief get the current position buffer - stores the position in world space for all pixels on screen
 * 
 * @return unsigned int the main position buffer
 */
unsigned int glgeGetPositionBuffer();

/**
 * @brief get the current roughness buffer
 * r: float roughness
 * g: float metalic
 * b: bool lit
 * 
 * @return unsigned int the main roughness buffer
 */
unsigned int glgeGetRoughnessBuffer();

/**
 * @brief get the buffer that stores the lit image
 * 
 * @return unsigned int the main lit buffer
 */
unsigned int glgeGetLighningBuffer();

/**
 * @brief checke if the current draw pass is for transparent or opaque objects
 * 
 * @return true : the current pass is for transparent objects | 
 * @return false : the current pass is for opaque objects
 */
bool glgeGetTransparencyPass();

/**
 * @brief set a new transparent combinatino shader
 * this shader will be used to combine the transparent and opace pass. 
 * state of blending function:
 * GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA
 * @param shader a pointer to the shader
 */
void glgeSetTransparencyCombineShader(Shader* shader);

/**
 * @brief get the shader that is used to combine the transparent and opaque geometry
 * 
 * @return Shader* a pointer to the shader
 */
Shader* glgeGetTransparencyCombineShader();

/**
 * @brief get if GLGE has a custom transparent combine shader
 * if GLGE has no custom transparent shader, it will delete the old one if a new one is bound
 * @return true : The shader will not be deleted if a new one is bound | 
 * @return false : The shader will be deleted if a new one is bound
 */
bool glgeGetIfCustomTransparentCombineShaderIsBound();

/**
 * @brief hide or show the window border
 * 
 * @param visible true : the window border will get shown | false : the window border will get hidden
 */
void glgeSetWindowBorderVisible(bool visible);

/**
 * @brief get if the window border Border
 * 
 * @return true : the window border is shown | 
 * @return false : the window border is hidden
 */
bool glgeGetWindowBorderVisible();

/**
 * @brief say if the window should automaticaly close upon window closing actions
 * 
 * @param forceOpen true : the window will not close upon window closing actions | false : the window will close upon window closing actions
 */
void glgeSetWindowForceOpen(bool forceOpen);

/**
 * @brief get if the window will close automaticaly upon window closing actions
 * 
 * @return true : the window will not close upon window closing actions | 
 * @return false : the window will close upon window closing actions
 */
bool glgeGetWindowForceOpen();

/**
 * @brief close the GLGE main window
 * 
 * @param force say if the window should close even if force open is active, default is true
 */
void glgeCloseWindow(bool force = true);

/**
 * @brief say if a window closing was initated in this tick
 * 
 * @return true : a window closing was initiated and the window will close | 
 * @return false : no window closing was initiated
 */
bool glgeGetWindowClosingInitiated();

/**
 * @brief override the issued closing of the GLGE main window
 * 
 * @param force say if this function should also execute if the window is force opend
 */
void glgeStopWindowClosing(bool force = true);

/**
 * @brief say if the window should always render on top of other windows
 * 
 * @param onTop true : the window will always render on top | false : the window won't always render on top
 */
void glgeSetWindowAlwaysOnTop(bool onTop);

/**
 * @brief get if the window renders always on top of other windows
 * 
 * @return true : the window will always render on top | 
 * @return false : the window won't always render on top
 */
bool glgeGetWindowAlwaysOnTop();

/**
 * @brief set how bright the window should render. 1.0 is the normal brightness, 0.0 is compleate black. 
 * WARNING: On some opperating systems, this effects the compleate monitor brightness, if the window is on top. 
 * @param brightness the birghtness of the window, must be at least 0.0
 */
void glgeSetWindowBrightness(float brightness);

/**
 * @brief get the current window brightness
 * 
 * @return float : the window brightness
 */
float glgeGetWindowBrightness();

/**
 * @brief add a window flag before the window is constructed
 * 
 * @param windowFlag the window flag
 */
void glgeAddWindowFlag(int windowFlag);

/**
 * @brief show or hide the window
 * 
 * @param show true : the window will be shown | hide : the window will be hidden
 */
void glgeShowHideWindow(bool show);

/**
 * @brief get if the window is shown or hidden
 * 
 * @return true : the window is shown | 
 * @return false : the window is hidden
 */
bool glgeGetWindowShown();

/**
 * @brief maximize the window
 * 
 * @param maximized true : the window will be maximized | false : the window will be normal 
 * @param force say if this function should execute if window resizing is disabled
 */
void glgeMaximize(bool maximized, bool force = false);

/**
 * @brief get if the window is maximized or minimized
 * 
 * @return true : the window is maximized | 
 * @return false : the window is minimized
 */
bool glgeGetWindowMaximized();

/**
 * @brief check if the glge main window is focused
 * 
 * @return true : the window is focused | 
 * @return false : the window is not focused
 */
bool glgeGetWindowFocus();

/**
 * @brief get if the mouse is touching the window
 * 
 * @return true : the mouse is touching the window | 
 * @return false : the mouse is not touching the window
 */
bool glgeGetMouseFocus();

/**
 * @brief 
 * 
 * @param mode 
 */
void glgeSetMouseGrabMode(bool mode);

/**
 * @brief Get the Mouse Grab Mode
 * 
 * @return true : the mouse grab is active | 
 * @return false : the mouse grab mode is inactive
 */
bool glgeGetMouseGrabMode();

/**
 * @brief focus one specific window
 * 
 * @param moveUp say if the window should move up in the stack
 */
void glgeFocusWindow(bool moveUp = true);

/**
 * @brief set if GLGE should stop the main loop if the main application window is exited
 * 
 * @param exit true : GLGE will stop if the main window is closed | false : GLGE will keep running if the main window is closed
 */
void glgeSetExitOnMainWindowClose(bool exit);

/**
 * @brief get if GLGE will stop the main loop once the main window is closed
 * 
 * @return true : GLGE will stop if the main window is closed | 
 * @return false : GLGE will keep running if the main window is closed
 */
bool glgeGetExitOnMainWindowClose();

/**
 * @brief get if GLGE uses OpenGL
 * 
 * @return true : GLGE uses OpenGL | 
 * @return false : GLGE uses Vulkan
 */
bool glgeUsesOpenGL();

/**
 * @brief get if GLGE uses Vulkan
 * 
 * @return true : GLGE uses Vulkan | 
 * @return false : GLGE ueses OpenGL
 */
bool glgeUsesVulkan();

#endif