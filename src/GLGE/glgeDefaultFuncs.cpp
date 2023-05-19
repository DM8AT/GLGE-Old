/**
 * @file glgeDefaultFuncs.cpp
 * @author DM8AT
 * @brief Define the functions for FreeGLUT to execute
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
#include <GL/freeglut.h>

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

    //clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //enable depth testing
    glEnable(GL_DEPTH_TEST);

    //a list of the active color buffers
    //GLenum usedColorBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

    //specify the number of used color buffers
    //glDrawBuffers(sizeof(usedColorBuffers) / sizeof(usedColorBuffers[0]), usedColorBuffers);

    //call the custom drawing function
    if(glgeHasDisplayCallback)
    {
        ((void(*)())glgeDisplayCallback)();
    }

    //write the data from the multi sample buffer to the post pocessing buffer
    glBlitFramebuffer(0,0, glgeWindowSize.x, glgeWindowSize.y, 0,0, glgeWindowSize.x, glgeWindowSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    //specify that the used color buffer is 0
    //glDrawBuffer(GL_NONE);

    //switch to the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //disable depth testing
    glDisable(GL_DEPTH_TEST);

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
    glBindTexture(GL_TEXTURE_2D, glgeFrameBufferTexture);
    //activate the second texture unit
    glActiveTexture(GL_TEXTURE1);
    //bind the normal texture
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, glgeFrameBufferNormalTexture);
    //draw the screen
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //unbind the normal texture
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    //activate the first texture unit
    glActiveTexture(GL_TEXTURE0);
    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    //unbind the shader
    glUseProgram(0);
    //unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void shadowPass(int index, mat4 projMat)
{
    //get the current light        //cast it to an light object    
    Light light = glgeLights[index][0];

    //calculate all 6 matrices for rotation & projection
    mat4 shadowTransform[] = {
        projMat * glgeLookAt(light.getPos(), light.getPos() + vec3( 1, 0, 0), vec3(0,-1,0)),
        projMat * glgeLookAt(light.getPos(), light.getPos() + vec3(-1, 0, 0), vec3(0,-1,0)),
        projMat * glgeLookAt(light.getPos(), light.getPos() + vec3( 0, 1, 0), vec3(0,0, 1)),
        projMat * glgeLookAt(light.getPos(), light.getPos() + vec3( 0,-1, 0), vec3(0,0,-1)),
        projMat * glgeLookAt(light.getPos(), light.getPos() + vec3( 0, 0, 1), vec3(0,-1,0)),
        projMat * glgeLookAt(light.getPos(), light.getPos() + vec3( 0, 0,-1), vec3(0,-1,0))
    };

    //resize the viewport
    glViewport(0,0,glgeShadowMapResolution, glgeShadowMapResolution);

    //bind the FBO from the light source
    light.bindShadowMap();

    //bind the shadow mapping shader
    glUseProgram(glgeShadowShader.getShader());

    //pass the shadow transformation matrices to the shader
    glUniformMatrix4fv(glgeShadowMatShadowLoc, 6, GL_FALSE, &shadowTransform[0].m[0][0]);
    //pass the far cliping plane to the shader
    glUniform1f(glgeFarShadowLoc, glgeMainCamera->getFarPlane());
    //pass the location of the light to the shader
    glUniform3f(glgeLightPosShadowLoc, light.getPos().x, light.getPos().y, light.getPos().z);

    //draw the scene
    if(glgeHasDisplayCallback)
    {
        ((void(*)())glgeDisplayCallback)();
    }

    //unbind the shader program
    glUseProgram(0);
}

void drawShadowPass()
{
    //define the tangens for the half fov to an constant (FOV = 90)
    const float tHF = 1.619775f;
    //get the current window aspect
    const float ar = glgeWindowAspect;
    //define the near cliping plane to 0.1
    const float near = 0.001;
    //get the far cliping plane from the camera
    const float far = glgeMainCamera->getFarPlane();
    //calculate the range of the near and far plane
    const float zRange = far - near;
    //calculate the projection matrix for the camera
    mat4 projMat = mat4(1.f/(tHF*ar), 0.f, 0.f, 0.f,
                        0.f, 1/tHF, 0.f, 0.f,
                        0.f, 0.f, (-near - far)/zRange, (2.f*far * near) / zRange,
                        0.f, 0.f, 1.f, 0.f);
    
    //set the clear color to the maximum of floats
    glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

    //say that the shadow pass is drawing
    glgeIsShadowPass = true;

    //loop over all existing light sources
    for (int i = 0; i < (int)glgeLights.size(); i++)
    {
        shadowPass(i, projMat);
    }

    //set the clear color to the default clear color
    glClearColor(glgeClearColor.x, glgeClearColor.y, glgeClearColor.z, glgeClearColor.w);

    //reset the window size
    glViewport(0,0,glgeWindowSize.x, glgeWindowSize.y);

    //unbind the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //end the shadow pass
    glgeIsShadowPass = false;
}

//////////////////////////
//Default GLGE callbacks//
//////////////////////////

//the default display callback for GLGE
void glgeDefaultDisplay()
{
    //say that the screen is currently drawn
    draw = false;

    //SHADOWS DON'T WORK IN THE MOMENT
    //check for bound lights
    /*if (glgeLights.size() != 0)
    {
        //calculate all shadows
        drawShadowPass();
    }*/

    //draw the scene for lighting
    drawLightingPass();

    //update the window
    glutSwapBuffers();

    //say that the screen can be re-drawn
    draw = true;
}

