/**
 * @file GLGEWindow.h
 * @author DM8AT
 * @brief this file contains a class to create a subwindow in GLGE
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_WINDOW_H_
#define _GLGE_WINDOW_H_

//include the GLGE Dependencys
#include "GLGELightingCore.h"
#include "GLGE3Dcore.h"
//include CML
#include "CML/CMLVec2.h"
#include "CML/CMLVec3.h"

//include dependencys from the default lib
#include <string>

class GLGEWindow
{
public:
    /**
     * @brief Default constructor for a new window
     */
    GLGEWindow();

    /**
     * @brief Construct a new GLGE window
     * 
     * @param name the name of the window
     * @param size the size of the window in pixels
     * @param pos the position of the window in pixels
     * @param flags the window creation flags
     */
    GLGEWindow(const char* name, vec2 size, vec2 pos = vec2(0), unsigned int flags = 0);

    /**
     * @brief Construct a new GLGE window
     * 
     * @param name the name of the window
     * @param width the width of the window in pixels
     * @param height the height of the window in pixels
     * @param pos the position of the window in pixels
     * @param flags the window creation flags
     */
    GLGEWindow(const char* name, unsigned int width, unsigned int height, vec2 pos = vec2(0), unsigned int flags = 0);

    /**
     * @brief Construct a new GLGE window
     * 
     * @param name the name of the window
     * @param size the size of the window in pixels
     * @param x the height of the window in pixels
     * @param y the position of the window in pixels
     * @param flags the window creation flags
     */
    GLGEWindow(const char* name, vec2 size, float x, float y, unsigned int flags = 0);

    /**
     * @brief Construct a new GLGE window
     * 
     * @param name the name of the window
     * @param width the width of the window in pixels
     * @param height the height of the window in pixels
     * @param x the x position in pixels
     * @param y the y position in pixels
     * @param flags the window creation flags
     */
    GLGEWindow(const char* name, unsigned int width, unsigned int height, float x, float y, unsigned int flags = 0);

    /**
     * @brief Destroy the GLGEWindow
     */
    void close();

    /**
     * @brief draw the content of the window
     */
    void draw();

    /**
     * @brief update the window
     */
    void tick();

    /**
     * @brief change the display name of the glge window
     * 
     * @param name the new name of the window
     */
    void rename(const char* name);

    /**
     * @brief change the size of the window
     * 
     * @param size the new size of the window in pixels
     * @param force say if the function should execute if resizing is locked
     */
    void setSize(vec2 size, bool force = false);

    /**
     * @brief change the size of the window
     * 
     * @param width the new width of the window in pixels
     * @param height the new height of the window in pixels
     * @param force say if the function should execute if resizing is locked
     */
    void setSize(unsigned int width, unsigned int height, bool force = false);

    /**
     * @brief Set the on-screen position of the window
     * 
     * @param pos the new position on the screen in pixels
     * @param force say if the function should execute if movement is locked
     */
    void setPos(vec2 pos, bool force = false);

    /**
     * @brief Set the on-screen position of the window
     * 
     * @param x the x-position in pixels
     * @param y the y-position in pixels
     * @param force say if the function should execute if movement is locked
     */
    void setPos(float x, float y, bool force = false);

    /**
     * @brief set a function to draw the contents of the window
     * 
     * @param drawFunc a function pointer to the windows draw function
     */
    void setDrawFunc(void (*drawFunc)());

    /**
     * @brief set a function that is called every tick
     * 
     * @param tickFunc a function pointer to the windows tick function
     */
    void setTickFunc(void (*tickFunc)());

    /**
     * @brief Set a function that runs on every window resize
     * 
     * @param resizeFunc a function pointer to the windows resize function
     */
    void setResizeFunc(void (*resizeFunc)(int, int));

    /**
     * @brief Get the Draw Func object
     * 
     * @return a function pointer to the function
     */
    void (*getDrawFunc())();

    /**
     * @brief Get the Tick Func object
     * 
     * @return a function pointer to the function
     */
    void (*getTickFunc())();

    /**
     * @brief Get the Resize Func object
     * 
     * @return a function pointer to the function
     */
    void (*getResizeFunc())(int, int);

    /**
     * @brief execute the draw function, if one is bound
     */
    void callDrawFunc();

    /**
     * @brief execute the tick function, if one is bound
     */
    void callTickFunc();

    /**
     * @brief execute the resize function, if one is bound
     * 
     * @param w the width to pass to the function
     * @param h the height to pass to the function
     */
    void callResizeFunc(int w, int h);

    /**
     * @brief reset the draw function
     */
    void clearDrawFunc();

    /**
     * @brief reset the tick function
     */
    void clearTickFunc();

    /**
     * @brief reset the resize function
     */
    void clearResizeFunc();

    /**
     * @brief get if the window has a draw function bound
     * 
     * @return true : the window currently has a draw function | 
     * @return false : the window dosn't have a draw function
     */
    bool getHasDrawFunc();

    /**
     * @brief get if the window has a tick function bound
     * 
     * @return true : the window currently has a tick function | 
     * @return false : the window dosn't have a tick function
     */
    bool getHasTickFunc();

    /**
     * @brief get if the window has a resize function bound
     * 
     * @return true : the window currently has a resize function | 
     * @return false : the window dosn't have a resize function
     */
    bool getHasResizeFunc();

    /**
     * @brief Get the size of the window
     * 
     * @return vec2 the width & height of the window in pixels
     */
    vec2 getSize();

    /**
     * @brief Get the position of the window
     * 
     * @return vec2 the x and y position of the window in pixels
     */
    vec2 getPos();

    /**
     * @brief Get the name of the window
     * 
     * @return std::string the window name
     */
    std::string getName();

    /**
     * @brief add the window to the GLGE window stack
     */
    void start();

    /**
     * @brief get a pointer to the SDL window
     * 
     * @return void* a pointer of type SDL_Window* to the SDL window
     */
    void* getSDLWindow();

    /**
     * @brief get the OpenGL context of the window
     * 
     * @return void* a pointer of type SDL_GLContext to the OpenGL context
     */
    void* getGLContext();

    /**
     * @brief Set the clear color for the window
     * 
     * @param clearColor the clear color
     * @param normalise true : the color range will be expended to 0 - 255 | false : the color range will be 0-1 (default)
     */
    void setClearColor(vec3 clearColor, bool normalise = false);

    /**
     * @brief Set the clear color for the window
     * 
     * @param r the red chanel of the clear color
     * @param g the green chanel of the clear color
     * @param b the blue chanel of the clear color
     * @param normalise true : the color range will be expended to 0 - 255 | false : the color range will be 0-1 (default)
     */
    void setClearColor(float r, float g, float b, bool normalise = false);

    /**
     * @brief Get the clear color of the window
     * 
     * @return vec3 the clear color in range 0-1
     */
    vec3 getClearColor();

    /**
     * @brief set a skybox for the window
     * 
     * @param top the top image for the skybox
     * @param bottom the bottom image for the skybox
     * @param left the left image for the skybox
     * @param right the right image for the skybox
     * @param front the front image for the skybox
     * @param back the back image for the skybox
     */
    void setSkybox(const char* top, const char* bottom, const char* left, const char* right, const char* front, const char* back);

    /**
     * @brief this function is called every time the window is moved
     * 
     * @param x the new x position of the window
     * @param y the new y position of the window
     */
    void moveFunc(int x, int y);
    
    /**
     * @brief this function is called if the window is resized
     * 
     * @param width the new width of the window
     * @param height the new height of the window
     */
    void resizeWindow(int width, int height);

    /**
     * @brief Get the Window aspect ratio
     * 
     * @return float the window apsect ratio
     */
    float getWindowAspect();

    /**
     * @brief enable backface culling
     */
    void enableBackfaceCulling();

    /**
     * @brief disable backface culling
     */
    void disableBackfaceCulling();

    /**
     * @brief get if the window is using backface culling
     * 
     * @return true : the window is using backface culling | 
     * @return false : the window is not using backface culling
     */
    bool useBackfaceCulling();

    /**
     * @brief Set the Lighting Shader for the window
     * 
     * @param shader the file of the lighting shader
     */
    void setLightingShader(const char* shader);

    /**
     * @brief Set the Lighting Shader for the window
     * 
     * @param shader the data of the lighting shader
     */
    void setLightingShader(std::string shader);

    /**
     * @brief Set the Lighting Shader for the window
     * 
     * @param shader the allready compiled shader
     */
    void setLightingShader(unsigned int shader);

    /**
     * @brief Get the Albedo Texture
     * 
     * @return unsigned int the albedo texture on the graphics card
     */
    unsigned int getAlbedoTex();

    /**
     * @brief Get the Normal Texture
     * 
     * @return unsigned int the normal texture on the graphics card
     */
    unsigned int getNormalTex();

    /**
     * @brief Get the Pos Texture
     * 
     * @return unsigned int the position texture on the graphics card
     */
    unsigned int getPosTex();

    /**
     * @brief Get the Roughness, Metallic and Lit Texture
     * 
     * @return unsigned int the Roughness, Metallic and Lit texture on the graphics card
     */
    unsigned int getRMLTex();

    /**
     * @brief Get the Output Texture
     * 
     * @return unsigned int the Output texture on the graphics card
     */
    unsigned int getOutTex();

    /**
     * @brief Get the Object ID, Depth and Max alpha Acum Texture
     * 
     * @return unsigned int the Object ID, Depth and Max alpha acum texture on the graphics card
     */
    unsigned int getEIDATex();

    /**
     * @brief Get the Last Frame texture
     * 
     * @return unsigned int the last frame texture on the graphics card
     */
    unsigned int getLastFrame();

    /**
     * @brief Set the Fullscreen Mode for the window
     * 
     * @param isFullscreen true : the window will be fullscreen | false : the window will be normal
     */
    void setFullscreenMode(bool isFullscreen);

    /**
     * @brief Set the Camera the window will use mainly
     * 
     * @param cam a pointer to the camera
     */
    void setCamera(Camera* cam);

    /**
     * @brief Get the main Camera from the window
     * 
     * @return Camera* a pointer to the main camera
     */
    Camera* getCamera();

    /**
     * @brief bind a light to the window
     * 
     * @param light a pointer to the light that should be bound
     */
    void bindLight(Light* light);

    /**
     * @brief Get all lights that are bound to the window
     * 
     * @return std::vector<Light*> a vector of pointers to all bound lights
     */
    std::vector<Light*> getLights();

    /**
     * @brief Get the Screen VBO
     * 
     * @return unsigned int an OpenGL pointer to the screen VBO
     */
    unsigned int getScreenVBO();

    /**
     * @brief add a post processing shader to the post processing shader stack
     * 
     * @param shader the shader that should be added
     * @return int the index of the shader in the post processing shader stack
     */
    int addPostProcessingShader(Shader* shader);

    /**
     * @brief add a post processing shader to the post processing shader stack
     * 
     * @param file a file to read the shader code from
     * @return Shader* a pointer to the created shader
     */
    Shader* addPostProcessingShader(const char* file);

    /**
     * @brief add a post processing shader to the post processing shader stack
     * 
     * @param shader the source code of the shader
     * @return Shader* a pointer to the created shader
     */
    Shader* addPostProcessingShader(std::string shader);

    /**
     * @brief add a post processing shader to the post processing shader stack
     * 
     * @param shader the OpenGL shader that should be passed
     * @return Shader* a pointer to the created shader
     */
    Shader* addPostProcessingShader(unsigned int shader);

    /**
     * @brief add a post processing function to the post processing function stack
     * 
     * @param func the function that should be added
     * @return int the index of the function in the post processing function stack
     */
    int addPostProcessingFunction(Shader (*func)(unsigned int));

    /**
     * @brief remove a shader from the post processing shader stack
     * 
     * @param index the index of the shader in the post processing shader stack
     * @param del true : the shader pointer will be deleted | false : the shader pointer will continue to exist
     */
    void removePostProcessingShader(unsigned int index, bool del = false);

    /**
     * @brief remove a shader from the post processing shader stack
     * 
     * @param shader the shader that should be removed from the post processing shader stack
     * @param del true : the shader pointer will be deleted | false : the shader pointer will continue to exist
     */
    void removePostProcessingShader(Shader* shader, bool del = false);

    /**
     * @brief remove a function from the post processing function stack
     * 
     * @param index the index of the function in the post processing function stack
     */
    void removePostProcessingFunction(unsigned int index);

    /**
     * @brief remove a function from the post processing function stack
     * 
     * @param func a pointer to the function that should be removed
     */
    void removePostProcessingFunction(Shader (*func)(unsigned int));
    
    /**
     * @brief Get the Post Processing Shader from the post processing shader stack
     * 
     * @param index the index of the shader
     * @return Shader* a pointer to the shader
     */
    Shader* getPostProcessingShader(unsigned int index);

    /**
     * @brief Get the Index of an post processing shader in the post processing shader stack
     * 
     * @param shader a pointer to the shader
     * @return unsigned int the index of the shader in the post processing stack
     */
    int getPostProcessingShaderIndex(Shader* shader);

    /**
     * @brief Get a Post Processing Func from the post processing function stack
     * 
     * @param index the index of the post processing function
     * @return Shader*(funx*)(unsigned int) a pointer to the function 
     */
    Shader (*getPostProcessingFunc(unsigned int index))(unsigned int);

    /**
     * @brief Get the index of an post processing function in the post processing function stack
     * 
     * @param func the post processing function
     * @return unsigned int the index of the function in the post processing function stack
     */
    int getPostProcessingFuncIndex(Shader (*func)(unsigned int));

    /**
     * @brief get if the window is fullscreen
     * 
     * @return true : the window is fullscreen | 
     * @return false : the window is not fullscreen
     */
    bool isFullscreen();

    /**
     * @brief Enable / disable window resizing
     * 
     * @param resizable true : the window will be allowed to resize | false : the window won't be able to resize (exept for resize function with force = true)
     */
    void setResizable(bool resizable);

    /**
     * @brief get if the window is allowed to resize
     * 
     * @return true : the window is allowed to resize | 
     * @return false : the window is not allowed to resize
     */
    bool isWindowResizable();

    /**
     * @brief Set if the window is movalbe
     * 
     * @param movalbe true : the window will be allowed to move | false : the window won't be able to move (exept for move function with force = true)
     */
    void setMovable(bool movalbe);

    /**
     * @brief Get if the window is allowed to move
     * 
     * @return true : the window is allowed to move | 
     * @return false : the window is not allowed to move
     */
    bool isMovable();

    /**
     * @brief Set the Icon of an window
     * 
     * @param file the file with the window icon
     */
    void setWindowIcon(const char* file);

    /**
     * @brief get if the window is currently drawing the transparent pass
     * 
     * @return true : the window is currently drawing the transparent pass | 
     * @return false : the window is currently not drawing the transparent pass
     */
    bool isTranparentPass();

    /**
     * @brief Set the Transparency Combine Shader for the window
     * 
     * @param shader a pointer to the shader
     */
    void setTransparencyCombineShader(Shader* shader);

    /**
     * @brief Get the Transparency Combine Shader from the window
     * 
     * @return Shader* a pointer to the shader
     */
    Shader* getTransparencyCombineShader();

    /**
     * @brief get if the window is using an custom transparency combine shader
     * 
     * @return true : the window is using a custom transparency combine shader | 
     * @return false : the window is using the default transprency combine shader 
     */
    bool isCustomTransparencyCombineShader();

    /**
     * @brief Set if the window border is visible
     * 
     * @param visible true : the window border will be visible | false : the window border will be hidden
     */
    void setBorderVisible(bool visible);

    /**
     * @brief get if the window border is visible
     * 
     * @return true : the window border is currently visible | 
     * @return false : the window border is currently hidden
     */
    bool isBorderVisible();

    /**
     * @brief Disable the normal window exit ways
     * 
     * @param forceOpen true : the window won't react to normal window closing methods ('x' Button, alt + F4) | false : the window will be closable as normal
     */
    void setWindowForceOpen(bool forceOpen);

    /**
     * @brief get if the window is forced to stay open
     * 
     * @return true : the window won't react to normal window closing methods ('x' Button, alt + F4) | 
     * @return false : the window will be closable as normal
     */
    bool isWindowForceOpen();

    /**
     * @brief initiate the window to close on the start of the next tick
     * 
     * @param force say if the closing should apply if the window is forced to stay open
     */
    void initiateClosing(bool force);

    /**
     * @brief get if the window will close on the start of the next tick
     * 
     * @return true : the window will close on the start of the next tick | 
     * @return false : the window is not set to close
     */
    bool isClosingInitiated();

    /**
     * @brief say that the window won't clos on the start of the next tick
     * 
     * @param force say if the closing stop should apply if the window is forced to stay open
     */
    void stopWindowClosing(bool force);

    /**
     * @brief Set if the window should always render on top of the other windows
     * 
     * @param onTop true : the window will always render on top of the other windows | false : the window will render normal
     */
    void setWindowAlwaysOnTop(bool onTop);

    /**
     * @brief get if the window always renders on top of the other windows
     * 
     * @return true : the window renders always on top | 
     * @return false : the window renders normal
     */
    bool isWindowAlwaysOnTop();

    /**
     * @brief Set the Window Brightness
     * 
     * @param brightness the window brightness (default is 1, can't go below 0)
     */
    void setBrightness(float brightness);

    /**
     * @brief Get the Window Brightness
     * 
     * @return float the current window brightness
     */
    float getBrightness();

    /**
     * @brief show / hide the window
     * 
     * @param showHide true : the window will be shown | false : the window will be hidden
     */
    void showHide(bool showHide);

    /**
     * @brief get if the window is shown
     * 
     * @return true : the window is shown | 
     * @return false : the window is hidden
     */
    bool isShown();

    /**
     * @brief minimize the window
     * 
     * @param maximizeMinimize true : the window will be maximized | false : the window will be normal
     * @param force says if the function should execute if window resizing is disabled
     */
    void maximize(bool maximized, bool force = false);

    /**
     * @brief get if the window is maximized
     * 
     * @return true : the window is maximized | 
     * @return false : the window is not maximized
     */
    bool isMaximized();

    /**
     * @brief minimizes the window
     * 
     * @param minimized true : the window will be minimized | false : the window will be normal
     * @param force says if the function should execute if window resizing is disabled
     */
    void minimize(bool minimized, bool force = false);

    /**
     * @brief gets if the window is minimized
     * 
     * @return true : the window is minimized | 
     * @return false : the window is not minimized
     */
    bool isMinimized();

    /**
     * @brief get if the window is focused
     * 
     * @return true : the window is focused | 
     * @return false : the window is not focused
     */
    bool isFocused();

    /**
     * @brief get if the window is focused by the mouse 
     * 
     * @return true : the window is focused by the mouse | 
     * @return false : the window is not focused by the mouse
     */
    bool isMouseFocused();

    /**
     * @brief Set the Mouse Grab Mode for the window
     * 
     * @param grabbed true : the mouse can't move outside the window | false : the mouse is free to move
     */
    void setMouseGrabMode(bool grabbed);

    /**
     * @brief Get the Mouse Grab Mode from the window
     * 
     * @return true : the mouse can't move outside the window | 
     * @return false : the mouse is free to move
     */
    bool getMouseGrabMode();

    /**
     * @brief set the window focus
     * 
     * @param moveUp true : the window will be moved up by focusing it | false : the window will be focused without moving it in front of other windows
     */
    void focuse(bool moveUp);

    /**
     * @brief bind a VBO to cover the whole screen in an rectangle
     */
    void bindScreenRect();

    /**
     * @brief unbind the VBO that covers the whole screen in an rectangle
     */
    void unbindScreenRect();

    /**
     * @brief make the window the one currently handeld
     */
    void makeCurrent();

