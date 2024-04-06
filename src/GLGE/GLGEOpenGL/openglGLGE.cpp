/**
 * @file GLGE.cpp
 * @author DM8AT
 * @brief declare the function for the base functions of GLGE
 * @version 0.1
 * @date 2023-02-09
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

//include the GLGE dependencys
#include "openglGLGE.h"
#include "../GLGEIndependend/glgeErrors.hpp"
#include "openglGLGEVars.hpp"
#include "openglGLGEFuncs.hpp"
#include "openglGLGEDefaultFuncs.hpp"
#include "../GLGEIndependend/glgePrivDefines.hpp"

//include acess to images
#include "../GLGEIndependend/glgeImage.h"

//include the standart librarys
#include <math.h>
#include <cstring>
#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <algorithm>

////////////////////
//Public functions//
////////////////////

//init function for glge
void glgeInit()
{
    //error check
    bool error = false;
    //if an error occoured, stop the function
    if (error)
    {
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_GLGE_INIT_ERROR << "\n";
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        }
    }

    #if __linux__
        glgeOperatingSystem = GLGE_OS_LINUX;
    #elif _WIN32
        glgeOperatingSystem = GLGE_OS_WINDOWS;
    #else
        glgeOperatingSystem = GLGE_OS_OTHER;
    #endif

    //initalise SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        //if sdl throws an value different than 0, print an error
        std::cerr << "[FATAL GLGE ERROR] GLGE failed to initalise SDL, SDL error: " << SDL_GetError() << "\n";
        //exit with an error
        exit(1);
    }
    //initalise fonts
    if (TTF_Init() != 0)
    {
        //printa a fatal error
        std::cerr << "[FATAL GLGE ERROR] GLGE failed to initalise SDL fonts, SDL font error: " << TTF_GetError() << "\n";
        //exit with an error
        exit(1);
    }

    //initalise to OpenGL 3.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

    //say that doublebuffering should be used
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //set the depth accuarcy
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    //set the current window index to 0 (the first window created)
    glgeCurrentWindowIndex = 0;
    //load the include defaults
    loadIncludeDefaults();
}

//first window creation methode
void glgeCreateWindow(const char* name, vec2 size, vec2 pos)
{
    //check for erros in the inputs
    bool error = false;
    //check if the size parameters are correct
    if (size.x <= 0 || size.y <= 0)
    {
        //if the size parameters are invalide, start to print an error
        if (glgeErrorOutput)
        {
            //print diffrent messages depending on the input size
            if (size.x < 0 || size.y < 0)
            {
                printf(GLGE_ERROR_WINDOW_SIZE_NEGATIVE);
            }
            else
            {
                printf(GLGE_ERROR_WINDOW_SIZE_ZERO);
            }
        }
        //say that an error occured
        error = true;
    }
    //check if the name pointer is invalide
    if (name == nullptr)
    {
        //print an error message
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_WINDOW_NAME_NULLPOINTER);
        }
        //say that an error occured
        error = true;
    }
    //check if the position is negative
    if (pos < vec2(0,0))
    {
        //print an error message
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_WINDOW_NEGATIVE_POSITION);
        }
        //say that an error occured
        error = true;
    }
    //if an error occured, return
    if (error)
    {
        //print an error message
        if (glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_WINDOW_INPUT_ERROR << "\n";
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }
    //start the window creation
    createWindow(name, size, pos);
}

//second window creation methode
void glgeCreateWindow(const char* name, int width, int height, vec2 pos)
{
    //check for erros in the inputs
    bool error = false;
    //check if the size parameters are correct
    if (width <= 0 || height <= 0)
    {
        //if the size parameters are invalide, start to print an error
        if (glgeErrorOutput)
        {
            //print diffrent messages depending on the input size
            if (width < 0 || height < 0)
            {
                printf(GLGE_ERROR_WINDOW_SIZE_NEGATIVE);
            }
            else
            {
                printf(GLGE_ERROR_WINDOW_SIZE_ZERO);
            }
        }
        //say that an error occured
        error = true;
    }
    //check if the name pointer is invalide
    if (name == nullptr)
    {
        //print an error message
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_WINDOW_NAME_NULLPOINTER);
        }
        //say that an error occured
        error = true;
    }
    //check if the position is negative
    if (pos < vec2(0,0))
    {
        //print an error message
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_WINDOW_NEGATIVE_POSITION);
        }
        //say that an error occured
        error = true;
    }
    //if an error occured, return
    if (error)
    {
        //print an error message
        if (glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_WINDOW_INPUT_ERROR << "\n";
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }
    //start the window creation
    createWindow(name, vec2(width, height), pos);
}

void glgeSetWindowTitle(const char* title)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set the name of an main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //change the window name
    glgeWindows[glgeMainWindowIndex]->rename(title);
}

//set if GLGE should output errors
void glgeSetErrorOutput(bool in)
{
    //set the boolean to controll error output
    glgeErrorOutput = in;
}

//get if GLGE should output errors
bool glgeGetErrorOutput()
{
    //return if GLGE should output errors
    return glgeErrorOutput;
}

//set if GLGE should print warnings
void glgeSetWarningOutput(bool out)
{
    glgeWarningOutput = out;
}

//get if GLGE should print warnings to the console
bool glgeGetWarningOutput()
{
    return glgeWarningOutput;
}

/**
 * @brief safely get a window from the glge window stack
 * 
 * @param windowID the ID of the window to acess
 * @return Window* a pointer to the window, NULL if the window acess failed
 */
