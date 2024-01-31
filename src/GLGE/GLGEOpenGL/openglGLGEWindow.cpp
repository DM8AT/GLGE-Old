/**
 * @file openglGLGEWindow.cpp
 * @author DM8AT
 * @brief define the window class of GLGE
 * @version 0.1
 * @date 2024-01-02
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

//check if glew is allready included
#ifndef _GLGE_GLEW_
//say that glew is now included
#define _GLGE_GLEW_
//include glew
#include <GL/glew.h>
//close the if for glew
#endif
//include the main header
#include "openglGLGEWindow.h"
//include GLGE dependencys
#include "openglGLGEVars.hpp"
#include "../GLGEInternal/glgeErrors.hpp"
#include "openglGLGEFuncs.hpp"
#include "../GLGEInternal/glgeImage.h"
#include "openglGLGEDefines.hpp"
#include "../GLGEInternal/glgePrivDefines.hpp"

//include default libs
#include <iostream>
#include <algorithm>

GLGEWindow::GLGEWindow()
{
    //say that the object exists
}

GLGEWindow::GLGEWindow(const char* name, vec2 size, vec2 pos, unsigned int flags)
{
    //add default flags
    flags |= (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    //store the window name
    this->name = std::string(name);
    //store the size
    this->size = size;
    //store the position
    this->pos = pos;

    //create the window
    SDL_Window* window = SDL_CreateWindow(name, pos.x, pos.y, size.x, size.y, flags);
    //check if the window could be created
    if (!window)
    {
        //check if an error should be outputed
        if (glgeErrorOutput)
        {
            //if the window couldn't be created, throw an error
            std::cerr << "[ERROR] GLGE couldn't create the window. SDL error: " << SDL_GetError() << "\n";
        }
        //check if the program should stop on an error
        if (glgeExitOnError)
        {
            //exit with an error
            exit(1);
        }
        //if it shouldn't stop, stop the function
        return;
    }
    //store the window
    this->window = (void*)window;
    //store the window ID
    this->id = SDL_GetWindowID(window);
    //add a dummy value to the window
    glgeWindows.push_back(NULL);

    //get the gl context
    SDL_GLContext con = SDL_GL_CreateContext(window);
    //check if the context is valide
    if (con == NULL)
    {
        //check if error output is enabled
        if (glgeErrorOutput)
        {
            //if the context is not valide, print an fatal error
            std::cerr << "[ERROR] GLGE could not create a OpenGL Context to the SDL Window. SDL Error: " << SDL_GetError() << "\n";
        }
        //check if the program should stop on an error
        if (glgeExitOnError)
        {
            //exit with an error
            exit(1);
        }
        //stop the function
        return;
    }
    //store the context
    this->glContext = con;

    //initalise glew
    if (glewInit() != GLEW_OK)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_GLEW_INIT_FAILED, glewGetErrorString(glewInit()));
            std::cerr << GLGE_ERROR_STR_GLEW_INIT << "\n";
        }
        //close the window
        SDL_DestroyWindow((SDL_Window*)this->window);
        //close the OpenGL context
        SDL_GL_DeleteContext(this->glContext);
        
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
        //stop the function
        return;
    }

    //bind the context
    SDL_GL_MakeCurrent((SDL_Window*)this->window, this->glContext);
    //disable VSync
    SDL_GL_SetSwapInterval(0);

    //clear the framebuffer of the window
    glClear(GL_COLOR_BUFFER_BIT);
    //update the window
    SDL_GL_SwapWindow((SDL_Window*)this->window);

    //check if the window index offset is uninitalised (-1)
    if (glgeWindowIndexOffset == -1)
    {
        //set the current index as offset (the first window should be 0)
        glgeWindowIndexOffset = this->id;
    }

    //say to cull backfasing triangles
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    this->backfaceCulling = true;

    //setup the depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    //disable multisampeling
    glDisable(GL_MULTISAMPLE);

    //create and bind the custom frame buffer
    glGenFramebuffers(1, &this->mainFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->mainFramebuffer);

    //generate the Render Buffer
    glGenRenderbuffers(1, &this->mainRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, this->mainRenderbuffer);
    //setup the storage for the render buffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->size.x, this->size.y);
    //attach an depth stencil
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->mainRenderbuffer);

    //generate the texture for the frame buffer
    glGenTextures(1, &this->mainAlbedoTex);
    glBindTexture(GL_TEXTURE_2D, this->mainAlbedoTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mainAlbedoTex, 0);
    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //generate a texture to store the normals
    glGenTextures(1, &this->mainNormalTex);
    glBindTexture(GL_TEXTURE_2D, this->mainNormalTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->mainNormalTex, 0);
    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //generate a texture to store the fragment position
    glGenTextures(1, &this->mainPosTex);
    glBindTexture(GL_TEXTURE_2D, this->mainPosTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->mainPosTex, 0);

    //generate a texture to store the fragment roughness
    glGenTextures(1, &this->mainRMLTex);
    glBindTexture(GL_TEXTURE_2D, this->mainRMLTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, this->mainRMLTex, 0);

    //generate a texture to store the lighting pass output
    glGenTextures(1, &this->mainOutTex);
    glBindTexture(GL_TEXTURE_2D, this->mainOutTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, this->mainOutTex, 0);

    //generate a texture to store the depth information
    glGenTextures(1, &this->mainEIDATex);
    glBindTexture(GL_TEXTURE_2D, this->mainEIDATex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, this->mainEIDATex, 0);

    //generate a texture to store the depth information
    glGenTextures(1, &this->mainSolidTex);
    glBindTexture(GL_TEXTURE_2D, this->mainSolidTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, this->mainSolidTex, 0);

    //generate a texture to store the last tick image
    glGenTextures(1, &this->mainTransparentAccumTex);
    glBindTexture(GL_TEXTURE_2D, this->mainTransparentAccumTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->size.x, this->size.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT7, GL_TEXTURE_2D, this->mainTransparentAccumTex, 0);

    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //unbind the render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    //specify the number of used color buffers
    glDrawBuffers(glgeLenUsedColorBuffers, glgeUsedColorBuffers);

    //check if the framebuffer compiled correctly
    unsigned int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //if the frame buffer compiled not correctly
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        //print an error
        std::cerr << GLGE_FATAL_ERROR_FRAMEBUFFER_NOT_COMPILED << fboStatus << "\n";
        //stop the program
        exit(1);
    }

    //create and bind the second custom frame buffer
    glGenFramebuffers(1, &this->lastTickFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->lastTickFramebuffer);

    //generate a texture to store the last tick image
    glGenTextures(1, &this->lastTickTex);
    glBindTexture(GL_TEXTURE_2D, this->lastTickTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->lastTickTex, 0);

    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //check if the framebuffer compiled correctly
    fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //if the frame buffer compiled not correctly
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        //print an error
        std::cerr << GLGE_FATAL_ERROR_FRAMEBUFFER_NOT_COMPILED << fboStatus << "\n";
        //stop the program
        exit(1);
    }

    //create and bind the fourth custom frame buffer
    glGenFramebuffers(1, &this->postProcessingFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->postProcessingFramebuffer);

    //generate a texture to store the lighting pass output
    glGenTextures(1, &this->postProcessingTex);
    glBindTexture(GL_TEXTURE_2D, this->postProcessingTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->postProcessingTex, 0);

    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //check if the framebuffer compiled correctly
    fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //if the frame buffer compiled not correctly
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        //print an error
        std::cerr << GLGE_FATAL_ERROR_FRAMEBUFFER_NOT_COMPILED << fboStatus << "\n";
        //stop the program
        exit(1);
    }

    //create an rectangle that covers the whole screen
    float rectangleVertices[] = 
    {
        //Coords   //texCoords
       -1.f, -1.f, 0.f, 0.f,
        1.f, -1.f, 1.f, 0.f,
       -1.f,  1.f, 0.f, 1.f,

        1.f, -1.f, 1.f, 0.f,
        1.f,  1.f, 1.f, 1.f,
       -1.f,  1.f, 0.f, 1.f
    };
    
    //create two buffers, one vertex array and one array buffer
    glGenVertexArrays(1, &this->screenRectVAO);
    //create two buffers, one vertex array and one array buffer
    glGenBuffers(1, &this->screenRectVBO);
    //bind the array buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->screenRectVBO);
    //load the data into the array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
    //unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //generate the shaders for the default lighting shader
    this->lightingShader = compileShader(GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER, GLGE_DEFAULT_LIGHTING_SHADER, "GLGE:BuildInPostProcessingVertexShader", "GLGE:BuildInPostProcessingFragmentShader");

    //get the uniforms form the shader
    bool albedo = this->getLightingUniforms();

    //check if the albedo map was found
    if (!albedo)
    {
        //check if an error should be outputed
        if (glgeErrorOutput)
        {
            std::cerr << "[GLGE ERROR] Could not find albedo map in default lighthing shader for window \"" << this->name.c_str() << "\"" << "\n";
        }
        //close the context
        SDL_GL_DeleteContext(this->glContext);
        //close the window
        SDL_DestroyWindow((SDL_Window*)this->window);
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
        //stop the function
        return;
    }

    //get the uniform for the screen texture in the lighting shader
    glUniform1i(glGetUniformLocation(this->lightingShader, "screenTexture"), 0);

    //create an cube of size 1
    float skyboxVerts[] = 
    {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    //create two buffers, one vertex array and one array buffer
    glGenVertexArrays(1, &this->skyboxVAO);
    //create two buffers, one vertex array and one array buffer
    glGenBuffers(1, &this->skyboxVBO);
    //bind the array buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->skyboxVBO);
    //load the data into the array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVerts), &skyboxVerts, GL_STATIC_DRAW);
    //unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //generate the shaders for the default lighting shader
    this->skyboxShader = glgeCompileShader(GLGE_DEFAULT_VERTEX_SKYBOX, GLGE_DEFAULT_FRAGMENT_SKYBOX);

    //get the uniforms
    //get the rotation uniform
    this->skyboxRot = getUniformVar(this->skyboxShader, "glgeRot");
    //get the projection uniform
    this->skyboxProj = getUniformVar(this->skyboxShader, "glgeProject");
    //get the sampler
    this->skyboxTexSampler = getUniformVar(this->skyboxShader, "glgeSkybox");

    //setup the basic transparent combine shader
    this->transparentCombineShader = new Shader(GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER, GLGE_DEFAULT_TRANSPARENT_COMBINE_SHADER);
    //load the uniforms
    //load the uniform for the accumulation texture
    this->transparentCombineShader->setCustomTexture("glgeTranspAccumTexture", this->mainTransparentAccumTex);
    //load the uniform for the count texture
    this->transparentCombineShader->setCustomTexture("glgeTranspCountTexture", this->mainEIDATex);
    //update the uniform binding
    this->transparentCombineShader->recalculateUniforms();
    //say that no custom shader is bound
    this->customTransparentCombineShader = false;

    //compile the default 3D shader
    this->default3DShader = glgeCompileShader(GLGE_DEFAULT_3D_VERTEX, GLGE_DEFAULT_3D_FRAGMENT);
    //compile the default 3D transparent shader
    this->default3DTransShader = glgeCompileShader(GLGE_DEFAULT_3D_VERTEX, GLGE_DEFAULT_TRANSPARENT_SHADER);
    //compile the default 2D shader
    this->default2DShader = glgeCompileShader(GLGE_DEFAULT_2D_VERTEX, GLGE_DEFAULT_2D_FRAGMENT);

    //initalise the textures correctly by calling the resize function
    this->resizeWindow(this->size.x, this->size.y);
}

GLGEWindow::GLGEWindow(const char* name, unsigned int width, unsigned int height, vec2 pos, unsigned int flags)
{
    //just pass to another constructor
    *this = GLGEWindow(name, vec2(width, height), pos, flags);
}

GLGEWindow::GLGEWindow(const char* name, vec2 size, float x, float y, unsigned int flags)
{
    //just pass to another constructor
    *this = GLGEWindow(name, size, vec2(x,y), flags);
}

GLGEWindow::GLGEWindow(const char* name, unsigned int width, unsigned int height, float x, float y, unsigned int flags)
{
    //just pass to another constructor
    *this = GLGEWindow(name, vec2(width, height), vec2(x,y), flags);
}

void GLGEWindow::close()
{
    //if this window has a window id is 0, stop the function
    if (this->id == 0)
    {
        return;
    }
    //get the window pointer
    SDL_Window* win = (SDL_Window*)this->window;
    //close the window
    SDL_DestroyWindow(win);
    //check if this is the main window
    if ((unsigned int)this->id-glgeWindowIndexOffset == glgeMainWindowIndex)
    {
        //say that the main window was destroyd
        glgeHasMainWindow = false;
    }
    //delete the window from the call stack
    glgeWindows[this->id-glgeWindowIndexOffset] = nullptr;
    //decrease the amount of active GLGE windows
    glgeActiveWindows -= 1;
}

void GLGEWindow::draw()
{
    //say that the window is currently drawing
    this->drawing = true;
    //make this the current active window
    this->makeCurrent();

    //bind the custom framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->mainFramebuffer);

    //only clear the first color buffer
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    //set the specified clear color
    glClearColor(this->clear.x, this->clear.y, this->clear.z, 1);

    //clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //specify the color buffers
    glDrawBuffers(glgeLenUsedColorBuffers, glgeUsedColorBuffers);

    //set the clear color to black
    glClearColor(0,0,0,0);

    //clear the other buffers
    glClear(GL_COLOR_BUFFER_BIT);

    //specify the color buffers to draw into
    glDrawBuffers(glgeLenAllUsedColorBuffers, glgeAllUsedColorBuffers);

    //enable depth testing
    glEnable(GL_DEPTH_TEST);

    //check if a skybox is active
    if (this->useSkybox)
    {
        //switch the depth function to greater equal
        glDepthFunc(GL_GEQUAL);
        //switch the order of the backface culling
        glCullFace(GL_FRONT);

        //draw the skybox
        //switch to the skybox shader
        glUseProgram(this->skyboxShader);
        //bind the skybox vertices
        glBindVertexArray(this->skyboxVAO);
        //bind the VBO
        glBindBuffer(GL_ARRAY_BUFFER, this->skyboxVBO);
        //activate the vertex attribute for the position
        glEnableVertexAttribArray(0);
        //load the position into the shader
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //push the projection matrix to the shader
        glUniformMatrix4fv(this->skyboxProj, 1, GL_FALSE, this->mainCamera->getProjectionMatrixPointer());
        //push the rotation to the shader
        glUniformMatrix4fv(this->skyboxRot, 1, GL_FALSE, this->mainCamera->getRotMatPointer());
        //activate the first texture unit
        glActiveTexture(GL_TEXTURE0);
        //bind the skybox
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyboxTex);

        //draw the skybox
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //unbind the skybox texture
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        //unbind the vertex attribute
        glDisableVertexAttribArray(0);
        //unbind the shader
        glUseProgram(0);
        //unbind the buffers
        //unbind the VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //reset the depth func
        glDepthFunc(GL_GREATER);
    }

    //check if backface culling is enabled
    if (this->backfaceCulling)
    {
        //if it is enabled, enable backface culling
        glEnable(GL_CULL_FACE);
        //setup the correct order
        glCullFace(GL_BACK);
    }
    else
    {
        //else, disable it
        glDisable(GL_CULL_FACE);
    }

    //call the custom drawing function
    this->callDrawFunc();

    //enable back face culling
    glEnable(GL_CULL_FACE);
    //switch the order of the backface culling to the correct order
    glCullFace(GL_BACK);
    //disable depth testing
    glDisable(GL_DEPTH_TEST);

    //check if lighting should be applied
    if ((int)lights.size() != 0)
    {
        //if it needs, apply the lighting shader

        //bind the post processing shader
        glUseProgram(this->lightingShader);
        //bind the screen rect
        this->bindScreenRect();
        //activate the uniform texture array
        glActiveTexture(GL_TEXTURE0);
        //clear the cube map texture (just for safty, can't have cube map and 2D at the same time)
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        //bind the framebuffer texture
        glBindTexture(GL_TEXTURE_2D, this->mainAlbedoTex);
        //activate the second texture unit
        glActiveTexture(GL_TEXTURE1);
        //bind the normal texture
        glBindTexture(GL_TEXTURE_2D, this->mainNormalTex);
        //activate the thired texture unit
        glActiveTexture(GL_TEXTURE2);
        //bind the position texture
        glBindTexture(GL_TEXTURE_2D, this->mainPosTex);
        //activate the fourth texture unit
        glActiveTexture(GL_TEXTURE3);
        //bind the roughness texture
        glBindTexture(GL_TEXTURE_2D, this->mainRMLTex);

        //pass the uniforms to the shader
        //pass the albedo map
        glUniform1i(this->albedoInLightingPass, 0);
        //pass the normal map
        glUniform1i(this->normalInLightingPass, 1);
        //pass the position map
        glUniform1i(this->positionInLightingPass, 2);
        //pass the roughness map
        glUniform1i(this->roughnessInLightingPass, 3);
        //check if a 3D camera is bound
        if (this->mainCamera != NULL)
        {
            //pass the camera position
            glUniform3f(this->camPosInLightingPass, this->mainCamera->getPos().x, this->mainCamera->getPos().y, this->mainCamera->getPos().z);
            //pass the far plane
            glUniform1f(this->farPlaneInLightingPass, this->mainCamera->getFarPlane());
            //bind the camera rotation
            glUniform3f(this->rotInLightingPass, this->mainCamera->getRotation().x, this->mainCamera->getRotation().y, glgeToRadians(180));
            //bind the camera projection matrix
            glUniformMatrix4fv(this->projInLightingPass, 1, false, this->mainCamera->getProjectionMatrixPointer());
        }
        //pass all the lights to the shader
        //load all light positions to the shader
        for (int i = 0; i < (int)this->lightPosInLightingPass.size(); i++)
        {
            //pass the light position
            glUniform3f(this->lightPosInLightingPass[i], lights[i]->getPos().x, lights[i]->getPos().y, lights[i]->getPos().z);
        }
        //load all light colors to the shader
        for (int i = 0; i < (int)lightColInLightingPass.size(); i++)
        {
            //pass the light color
            glUniform3f(lightColInLightingPass[i], lights[i]->getColor().x, lights[i]->getColor().y, lights[i]->getColor().z);
        }
        //load all light intensitys to the shader
        for (int i = 0; i < (int)lightIntInLightingPass.size(); i++)
        {
            //pass the light intensity
            glUniform1f(lightIntInLightingPass[i], lights[i]->getInsensity());
        }
        //pass the amount of active lights
        glUniform1i(activeLightInLightingPass, (int)lights.size());
        
        //draw the screen
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //unbind the normal texture
        glBindTexture(GL_TEXTURE_2D, 0);
        //activate the thired texture unit
        glActiveTexture(GL_TEXTURE2);
        //unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
        //activate the second texture unit
        glActiveTexture(GL_TEXTURE1);
        //unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
        //activate the first texture unit
        glActiveTexture(GL_TEXTURE0);
        //unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
        //unbind the shader
        glUseProgram(0);
        //unbind the screen rect
        this->unbindScreenRect();
    }

    //call the custom drawing function for transparent objects
    if(this->hasDrawFunc)
    {
        //say that the second pass is the transparent pass
        this->transparentPass = true;
        //enable alpha blending
        glEnable(GL_BLEND);
        //enable depth testing
        glEnable(GL_DEPTH_TEST);
        //disable writes to the depth buffer
        glDepthMask(GL_FALSE);
        //switch to addaptive blending for color attachment 5 and 7
        glBlendFunci(5,GL_ONE, GL_ONE);
        glBlendFunci(7,GL_ONE, GL_ONE);
        
        //check if backface culling is enabled
        if (this->backfaceCulling)
        {
            //if it is enabled, enable backface culling
            glEnable(GL_CULL_FACE);
            //setup the correct culling
            glCullFace(GL_BACK);
        }
        else
        {
            //else, disable it
            glDisable(GL_CULL_FACE);
        }

        //call the draw function
        this->callDrawFunc();
        //switch to inverted normal blend func
        glBlendFunci(4,GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

        //bind the shader
        this->transparentCombineShader->applyShader();
        //bind the screen rect
        this->bindScreenRect();

        //draw the screen
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //make sure to enable texture unit 0
        glActiveTexture(GL_TEXTURE0);
        //unbind the screen rect
        this->unbindScreenRect();
        //remove the shader
        this->transparentCombineShader->removeShader();

        //reenable depth mask
        glDepthMask(GL_TRUE);
        //disable depth testing
        glDisable(GL_DEPTH_TEST);
        //diable alpha blending
        glDisable(GL_BLEND);
        //reset the transparent pass to false
        this->transparentPass = false;
        //re-enable backface culling
        glEnable(GL_CULL_FACE);
        //setup the culling order
        glCullFace(GL_BACK);
    }

    //bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //say that the momentan pass is the first
    this->firstPPSPass = true;

    //loop over all post-processing shader
    for (Shader* shader : this->ppsShaderStack)
    {
        //bind the correct framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //activate the shader
        shader->applyShader();
        //take care of the uniforms
        getDefaultUniformsFromPostProcessingShader(shader);
        //update all uniforms
        shader->recalculateUniforms();
        //bind the screen rect
        this->bindScreenRect();

        //draw the screen
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //deactivate the shader
        shader->removeShader();
        //make sure to enable texture unit 0
        glActiveTexture(GL_TEXTURE0);
        //unbind the screen rect
        this->unbindScreenRect();

        //bind the default FBO for reading
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        //bind the light framebuffer as writing
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->postProcessingFramebuffer);
        //copy the framebuffers
        glBlitFramebuffer(0,0, this->size.x, this->size.y, 0,0, this->size.x, this->size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        //bind the default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //say that an pass finished
        this->firstPPSPass = false;
    }


    //loop over all post-processing shader
    for (Shader (*shaderFunc)(unsigned int) : this->ppsFunctionStack)
    {
        //bind the correct FBO
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //create a shader to store the shader from the function
        Shader shader;
        //check if this is the first post-processing pass
        if (this->firstPPSPass)
        {
            //get the shader using the custom function and the lit image
            shader = shaderFunc(this->mainOutTex);
        }
        else
        {
            //get the shader using the custom function and the image from the last PPS
            shader = shaderFunc(this->postProcessingTex);
        }
        //get the default uniforms
        getDefaultUniformsFromPostProcessingShader(&shader);
        //get all uniforms (this is done automaticaly, because it is needed)
        shader.recalculateUniforms();
        //activate the shader
        shader.applyShader();
        //bind the array buffer
        glBindBuffer(GL_ARRAY_BUFFER, this->screenRectVBO);
        //activate the vertex attribute for the position
        glEnableVertexAttribArray(0);
        //load the position into the shader
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        //activate the vertex attrivute for the texture coordinate
        glEnableVertexAttribArray(1);
        //load the texture coordinate into the shader
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        //draw the screen
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //deactivate the shader
        shader.removeShader();
        //make sure to enable texture unit 0
        glActiveTexture(GL_TEXTURE0);
        //unbind the buffer
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //deactivate the vertex attrib array pointers
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        //bind the default FBO for reading
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        //bind the light framebuffer as writing
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->postProcessingFramebuffer);
        //copy the framebuffers
        glBlitFramebuffer(0,0, this->size.x, this->size.y, 0,0, this->size.x, this->size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        //bind the default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //say that an pass finished
        this->firstPPSPass = false;
    }

    //check if any post-processing shader was drawn
    if (this->firstPPSPass)
    {
        //bind the light framebuffer for reading
        glBindFramebuffer(GL_READ_FRAMEBUFFER, this->mainFramebuffer);
        //bind the default fbo for drawing
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        //copy the framebuffers
        glBlitFramebuffer(0,0, this->size.x, this->size.y, 0,0, this->size.x, this->size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

    //copy the last frame to another fragment shader
    //bind the default framebuffer as read only
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    //bind the custom framebuffer as draw only
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->lastTickFramebuffer);
    //copy the framebuffer
    glBlitFramebuffer(0,0, this->size.x, this->size.y, 0,0, this->size.x, this->size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    //bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //update the window surface
    SDL_GL_SwapWindow((SDL_Window*)this->window);

    //say that the window is no longer drawing
    this->drawing = false;
}

void GLGEWindow::tick()
{
    //store the actual window width
    int w;
    //store the actual window height
    int h;
    //get the window size
    SDL_GetWindowSize((SDL_Window*)this->window,&w,&h);

    //check if the window size is correct
    if ((this->size.x != w) || (this->size.y != h))
    {
        //check if a window resize is allowed
        if (!this->allowWindowResize)
        {
            //if a window resize is not allowed, reshape the window to the inputed valide size
            SDL_SetWindowSize((SDL_Window*)this->window, this->size.x, this->size.y);
        }
    }

    //store the actual window width
    int x;
    //store the actual window height
    int y;
    //get the window size
    SDL_GetWindowPosition((SDL_Window*)this->window,&x,&y);

    //check if the window was moved
    if ((this->pos.x != x) || (this->pos.y != y))
    {
        //check if a window moevement is allowed
        if (!this->allowWindowMovement)
        {
            //if window movement is not allowed, move the window back
            SDL_SetWindowPosition((SDL_Window*)this->window, this->pos.x, this->pos.y);
        }
    }

    //call the tick function
    this->callTickFunc();
}

void GLGEWindow::rename(const char* name)
{
    //update the window name
    SDL_SetWindowTitle((SDL_Window*)this->window, name);
}

void GLGEWindow::setSize(vec2 size, bool force)
{
    //check if force is not enabled
    if (!force)
    {
        //check if window resizing is not allowed
        if (!this->allowWindowResize)
        {
            //stop the function
            return;
        }
    }
    //check if the window size is valide
    if (size.x<1 || size.y<1)
    {
        //check if a warning should print
        if (glgeWarningOutput)
        {
            //print an warning
            printf("[GLGE WARNING] a window size of w:%f,h:%f is not allowed, the minimum size is w:1,h1:\n", size.x, size.y);
        }
        //stop the function
        return;
    }
    //update the window size
    SDL_SetWindowSize((SDL_Window*)this->window, size.x, size.y);
    //store the new window size
    this->size = size;
    //call the resize function
    this->resizeFunc(size.x, size.y);
}

void GLGEWindow::setSize(unsigned int width, unsigned int height, bool force)
{
    //cast the input to another function
    this->setSize(vec2(width, height), force);
}

void GLGEWindow::setPos(vec2 pos, bool force)
{
    //check if the movement is not forced
    if (!force)
    {
        //check if window movement is not allowed
        if (!this->allowWindowMovement)
        {
            //stop the function
            return;
        }
    }
    //update the window position
    SDL_SetWindowPosition((SDL_Window*)this->window, pos.x, pos.y);
    //store the new position
    this->pos = pos;
}

void GLGEWindow::setPos(float x, float y, bool force)
{
    //pass the input to another function
    this->setPos(vec2(x,y), force);
}

std::string GLGEWindow::getName()
{
    //return the name
    return this->name;
}

void GLGEWindow::setDrawFunc(void (*func)())
{
    //check if an error occured
    bool error = false;
    if(func == nullptr)
    {
        //print an error
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_FUNC_IS_NULLPOINTER);
        }
        //say that an error occured
        error = true;
    }

    //if an error occured, exit
    if(error)
    {
        //print an exit message
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_BIND_MAIN_CALLBACK << "\n";
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //set the display callback
    this->drawFunc = func;

    //say that an display callback is bound
    this->hasDrawFunc = true;
}

void GLGEWindow::setTickFunc(void (*func)())
{
    //check if an error occured
    bool error = false;
    if(func == nullptr)
    {
        //print an error
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_FUNC_IS_NULLPOINTER);
        }
        //say that an error occured
        error = true;
    }

    //if an error occured, exit
    if(error)
    {
        //print an exit message
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_BIND_MAIN_CALLBACK << "\n";
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //set the display callback
    this->tickFunc = func;

    //say that an display callback is bound
    this->hasTickFunc = true;
}

void GLGEWindow::setResizeFunc(void (*func)(int, int))
{
    //check if an error occured
    bool error = false;
    if(func == nullptr)
    {
        //print an error
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_FUNC_IS_NULLPOINTER);
        }
        //say that an error occured
        error = true;
    }

    //if an error occured, exit
    if(error)
    {
        //print an exit message
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_BIND_MAIN_CALLBACK << "\n";
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //set the display callback
    this->resizeFunc = func;

    //say that an display callback is bound
    this->hasResizeFunc = true;
}

void GLGEWindow::setInitFunc(void (*func)())
{
    //check if an error occured
    bool error = false;
    if(func == nullptr)
    {
        //print an error
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_FUNC_IS_NULLPOINTER);
        }
        //say that an error occured
        error = true;
    }

    //if an error occured, exit
    if(error)
    {
        //print an exit message
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_BIND_MAIN_CALLBACK << "\n";
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //set the init func
    this->initFunc = func;

    //say that an init func is bound
    this->hasInitFunc = true;
}

void (*GLGEWindow::getDrawFunc())()
{
    //return the function, if none is bound, return 0
    return this->hasDrawFunc ? this->drawFunc : NULL;
}

void (*GLGEWindow::getTickFunc())()
{
    //return the function, if none is bound, return 0
    return this->hasTickFunc ? this->tickFunc : NULL;
}

void (*GLGEWindow::getResizeFunc())(int, int)
{
    //return the function, if none is bound, return 0
    return this->hasResizeFunc ? this->resizeFunc : NULL;
}

void (*GLGEWindow::getInitFunc())()
{
    //return the function, if none is bound, return 0
    return this->hasInitFunc ? this->initFunc : NULL;
}

void GLGEWindow::callDrawFunc()
{
    //check if the draw function is a nullpointer
    if (this->hasDrawFunc)
    {
        //if it is not, call the function
        ((void(*)())this->drawFunc)();
    }
}

void GLGEWindow::callTickFunc()
{
    //check if the draw function is a nullpointer
    if (this->hasTickFunc)
    {
        //if it is not, call the function
        ((void(*)())this->tickFunc)();
    }
}

void GLGEWindow::callResizeFunc(int w, int h)
{
    //check if the draw function is a nullpointer
    if (this->hasResizeFunc)
    {
        //if it is not, call the function
        ((void(*)(int,int))this->resizeFunc)(w, h);
    }
}

void GLGEWindow::callInitFunc()
{
    //check if the draw function is a nullpointer
    if (this->hasInitFunc)
    {
        //if it is not, call the function
        ((void(*)())this->initFunc)();
    }
}

void GLGEWindow::clearDrawFunc()
{
    //set the draw function to an nullpointer
    this->drawFunc = NULL;
    //say that no draw function is bound
    this->hasDrawFunc = false;
}

void GLGEWindow::clearTickFunc()
{
    //set the draw function to an nullpointer
    this->tickFunc = NULL;
    //say that no draw function is bound
    this->hasTickFunc = false;
}

void GLGEWindow::clearResizeFunc()
{
    //set the draw function to an nullpointer
    this->resizeFunc = NULL;
    //say that no draw function is bound
    this->hasResizeFunc = false;
}

void GLGEWindow::clearInitFunc()
{
    //set the draw function to an nullpointer
    this->initFunc = NULL;
    //say that no draw function is bound
    this->hasInitFunc = false;
}

bool GLGEWindow::getHasDrawFunc()
{
    //return if a draw function is bound
    return this->hasDrawFunc;
}

bool GLGEWindow::getHasTickFunc()
{
    //return if a tick function is bound
    return this->hasTickFunc;
}

bool GLGEWindow::getHasResizeFunc()
{
    //return if a resize function is bound
    return this->hasResizeFunc;
}

bool GLGEWindow::getHasInitFunc()
{
    //return if a resize function is bound
    return this->hasInitFunc;
}

void GLGEWindow::start()
{
    //add the inputed pointer
    glgeWindows[this->id-glgeWindowIndexOffset] = this;
    //increase the amount of active GLGE windows
    glgeActiveWindows++;
    //bind this to the current window
    this->makeCurrent();
    //call the init function
    this->callInitFunc();
}

vec2 GLGEWindow::getPos()
{
    //return the current position
    return this->pos;
}

vec2 GLGEWindow::getSize()
{
    //return the size
    return this->size;
}

void* GLGEWindow::getSDLWindow()
{
    //return the current window pointer
    return this->window;
}

void* GLGEWindow::getGLContext()
{
    //return the OpenGL context
    return this->glContext;
}

void GLGEWindow::setClearColor(vec3 clearColor, bool normalise)
{
    //pass to the other function
    this->setClearColor(clearColor.x, clearColor.y, clearColor.z, normalise);
}

void GLGEWindow::setClearColor(float r, float g, float b, bool normalise)
{
    //check for errors in the input
    bool error = false;
    //check if the values are negative
    if (r < 0 || g < 0 || b < 0)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_NEGAITVE_COLOR_VAL);
        }
        error = true;
    }

    //if the values need to be normalised, normalise them
    if (normalise)
    {
        r /= 255.f;
        g /= 255.f;
        b /= 255.f;
    }

    //check for out of bound values
    if (r > 1.f || g > 1.f || b > 1.f)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_COLOR_VALUE_OUT_OF_BOUNDS);
        }
        error = true;
    }

    //if an error occured, exit
    if (error)
    {
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_SET_CLEAR_COLOR << "\n";
        }
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //store the clear color
    this->clear = vec3(r,g,b);

    //disable the skybox
    this->useSkybox = false;
}

vec3 GLGEWindow::getClearColor()
{
    //return the clear color
    return this->clear;
}

void GLGEWindow::setSkybox(const char* top, const char* bottom, const char* left, const char* right, const char* front, const char* back)
{
    //variables to store the image data
    //the image width
    int width;
    //the image height
    int height;
    //the amount of channels in the image
    int nrChannels;
    //the image data
    unsigned char *data;

    //activate the first texture unit
    glActiveTexture(GL_TEXTURE0);
    //create a new skybox texture
    glGenTextures(1, &this->skyboxTex);
    //store the texture mode
    int texMode = GL_RGB;
    //bind the texture as a cube map
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyboxTex);
    //load the top image
    data = glgeLoad(top, &width, &height, &nrChannels);
    //check if the width is valide
    if (width > GL_MAX_TEXTURE_SIZE)
    {
        //check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] the skybox texture was too larg: specified size was " << width << ", mixmal allowed size is " << GL_MAX_TEXTURE_BUFFER_SIZE << "\n";
        }
        //check if GLGE should crash on an error
        if (glgeExitOnError)
        {
            //close the program with an error
            exit(1);
        }
    }
    //check if the height is valide
    if (height > GL_MAX_TEXTURE_SIZE)
    {
        //check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] the skybox texture was too larg: specified size was " << height << ", mixmal allowed size is " << GL_MAX_TEXTURE_BUFFER_SIZE << "\n";
        }
        //check if GLGE should crash on an error
        if (glgeExitOnError)
        {
            //close the program with an error
            exit(1);
        }
    }
    //check if a alpha channel exists
    if (nrChannels == 4)
    {
        //set the texture mode to also use an alpha channel
        texMode = GL_RGBA;
    }
    else
    {
        //set the texture mode to only rgb
        texMode = GL_RGB;
    }
    //load the image to y-positive
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 
                0, texMode, width, height, 0, texMode, GL_UNSIGNED_BYTE, data);
    //clear the texture data
    glgeImageFree(data);
    //load the bottom image
    data = glgeLoad(bottom, &width, &height, &nrChannels);
    //load the image to y-negative
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 
        0, texMode, width, height, 0, texMode, GL_UNSIGNED_BYTE, data);
    //clear the texture data
    glgeImageFree(data);
    //load the left image
    data = glgeLoad(left, &width, &height, &nrChannels);
    //load the image to x-negative
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 
        0, texMode, width, height, 0, texMode, GL_UNSIGNED_BYTE, data);
    //clear the texture data
    glgeImageFree(data);
    //load the right image
    data = glgeLoad(right, &width, &height, &nrChannels);
    //load the image to x-positive
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
        0, texMode, width, height, 0, texMode, GL_UNSIGNED_BYTE, data);
    //clear the texture data
    glgeImageFree(data);
    //load the front image
    data = glgeLoad(front, &width, &height, &nrChannels);
    //load the image to z-positive
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 
        0, texMode, width, height, 0, texMode, GL_UNSIGNED_BYTE, data);
    //clear the texture data
    glgeImageFree(data);
    //load the back image
    data = glgeLoad(back, &width, &height, &nrChannels);
    //load the image to z-negative
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 
        0, texMode, width, height, 0, texMode, GL_UNSIGNED_BYTE, data);
    //clear the texture data
    glgeImageFree(data);

    //set the default texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);
    //but watch out to set them to 3D
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //unbind the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    //say that the skybox is active
    this->useSkybox = true;
}

void GLGEWindow::moveFunc(int x, int y)
{

    //check if it is allowed to move the window
    if (this->allowWindowMovement)
    {
        //if it is allowed, store the new window position
        this->pos = vec2(x,y);
    }

}

void GLGEWindow::resizeWindow(int width, int height)
{
    //update the internal parameters
    //recalculate the window aspect ratio
    this->aspect = ((float)width/(float)height);
    //updated the stored window size
    this->size = vec2(width,height);

    //check if the window is in fullscreen mode
    if (!this->fullscreen)
    {
        //if it is not currently fullscreen mode, store the window size in the normal window size variable too
        this->normSize = this->size;
    }
    
    //update all the renderbuffer sizes

    //update the window size of the frame buffer
    glBindRenderbuffer(GL_RENDERBUFFER, this->mainRenderbuffer);
    //setup the storage for the render buffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->size.x, this->size.y);

    //unbind the renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //update the render texture parameters
    glBindTexture(GL_TEXTURE_2D, this->mainAlbedoTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);

    //update all the texture sizes
    //update the normal texture parameters
    glBindTexture(GL_TEXTURE_2D, this->mainNormalTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);

    //update the position texture parameters
    glBindTexture(GL_TEXTURE_2D, this->mainPosTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);

    //update the roughness texture parameters
    glBindTexture(GL_TEXTURE_2D, this->mainRMLTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);

    //update the depth and alpha texture parameters
    glBindTexture(GL_TEXTURE_2D, this->mainEIDATex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);

    //update the solid lit texture
    glBindTexture(GL_TEXTURE_2D, this->mainSolidTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);

    //update the transparent accumulation texture
    glBindTexture(GL_TEXTURE_2D, this->mainTransparentAccumTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->size.x, this->size.y, 0, GL_RGBA, GL_FLOAT, NULL);

    //update the last tick texture
    glBindTexture(GL_TEXTURE_2D, this->lastTickTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);

    //update the lighting texture
    glBindTexture(GL_TEXTURE_2D, this->mainOutTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);

    //update the post processing texture
    glBindTexture(GL_TEXTURE_2D, this->postProcessingTex);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->size.x, this->size.y, 0, GL_RGB, GL_FLOAT, NULL);

    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //update the OpenGL viewport
    glViewport(0,0, width,height);
}

bool GLGEWindow::getLightingUniforms()
{
    //let the error messages set for the albedo map, you want the albedo map in most cases
    this->albedoInLightingPass = getUniformVar(this->lightingShader, "glgeAlbedoMap");
    //next, turn the errors temporarely off
    //get if the errors are turned on
    bool error = glgeGetErrorOutput();
    //turn the errors of
    glgeSetErrorOutput(false);
    //get the unform for the normal map
    this->normalInLightingPass = getUniformVar(this->lightingShader, "glgeNormalMap");
    //get the uniform for the position map
    this->positionInLightingPass = getUniformVar(this->lightingShader, "glgePositionMap");
    //get the uniform for the roughness map
    this->roughnessInLightingPass = getUniformVar(this->lightingShader, "glgeRoughnessMap");
    
    //reset the uniform list
    this->lightColInLightingPass.clear();
    this->lightIntInLightingPass.clear();
    this->lightPosInLightingPass.clear();

    //set the prefix for the light position
    std::string prefixLightPos = std::string("glgeLightPos[");
    //set the prefix for the light color
    std::string prefixLightCol = std::string("glgeLightColor[");
    //set the prefix for the light intensity
    std::string prefixLightInt = std::string("glgeLightInt[");

    for (int i = 0; i < (int)this->lights.size(); i++)
    {
        //get the uniform for the light color
        this->lightColInLightingPass.push_back(getUniformVar(this->lightingShader, (prefixLightCol + std::to_string(i) + std::string("]")).c_str()));
        //get the uniform for the light intensity
        this->lightIntInLightingPass.push_back(getUniformVar(this->lightingShader, (prefixLightInt + std::to_string(i) + std::string("]")).c_str()));
        //get the uniform for the light position
        this->lightPosInLightingPass.push_back(getUniformVar(this->lightingShader, (prefixLightPos + std::to_string(i) + std::string("]")).c_str()));
    }

    //get the uniform for the amount of active lights
    this->activeLightInLightingPass = getUniformVar(this->lightingShader, "glgeActiveLights");
    //get the uniform for the camera position
    this->camPosInLightingPass = getUniformVar(this->lightingShader, "glgeCameraPos");
    //get the uniform for the far plane
    this->farPlaneInLightingPass = getUniformVar(this->lightingShader, "glgeFarPlane");
    //get the camera rotation matrix in the lighting shader
    this->rotInLightingPass = getUniformVar(this->lightingShader, "glgeCameraRot");
    //get the camera projection matrix in the lighting shader
    this->projInLightingPass = getUniformVar(this->lightingShader, "glgeProject");
    //reset the error output
    glgeSetErrorOutput(error);

    //if the albedo LightingPass is -1, then there is no albedo map, so return false
    return (this->albedoInLightingPass != -1);
}

void GLGEWindow::getDefaultUniformsFromPostProcessingShader(Shader* shader)
{
    //check if the current pass is the first
    if (this->firstPPSPass)
    {
        //if it is, get the main image variable from the shader to the lighting texture
        shader->setCustomTexture("glgeMainImage", this->mainOutTex);
    }
    else
    {
        //else, get the main image variable from the shader to the post processing texture
        shader->setCustomTexture("glgeMainImage", this->postProcessingTex);
    }
    //get the position map from the post processing shader
    shader->setCustomTexture("glgeAlbedoMap", this->mainAlbedoTex);
    //get the position map from the post processing shader
    shader->setCustomTexture("glgeNormalMap", this->mainNormalTex);
    //get the position map from the post processing shader
    shader->setCustomTexture("glgePositionMap", this->mainPosTex);
    //get the position map from the post processing shader
    shader->setCustomTexture("glgeRoughnessMap", this->mainRMLTex);
    //pass the window size to the shader
    shader->setCustomVec2("glgeWindowSize", this->size);
}

float GLGEWindow::getWindowAspect()
{
    //return the aspect ratio
    return this->aspect;
}

void GLGEWindow::enableBackfaceCulling()
{
    //check if the window is currently drawing
    if (this->drawing)
    {
        //if it is, update the backface culling imediatly
        glEnable(GL_CULL_FACE);
    }
    //say that face culling is enabled
    this->backfaceCulling = false;
}

void GLGEWindow::disableBackfaceCulling()
{
    //check if the window is currently drawing
    if (this->drawing)
    {
        //if it is, update the backface culling imediatly
        glDisable(GL_CULL_FACE);
    }
    //say that face culling is disabled
    this->backfaceCulling = false;
}

bool GLGEWindow::useBackfaceCulling()
{
    //return if the window is using backface culling
    return this->backfaceCulling;
}

void GLGEWindow::setLightingShader(const char* shader)
{
    //create strings for the shader
    std::string data;

    //read the files
    if (!readFile(shader, data))
    {
        //output an error message
        if (glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_OBJECT_COMPILE_SHADERS << "\n";
        }
        //stop the script
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //compile the shader and save it
    this->lightingShader = glgeCompileShader(GLGE_EMPTY_VERTEX_SHADER, data);

    //get the post processing uniforms
    bool albedo = this->getLightingUniforms();

    //if the albedo wasn't found, print out the filename
    if (!albedo)
    {
        printf(GLGE_ERROR_OCCURED_IN_FILE, shader);
    }
}

void GLGEWindow::setLightingShader(std::string shader)
{
    //compile the shader and save it
    this->lightingShader = glgeCompileShader(GLGE_EMPTY_VERTEX_SHADER, shader);

    //get the post processing uniforms
    bool albedo = this->getLightingUniforms();

    //if the albedo wasn't found, print out the filename
    if (!albedo)
    {
        printf("[GLGE ERROR INFO] Error occured in shader compiled from custom source");
    }
}

void GLGEWindow::setLightingShader(unsigned int shader)
{
    //store the inputed shader
    this->lightingShader = shader;

    //get the post processing uniforms
    bool albedo = this->getLightingUniforms();

    //if the albedo wasn't found, print out the filename
    if (!albedo)
    {
        printf("[GLGE ERROR INFO] Error occured in shader set by shader ID. \n                 ID : %d\n", shader);
    }
}

unsigned int GLGEWindow::getAlbedoTex()
{
    //return the albedo texture
    return this->mainAlbedoTex;
}

unsigned int GLGEWindow::getNormalTex()
{
    //return the normal texture
    return this->mainNormalTex;
}

unsigned int GLGEWindow::getPosTex()
{
    //return the position texture
    return this->mainPosTex;
}

unsigned int GLGEWindow::getRMLTex()
{
    //return the rml texture
    return this->mainRMLTex;
}

unsigned int GLGEWindow::getOutTex()
{
    //return the output texture+
    return this->mainOutTex;
}

unsigned int GLGEWindow::getEIDATex()
{
    //return the eida texture
    return this->mainEIDATex;
}

unsigned int GLGEWindow::getLastFrame()
{
    //return the last frame texture
    return this->lastTickTex;
}

void GLGEWindow::setFullscreenMode(bool isFullscreen)
{
    //store the inputed variable to say if should be in fullscreen mode
    this->fullscreen = isFullscreen;

    //check if it should be fullscreen
    if (isFullscreen)
    {
        //store the current window size
        this->normSize = this->size;
        //set the screen size to the complete screen
        SDL_SetWindowSize((SDL_Window*)this->window, glgeMainDisplay.w, glgeMainDisplay.h);
        //use SDL to set the fullscreen
        SDL_SetWindowFullscreen((SDL_Window*)this->window, SDL_WINDOW_FULLSCREEN);
        //resize everything
        this->resizeWindow(glgeMainDisplay.w, glgeMainDisplay.h);
    }
    else
    {
        //use SDL to set the window to the original size
        SDL_SetWindowFullscreen((SDL_Window*)this->window, 0);
        //set the screen size to the complete screen
        SDL_SetWindowSize((SDL_Window*)this->window, this->normSize.x, this->normSize.y);
        //resize everything
        this->resizeWindow(this->normSize.x, this->normSize.y);
    }
}

void GLGEWindow::setCamera(Camera* cam)
{
    //store the inputed camera
    this->mainCamera = cam;
    //say the camera that it is bound to this window
    cam->setWindowIndex(this->id-1);
}

Camera* GLGEWindow::getCamera()
{
    //return the camera
    return this->mainCamera;
}

void GLGEWindow::bindLight(Light* l)
{
    //store the inputed light source, if it isn't NULL
    if (l != nullptr)
    {
        this->lights.push_back(l);
    }

    //if the amount of bound light sources is bigger than the maximum supported light sources
    if ((int)this->lights.size() > GLGE_LIGHT_SOURCE_MAX)
    {
        //write a warning if warning printing is enabled
        if (glgeWarningOutput)
        {
            std::cout << GLGE_WARNING_MORE_LIGHTS_THAN_DEFAULT_SHADER_SUPPORT << (int)this->lights.size() << "\n";
        }
        //remove the last element
        this->lights.pop_back();
        //stop the function
        return;
    }

    //get the needed uniforms
    this->getLightingUniforms();
}

std::vector<Light*> GLGEWindow::getLights()
{
    //return all lights
    return this->lights;
}

unsigned int GLGEWindow::getScreenVBO()
{
    //return the screen VBO
    return this->screenRectVBO;
}

int GLGEWindow::addPostProcessingShader(Shader* shader)
{
    //store the shader
    this->ppsShaderStack.push_back(shader);
    //check if the shader contains the main image
    if (glgeGetUniformVar(shader->getShader(), "glgeMainImage") == -1)
    {
        //check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] uniform \"glgeMainImage\" is undefined for post-processing shader passed by pointer" << "\n";
        }
        //check if glge should exit on errors
        if (glgeExitOnError)
        {
            //stop the program with an error
            exit(1);
        }
        //stop the function
        return -1;
    }
    //setup all default uniforms
    getDefaultUniformsFromPostProcessingShader(shader);
    //store the current error output state
    bool err = glgeGetErrorOutput();
    //disable the error output
    glgeSetErrorOutput(false);
    //update the shader
    shader->recalculateUniforms();
    //reset error output
    glgeSetErrorOutput(err);
    //return the index
    return (int)this->ppsShaderStack.size() - 1;
}

Shader* GLGEWindow::addPostProcessingShader(const char* file)
{
    //create the new shader
    Shader* shader = new Shader(GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER, file);
    //check if the shader contains the main image
    if (glgeGetUniformVar(shader->getShader(), "glgeMainImage") == -1)
    {
        //check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] File " << file << 
            " was inputed as an post-processing shader, but the uniform \"glgeMainImage\" for the input of the image is undefined" << "\n";
        }
    }
    //store a new shader in the post-processing stack
    this->ppsShaderStack.push_back(shader);
    //setup all default uniforms
    getDefaultUniformsFromPostProcessingShader(shader);
    //store the current error output state
    bool err = glgeGetErrorOutput();
    //disable the error output
    glgeSetErrorOutput(false);
    //update the shader
    shader->recalculateUniforms();
    //reset error output
    glgeSetErrorOutput(err);
    //output the shader
    return shader;
}

Shader* GLGEWindow::addPostProcessingShader(std::string sc)
{
    //create the new shader
    Shader* shader = new Shader(GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER, sc);
    //check if the shader contains the main image
    if (glgeGetUniformVar(shader->getShader(), "glgeMainImage") == -1)
    {
        //check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] uniform \"glgeMainImage\" is undefined for post-processing shader" << "\n" << 
            "Shader source: " << "\n" << sc << "\n";
        }
        //check if glge should exit on errors
        if (glgeExitOnError)
        {
            //stop the program with an error
            exit(1);
        }
        //stop the function
        return NULL;
    }
    //store a new shader in the post-processing stack
    this->ppsShaderStack.push_back(shader);
    //setup all default uniforms
    getDefaultUniformsFromPostProcessingShader(shader);
    //store the current error output state
    bool err = glgeGetErrorOutput();
    //disable the error output
    glgeSetErrorOutput(false);
    //update the shader
    shader->recalculateUniforms();
    //reset error output
    glgeSetErrorOutput(err);
    //output the shader
    return shader;
}

Shader* GLGEWindow::addPostProcessingShader(unsigned int s)
{
    //create the new shader
    Shader* shader = new Shader(s);
    //check if the shader contains the main image
    if (glgeGetUniformVar(shader->getShader(), "glgeMainImage") == -1)
    {
        //check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] uniform \"glgeMainImage\" is undefined for post-processing shader passed by shader pointer" << "\n";
        }
        //check if glge should exit on errors
        if (glgeExitOnError)
        {
            //stop the program with an error
            exit(1);
        }
        //stop the function
        return NULL;
    }
    //store a new shader in the post-processing stack
    this->ppsShaderStack.push_back(shader);
    //setup all default uniforms
    getDefaultUniformsFromPostProcessingShader(shader);
    //store the current error output state
    bool err = glgeGetErrorOutput();
    //disable the error output
    glgeSetErrorOutput(false);
    //update the shader
    shader->recalculateUniforms();
    //reset error output
    glgeSetErrorOutput(err);
    //output the shader
    return shader;
}

int GLGEWindow::addPostProcessingFunction(Shader (*func)(unsigned int))
{
    //check if the function is a nullpointer
    if (func == NULL)
    {
        //check if error should print
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] can't add a nullpointer as an post processing function\n";
        }
        //check if GLGE should exit on an error
        if (glgeExitOnError)
        {
            //stop the program
            exit(1);
        }
        //stop the function
        return -1;
    }
    //store the new post processing function
    this->ppsFunctionStack.push_back(func);
    //return the index
    return (int)this->ppsFunctionStack.size() - 1;
}

void GLGEWindow::removePostProcessingShader(unsigned int index, bool del)
{
    //check if the index is in range
    if (index > (unsigned int)this->ppsShaderStack.size())
    {
        //check if error should print
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] index " << index << " is out of range of post processing shader list with length" << (int)this->ppsShaderStack.size() <<"\n";
        }
        //check if GLGE should exit on an error
        if (glgeExitOnError)
        {
            //stop the program
            exit(1);
        }
        //stop the function
        return;
    }
    if (del)
    {
        //store the pointer
        Shader* s = this->ppsShaderStack[index];
        //remove the shader from the stack
        this->ppsShaderStack.erase(this->ppsShaderStack.begin() + index);
        //delete the pointer
        delete s;
    }
    else
    {
        //remove the shader from the stack
        this->ppsShaderStack.erase(this->ppsShaderStack.begin() + index);
    }
}

void GLGEWindow::removePostProcessingShader(Shader* shader, bool del)
{
    //first, get the index
    int i = this->getPostProcessingShaderIndex(shader);
    //delete the shader
    this->removePostProcessingShader(i, del);
}

void GLGEWindow::removePostProcessingFunction(unsigned int index)
{
    //check if the index is in range
    if (index > (unsigned int)this->ppsFunctionStack.size())
    {
        //check if error should print
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] index " << index << " is out of range of post processing function list with length" << (int)this->ppsFunctionStack.size() <<"\n";
        }
        //check if GLGE should exit on an error
        if (glgeExitOnError)
        {
            //stop the program
            exit(1);
        }
        //stop the function
        return;
    }
    //delete the shader
    this->ppsFunctionStack.erase(this->ppsFunctionStack.begin() + index);
}

void GLGEWindow::removePostProcessingFunction(Shader (*func)(unsigned int))
{
    //get the index of the function
    int i = this->getPostProcessingFuncIndex(func);
    //delete the functoin
    this->removePostProcessingFunction(i);
}

Shader* GLGEWindow::getPostProcessingShader(unsigned int index)
{
    //check if the index is in range
    if (index > (unsigned int)this->ppsShaderStack.size())
    {
        //check if error should print
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] index " << index << " is out of range of post processing shader list with length" << (int)this->ppsShaderStack.size() <<"\n";
        }
        //check if GLGE should exit on an error
        if (glgeExitOnError)
        {
            //stop the program
            exit(1);
        }
        //stop the function
        return NULL;
    }
    //return the shader at the index
    return this->ppsShaderStack[index];
}

int GLGEWindow::getPostProcessingShaderIndex(Shader* shader)
{
    //find the index of the shader
    std::vector<Shader*>::iterator iter = std::find(this->ppsShaderStack.begin(), this->ppsShaderStack.end(), shader);
    //check if the element wasn't found
    if (iter == this->ppsShaderStack.cend())
    {
        //quit the function with -1
        return -1;
    }
    else
    {
        //return the index
        return std::distance(this->ppsShaderStack.begin(), iter);
    }
}

Shader (*GLGEWindow::getPostProcessingFunc(unsigned int index))(unsigned int)
{
    //check if the index is in range
    if (index > (unsigned int)this->ppsFunctionStack.size())
    {
        //check if error should print
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] index " << index << " is out of range of post processing function list with length" << (int)this->ppsFunctionStack.size() <<"\n";
        }
        //check if GLGE should exit on an error
        if (glgeExitOnError)
        {
            //stop the program
            exit(1);
        }
        //stop the function
        return NULL;
    }
    //return the function
    return this->ppsFunctionStack[index];
}

int GLGEWindow::getPostProcessingFuncIndex(Shader (*func)(unsigned int))
{
    //find the index of the shader
    std::vector<Shader (*)(unsigned int)>::iterator iter = std::find(this->ppsFunctionStack.begin(), this->ppsFunctionStack.end(), func);
    //check if the element wasn't found
    if (iter == this->ppsFunctionStack.cend())
    {
        //quit the function with -1
        return -1;
    }
    else
    {
        //return the index
        return std::distance(this->ppsFunctionStack.begin(), iter);
    }
}

bool GLGEWindow::isFullscreen()
{
    //return if the window is fullscreen
    return this->fullscreen;
}

void GLGEWindow::setResizable(bool resizable)
{
    //set the correct window resize mode
    SDL_SetWindowResizable((SDL_Window*)this->window, SDL_bool(resizable));
    //store the inputed mode
    this->allowWindowResize = resizable;
}

bool GLGEWindow::isWindowResizable()
{
    //output if the window is resizable
    return this->allowWindowResize;
}

void GLGEWindow::setMovable(bool movalbe)
{
    //store the boolean
    this->allowWindowMovement = movalbe;
}

bool GLGEWindow::isMovable()
{
    //return the boolean
    return this->allowWindowMovement;
}

void GLGEWindow::setWindowIcon(const char* file)
{
    //load the image
    SDL_Surface* img = loadImage(file);
    //check if the image was created sucessfully
    if (!img)
    {
        //check if GLGE should print an error
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] failed to create SDL_Surface for image to change the window icon, SDL error: " << SDL_GetError() << "\n";
        }
        //check if GLGE should crash on an error
        if (glgeExitOnError)
        {
            //stop with an error
            exit(1);
        }
    }
    //set the current application window's icon
    SDL_SetWindowIcon((SDL_Window*)this->window, img);
}

bool GLGEWindow::isTranparentPass()
{
    //return if this is the transparent pass
    return this->transparentPass;
}

void GLGEWindow::setTransparencyCombineShader(Shader* shader)
{
    //check if the inputed pointer is a null pointer
    if (shader == NULL)
    {
        //check if warning output is enabled
        if (glgeWarningOutput)
        {
            //print a warning
            printf("[GLGE WARNING] A null pointer is not a valid transparent combine shader\n");
        }
        //stop the function
        return;
    }
    //check if a custom shader is bound
    if (!this->customTransparentCombineShader)
    {
        //if not, delete the old shader
        delete this->transparentCombineShader;
    }
    //store the shader
    this->transparentCombineShader = shader;
    //say that a custom shader is bound
    this->customTransparentCombineShader = true;
}

Shader* GLGEWindow::getTransparencyCombineShader()
{
    //return the transpranet combine shader if it exists, else, return a nullpointer
    return this->customTransparentCombineShader ? this->transparentCombineShader : NULL;
}

bool GLGEWindow::isCustomTransparencyCombineShader()
{
    //return if this is a custom transparent combine shader
    return this->customTransparentCombineShader;
}

void GLGEWindow::setBorderVisible(bool visible)
{
    //store the new window border state
    this->borderIsVisible = visible;
    //update the window border
    SDL_SetWindowBordered((SDL_Window*)this->window, SDL_bool(visible));
}

bool GLGEWindow::isBorderVisible()
{
    //return if the border is visible
    return this->borderIsVisible;
}

void GLGEWindow::setWindowForceOpen(bool forceOpen)
{
    //store if the window is force opend
    this->forceOpen = forceOpen;
}

bool GLGEWindow::isWindowForceOpen()
{
    //return if the window is forced to stay open
    return this->forceOpen;
}

void GLGEWindow::initiateClosing(bool force)
{
    //check if this function should run
    if ((this->opClose) || ((!force) && !this->opClose))
    {
        //stop this function
        return;
    }
    //say that the window won't be closed
    this->opClose = true;
}

bool GLGEWindow::isClosingInitiated()
{
    //return if closing is initiated
    return this->opClose;
}

void GLGEWindow::stopWindowClosing(bool force)
{
    //check if this function should run
    if ((!this->opClose) || ((!force) && this->opClose))
    {
        //stop this function
        return;
    }
    //say that the window won't be closed
    this->opClose = false;
}

void GLGEWindow::setWindowAlwaysOnTop(bool onTop)
{
    //store the new state
    this->alwaysOnTop = onTop;
    //update the window
    SDL_SetWindowAlwaysOnTop((SDL_Window*)this->window, SDL_bool(onTop));
}

bool GLGEWindow::isWindowAlwaysOnTop()
{
    //return if the window is always on top
    return this->alwaysOnTop;
}

void GLGEWindow::setBrightness(float brightness)
{
    //check if the brightness is less than 0
    if (brightness < 0.f)
    {
        //check if a warning should be printed
        if (glgeWarningOutput)
        {
            //print a warning
            printf("[GLGE WARNING] a brightness of %f is not allowed, the brightness must be at least 0.f\n", brightness);
        }
        //stop this function
        return;
    }
    //store the new window brightness
    this->brightness = brightness;
    //update the window brightness
    SDL_SetWindowBrightness((SDL_Window*)this->window, brightness);
}

float GLGEWindow::getBrightness()
{
    //return the current brightness
    return this->brightness;
}

void GLGEWindow::showHide(bool showHide)
{
    //store the new show/hide state
    this->shown = showHide;
    //update the window
    if (showHide)
    {
        //show the window
        SDL_ShowWindow((SDL_Window*)this->window);
    }
    else
    {
        //hide the window
        SDL_HideWindow((SDL_Window*)this->window);
    }
}

bool GLGEWindow::isShown()
{
    //return if the window is shown
    return this->shown;
}

void GLGEWindow::maximize(bool maximized, bool force)
{
    //check if this function should execute
    if (!this->allowWindowResize && !force)
    {
        //stop this function
        return;
    }
    //store the new state
    this->maximized = maximized;
    //say that the window can't be minimized any more
    this->minimized = false;
    //maximize or restore the window
    if (maximized)
    {
        //maximize the window
        SDL_MaximizeWindow((SDL_Window*)this->window);
    }
    else
    {
        //minize the window
        SDL_RestoreWindow((SDL_Window*)this->window);
    }
    //store the width of the window
    int w = 0;
    //store the height of the window
    int h = 0;
    //get the size of the window
    SDL_GetWindowSize((SDL_Window*)this->window, &w, &h);
    //call the resize function
    this->resizeWindow(w,h);
}

bool GLGEWindow::isMaximized()
{
    //return if the window is maximized
    return this->maximized;
}

void GLGEWindow::minimize(bool min, bool force)
{
    //check if this function should execute
    if (!this->allowWindowResize && !force)
    {
        //stop this function
        return;
    }
    //store the new state
    this->minimized = min;
    //say that the window can't be maximized any more
    this->maximized = false;
    //restore or minize the window
    if (min)
    {
        //maximize the window
        SDL_MinimizeWindow((SDL_Window*)this->window);
    }
    else
    {
        //minize the window
        SDL_RestoreWindow((SDL_Window*)this->window);
    }
}

bool GLGEWindow::isMinimized()
{
    //return if the window is minimized
    return this->minimized;
}

bool GLGEWindow::isFocused()
{
    //get the input flags and maks out the input focus, then return it as an bool
    return (bool)(SDL_GetWindowFlags((SDL_Window*)this->window) & SDL_WINDOW_INPUT_FOCUS);
}

bool GLGEWindow::isMouseFocused()
{
    //get the input flags and maks out the mouse focus, then return it as an bool
    return (bool)(SDL_GetWindowFlags((SDL_Window*)this->window) & SDL_WINDOW_MOUSE_FOCUS);
}

void GLGEWindow::setMouseGrabMode(bool grabbed)
{
    //update the intern mode
    this->mouseGrabbed = grabbed;
    //update the mode for the main window
    SDL_SetWindowMouseGrab((SDL_Window*)this->window, SDL_bool(grabbed));
}

bool GLGEWindow::getMouseGrabMode()
{
    //return the mouse grab mode
    return this->mouseGrabbed;
}

void GLGEWindow::focuse(bool moveUp)
{
    //check if the window should move up
    if (moveUp)
    {
        //use the function that moves the window up
        SDL_RaiseWindow((SDL_Window*)this->window);
    }
    else
    {
        //use the function that dosn't move the window up
        SDL_SetWindowInputFocus((SDL_Window*)this->window);
    }
}

void GLGEWindow::bindScreenRect()
{
    //check if the rect is bound
    if (this->screenRectBound)
    {
        //stop the function
        return;
    }
    //bind the vertex array
    glBindVertexArray(this->screenRectVAO);
    //bind the array buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->screenRectVBO);
    //activate the vertex attribute for the position
    glEnableVertexAttribArray(0);
    //load the position into the shader
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    //activate the vertex attrivute for the texture coordinate
    glEnableVertexAttribArray(1);
    //load the texture coordinate into the shader
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    //say that the screen rect is bound
    this->screenRectBound = true;
}

void GLGEWindow::unbindScreenRect()
{
    //check if the rect is unbound
    if (!this->screenRectBound)
    {
        //stop the function
        return;
    }
    //unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //deactivate the vertex attrib array pointers
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    //say that the screen rect is not bound
    this->screenRectBound = false;
}

void GLGEWindow::makeCurrent()
{
    //store the window ID as currently active by OpenGL
    glgeCurrentWindowIndex = this->id-glgeWindowIndexOffset;
    //bind the OpenGL context to the window
    SDL_GL_MakeCurrent((SDL_Window*)this->window, this->glContext);
}

int GLGEWindow::getDefault3DShader()
{
    //return the default shader
    return this->default3DShader;
}

int GLGEWindow::getDefault3DTransparentShader()
{
    //return the default shader
    return this->default3DTransShader;
}

int GLGEWindow::getDefault2DShader()
{
    //return the default shader
    return this->default2DShader;
}