private:
    //////////////////////////////////
    //   Private handler functions  //
    //////////////////////////////////
    
    /**
     * @brief Get the uniform vars from the lighing shader
     * 
     * @return true : the albedo texture was found sucessfully | 
     * @return false : the albedo texture could not be found
     */
    bool getLightingUniforms();

    /**
     * @brief Get the default uniforms from the post processing shader
     * @param shader the post processing shader to get the 
     */
    void getDefaultUniformsFromPostProcessingShader(Shader* shader);

    

    /////////////////
    //  Variables  //
    /////////////////

    //store the window pointer (void* to not give acess to SDL2)
    void* window;
    //store the OpenGL context (void* to not give acess to SDL2)
    void* glContext;
    //store the window ID
    int id = 0;
    //store the name
    std::string name;
    //store the size
    vec2 size;
    //store the window size outside of fullscreen
    vec2 normSize;
    //store the position
    vec2 pos;
    //store the window aspect ratio
    float aspect = 0.f;
    //store if the window is fullscreen
    bool fullscreen = false;
    //store the clear color
    vec3 clear;
    //store if the window has a draw func
    bool hasDrawFunc = false;
    //store if the window has a tick func
    bool hasTickFunc = false;
    //store if the window has a resize func
    bool hasResizeFunc = false;
    //store the draw func
    void (*drawFunc)();
    //store the tick func
    void (*tickFunc)();
    //store the resize func
    void (*resizeFunc)(int, int);
    //store the main camera
    Camera* mainCamera = NULL;
    //get if the window is currently drawing
    bool drawing = false;
    //store if this is the transparent pass
    bool transparentPass = false;
    //store if the border is visible
    bool borderIsVisible = true;
    //store if the window is allways on top
    bool alwaysOnTop = false;
    //store if the window is force opend
    bool forceOpen = false;
    //store if the window should close
    bool opClose = false;
    //store the window brightness
    float brightness = 1;
    //store if the window is shown or hidden
    bool shown = true;
    //store if the window is maximized
    bool maximized = false;
    //store if the window is minimized
    bool minimized = false;
    //store the mouse grab mode
    bool mouseGrabbed = false;
    //store if the rectangle that covers the whole screen is bound
    bool screenRectBound = false;
    
    /*
        Window constrains
    */
    //store if window movement is allowed
    bool allowWindowMovement = true;
    //store if window resizing is allowed
    bool allowWindowResize = true;
    
    /*
        OpenGL Storage
    */
    //internal store parameters
    bool backfaceCulling = false;
    
    /*
        Main Framebuffer
    */
    //store the main framebuffer
    unsigned int mainFramebuffer = 0;
    //store the main renderbuffer
    unsigned int mainRenderbuffer = 0;
    //store the albedo texture
    unsigned int mainAlbedoTex = 0;
    //store the normal texture
    unsigned int mainNormalTex = 0;
    //store the position texture
    unsigned int mainPosTex = 0;
    //store the roughness, metalic and lit texture
    unsigned int mainRMLTex = 0;
    //store the lighting image output
    unsigned int mainOutTex = 0;
    //store the texture for Object IDs, Depth and transparent image max accumulation
    unsigned int mainEIDATex = 0;
    //store the texture for only the solid objects
    unsigned int mainSolidTex = 0;
    //store the transparent accumulation texture
    unsigned int mainTransparentAccumTex = 0;

    /*
        Last Tick Framebuffer
    */
    //store the framebuffer from the last tick
    unsigned int lastTickFramebuffer = 0;
    //store the texture from the last tick
    unsigned int lastTickTex = 0;

    /*
        Post Processing Framebuffer
    */
    //store the framebuffer for post processing
    unsigned int postProcessingFramebuffer;
    //store the image for post processing
    unsigned int postProcessingTex;
    //store the post processing function stack
    std::vector<Shader (*)(unsigned int)> ppsFunctionStack = {};
    //store the post processing shader stack
    std::vector<Shader*> ppsShaderStack = {};
    //store if this is the first post processing pass
    bool firstPPSPass;

    /*
        Store default Vertex and Index buffers
    */
    //store a vertex buffer for a rectangle that covers the whole screen
    unsigned int screenRectVBO = 0;
    //store the vertex array object
    unsigned int screenRectVAO = 0;
    //store the skybox vertices
    unsigned int skyboxVAO = 0;
    //store a vertex buffer for a skybox
    unsigned int skyboxVBO = 0;

    /*
        information about the lighting shader
    */
    //store the lighting shader
    unsigned int lightingShader = 0;
    //store the position of the uniform for the albedo map in the post processing shader
    int albedoInLightingPass = -1;
    //store the position of the uniform for the normal map in the post processing shader
    int normalInLightingPass = -1;
    //store the position of the uniform for the position map in the post processing shader
    int positionInLightingPass = -1;
    //store the position of the uniform for the roughness map in the post processing shader
    int roughnessInLightingPass = -1;
    //store the position of the light color uniform in the Lighting Pass shader
    std::vector<int> lightColInLightingPass = {};
    //store the position of the light intensity uniform in the Lighting Pass shader
    std::vector<int> lightIntInLightingPass = {};
    //store the position of the light position uniform in the Lighting Pass shader
    std::vector<int> lightPosInLightingPass = {};
    //store the position of the active light uniform in the Lighting Pass shader
    int activeLightInLightingPass = -1;
    //store the position of the camera positin uniform in the Lighting Pass shader
    int camPosInLightingPass = -1;
    //store the position of the far plane uniform in the Lighting Pass shader
    int farPlaneInLightingPass = -1;
    //store the rotation matrix in the lighting shader
    int rotInLightingPass = -1;
    //pass the projection matrix to the lighting shader
    int projInLightingPass = -1;
    //store the lights
    std::vector<Light*> lights;

    /*
        information about the skybox
    */
    //store the skybox shader
    unsigned int skyboxShader = 0;
    //store the rotation mat for the skybox shader
    int skyboxRot = -1;
    //store the projection mat for the skybox shader
    int skyboxProj = -1;
    //store the texture for the skybox shader
    int skyboxTexSampler = -1;
    //store the texture for the skybox
    unsigned int skyboxTex = 0;
    //say if a skybox is used
    bool useSkybox = false;

    /*
        the transparent combine shader
    */
    //store the transparent combination shader
    Shader* transparentCombineShader;
    //store if a custom transparent combination shader is bound
    bool customTransparentCombineShader = false;
};

#endif