Window* safeWindowAcess(unsigned int windowID)
{
    //check if the window pointer is 0
    if ((windowID-glgeWindowIndexOffset) > (unsigned int)glgeWindows.size())
    {
        //check if a warning should be printed
        if (glgeWarningOutput)
        {
            //print the warning
            printf("[GLGE WARNING] tried to acess a window out of range\n");
        }
        //stop the function
        return NULL;
    }
    //get the window pointer
    Window* wptr = glgeWindows[windowID-glgeWindowIndexOffset];
    //check if the window pointer is a nullpointer
    if (wptr == NULL)
    {
        //check if a warning should be printed
        if (glgeWarningOutput)
        {
            //print the warning
            printf("[GLGE WARNING] tried to execute a window function on an not started window\n");
        }
        //stop the function
        return NULL;
    }
    //return the pointer
    return wptr;
}

//start the main loop of GLGE
void glgeRunMainLoop()
{
    //check if at least one window exists
    if ((int)glgeWindows.size() < 1)
    {
        //print an error
        std::cerr << "[GLGE FATAL ERROR] GLGE can't start the main loop until at least one window was created\n";
        //close the program
        exit(1);
    }

    //start SDL2 text input
    SDL_StartTextInput();

    //store if the file should stop
    bool running = true;

    //run the main loop while the program is not stopped
    while (running)
    {
        //clear whatever was written last tick
        glgeTypedThisTick = "";
        //store the current SDL event
        SDL_Event event;
        //check all SDL events
        while (SDL_PollEvent(&event))
        {
            //check if the loop ended
            if (!running)
            {
                //stop the switch
                break;
            }
            //check the event type
            switch (event.type)
            {
                case SDL_TEXTINPUT:
                    glgeTypedThisTick += event.text.text;
                    break;
                case SDL_KEYDOWN:
                {
                    //handle the keydown
                    glgeDefaultKeyFunc(event.key.keysym.scancode);
                    //jump to the next event
                    break;
                }
                case SDL_KEYUP:
                {
                    //if the key is relsed, then handle the key up
                    glgeDefaultKeyUpFunc(event.key.keysym.scancode);
                    //jump to the next event
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    //handle a mouse button press
                    glgeDefaultMouseFunc(event.button.button, GLGE_MOUSE_BUTTON_PRESS);
                    //jump to the next event
                    break;
                }
                case SDL_MOUSEBUTTONUP:
                {
                    //handle the mouse button release
                    glgeDefaultMouseFunc(event.button.button, GLGE_MOUSE_BUTTON_RELEASE);
                    //jump to the next event
                    break;
                }
                case SDL_MOUSEMOTION:
                {
                    //create two integers to store the mouse position
                    int x,y;
                    //get the active mouse position
                    SDL_GetMouseState(&x,&y);
                    //store the screen size
                    vec2 s = glgeWindows[event.window.windowID-glgeWindowIndexOffset]->getSize();
                    //store the mouse position
                    glgeMouse.pos = vec2(float(x) / s.x, float(y) / s.y);
                    //store the pixel the mouse is on
                    glgeMouse.posPixel = vec2(x,y);
                    //get the mouse on the screen
                    SDL_GetGlobalMouseState(&x, &y);
                    //store the screen size
                    s = glgeGetScreenSize();
                    //store the mouse position
                    glgeMouse.screenPos = vec2(float(x) / s.x, float(y) / s.y);
                    //store the pixel the mouse is on
                    glgeMouse.screenPosPixel = vec2(x,y);
                    //jump to the next event
                    break;
                }
                case SDL_MOUSEWHEEL:
                {
                    //handle the mouse scrolling
                    glgeDefaultMouseFunc(GLGE_MOUSE_SCROLL, event.wheel.y);
                    //jump to the next event
                    break;
                }
                case SDL_WINDOWEVENT:
                {
                    switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_MINIMIZED:
                        {
                            //store the window pointer
                            Window* wptr = safeWindowAcess(event.window.windowID);
                            //check if the safe window acess was sucessfull
                            if (wptr == NULL)
                            {
                                //if the acess could not be done safe, skip
                                break;
                            }
                            //minimize the window
                            wptr->minimize(true);
                            //stop the itteration
                            break;
                        }
                        case SDL_WINDOWEVENT_RESTORED:
                        {
                            //store the window pointer
                            Window* wptr = safeWindowAcess(event.window.windowID);
                            //check if the safe window acess was sucessfull
                            if (wptr == NULL)
                            {
                                //if the acess could not be done safe, skip
                                break;
                            }
                            //restore the window using the minimize function
                            wptr->minimize(false);
                            //stop the itteration
                            break;
                        }
                        case SDL_WINDOWEVENT_MAXIMIZED:
                        {
                            //store the window pointer
                            Window* wptr = safeWindowAcess(event.window.windowID);
                            //check if the safe window acess was sucessfull
                            if (wptr == NULL)
                            {
                                //if the acess could not be done safe, skip
                                break;
                            }
                            //call the maximize function
                            wptr->maximize(true);
                            //stop the itteration
                            break;
                        }
                        case SDL_WINDOWEVENT_RESIZED:
                        {
                            //store the window pointer
                            Window* wptr = safeWindowAcess(event.window.windowID);
                            //check if the safe window acess was sucessfull
                            if (wptr == NULL)
                            {
                                //if the acess could not be done safe, skip
                                break;
                            }
                            //call the resize function
                            wptr->resizeWindow(event.window.data1, event.window.data2);
                            //stop the itteration
                            break;
                        }
                        case SDL_WINDOWEVENT_CLOSE:
                        {
                            //store the window pointer
                            Window* wptr = safeWindowAcess(event.window.windowID);
                            //check if the safe window acess was sucessfull
                            if (wptr == NULL)
                            {
                                //if the acess could not be done safe, skip
                                break;
                            }
                            //close the window
                            wptr->close();
                            //check if the window is the main window
                            if (event.window.windowID-glgeWindowIndexOffset == glgeMainWindowIndex)
                            {
                                //check if GLGE should close if the main window closes
                                if (glgeExitOnMainWindowClose)
                                {
                                    //stop the function
                                    running = false;
                                }
                            }
                            //check if all windows are closed
                            if (glgeActiveWindows == 0)
                            {
                                //stop the main loop
                                running = false;
                            }
                            //jump to the next window
                            break;
                        }
                        //by default, stop the function
                        default:
                            break;
                    }
                    //if not, stop this function
                    break;
                }
                //by default, stop the function
                default:
                    break;
            }
        }
        //loop over all windows
        for (int i = 0; i < (int)glgeWindows.size(); i++)
        {
            //check if the window is a nullpointer
            if (glgeWindows[i] == NULL)
            {
                //skip the window
                continue;
            }
            //get if the windows should close
            bool close = glgeWindows[i]->isClosingInitiated();
            //if they should close, close them
            if (close)
            {
                //close the window
                glgeWindows[i]->close();
                //check if the window is the main window
                if (((unsigned int)i == glgeMainWindowIndex))
                {
                    //check if GLGE should close if the main window closes
                    if (glgeExitOnMainWindowClose)
                    {
                        //stop the function
                        running = false;
                        //loop over all windows
                        for (Window* win : glgeWindows)
                        {
                            //skip if the window is a nullpointer
                            if (!win) { continue; }
                            //close the window
                            win->close();
                        }
                    }
                }
            }
            //check if all windows are closed
            if (glgeActiveWindows == 0)
            {
                //stop the main loop
                running = false;
            }
        }
        //loop over all windows
        for (int i = 0; i < (int)glgeWindows.size(); i++)
        {
            //check if the window is a nullpointer
            if (glgeWindows[i] == NULL)
            {
                //skip the window
                continue;
            }
            //get the window pointer
            Window* wptr = glgeWindows[i];
            //check if the window poitner is a nullpointer
            if (wptr == NULL) { continue; }
            //call the draw function
            wptr->draw();
            //update the window surface
            SDL_GL_SwapWindow((SDL_Window*)wptr->getSDLWindow());
        }

        //run a tick
        glgeDefaultTimer();
    }
    //stop SDL text input
    SDL_StopTextInput();
}

