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
#include <GL/freeglut.h>

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

//store the clear color
vec4 glgeClearColor = vec4(1,1,1,1);

//should error be outputed?
bool glgeErrorOutput = true;

//should warnings be outputed?
bool glgeWarningOutput = true;

//is an main window created?
bool glgeHasMainWindow = false;

//additional display function
void (*glgeDisplayCallback)() = nullptr;

//is an display callback bound?
bool glgeHasDisplayCallback = false;

//the maximal frames per second the window should run on
int glgeMaxFPS = 60;

//the current frames per second the window is running on
int glgeCurrentFPS = 0;

//the time sinse the last update
float glgeDeltaTime = 0.f;

//the time the last tick started at
float glgeTickTime = 0.f;

//a callback for a function called every tick
void (*glgeMainCallback)() = nullptr;

//is a main callback function bound?
bool glgeHasMainCallback = false;

//store the name for the camera matrix
char* glgeCamMatrix = (char*)"camMat";

//store the window aspect
float glgeWindowAspect = 1.f;

//store the keys
Keys glgePressedKeys = Keys();

//store the mouse information
Mouse glgeMouse = Mouse();

//store the backface culling mode
bool glgeUseCulling = false;

//store the frame buffer, default is 0
unsigned int glgeFBO = 0;

//store the texture for the frame buffer
unsigned int glgeFrameBufferTexture = 0;

//store the direction normal for post-processing
unsigned int glgeFrameBufferNormalTexture = 0;

//store the texture for the multi sample framebuffer
unsigned int glgeFrameBufferMultisampleTexture = 0;

//store the custom multi sample frame buffer
unsigned int glgeMultFBO = 0;

//store the custom multi sample render buffer
unsigned int glgeMultRBO = 0;

//store the RBO
unsigned int glgeRBO = 0;

//store the amount of samples for GLGE
unsigned int glgeSamples = 4;

//store the window size
vec2 glgeWindowSize = vec2(0,0);

//set the screen VBO to default 0
unsigned int glgeScreenVBO = 0;

//set the screen VAO to default 0
unsigned int glgeScreenVAO = 0;

//set the default post processing shader to 0
int glgePostProcessingShader = 0;

//create and setup the lights vector
std::vector<Light*> glgeLights;

//store if multisampling is enabled
bool glgeUseMultisampling = true;

//store the resolution of the shadow map
unsigned int glgeShadowMapResolution = 1000;

//store the shader for shadow mapping
Shader glgeShadowShader;

//store the uniform for the model matrix in the shadow shader
GLuint glgeModelMatShadowLoc = 0;

//store the uniform for the shdow matrices in the shadow shader
GLuint glgeShadowMatShadowLoc = 0;

//store the uniform for the far plane in the shadow shader
GLuint glgeFarShadowLoc = 0;

//store the uniform for the light position in the shadow shader
GLuint glgeLightPosShadowLoc = 0;

//store the light world position
GLuint glgeLightWorldPosUniform = 0;

//store how to sample images
GLenum glgeWrapingMode = GL_NEAREST;

//store if the shadow pass is currently drawing
bool glgeIsShadowPass = false;

//store the main camera for GLGE
Camera* glgeMainCamera = NULL;