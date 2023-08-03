/**
 * @file glgeDefaultFuncs.cpp
 * @author DM8AT
 * @brief Define the functions for SDL to execute
 * @version 0.1
 * @date 2023-02-09
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

//include the GLGE dependencys
#include "glgeErrors.hpp"
#include "GLGEKlasses.hpp"
#include "glgeVars.hpp"
#include "glgeFuncs.hpp"

//include math things
#include "GLGEMath.h"

//include the OpenGL dependencys
#include <GL/glew.h>
#include <GL/glu.h>

//include the needed default librarys
#include <iostream>
#include <cfloat>

//Private vars
bool draw = true;

///////////////////////////////////
//SUB FUNCTIONS FOR THE CALLBACKS//
///////////////////////////////////

void drawLightingPass()
{
    //bind the custom framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, glgeFBO);

    //only clear the first color buffer
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    //set the specified clear color
    glClearColor(glgeClearColor.x, glgeClearColor.y, glgeClearColor.z, glgeClearColor.w);

    //clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //specify the color buffers
    glDrawBuffers(glgeLenUsedColorBuffers, glgeUsedColorBuffers);

    //set the clear color to black
    glClearColor(-2,-2,-2,0);

    //clear the other buffers
    glClear(GL_COLOR_BUFFER_BIT);

    //specify the color buffers to draw into
    glDrawBuffers(glgeLenAllUsedColorBuffers, glgeAllUsedColorBuffers);

    //enable depth testing
    glEnable(GL_DEPTH_TEST);

    //call the custom drawing function
    if(glgeHasDisplayCallback)
    {
        ((void(*)())glgeDisplayCallback)();
    }

    //switch to the lighting framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, glgeLightingFBO);

    //clear the frame buffer
    glClear(GL_COLOR_BUFFER_BIT);

    //disable depth testing
    glDisable(GL_DEPTH_TEST);

    //check if lighting should be applied
    if ((int)glgeLights.size() != 0)
    {
        //if it needs, apply the lighting shader

        //bind the post processing shader
        glUseProgram(glgeLightingShader);
        //bind the vertex array
        glBindVertexArray(glgeScreenVAO);
        //bind the array buffer
        glBindBuffer(GL_ARRAY_BUFFER, glgeScreenVBO);
        //activate the vertex attribute for the position
        glEnableVertexAttribArray(0);
        //load the position into the shader
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        //activate the vertex attrivute for the texture coordinate
        glEnableVertexAttribArray(1);
        //load the texture coordinate into the shader
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        //activate the uniform texture array
        glActiveTexture(GL_TEXTURE0);
        //bind the framebuffer texture
        glBindTexture(GL_TEXTURE_2D, glgeFrameAlbedoMap);
        //activate the second texture unit
        glActiveTexture(GL_TEXTURE1);
        //bind the normal texture
        glBindTexture(GL_TEXTURE_2D, glgeFrameNormalMap);
        //activate the thired texture unit
        glActiveTexture(GL_TEXTURE2);
        //bind the position texture
        glBindTexture(GL_TEXTURE_2D, glgeFramePositionMap);
        //activate the fourth texture unit
        glActiveTexture(GL_TEXTURE3);
        //bind the roughness texture
        glBindTexture(GL_TEXTURE_2D, glgeFrameRoughnessMap);

        //pass the uniforms to the shader
        //pass the albedo map
        glUniform1i(glgeAlbedoInLightingPass, 0);
        //pass the normal map
        glUniform1i(glgeNormalInLightingPass, 1);
        //pass the position map
        glUniform1i(glgePositionInLightingPass, 2);
        //pass the roughness map
        glUniform1i(glgeRoughnessInLightingPass, 3);
        //check if a 3D camera is bound
        if (glgeMainCamera != NULL)
        {
            //pass the camera position
            glUniform3f(glgeCamPosInLightingPass, glgeMainCamera->getPos().x, glgeMainCamera->getPos().y, glgeMainCamera->getPos().z);
            //pass the far plane
            glUniform1f(glgeFarPlaneInLightingPass, glgeMainCamera->getFarPlane());
        }
        //pass all the lights to the shader
        //load all light positions to the shader
        for (int i = 0; i < (int)glgeLightPosInLightingPass.size(); i++)
        {
            //pass the light position
            glUniform3f(glgeLightPosInLightingPass[i], glgeLights[i]->getPos().x, glgeLights[i]->getPos().y, glgeLights[i]->getPos().z);
        }
        //load all light colors to the shader
        for (int i = 0; i < (int)glgeLightColInLightingPass.size(); i++)
        {
            //pass the light color
            glUniform3f(glgeLightColInLightingPass[i], glgeLights[i]->getColor().x, glgeLights[i]->getColor().y, glgeLights[i]->getColor().z);
        }
        //load all light intensitys to the shader
        for (int i = 0; i < (int)glgeLightIntInLightingPass.size(); i++)
        {
            //pass the light intensity
            glUniform1f(glgeLightIntInLightingPass[i], glgeLights[i]->getInsensity());
        }
        //pass the amount of active lights
        glUniform1i(glgeActiveLightInLightingPass, (int)glgeLights.size());

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
        //unbind the buffer
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //deactivate the vertex attrib array pointers
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
    else
    {
        //if not, copy the framebuffer over to save a draw pass
        //bind the default FBO for reading
        glBindFramebuffer(GL_READ_FRAMEBUFFER, glgeFBO);
        //bind the light framebuffer as writing
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glgeLightingFBO);
        //copy the framebuffers
        glBlitFramebuffer(0,0, glgeWindowSize.x, glgeWindowSize.y, 0,0, glgeWindowSize.x, glgeWindowSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

    //bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //clear the frame buffer
    glClear(GL_COLOR_BUFFER_BIT);

    //bind the post processing shader
    glUseProgram(glgePostProcessingShader);
    //bind the vertex array
    glBindVertexArray(glgeScreenVAO);
    //bind the array buffer
    glBindBuffer(GL_ARRAY_BUFFER, glgeScreenVBO);
    //activate the vertex attribute for the position
    glEnableVertexAttribArray(0);
    //load the position into the shader
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    //activate the vertex attrivute for the texture coordinate
    glEnableVertexAttribArray(1);
    //load the texture coordinate into the shader
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    //activate the uniform texture array
    glActiveTexture(GL_TEXTURE0);
    //bind the framebuffer texture
    glBindTexture(GL_TEXTURE_2D, glgeLightingImageOut);
    //pass the uniform to the framebuffer
    glUniform1i(glgeMainImageInPPS,0);

    //draw the screen
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //activate the thired texture unit
    glActiveTexture(GL_TEXTURE2);
    //unbind the shader
    glUseProgram(0);
    //unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //deactivate the vertex attrib array pointers
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    //copy the render to another fragment shader
    //bind the default framebuffer as read only
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    //bind the custom framebuffer as draw only
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glgeFBOLastTick);
    //copy the framebuffer
    glBlitFramebuffer(0,0, glgeWindowSize.x, glgeWindowSize.y, 0,0, glgeWindowSize.x, glgeWindowSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    //bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //bind the default texture unit
    glActiveTexture(GL_TEXTURE0);
    //bind the default texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

//////////////////////////
//Default GLGE callbacks//
//////////////////////////

//the default display callback for GLGE
void glgeDefaultDisplay()
{
    //say that the screen is currently drawn
    draw = false;

    //draw the scene for lighting
    drawLightingPass();

    //update the window
    SDL_GL_SwapWindow(glgeMainWindow);

    //say that the screen can be re-drawn
    draw = true;
}

//the default timer function for GLGE
void glgeDefaultTimer()
{
    //stop, if no window exists
    if (!glgeMainWindow)
    {
        return;
    }

    //update the time it took for one tick to occure in seconds
    glgeDeltaTime = (SDL_GetTicks() - glgeTickTime);

    //calculate the current frames per second
    glgeCurrentFPS = 1.f/((SDL_GetTicks() - glgeTickTime)/1000.f);

    //update the time since last tick
    glgeTickTime = SDL_GetTicks();

    //output all OpenGL errors
    GLenum error;
    //loop over all errors
    while((error = glGetError()) != GL_NO_ERROR)
    {
        //output the error and connect it to the OpenGL error
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_OPEN_GL_ERROR, getGLErrorString(error));
        }
        //stop the script
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //store the actual window width
    int w;
    //store the actual window height
    int h;
    //get the window size
    SDL_GetWindowSize(glgeMainWindow,&w,&h);

    //check if the window size is correct
    if ((glgeWindowSize.x != w) || (glgeWindowSize.y != h))
    {
        //check if a window resize is allowed
        if (!glgeAllowWindowResize)
        {
            //if a window resize is not allowed, reshape the window to the inputed valide size
            SDL_SetWindowSize(glgeMainWindow, glgeWindowSize.x, glgeWindowSize.y);
        }
    }

    //store the actual window width
    int x;
    //store the actual window height
    int y;
    //get the window size
    SDL_GetWindowPosition(glgeMainWindow,&x,&y);

    //check if the window was moved
    if ((glgeWindowPosition.x != x) || (glgeWindowSize.y != y))
    {
        //check if a window moevement is allowed
        if (!glgeAllowWindowMovement)
        {
            //if window movement is not allowed, move the window back
            SDL_SetWindowPosition(glgeMainWindow,glgeWindowPosition.x, glgeWindowPosition.y);
        }
    }

    //get the active mouse position
    SDL_GetMouseState(&x,&y);

    //store the mouse position
    glgeMouse.pos = vec2(x/glgeWindowSize.x,y/glgeWindowSize.y);
    //store the pixel the mouse is on
    glgeMouse.posPixel = vec2(x,y);

    //get the current display
    SDL_GetCurrentDisplayMode(0, &glgeMainDisplay);

    //if GLGE has an additional main function, call it
    if(glgeHasMainCallback)
    {
        ((void(*)())glgeMainCallback)();
    }

    //set the mouse wheel to 0
    glgeMouse.mouseWeel = 0;

    //clear the key from the last tick
    glgeKeysThisTick.clear();
    //clear the releasd keys
    glgeKeysRelesdThisTick.clear();

    //calculate the time to wait
    int waitTime = std::floor((1000.f/glgeMaxFPS) - (1000.f/glgeTickTime));

    //check if the wait time is negative
    if (waitTime < 0)
    {
        //if it is, set it to 0
        waitTime = 0;
    }

    //limit the framerate
    SDL_Delay(waitTime);
}

//default keyboard function
void glgeDefaultKeyFunc(int key)
{
    //check if the key was allready pressed
    if (!glgeKeysRelesdThisTick.getKey(key))
    {
        //store the pressed key in the variable for one tick
        glgeKeysThisTick.keyUpdate(key, true);
    }

    //store the pressed key
    glgePressedKeys.keyUpdate(key, true);
}

//default keyboard up function
void glgeDefaultKeyUpFunc(int key)
{
    //store wich key is no longer pressed
    glgePressedKeys.keyUpdate(key, false);

    //store which key is relesd
    glgeKeysRelesdThisTick.keyUpdate(key, true);
}

void glgeDefaultMouseFunc(int button, int state)
{
    //update the mouse
    glgeMouse.update(button, state);
}

void glgeDefaultResizeFunc(int width, int height)
{
    //check if glge is allowed to change the window size
    if (glgeAllowWindowResize)
    {
        //if the window can change size, update the size
        resizeWindow(width, height);
    }

    //after that, call the customizable function that should be called on resize
    //check if the function is a nullpointer
    if (!(glgeOnWindowResize == nullptr))
    {
        //if it is not a nullpointer, call the function
        ((void(*)(int,int))glgeOnWindowResize)(width, height);
    }
}

void glgeDefaultMoveFunc(int x, int y)
{
    //check if it is allowed to move the window
    if (glgeAllowWindowMovement)
    {
        //if it is allowed, store the new window position
        glgeWindowPosition = vec2(x,y);
    }
}