//first function to set the clear color
void glgeSetClearColor(float r, float g, float b, bool normalise)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set a clear color for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //call the function from the main window
    glgeWindows[glgeMainWindowIndex]->setClearColor(r,g,b, normalise);
}

//second function to set the clear color
void glgeSetClearColor(vec3 color, bool normalise)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set a clear color for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //call the function from the main window
    glgeWindows[glgeMainWindowIndex]->setClearColor(color, normalise);
}

//thired function to set the clear color
void glgeSetClearColor(vec4 color, bool normalise)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set a clear color for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //call the function from the main window
    glgeWindows[glgeMainWindowIndex]->setClearColor(color.x,color.y,color.z, normalise);
}

void glgeSetSkybox(const char* top, const char* bottom, const char* left, const char* right, const char* front, const char* back)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set a skybox for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //call the function from the main window
    glgeWindows[glgeMainWindowIndex]->setSkybox(top, bottom, left, right, front, back);
}

//bind a display func callback
void glgeBindDisplayFunc(void (*func)())
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set a display function for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //call the function from the main window
    glgeWindows[glgeMainWindowIndex]->setDrawFunc(func);
}

//debind a display func callback
void glgeClearDisplayFunc()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't clear a display function for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //call the function from the main window
    glgeWindows[glgeMainWindowIndex]->clearDrawFunc();
}

//bind a main callback function
void glgeBindMainFunc(void (*func)())
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set a tick function for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //call the function from the main window
    glgeWindows[glgeMainWindowIndex]->setTickFunc(func);
}

//unbind the main callback
void glgeClearMainFunc()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't clear a tick function for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //call the function from the main window
    glgeWindows[glgeMainWindowIndex]->clearTickFunc();
}

