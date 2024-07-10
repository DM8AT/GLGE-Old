/**
 * @file openglGLGERenderTarget.cpp
 * @author DM8AT
 * @brief define the opengl dependand render targets of GLGE
 * @version 0.1
 * @date 2024-01-02
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#include "openglGLGERenderTarget.h"

//include the OpenGL dependencys
#include "openglGLGEVars.hpp"
#include "../GLGEIndependend/glgeErrors.hpp"

#include <iostream>

/////////////////
//RENDER TARGET//
/////////////////

//PUBLIC

RenderTarget::RenderTarget()
{
    //initalise everyting
}

RenderTarget::RenderTarget(vec2 size)
{
    //generate the texture and the render target
    this->generateTexture(size.x, size.y, true);
    //store the current window index as the own window index
    this->windowIndex = glgeCurrentWindowIndex;
}

RenderTarget::RenderTarget(int width, int height)
{
    //generate the texture and the render target
    this->generateTexture(width, height, true);
    //store the current window index as the own window index
    this->windowIndex = glgeCurrentWindowIndex;
}

RenderTarget::RenderTarget(unsigned int glTexture)
{
    //store the inputed texture
    this->texture = glTexture;
    //two ints to store the width and height
    int w,h = 0;
    //activate texture 0
    glActiveTexture(GL_TEXTURE0);
    //bind the texture
    glBindTexture(GL_TEXTURE_2D, this->texture);
    //get the width from the texture
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    //get the height from the texture
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
    //generate the render target
    this->generateTexture(w,h, false);
    //store the current window index as the own window index
    this->windowIndex = glgeCurrentWindowIndex;
}

void RenderTarget::draw()
{
    //check if the object belongs to the window
    if (glgeCurrentWindowIndex != this->windowIndex)
    {
        //check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] Can't draw an render target in an window it was not created in.\n";
        }
        //check if the program should close on an error
        if (glgeExitOnError)
        {
            //close with an error
            exit(1);
        }
        //stop the function
        return;
    }
    //check if the shader is a nullpointer
    if (this->shader == NULL)
    {
        //print an warning
        if (glgeWarningOutput)
        {
            //print the warning
            std::cerr << "[GLGE WARNING] shader for a custom render target is a nullpointer, but it\'s draw methode was called" << "\n";
        }
        //stop the script
        return;
    }

    //disable depth testing
    glDisable(GL_DEPTH_TEST);
    //enable color blending
    glEnable(GL_BLEND);
    //setup the blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //check if a callback function is bound
    if (this->callback != NULL)
    {
        //call the callback function, taking this as an input
        this->callback(this);
    }

    //activate the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    //say that now a custom framebuffer is bound
    glgeCurrentFramebufferType = GLGE_FRAMEBUFFER_CUSTOM_RENDER_TEXTURE;

    //activate the shader
    this->shader->applyShader();

    //bind the screen rect
    glgeWindows[this->windowIndex]->bindScreenRect();

    //draw the screen
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //check if debug data gathering is enabled
    if (glgeGatherDebugInfo)
    {
        //increment the amount of draw passes
        glgeDrawCallCountT++;
        //increment the amount of triangles by 2 (the amount of triangles in a rectangle)
        glgeTriangleCountT += 2;
    }

    //unbind the screen rect
    glgeWindows[this->windowIndex]->unbindScreenRect();

    //unbind the shader
    this->shader->removeShader();
    //make sure to enable texture unit 0
    glActiveTexture(GL_TEXTURE0);

    //unbind the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //say that now the default window surface is bound
    glgeCurrentFramebufferType = GLGE_FRAMEBUFFER_WINDOW_SURFACE;
}

void RenderTarget::changeSize(vec2 s)
{
    //update the inernal size
    this->updateSize(s);
}

void RenderTarget::setCustomCallbackFunction(void(*func)(RenderTarget*))
{
    //store the inputed function (NULL is catched later)
    this->callback = func;
}

void RenderTarget::changeSize(int newWidth, int newHeight)
{
    //update the inernal size
    this->updateSize(vec2(newWidth, newHeight));
}

void RenderTarget::setShader(Shader* shader, bool ownShader)
{
    //check if an own shader is currently used
    if (ownShader)
    {
        //if it is, delete the shader
        delete this->shader;
    }
    //store the inputed shader
    this->shader = shader;
    //say that the shader isn't its own
    this->ownShader = ownShader;
}

void RenderTarget::setShader(unsigned int shader)
{
    //check if an own shader is currently used
    if (ownShader)
    {
        //if it is, delete the shader
        delete this->shader;
    }
    //generate the shader
    this->shader = new Shader(shader);
    //say that the sader is its own
    this->ownShader = true;
}

Shader* RenderTarget::getShader()
{
    //return the shader
    return this->shader;
}

unsigned int RenderTarget::getTexture()
{
    //return the texture
    return this->texture;
}

vec2 RenderTarget::getSize()
{
    //return the stored size
    return this->size;
}

//PRIVATE

void RenderTarget::generateTexture(int w, int h, bool genTexture)
{
    //check if the size is to small
    if ((w < 1) || (h < 1))
    {
        //check if an error should be outputed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] an size of " << w << ", " << h << "\n";
        }
        //check if the program should close on an error
        if (glgeExitOnError)
        {
            //close with error code 1
            exit(1);
        }
    }

    //create and bind the custom frame buffer
    glGenFramebuffers(1, &this->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

    if (genTexture)
    
    {
        //generate a texture to store the render target texture
        glGenTextures(1, &this->texture);
        glBindTexture(GL_TEXTURE_2D, this->texture);
        //set the texture parameters so it dosn't loop around the screen
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //bind the texture to the frame buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);
        //unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        //bind the texture
        glBindTexture(GL_TEXTURE_2D, this->texture);
        //bind the texture to the frame buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);
        //unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    //check if the framebuffer compiled correctly
    unsigned int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //if the frame buffer compiled not correctly
    if ( fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        //print an error
        std::cerr << GLGE_FATAL_ERROR_FRAMEBUFFER_NOT_COMPILED << fboStatus << "\n";
        //stop the program
        exit(1);
    }

    //store the width and height
    this->size = vec2(w,h);
    //update the parameter if the texture is owned
    this->ownTex = genTexture;
    //unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //unbind the render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderTarget::updateSize(vec2 s)
{
    //check if the size is to small
    if ((s.x < 1.f) || (s.y < 1.f))
    {
        //check if an error should be outputed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] an size of " << s.x << ", " << s.y << " is not possible for a render target" << "\n";
        }
        //check if the program should close on an error
        if (glgeExitOnError)
        {
            //close with error code 1
            exit(1);
        }
    }
    //check if the texture exists
    if (this->texture != 0)
    {
        //if it dose, resize it

        //bind texture unit 0
        glActiveTexture(GL_TEXTURE0);
        //unbind any 3D textures
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        //update the texture
        glBindTexture(GL_TEXTURE_2D, this->texture);
        //set the texture parameters so it dosn't loop around the screen
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, s.x, s.y, 0, GL_RGBA, GL_FLOAT, NULL);
        //unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    //change the internal size
    this->size = s;
}