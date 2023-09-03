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

//store the application window
extern SDL_Window* glgeMainWindow;

//store the OpenGL context to the main window
extern SDL_GLContext glgeMainContext;

//store the display mode
extern SDL_DisplayMode glgeMainDisplay;

//store the true window size
extern vec2 glgeTrueWindowSize;

//store the clear color
extern vec4 glgeClearColor;

//should error be outputed?
extern bool glgeErrorOutput;

//should glge throw an error and crash if an normal error occures
extern bool glgeExitOnError;

//should warnings be outputed?
extern bool glgeWarningOutput;

//is an main window created?
extern bool glgeHasMainWindow;

//additional display function
extern void (*glgeDisplayCallback)();

//is an display callback bound?
extern bool glgeHasDisplayCallback;

//the maximal frames per second the window should run on
extern int glgeMaxFPS;

//the current frames per second the window is running on
extern int glgeCurrentFPS;

//the time sinse the last update
extern float glgeDeltaTime;

//the time the last tick started at
extern float glgeTickTime;

//a callback for a function called every tick
extern void (*glgeMainCallback)();

//a callback to a function, is called if the window is resize
//int 1 = width, int 2 = height
extern void (*glgeOnWindowResize)(int, int);

//is a main callback function bound?
extern bool glgeHasMainCallback;

//store the name for the move matrix
extern char* glgeCamMatrix;

//store the window aspect
extern float glgeWindowAspect;

//store all pressed keys
extern Keys glgePressedKeys;

//store the keys that where pressed for one frame
extern Keys glgeKeysThisTick;

//store the keys that where relesd for one frame
extern Keys glgeKeysRelesdThisTick;

//store the mouse information
extern Mouse glgeMouse;

//store the backface culling mode
extern bool glgeUseCulling;

//store the custom frame buffer
extern unsigned int glgeFBO;

//store the color from the frame buffer in an texture
extern unsigned int glgeFrameAlbedoMap;

//store the direction normal for post-processing
extern unsigned int glgeFrameNormalMap;

//store the fragment position in a texture
extern unsigned int glgeFramePositionMap;

//store the roughness in a texture r=roughness, g=metalness, b=unlit
extern unsigned int glgeFrameRoughnessMap;

//store the renderd image
extern unsigned int glgeFrameLastTick;

//store a framebuffer to store the image from the last tick
extern unsigned int glgeFBOLastTick;

//store a renderbuffer to store the image from last tick
extern unsigned int glgeRBOLastTick;

//store a framebuffer to store the image from the lighting pass
extern unsigned int glgeFBOLighintPass;

//store a renderbuffer to store the image from lighting pass
extern unsigned int glgeFBOLightingPass;

//store the position of the uniform for the albedo map in the post processing shader
extern int glgeAlbedoInLightingPass;

//store the position of the uniform for the normal map in the post processing shader
extern int glgeNormalInLightingPass;

//store the position of the uniform for the position map in the post processing shader
extern int glgePositionInLightingPass;

//store the position of the uniform for the roughness map in the post processing shader
extern int glgeRoughnessInLightingPass;

//store the position of the light color uniform in the Lighting Pass shader
extern std::vector<int> glgeLightColInLightingPass;

//store the position of the light intensity uniform in the Lighting Pass shader
extern std::vector<int> glgeLightIntInLightingPass;

//store the position of the light position uniform in the Lighting Pass shader
extern std::vector<int> glgeLightPosInLightingPass;

//store the position of the active light uniform in the Lighting Pass shader
extern int glgeActiveLightInLightingPass;

//store the position of the camera positin uniform in the Lighting Pass shader
extern int glgeCamPosInLightingPass;

//store the position of the far plane uniform in the Lighting Pass shader
extern int glgeFarPlaneInLightingPass;

//store the rotation matrix in the lighting shader
extern int glgeRotInLightingPass;

//pass the projection matrix to the lighting shader
extern int glgeProjInLightingPass;

//store if the skybox is active
extern bool glgeUseSkybox;

//store the skybox in an cube map
extern unsigned int glgeSkyboxCube;

//store the geometry for the skybox
extern unsigned int glgeSkyboxBuffer;

//store the shader for the skybox
extern unsigned int glgeSkyboxShader;

//store the sampler in the skybox shader
extern int glgeSkyboxSampler;

//store the rotation in the skybox shader
extern int glgeSkyboxRotation;

//store the projection matrix in the skybox
extern int glgeSkyboxProject;

//store the active color buffers
extern GLenum glgeUsedColorBuffers[];

//store the length of the unsigned buffer array
extern unsigned int glgeLenUsedColorBuffers;

//store all the color buffers
extern GLenum glgeAllUsedColorBuffers[];

//store the length of all used color buffers
extern unsigned int glgeLenAllUsedColorBuffers;

//store the RBO
extern unsigned int glgeRBO;

//store the RBO for the normals
extern unsigned int glgeRBONormal;

//store the window size
extern vec2 glgeWindowSize;

//store the rectangle that covers the whole screen
extern unsigned int glgeScreenVBO, glgeScreenVAO;

//store the shader for the lighting pass
extern int glgeLightingShader;

//store the shader for the post processing pass
extern int glgePostProcessingShader;

//store the main image in the post processing shader
extern int glgeMainImageInPPS;

//store the position of the uniform for the albedo map in the post processing shader
extern int glgeAlbedoInPPS;

//store the position of the uniform for the normal map in the post processing shader
extern int glgeNormalInPPS;

//store the position of the uniform for the position map in the post processing shader
extern int glgePositionInPPS;

//store the position of the uniform for the roughness map in the post processing shader
extern int glgeRoughnessInPPS;

//store the frame buffer for the lighting pass
extern unsigned int glgeLightingFBO;

//store the render buffer for the ligthing pass
extern unsigned int glgeLightingRBO;

//store the output image for the lighting pass
extern unsigned int glgeLightingImageOut;

//create a vector to store all lights
extern std::vector<Light*> glgeLights;

//store the resolution for the shadow map
extern unsigned int glgeShadowMapResolution;

//store the uniform for the model matrix in the shadow shader
extern GLuint glgeModelMatShadowLoc;

//store the uniform for the shdow matrices in the shadow shader
extern GLuint glgeShadowMatShadowLoc;

//store the uniform for the far plane in the shadow shader
extern GLuint glgeFarShadowLoc;

//store the uniform for the light position in the shadow shader
extern GLuint glgeLightPosShadowLoc;

//store the shader for shadow mapping
extern Shader glgeShadowShader;

//store the light world position
extern GLuint glgeLightWorldPosUniform;

//store how to sample images
extern GLenum glgeWrapingMode;

//store if the shadow pass is currently drawing
extern bool glgeIsShadowPass;

//store the main camera for GLGE
extern Camera* glgeMainCamera;

//say if the down-and upsampleing should be done
extern bool glgeDownUpSampeling;

//say if the window is currently in fullscreen mode
extern bool glgeFullscreen;

//store the window size outside of the fullscreen mode
extern vec2 glgeNormalWindowSize;

//say if glge should allow if the window is resized
extern bool glgeAllowWindowResize;

//store the position of the window
extern vec2 glgeWindowPosition;

//say if it is allowed to move the window
extern bool glgeAllowWindowMovement;

//store the opperating system the application is compiled for
extern int glgeOperatingSystem;

#endif