void glgeSetMaxFPS(int fps)
{
    //check for errors
    bool error = false;
    //check if the maximal frames per second are less or equal to 0
    if (fps <= 0)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_FPS_LESS_ONE);
        }
        //say that an error occured
        error = true;
    }

    //if an error occured, break
    if (error)
    {
        //output an exit message
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_SET_MAX_FPS << "\n";
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //update the swap interval for SDL to imediate updates
    SDL_GL_SetSwapInterval(0);

    //set the maximal frames per second
    glgeMaxFPS = fps;
}

//get the current frames per second
int glgeGetCurrentFPS()
{
    //return the current FPS
    return glgeCurrentFPS;
}

//get the maximal frames per second
int glgeGetMaxFPS()
{
    //return the max FPS
    return glgeMaxFPS;
}

//return if GLGE has an additional main function bound
bool glgeHasMainFunc()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get if a main window has a main func, if no main window exists\n");
        }
        //stop the function
        return false;
    }
    //get if the window has a main func
    return glgeWindows[glgeMainWindowIndex]->getHasTickFunc();
}

//return if GLGE has an additional display function bound
bool glgeHasDisplayFunc()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get a draw function for a main window, if no main window exists\n");
        }
        //stop the function
        return false;
    }
    //call the function from the main window
    return glgeWindows[glgeMainWindowIndex]->getHasDrawFunc();
}

//return if GLGE has a window bound
bool glgeHasWindow()
{
    return glgeHasMainWindow;
}

//set the move matrix name
void glgeSetMoveMatrixName(const char* name)
{
    //check if the input is valide
    if(name == nullptr)
    {
        //output an error message and say where the error occured
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_INVALIDE_INPUT);
            std::cerr << GLGE_ERROR_STR_GLGE_SET_MOV_MAT_NAME << "\n";
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }
    //sore the name for the move matrix
    glgeCamMatrix = (char*)name;
}

//get the momantan name of the move matrix
char* glgeGetMoveMatrixName()
{
    //return the name of the move matrix
    return glgeCamMatrix;
}

//get the aspect of the window
float glgeGetWindowAspect()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get the window aspect for a main window, if no main window exists\n");
        }
        //stop the function
        return 0.f;
    }
    //call the function from the main window
    return glgeWindows[glgeMainWindowIndex]->getWindowAspect();
}

//get the size of the window
vec2 glgeGetWindowSize()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get the window size for a main window, if no main window exists\n");
        }
        //stop the function
        return vec2(0);
    }
    //call the function from the main window
    return glgeWindows[glgeMainWindowIndex]->getSize();
}

//return the pressed keys
Keys glgeGetKeys()
{
    //return the pressed keys
    return glgePressedKeys;
}

Keys glgeGetToggledKeys()
{
    //return the keys from this tick
    return glgeKeysThisTick;
}

//get the delta time
float glgeGetDeltaTime()
{
    //return the delta time
    return glgeDeltaTime;
}

//add a shader to an shader program
void glgeAddShader(unsigned int shaderProgram, const char* shadertext, unsigned int shaderType)
{
    //create a new shader with the inputed type
    unsigned int shaderObj = glCreateShader(shaderType);

    //check if the shader object could be created
    if (shaderObj == 0)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            std::cout << GLGE_ERROR_COULD_NOT_CREATE_SHADER << shaderType << "\n";
            //print where the error occured
            std::cerr << GLGE_ERROR_STR_OBJECT_ADD_SHADER << "\n";
        }
        //stop the script
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //set a GLchar to the inputed text
    const GLchar* p[1];
    p[0] = shadertext;

    //store the length of the text
    int lengths[1];
    lengths[0] = strlen(shadertext);

    //set the shader source code
    glShaderSource(shaderObj, 1, p, lengths);

    //compile to the new shader object
    glCompileShader(shaderObj);

    //check for compiling errors
    int success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

    //if there was an error, print a message and exit
    if (!success)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            //create an info log to store the error created by open gl
            GLchar InfoLog[1024];
            glGetShaderInfoLog(shaderObj, 1024, NULL, InfoLog);
            //print the message
            printf(GLGE_ERROR_SHADER_COMPILE_ERROR, shaderType, InfoLog);
            //print where the error occured
            std::cerr << GLGE_ERROR_STR_OBJECT_ADD_SHADER << "\n";
        }
        //stop the script
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //attach the shader object to the final program
    glAttachShader(shaderProgram, shaderObj);
}

//get a uniform variable from a shader
int glgeGetUniformVar(unsigned int program, const char* name)
{
    //create the output variable
    int ret;
    //set the output variable to the uniform variable in the shader
    ret = glGetUniformLocation(program, name);
    //if the id is -1, output an error
    if (ret == -1)
    {
        //get the length of the name
        int length = (int)std::string(name).length();
        //output an error message, if the length is less than 4
        if (glgeErrorOutput && (length < 4))
        {
            printf(GLGE_ERROR_UNIFORM_VAR_NOT_FOUND, name);
            //say where the error occured
            std::cerr << GLGE_ERROR_STR_OBJECT_GET_UNIFORM_VARIABLE << "\n";
        }
        else
        {
            //output an error message, if the name dosn't starts with glge
            if (glgeErrorOutput && !(name[0]=='g' && name[1]=='l' && name[2]=='g' && name[3]=='e'))
            {
                printf(GLGE_ERROR_UNIFORM_VAR_NOT_FOUND, name);
                //say where the error occured
                std::cerr << GLGE_ERROR_STR_OBJECT_GET_UNIFORM_VARIABLE << "\n";
            }
        }
        //return -1
        return -1;
    }
    //if no error occured, return the id of the uniform variable
    return ret;
}

