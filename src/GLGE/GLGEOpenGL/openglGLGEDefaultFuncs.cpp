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
//check if glew is allready included
#ifndef _GLGE_GLEW_
//say that glew is now included
#define _GLGE_GLEW_
//include glew
#include <GL/glew.h>
//close the if for glew
#endif

//include the GLGE dependencys
#include "../GLGEIndependend/glgeErrors.hpp"
#include "../GLGEIndependend/GLGEKlasses.hpp"
#include "openglGLGEVars.hpp"
#include "openglGLGEFuncs.hpp"

//include math things
#include "../GLGEMath.h"

//include the needed default librarys
#include <iostream>
#include <cfloat>

//Private vars
bool draw = true;

///////////////////////////////////
//SUB FUNCTIONS FOR THE CALLBACKS//
///////////////////////////////////


//the default timer function for GLGE
void glgeDefaultTimer()
{
    //update the time it took for one tick to occure in seconds
    glgeDeltaTime = (SDL_GetTicks() - glgeTickTime);

    //calculate the current frames per second
    glgeCurrentFPS = 1.f/((SDL_GetTicks() - glgeTickTime)/1000.f);

    //update the time since last tick
    glgeTickTime = SDL_GetTicks();

    //check if an error occured
    bool err = false;
    //output all OpenGL errors
    GLenum error;
    //loop over all errors
    while((error = glGetError()) != GL_NO_ERROR)
    {
        //output the error and connect it to the OpenGL error
        if(glgeErrorOutput)
        {
            //check if an error allready occured
            if (err)
            {
                //if it did, only print the openGL error message
                printf("%s\n", getGLErrorString(error));
            }
            else
            {
                //print the compleate error message
                printf(GLGE_ERROR_OPEN_GL_ERROR, getGLErrorString(error));
            }
        }
        //say that an error occured
        err = true;
    }
    //store the SDL2 error string
    std::string str = "";
    //get the last SDL2 error
    str = std::string(SDL_GetError());
    //clean up the errors
    SDL_ClearError();
    //check if an error occured
    if (str != "")
    {
        //check if error output is enabled
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] an SDL2 error occured. Check the SDL2 error for more information: " << str << "\n";
        }
        //stop the script if an error occured an GLGE is supposed to
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        }
    }
    //stop the script if an error occured an GLGE is supposed to
    if (glgeExitOnError && err)
    {
        //only exit the program if glge is tolled to exit on an error
        exit(1);
    }

    //update the main display
    SDL_GetCurrentDisplayMode(0, &glgeMainDisplay);
    //loop over all windows
    for (int i = 0; i < (int)glgeWindows.size(); i++)
    {
        //set the current window
        glgeCurrentWindowIndex = i;
        //get the window pointer
        Window* wptr = glgeWindows[i];
        //check if the window poitner is a nullpointer
        if (wptr == NULL) { continue; }
        //if not, run a window tick
        wptr->tick();
    }

    //set the mouse wheel to 0
    glgeMouse.mouseWheel = 0;

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