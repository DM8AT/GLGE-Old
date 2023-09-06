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

//store the application window
SDL_Window* glgeMainWindow = NULL;

//store the OpenGL context to the main window
SDL_GLContext glgeMainContext;

//store the display mode
SDL_DisplayMode glgeMainDisplay;

//store the true window size
vec2 glgeTrueWindowSize = vec2(0,0);

//store the clear color
vec4 glgeClearColor = vec4(1,1,1,1);

//should error be outputed?
bool glgeErrorOutput = true;

//should glge throw an error and crash if an normal error occures
bool glgeExitOnError = true;

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

//a callback to a function, is called if the window is resize
//int 1 = width, int 2 = height
void (*glgeOnWindowResize)(int, int) = nullptr;

//is a main callback function bound?
bool glgeHasMainCallback = false;

//store the name for the camera matrix
char* glgeCamMatrix = (char*)"camMat";

//store the window aspect
float glgeWindowAspect = 1.f;

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

//store the frame buffer, default is 0
unsigned int glgeFBO = 0;

//store the color from the frame buffer in an texture
unsigned int glgeFrameAlbedoMap = 0;

//store the direction normal for post-processing
unsigned int glgeFrameNormalMap = 0;

//store the fragment position in a texture
unsigned int glgeFramePositionMap = 0;

//store the roughness in a texture
unsigned int glgeFrameRoughnessMap = 0;

//store the renderd image
unsigned int glgeFrameLastTick = 0;

//store a framebuffer to store the image from the last tick
unsigned int glgeFBOLastTick = 0;

//store a renderbuffer to store the image from last tick
unsigned int glgeRBOLastTick = 0;

//store a framebuffer to store the image from the lighting pass
unsigned int glgeFBOLighintPass = 0;

//store a renderbuffer to store the image from lighting pass
unsigned int glgeFBOLightingPass = 0;

//store the position of the uniform for the albedo map in the post processing shader
int glgeAlbedoInLightingPass = -1;

//store the position of the uniform for the normal map in the post processing shader
int glgeNormalInLightingPass = -1;

//store the position of the uniform for the position map in the post processing shader
int glgePositionInLightingPass = -1;

//store the position of the uniform for the roughness map in the post processing shader
int glgeRoughnessInLightingPass = -1;

//store the position of the light color uniform in the Lighting Pass shader
std::vector<int> glgeLightColInLightingPass = {};

//store the position of the light intensity uniform in the Lighting Pass shader
std::vector<int> glgeLightIntInLightingPass = {};

//store the position of the light position uniform in the Lighting Pass shader
std::vector<int> glgeLightPosInLightingPass = {};

//store the position of the active light uniform in the Lighting Pass shader
int glgeActiveLightInLightingPass = -1;

//store the position of the camera positin uniform in the Lighting Pass shader
int glgeCamPosInLightingPass = -1;

//store the position of the far plane uniform in the Lighting Pass shader
int glgeFarPlaneInLightingPass = -1;

//store the rotation matrix in the lighting shader
int glgeRotInLightingPass = -1;

//pass the projection matrix to the lighting shader
int glgeProjInLightingPass = -1;

//store if the skybox is active
bool glgeUseSkybox = false;

//store the skybox in an cube map
unsigned int glgeSkyboxCube = 0;

//store the geometry for the skybox
unsigned int glgeSkyboxBuffer = 0;

//store the VBO for a cube
unsigned int glgeCubeVBO = 0;

//store the IBO for a cube
unsigned int glgeCubeIBO = 0;

//store the shader for the skybox
unsigned int glgeSkyboxShader = 0;

//store the sampler in the skybox shader
int glgeSkyboxSampler = -1;

//store the rotation in the skybox shader
int glgeSkyboxRotation = -1;

//store the projection matrix in the skybox
int glgeSkyboxProject = -1;

//store the active color buffers
GLenum glgeUsedColorBuffers[] = {GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};

//store the length of the unsigned buffer array
unsigned int glgeLenUsedColorBuffers = sizeof(glgeUsedColorBuffers) / sizeof(glgeUsedColorBuffers[0]);

//store all the color buffers
GLenum glgeAllUsedColorBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};

//store the length of all used color buffers
unsigned int glgeLenAllUsedColorBuffers = sizeof(glgeAllUsedColorBuffers) / sizeof(glgeAllUsedColorBuffers[0]);

//store the RBO
unsigned int glgeRBO = 0;

//store the RBO for the normals
unsigned int glgeRBONormal = 0;

//store the amount of samples for GLGE
unsigned int glgeSamples = 4;

//store the window size
vec2 glgeWindowSize = vec2(0,0);

//set the screen VBO to default 0
unsigned int glgeScreenVBO = 0;

//set the screen VAO to default 0
unsigned int glgeScreenVAO = 0;

//set the default post processing shader to 0
int glgeLightingShader = 0;

//store the shader for the post processing pass
int glgePostProcessingShader = 0;

//store the main image in the post processing shader
int glgeMainImageInPPS = 0;

//store the position of the uniform for the albedo map in the post processing shader
int glgeAlbedoInPPS = 0;

//store the position of the uniform for the normal map in the post processing shader
int glgeNormalInPPS = 0;

//store the position of the uniform for the position map in the post processing shader
int glgePositionInPPS = 0;

//store the position of the uniform for the roughness map in the post processing shader
int glgeRoughnessInPPS = 0;

//store the frame buffer for the lighting pass
unsigned int glgeLightingFBO = 0;

//store the render buffer for the ligthing pass
unsigned int glgeLightingRBO = 0;

//store the output image for the lighting pass
unsigned int glgeLightingImageOut = 0;

//store the uniforms in the post processing shader
std::map<std::string, GLuint> glgePPSUniforms = {};

//create and setup the lights vector
std::vector<Light*> glgeLights;

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
GLenum glgeInterpolationMode = GL_NEAREST;

//store if the shadow pass is currently drawing
bool glgeIsShadowPass = false;

//store the main camera for GLGE
Camera* glgeMainCamera = NULL;

//say if the down-and upsampleing should be done
bool glgeDownUpSampeling = true;

//say if the window is currently in fullscreen mode
bool glgeFullscreen = false;

//store the window size outside of the fullscreen mode
vec2 glgeNormalWindowSize = vec2(0,0);

//say if glge should allow if the window is resized
bool glgeAllowWindowResize = true;

//store the position of the window
vec2 glgeWindowPosition = vec2(0,0);

//say if it is allowed to move the window
bool glgeAllowWindowMovement = true;

//store the opperating system the application is compiled for
int glgeOperatingSystem = -1;