//compile and add the shaders to this object
unsigned int glgeCompileShader(const char* fileNameVS, const char* fileNameFS)
{
    //create strings for the shaders
    std::string vs, fs;

    //read the files
    if (!readFile(fileNameVS, vs))
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

    //read the second file
    if (!readFile(fileNameFS, fs))
    {
        //if the file can't be read, output an error message
        if (glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_OBJECT_COMPILE_SHADERS << "\n";
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }
    
    //just cast to another function
    return glgeCompileShader(vs, fs);
}

unsigned int glgeCompileShader(std::string fileDataVertex, std::string fileDataFragment)
{
    //create a new shader program
    unsigned int shaderProgram = glCreateProgram();

    //check if the shader could be created
    if (shaderProgram == 0)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_COULD_NOT_CREATE_SHADER);
            //say where the error occured
            std::cerr << GLGE_ERROR_STR_OBJECT_COMPILE_SHADERS << "\n";
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //pre-compile the vertex shader
    std::string vData = precompileShaderSource(fileDataVertex);
    //add the shader program from the first file
    glgeAddShader(shaderProgram, vData.c_str(), GL_VERTEX_SHADER);

    //pre-compile the fragment shader
    std::string fData = precompileShaderSource(fileDataFragment);
    //add the shader program from the second file
    glgeAddShader(shaderProgram, fData.c_str(), GL_FRAGMENT_SHADER);

    //create an variable to check for success
    int success = 0;
    //setup an error log
    GLchar ErrorLog[1024] = {0};

    //link the shader program
    glLinkProgram(shaderProgram);

    //get the program iv from the shader
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    //check if the program linking was no success
    if (success == 0)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            //get the error from open gl and output it with an custom message
            glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
            printf(GLGE_ERROR_SHADER_VALIDATE_ERROR, ErrorLog);
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //check if the program is valide
    glValidateProgram(shaderProgram);
    //get the program iv again
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    //check for success
    if (!success)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            //get the error from open gl and output it with an custom message
            glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
            printf(GLGE_ERROR_SHADER_VALIDATE_ERROR, ErrorLog);
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //return the shader program in the GLGE Object
    return shaderProgram;
}

unsigned int glgeTextureFromFile(const char* name, vec2* sP)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = glgeLoadImage(name, &width, &height, &nrChannels);
    if(data)
    {
        if (nrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else if (nrChannels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    else
    {
        std::cout << "Failed to load texture file: " << name << "\n";
    }

    //store the size if the input is not NULL
    if (sP != NULL)
    {
        sP->x = width;
        sP->y = height;
    }

    //clear memory and return the texture
    glgeImageFree(data);
    return texture;
}

vec2 glgeGetTextureSize(const char* name)
{
    int w, h, c;
    unsigned char* data = glgeLoadImage(name, &w, &h, &c, 0);
    if(!data)
    {
        std::cout << "Failed to load texture file: " << name << "\n";
    }
    glgeImageFree(data);
    return vec2(h,w);
}

Mouse glgeGetMouse()
{
    //return the stored mouse data
    return glgeMouse;
}

void glgeEnableBackfaceCulling()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set the backface culling for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //call the function from the main window
    glgeWindows[glgeMainWindowIndex]->enableBackfaceCulling();
}

void glgeDisableBackfaceCulling()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set the backface culling for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //call the function from the main window
    glgeWindows[glgeMainWindowIndex]->disableBackfaceCulling();
}

void glgeSwapBackfaceCulling()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set the backface culling for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    bool c = glgeWindows[glgeMainWindowIndex]->useBackfaceCulling();
    //set the face mode to the opposite it currently is
    if (c)
    {
        glgeWindows[glgeMainWindowIndex]->enableBackfaceCulling();
    }
    else
    {
        glgeWindows[glgeMainWindowIndex]->disableBackfaceCulling();
    }
}

void glgeSetBackfaceCulling(bool status)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set the backface culling for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }

    //set the face mode to the inputed status
    if (status)
    {
        glgeWindows[glgeMainWindowIndex]->enableBackfaceCulling();
    }
    else
    {
        glgeWindows[glgeMainWindowIndex]->disableBackfaceCulling();
    }
}

bool glgeGetBackfaceCullingStatus()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get if a main window uses backface culling, if no main window exists\n");
        }
        //stop the function
        return false;
    }
    //get if backface culling is used
    return glgeWindows[glgeMainWindowIndex]->useBackfaceCulling();
}

void glgeSetLightingShader(const char* lightingShaderFile)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set the lighting shader for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }

    //set the lighing shader
    glgeWindows[glgeMainWindowIndex]->setLightingShader(lightingShaderFile);
}

void glgeSetLightingShader(std::string LightingShader)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set the lighting shader for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }

    //set the lighing shader
    glgeWindows[glgeMainWindowIndex]->setLightingShader(LightingShader);
}

void glgeSetLightingShader(unsigned int shader)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set the lighting shader for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }

    //set the lighing shader
    glgeWindows[glgeMainWindowIndex]->setLightingShader(shader);
}

