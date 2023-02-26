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

//include the OpenGL dependencys
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

//include the needed default librarys
#include <iostream>

//////////////////////////
//Default GLGE callbacks//
//////////////////////////

//the default display callback for GLGE
void glgeDefaultDisplay()
{
    ///////ERROR IS FRAME BUFFER/////
    //bind the custom framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, glgeMultFBO);
    //clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //enable depth testing
    glEnable(GL_DEPTH_TEST);

    //call the custom drawing function
    if(glgeHasDisplayCallback)
    {
        ((void(*)())glgeDisplayCallback)();
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, glgeMultFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glgeFBO);
    glBlitFramebuffer(0,0, glgeWindowSize.x, glgeWindowSize.y, 0,0, glgeWindowSize.x, glgeWindowSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

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
    //bind the framebuffer texture
    glBindTexture(GL_TEXTURE_2D, glgeFrameBufferTexture);
    //draw the screen
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    //unbind the shader
    glUseProgram(0);
    //unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //update the window
    glutSwapBuffers();
}

//the default timer function for GLGE
void glgeDefaultTimer(int)
{
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

    //update the window size of the frame buffer Multisample
    glBindRenderbuffer(GL_RENDERBUFFER, glgeMultRBO);
    //setup the storage for the render buffer Multisample
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, glgeSamples, GL_DEPTH24_STENCIL8, glgeWindowSize.x, glgeWindowSize.y);

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

    //update the render texture parameters for the MultiSample texture
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, glgeFrameBufferMultisampleTexture);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, glgeSamples, GL_RGBA, glgeWindowSize.x, glgeWindowSize.y, GL_TRUE);
    //ungind the multisample texture
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

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