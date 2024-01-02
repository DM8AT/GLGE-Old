/**
 * @file openglGLGERenderTarget.h
 * @author DM8AT
 * @brief declare the OpenGL dependeand interface for the render target
 * @version 0.1
 * @date 2024-01-02
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_RENDER_TARGET_OGL_H_
#define _GLGE_RENDER_TARGET_OGL_H_

//include shaders
#include "openglGLGEShaderCore.h"

/**
 * @brief an custom texture to render to
 */
class RenderTarget
{
public:
    /**
     * @brief default constructor for a render target
     */
    RenderTarget();

    /**
     * @brief construct a render target with a specified size
     * 
     * @param size the size of the texture to render to
     */
    RenderTarget(vec2 size);

    /**
     * @brief construct a render target with a specified width and height
     * 
     * @param width the width of the texture to render to
     * @param height the height of the texture to render to
     */
    RenderTarget(int width, int height);

    /**
     * @brief construct a render target from an OpenGL texture
     * 
     * @param glTexture 
     */
    RenderTarget(unsigned int glTexture);

    /**
     * @brief draw to the custom render target
     */
    void draw();

    /**
     * @brief change the size of the render target
     * 
     * @param newSize the new size for the render target
     */
    void changeSize(vec2 newSize);

    /**
     * @brief Set a function the render target will automatically call when drawing
     * The input to the function is automaticaly a poitner to the object it self, so it can be manipulated
     * @param func a function pointer
     */
    void setCustomCallbackFunction(void(*func)(RenderTarget*));

    /**
     * @brief change the size of the render target
     * 
     * @param newWidth the new width of the render target
     * @param newHeight the new height of the render target
     */
    void changeSize(int newWidth, int newHeight);

    /**
     * @brief set the shader for the object to use while rendering (default: default pps shader)
     * 
     * @param shader a pointer to the shader object
     * @param ownShader say if the shader is only used by the render target and should be deleted if the render target gets deleted
     */
    void setShader(Shader* shader, bool ownShader = false);

    /**
     * @brief set the shader for the object to use while rendering (default: default pps shader)
     * 
     * @param shader an compiled OpenGL shader
     */
    void setShader(unsigned int shader);

    /**
     * @brief get the shader from the custom render target
     * 
     * @return Shader* a pointer to the shader
     */
    Shader* getShader();

    /**
     * @brief get the renderd texture from the render target
     * 
     * @return unsigned int an OpenGL texture
     */
    unsigned int getTexture();

    /**
     * @brief Get the size of the render target
     * 
     * @return vec2 the size | x : width ; y : height
     */
    vec2 getSize();

private:
    //store the width and height of the render target
    vec2 size = vec2();
    //store the framebuffer
    unsigned int FBO = 0;
    //store the texture
    unsigned int texture = 0;
    //store the shader to render with
    Shader* shader = NULL;
    //say if the object stores an texture generated itself
    bool ownTex = false;
    //say if the shader is its own
    bool ownShader = false;
    //store a custom callback function
    void(*callback)(RenderTarget*) = NULL;
    //store the window index of the render target
    int windowIndex = -1;

    /**
     * @brief generate the texture for the render target as well as the render target it self
     * 
     * @param w the width of the texture / render target
     * @param h the height of the texture / render target
     * @param genTexture say if the texture should be generated as well
     */
    void generateTexture(int w, int h, bool genTexture);

    /**
     * @brief change the size to an new size
     * 
     * @param size the new size
     */
    void updateSize(vec2 size);
};

#endif