Shader* glgeSetPostProsessingShader(const char* shaderFile)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't add a post processing shader for a main window, if no main window exists\n");
        }
        //stop the function
        return NULL;
    }

    //pass the call to the main window
    return glgeWindows[glgeMainWindowIndex]->addPostProcessingShader(shaderFile);
}
Shader* glgeSetPostProsessingShader(std::string shaderSource)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't add a post processing shader for a main window, if no main window exists\n");
        }
        //stop the function
        return NULL;
    }

    //pass the call to the main window
    return glgeWindows[glgeMainWindowIndex]->addPostProcessingShader(shaderSource);
}

Shader* glgeSetPostProsessingShader(unsigned int s)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't add a post processing shader for a main window, if no main window exists\n");
        }
        //stop the function
        return NULL;
    }

    //pass the call to the main window
    return glgeWindows[glgeMainWindowIndex]->addPostProcessingShader(s);
}

int glgeSetPostProsessingShader(Shader* shader)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't add a post processing shader for a main window, if no main window exists\n");
        }
        //stop the function
        return -1;
    }

    //pass the call to the main window
    return glgeWindows[glgeMainWindowIndex]->addPostProcessingShader(shader);
}

Shader* glgeGetPostProcessingShader(int index)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't add get a post processing shader from an main window, if no main window exists\n");
        }
        //stop the function
        return NULL;
    }

    //pass the call to the main window
    return glgeWindows[glgeMainWindowIndex]->getPostProcessingShader(index);
}

int glgeGetIndexOfPostProcessingShader(Shader* shader)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't add get a post processing shader from an main window, if no main window exists\n");
        }
        //stop the function
        return -1;
    }

    //pass the call to the main window
    return glgeWindows[glgeMainWindowIndex]->getPostProcessingShaderIndex(shader);
}

void glgeDeletePostProcessingShader(int index, bool del)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't add get a post processing shader from an main window, if no main window exists\n");
        }
        //stop the function
        return;
    }

    //pass the call to the main window
    glgeWindows[glgeMainWindowIndex]->removePostProcessingShader(index, del);
}

void glgeDeletePostProcessingShader(Shader* shader, bool del)
{
    //find the index of the shader
    int index = glgeGetIndexOfPostProcessingShader(shader);
    //check if the index is -1
    if (index == -1)
    {
        //check if an warning should be printed
        if (glgeWarningOutput)
        {
            std::cerr << "[GLGE WARNING] tryed to remove shader that wasn't part of post-processing pipeline" << "\n";
        }
        //quit the function
        return;
    }
    else
    {
        //call the function to remove by index
        glgeDeletePostProcessingShader(index, del);
    }
    //set the inputed shader to the Nullpointer
    shader = NULL;
}

void glgeSetInterpolationMode(unsigned int mode)
{
    //store the inputed Interpolation mode
    glgeInterpolationMode = mode; 
}

void glgeWarpPointer(vec2 pointerPos, unsigned int space)
{
    //check if the space is screen space
    if (space == GLGE_SCREEN_SPACE)
    {
        //store the position
        vec2 pos = pointerPos;
        //move the origin to the middle of the screen
        pos += (glgeGetScreenSize() / vec2(2));
        //warp the mouse to the wanted pixel on screen
        SDL_WarpMouseGlobal(pos.x, pos.y);
    }
    //else, work in window space
    else
    {
        //store the position
        vec2 pos = pointerPos;
        //move the position to the screen middle
        pos += vec2(0.5);
        //scale the position to convert it from normalised window space to window screen space
        pos = pos.scale(glgeWindows[glgeCurrentWindowIndex]->getSize());
        //use the glut function to warp the pointer to the specified position
        SDL_WarpMouseInWindow((SDL_Window*)glgeWindows[glgeCurrentWindowIndex]->getSDLWindow(), pos.x,pos.y);
    }
}

void glgeWarpPointer(float x, float y, unsigned int space)
{
    //warp the pointer to the vec2 cast of x and y
    glgeWarpPointer(vec2(x,y), space);
}

void glgeSetCursor(int curser)
{
    //check if the curser should be invisible
    if (curser == GLGE_CURSOR_STYLE_NONE)
    {
        //if it should, hide the cursor
        SDL_ShowCursor(SDL_DISABLE);

        //stop the function
        return;
    }
    //else, show the cursor
    SDL_ShowCursor(SDL_ENABLE);
    //create the system courser
    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SystemCursor(curser));
    //check if the creation was sucessfull
    if (cursor == NULL)
    {
        //if not, check if errors should be printed
        if (glgeErrorOutput)
        {
            //if they should, print an error
            std::cerr << "[GLGE ERROR] couldn't create SDL system coursor, SDL error: " << SDL_GetError() << "\n";
        }
        //check if GLGE should exit on an error
        if (glgeExitOnError)
        {
            //if it should, close the program with an error
            exit(1);
        }
    }
    //set the curser to the inputed default
    SDL_SetCursor(cursor);
}

vec2 glgeGetScreenSize()
{
    //store the display mode
    SDL_DisplayMode display;
    //get the display mode from SDL
    SDL_GetCurrentDisplayMode(0, &display);
    //return the screen size
    return vec2(display.w, display.h);
}

