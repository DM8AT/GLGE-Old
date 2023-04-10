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

//store the clear color
extern vec4 glgeClearColor;

//should error be outputed?
extern bool glgeErrorOutput;

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

//is a main callback function bound?
extern bool glgeHasMainCallback;

//store the name for the move matrix
extern char* glgeCamMatrix;

//store the window aspect
extern float glgeWindowAspect;

//store all pressed keys
extern Keys glgePressedKeys;

//store the mouse information
extern Mouse glgeMouse;

//store the backface culling mode
extern bool glgeUseCulling;

//store the custom frame buffer
extern unsigned int glgeFBO;

//store the custom multi sample frame buffer
extern unsigned int glgeMultFBO;

//store the custom multi sample render buffer
extern unsigned int glgeMultRBO;

//store the frame buffer texture
extern unsigned int glgeFrameBufferTexture;

//store the direction normal for post-processing
extern unsigned int glgeFrameBufferNormalTexture;

//store the multi sample frame buffer texture
extern unsigned int glgeFrameBufferMultisampleTexture;

//store the RBO
extern unsigned int glgeRBO;

//store the window size
extern vec2 glgeWindowSize;

//store the amount of samples for GLGE
extern unsigned int glgeSamples;

//store the rectangle that covers the whole screen
extern unsigned int glgeScreenVBO, glgeScreenVAO;

//store the shader for the screen(Post processing shader)
extern int glgePostProcessingShader;

//create a vector to store all lights
extern std::vector<Light*> glgeLights;

//store if multisampling is enabled
extern bool glgeUseMultisampling;

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

#endif