//the default timer function for GLGE
void glgeDefaultTimer(int)
{
    //stop, if no window exists
    if (glutGetWindow() == 0)
    {
        return;
    }
    
    //initalise the redrawing of the screen
    glutPostRedisplay();

    //update the time since last tick
    glgeDeltaTime = (glutGet(GLUT_ELAPSED_TIME) - glgeTickTime);
    
    //update the start time of the tick
    glgeTickTime = glutGet(GLUT_ELAPSED_TIME);

    //calculate the current frames per second
    glgeCurrentFPS = 1000/glgeDeltaTime;

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
        exit(1);
    }

    //recalculate the window aspect
    glgeWindowAspect = ((float)glutGet(GLUT_WINDOW_WIDTH))/((float)glutGet(GLUT_WINDOW_HEIGHT));

    //if GLGE has an additional main function, call it
    if(glgeHasMainCallback)
    {
        ((void(*)())glgeMainCallback)();
    }

    //update the stored window size
    glgeWindowSize = vec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

    //update the window size of the frame buffer
    glBindRenderbuffer(GL_RENDERBUFFER, glgeRBO);
    //setup the storage for the render buffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, glgeWindowSize.x, glgeWindowSize.y);

    //unbind the renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //update the render texture parameters
    glBindTexture(GL_TEXTURE_2D, glgeFrameBufferTexture);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, glgeWindowSize.x, glgeWindowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //set the mouse wheel to 0
    glgeMouse.mouseWeel = 0;

    //recall this function so that it is calld {glgeMaxFPS} times a second
    glutTimerFunc(1000/glgeMaxFPS,glgeDefaultTimer,0);
}

//default keyboard function
void glgeDefaultKeyFunc(unsigned char key, int, int)
{
    //store the pressed key
    glgePressedKeys.keyUpdate(key, true);
}

//default keyboard up function
void glgeDefaultKeyUpFunc(unsigned char key, int, int)
{
    //store wich key is no longer pressed
    glgePressedKeys.keyUpdate(key, false);
}

//special keyboard function
void glgeDefaultSpecKeyFunc(int key, int, int)
{
    //store the pressed key
    glgePressedKeys.keyUpdate(key, true);
}

//special keyboard up func
void glgeDefaultSpecKeyUpFunc(int key, int, int)
{
    //store which key is no longer pressed
    glgePressedKeys.keyUpdate(key, false);
}

void glgeDefaultMouseFunc(int button, int state, int x, int y)
{
    //update the mouse
    glgeMouse.update(button, state, x, y);

    //update the mouse positoin
    glgeMouse.updatePos(x,y);
}

void glgeDefaultPassiveMotionFunc(int x, int y)
{
    //update the mouse positoin
    glgeMouse.updatePos(x,y);
}