float glgeGetCurrentElapsedTime()
{
    return SDL_GetTicks();
}

unsigned int glgeGetMainAlbedoMap()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get a albedo map from a main window, if no main window exists\n");
        }
    }

    //set the lighing shader
    return glgeWindows[glgeMainWindowIndex]->getAlbedoTex();
}

unsigned int glgeGetMainNormalMap()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get a normal map from a main window, if no main window exists\n");
        }
        //stop the function
        return 0;
    }

    //set the lighing shader
    return glgeWindows[glgeMainWindowIndex]->getNormalTex();
}

unsigned int glgeGetMainPositionMap()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get a position map from a main window, if no main window exists\n");
        }
        //stop the function
        return 0;
    }

    //set the lighing shader
    return glgeWindows[glgeMainWindowIndex]->getPosTex();
}

unsigned int glgeGetMainRoughnessMap()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get a roughness map from a main window, if no main window exists\n");
        }
        //stop the function
        return 0;
    }

    //set the lighing shader
    return glgeWindows[glgeMainWindowIndex]->getRMLTex();
}

unsigned int glgeGetLastFrame()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get the last frame from a main window, if no main window exists\n");
        }
        //stop the function
        return 0;
    }

    //set the lighing shader
    return glgeWindows[glgeMainWindowIndex]->getLastFrame();
}

void glgeSetExitOnError(bool exitOnError)
{
    //set the exit on error variable to the input
    glgeExitOnError = exitOnError;
}

void glgeToggleExitOnError()
{
    //set the exit on error variable to not exit on error
    glgeExitOnError = !glgeExitOnError;
}

bool glgeGetExitOnError()
{
    //return the local value to store if glge should exit on an error
    return glgeExitOnError;
}

void glgeBindOnWindowResizeFunc(void (*func)(int, int))
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set a resize func for a main window, if no main window exists\n");
        }
    }

    //set the lighing shader
    return glgeWindows[glgeMainWindowIndex]->setResizeFunc(func);
}

void glgeSetFullscreenMode(bool isFullscreen)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set the fullscreen mode for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->setFullscreenMode(isFullscreen);
}

void glgeToggleFullscreen()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get the fullscreen mode from an main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //use glge to set the fullscreen to the inverse of the fullscreen
    glgeSetFullscreenMode(!glgeWindows[glgeMainWindowIndex]->isFullscreen());
}

bool glgeIsFullscreen()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get the fullscreen mode from an main window, if no main window exists\n");
        }
        //stop the function
        return false;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->isFullscreen();
}

void glgeResizeWindow(int width, int height, bool force)
{
    //pass to the other function
    glgeResizeWindow(vec2(width, height), force);
}

void glgeResizeWindow(vec2 size, bool force)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get the fullscreen mode from an main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->setSize(size, force);
}

void glgeSetWindowResizable(bool resizable)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set the window resize mode for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->setResizable(resizable);
}

void glgeToggleWindowResizable()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't toggle the window resize mode of an main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->setResizable(!glgeWindows[glgeMainWindowIndex]->isWindowResizable());
}

bool glgeIsWindowResizable()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get the window resize mode from an main window, if no main window exists\n");
        }
        //stop the function
        return false;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->isWindowResizable();
}

void glgeSetWindowPosition(vec2 position, bool force)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set the window position of a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->setPos(position, force);
}

void glgeSetWindowPosition(int x, int y, bool force)
{
    //pass to the other function
    glgeSetWindowPosition(vec2(x,y), force);
}

vec2 glgeGetWindowPosition()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get the window position from an main window, if no main window exists\n");
        }
        //stop the function
        return vec2(0);
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->getPos();
}

void glgeSetWindowMoveable(bool allowMovement)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't allow or deny window movement of a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->setMovable(allowMovement);
}

void glgeToggleWindowMoveable()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't allow or deny window movement of a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->setMovable(!glgeWindows[glgeMainWindowIndex]->isMovable());
}

bool glgeGetWindowMoveable()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get window movement constrains from an main window, if no main window exists\n");
        }
        //stop the function
        return false;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->isMovable();
}

void glgeSetWindowIcon(const char* file)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get window movement constrains from an main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->setWindowIcon(file);
}

unsigned int glgeGetAlbedoBuffer()
{
    //return the main albedo texture
    return glgeGetMainAlbedoMap();
}

unsigned int glgeGetNormalBuffer()
{
    //return the main normal map
    return glgeGetMainNormalMap();
}

unsigned int glgeGetPositionBuffer()
{
    //return the main position map
    return glgeGetMainPositionMap();
}

unsigned int glgeGetRoughnessBuffer()
{
    //return the main roughness map
    return glgeGetMainRoughnessMap();
}

unsigned int glgeGetLighningBuffer()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get output texture from an main window, if no main window exists\n");
        }
        //stop the function
        return 0;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->getOutTex();
}

bool glgeGetTransparencyPass()
{
    //return if the current window is in the transparency pass
    return glgeWindows[glgeCurrentWindowIndex]->isTranparentPass();
}

void glgeSetTransparencyCombineShader(Shader* shader)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set transprent combine shader for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->setTransparencyCombineShader(shader);
}

Shader* glgeGetTransparencyCombineShader()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get transprent combine shader from an main window, if no main window exists\n");
        }
        //stop the function
        return NULL;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->getTransparencyCombineShader();
}

bool glgeGetIfCustomTransparentCombineShaderIsBound()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get if a transprent combine shader from an main window is custom, if no main window exists\n");
        }
        //stop the function
        return NULL;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->isCustomTransparencyCombineShader();
}

void glgeSetWindowBorderVisible(bool visible)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set the window border visibliety of a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->setBorderVisible(visible);
}

bool glgeGetWindowBorderVisible()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get the window border visibliety from an main window, if no main window exists\n");
        }
        //stop the function
        return false;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->isBorderVisible();
}

void glgeSetWindowForceOpen(bool forceOpen)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't set the main window force open, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->setWindowForceOpen(forceOpen);
}

bool glgeGetWindowForceOpen()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get if the main window is forced open, if no main window exists\n");
        }
        //stop the function
        return false;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->isWindowForceOpen();
}

void glgeCloseWindow(bool force)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't initiate the closing of an main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->initiateClosing(force);
}

bool glgeGetWindowClosingInitiated()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get if the main window should close, if no main window exists\n");
        }
        //stop the function
        return false;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->isClosingInitiated();
}

void glgeStopWindowClosing(bool force)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't de-initiate the closing of an main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->stopWindowClosing(force);
}

void glgeSetWindowAlwaysOnTop(bool onTop)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] set a main window always on top, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->setWindowAlwaysOnTop(onTop);
}

bool glgeGetWindowAlwaysOnTop()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] get if a main window is always on top, if no main window exists\n");
        }
        //stop the function
        return false;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->isWindowAlwaysOnTop();
}

void glgeSetWindowBrightness(float brightness)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] set the brightness of a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->setBrightness(brightness);
}

float glgeGetWindowBrightness()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] get the brightness from an main window, if no main window exists\n");
        }
        //stop the function
        return 0;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->getBrightness();
}

void glgeAddWindowFlag(int windowFlag)
{
    //check if the window was already created
    if (glgeHasMainWindow)
    {
        //check if warnings should be printed
        if (glgeWarningOutput)
        {
            //print the warning
            printf("[GLGE WARNING] window falg can't be set if a window was allready opend\n");
        }
        //stop the function
        return;
    }
    //store if the flag is valide
    bool val = false;
    //check all valid flags
    switch (windowFlag)
    {
        //check the flags, if it is one, say that it is valide
        case GLGE_WINDOW_FLAG_ALLOW_HIGHDPI:
        case GLGE_WINDOW_FLAG_POPUP_MENU:
        case GLGE_WINDOW_FLAG_SKIP_TASKBAR:
        case GLGE_WINDOW_FLAG_TOOLTIP:
        case GLGE_WINDOW_FLAG_UTILITY:
            val = true;
            break;
        default:
            val = false;
            break;
    }
    //check if the flag is valide
    if (!val)
    {
        //if not, check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] inputed an invalide flag as a window flag\n";
        }
        //check if glge should close on error
        if (glgeExitOnError)
        {
            //stop the program
            exit(1);
        }
        //else, stop the function
        return;
    }
    //set the window flag
    glgeWindowFlags = SDL_WindowFlags(glgeWindowFlags | windowFlag);
}

void glgeShowHideWindow(bool show)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] show or hide a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->showHide(show);
}

bool glgeGetWindowShown()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] get if a main window is shown or hiden, if no main window exists\n");
        }
        //stop the function
        return false;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->isShown();
}

void glgeMaximize(bool maximized, bool force)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] maximize a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->maximize(maximized, force);
}

bool glgeGetWindowMaximized()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] get if a main window is maximized, if no main window exists\n");
        }
        //stop the function
        return false;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->isMaximized();
}

bool glgeGetWindowFocus()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] get if a main window is focused, if no main window exists\n");
        }
        //stop the function
        return false;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->isFocused();
}

bool glgeGetMouseFocus()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] get if a main window is focused by the mouse, if no main window exists\n");
        }
        //stop the function
        return false;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->isMouseFocused();
}

void glgeSetMouseGrabMode(bool mode)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] set the mouse grab mode for a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->setMouseGrabMode(mode);
}

bool glgeGetMouseGrabMode()
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] get if a main window is grabbing the mouse, if no main window exists\n");
        }
        //stop the function
        return false;
    }
    //get if the main window is using backface culling
    return glgeWindows[glgeMainWindowIndex]->getMouseGrabMode();
}

void glgeFocusWindow(bool moveUp)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] focus a main window, if no main window exists\n");
        }
        //stop the function
        return;
    }
    //get if the main window is using backface culling
    glgeWindows[glgeMainWindowIndex]->focuse(moveUp);
}

void glgeSetExitOnMainWindowClose(bool exit)
{
    //set the variable
    glgeExitOnMainWindowClose = exit;
}

bool glgeGetExitOnMainWindowClose()
{
    //return the boolean
    return glgeExitOnMainWindowClose;
}

bool glgeUsesOpenGL()
{
    //say that OpenGL is used
    return true;
}

bool glgeUsesVulkan()
{
    //say that OpenGL is used
    return false;
}