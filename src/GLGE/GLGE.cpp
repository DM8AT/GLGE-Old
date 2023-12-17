/**
 * @file GLGE.cpp
 * @author DM8AT
 * @brief declare the function for the base functions of GLGE
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
#include "GLGE.h"
#include "glgeErrors.hpp"
#include "glgeVars.hpp"
#include "glgeFuncs.hpp"
#include "glgeDefaultFuncs.hpp"
#include "glgePrivDefines.hpp"

//include acess to images
#include "glgeImage.h"

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
    SDL_Init(SDL_INIT_EVERYTHING);
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
    SDL_SetWindowTitle(glgeMainWindow, title);
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

//start the main loop of GLGE
void glgeRunMainLoop()
{
    //check if a main window is bound
    if (glgeHasMainWindow == false)
    {
        //print an error
        std::cerr << "[GLGE FATAL ERROR] forgot to create window before running main loop \n";
        //close the program
        exit(1);
    }

    //store if the file should stop
    bool running = true;

    //run the main loop while the program is not stopped
    while (running)
    {
        //store the current SDL event
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    //handle the keydown
                    glgeDefaultKeyFunc(event.key.keysym.scancode);
                    //jump to the next event
                    break;
                case SDL_KEYUP:
                    //if the key is relsed, then handle the key up
                    glgeDefaultKeyUpFunc(event.key.keysym.scancode);
                    //jump to the next event
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    //handle a mouse button press
                    glgeDefaultMouseFunc(event.button.button, GLGE_MOUSE_BUTTON_PRESS);
                    //jump to the next event
                    break;
                case SDL_MOUSEBUTTONUP:
                    //handle the mouse button release
                    glgeDefaultMouseFunc(event.button.button, GLGE_MOUSE_BUTTON_RELEASE);
                    //jump to the next event
                    break;
                case SDL_MOUSEMOTION:
                    //create two integers to store the mouse position
                    int x,y;
                    //get the active mouse position
                    SDL_GetMouseState(&x,&y);
                    //store the mouse position
                    glgeMouse.pos = vec2(float(x) / glgeWindowSize.x, float(y) / glgeWindowSize.y);
                    //store the pixel the mouse is on
                    glgeMouse.posPixel = vec2(x,y);
                    //jump to the next event
                    break;
                case SDL_MOUSEWHEEL:
                    //handle the mouse scrolling
                    glgeDefaultMouseFunc(GLGE_MOUSE_SCROLL, event.wheel.y);
                    //jump to the next event
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            //Change view port to the new window dimensions
                            glgeDefaultResizeFunc(event.window.data1, event.window.data2);
                            //Update window content
                            glgeDefaultDisplay();
                            break;
                    }
            }
            //check if the window should close:
            if (((event.type == SDL_QUIT) && (!glgeWindowForceOpen)) || glgeOPCloseWindow)
            {
                //if it should close, say that the loop is nolonger running
                running = false;
                //break out of the switch
                break;
            }
        }

        //draw the screen
        glgeDefaultDisplay();
        //run a tick
        glgeDefaultTimer();
    }
}

//first function to set the clear color
void glgeSetClearColor(float r, float g, float b, bool normalise)
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
    glgeClearColor = vec4(r,g,b,1.f);

    //disable the skybox
    glgeUseSkybox = false;
}

//second function to set the clear color
void glgeSetClearColor(vec3 color, bool normalise)
{
    //check for errors in the input
    bool error = false;
    //check if the values are negative
    if (color.x < 0 || color.y < 0 || color.z < 0)
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
        color /= vec3(255,255,255);
    }

    //check for out of bound values
    if (color.x > 1.f || color.y > 1.f || color.z > 1.f)
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
    glgeClearColor = vec4(color.x,color.y,color.z,1.f);

    //disable the skybox
    glgeUseSkybox = false;
}

//thired function to set the clear color
void glgeSetClearColor(vec4 color, bool normalise)
{
    //check for errors in the input
    bool error = false;
    //check if the values are negative
    if (color.x < 0 || color.y < 0 || color.z < 0)
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
        color /= vec4(255,255,255,1);
    }

    //check for out of bound values
    if (color.x > 1.f || color.y > 1.f || color.z > 1.f)
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
    glgeClearColor = vec4(color.x,color.y,color.z,1.f);

    //disable the skybox
    glgeUseSkybox = false;
}

void glgeSetSkybox(const char* top, const char* bottom, const char* left, const char* right, const char* front, const char* back)
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
    glGenTextures(1, &glgeSkyboxCube);
    //store the texture mode
    int texMode = GL_RGB;
    //bind the texture as a cube map
    glBindTexture(GL_TEXTURE_CUBE_MAP, glgeSkyboxCube);
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
    glgeUseSkybox = true;
}

//bind a display func callback
void glgeBindDisplayFunc(void (*func)())
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
            std::cerr << GLGE_ERROR_STR_BIND_DISPLAY_CALLBACK << "\n";
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //set the display callback
    glgeDisplayCallback = func;

    //say that an display callback is bound
    glgeHasDisplayCallback = true;
}

//debind a display func callback
void glgeClearDisplayFunc()
{
    //set the display func callback to the nullpointer
    glgeDisplayCallback = nullptr;

    //say that no display function is bound
    glgeHasDisplayCallback = false;
}

//bind a main callback function
void glgeBindMainFunc(void (*func)())
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
    glgeMainCallback = func;

    //say that an display callback is bound
    glgeHasMainCallback = true;
}

//unbind the main callback
void glgeClearMainFunc()
{
    //set the main function callback to the nullpointer
    glgeMainCallback = nullptr;

    //say that no main callback is bound
    glgeHasMainCallback = false;
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
    return glgeHasMainCallback;
}

//return if GLGE has an additional display function bound
bool glgeHasDisplayFunc()
{
    return glgeHasDisplayCallback;
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
    //return the window aspect
    return glgeWindowAspect;
}

//get the size of the window
vec2 glgeGetWindowSize()
{
    //return the window size
    return glgeWindowSize;
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
        //return 0
        return -1;
    }
    //if no error occured, return the id of the uniform variable
    return ret;
}

//compile and add the shaders to this object
unsigned int glgeCompileShader(const char* fileNameVS, const char* fileNameFS)
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
    //add the shader program from the first file
    glgeAddShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);

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
    //add the shader program from the second file
    glgeAddShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

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

    //say open GL to use the shader program
    glUseProgram(shaderProgram);
    //return the shader program in the GLGE Object
    return shaderProgram;
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

    //add the shader program from the first file
    glgeAddShader(shaderProgram, fileDataVertex.c_str(), GL_VERTEX_SHADER);

    //add the shader program from the second file
    glgeAddShader(shaderProgram, fileDataFragment.c_str(), GL_FRAGMENT_SHADER);

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

    //say open GL to use the shader program
    glUseProgram(shaderProgram);
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
    unsigned char *data = glgeLoad(name, &width, &height, &nrChannels);
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
    unsigned char* data = glgeLoad(name, &w, &h, &c, 0);
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
    //say that face culling is enabled
    glEnable(GL_CULL_FACE);
    glgeUseCulling = true;
}

void glgeDisableBackfaceCulling()
{
    //say that face culling is disabled
    glDisable(GL_CULL_FACE);
    glgeUseCulling = false;
}

void glgeSwapBackfaceCulling()
{
    //set the face mode to the opposite it currently is
    if (glgeUseCulling)
    {
        glgeDisableBackfaceCulling();
    }
    else
    {
        glgeEnableBackfaceCulling();
    }
}

void glgeSetBackfaceCulling(bool status)
{
    //set the face mode to the inputed status
    if (status)
    {
        glgeDisableBackfaceCulling();
    }
    else
    {
        glgeEnableBackfaceCulling();
    }
}

bool glgeGetBackfaceCullingStatus()
{
    //return the current backface culling mode
    return glgeUseCulling;
}

void glgeSetLightingShader(const char* lightingShaderFile)
{
    //create strings for the shader
    std::string data;

    //read the files
    if (!readFile(lightingShaderFile, data))
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
    glgeLightingShader = glgeCompileShader(GLGE_EMPTY_VERTEX_SHADER, data);

    //get the post processing uniforms
    bool albedo = getUniformsForLightingShader();

    //if the albedo wasn't found, print out the filename
    if (!albedo)
    {
        printf(GLGE_ERROR_OCCURED_IN_FILE, lightingShaderFile);
    }
}

void glgeSetLightingShader(std::string LightingShader)
{
    //compile the shader and save it
    glgeLightingShader = glgeCompileShader(GLGE_EMPTY_VERTEX_SHADER, LightingShader);

    //get the post processing uniforms
    bool albedo = getUniformsForLightingShader();

    //if the albedo wasn't found, print out the filename
    if (!albedo)
    {
        printf("[GLGE ERROR INFO] Error occured in shader compiled from custom source");
    }
}

void glgeSetLightingShader(unsigned int shader)
{
    //store the inputed shader as the Lighting shader
    glgeLightingShader = shader;

    //get the post processing uniforms
    bool albedo = getUniformsForLightingShader();

    //if the albedo wasn't found, print out the filename
    if (!albedo)
    {
        printf("[GLGE ERROR INFO] Error occured in shader set by shader ID. \n                 ID : %d\n", shader);
    }
}

Shader* glgeSetPostProsessingShader(const char* shaderFile)
{
    //create the new shader
    Shader* shader = new Shader(GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER, shaderFile);
    //check if the shader contains the main image
    if (glgeGetUniformVar(shader->getShader(), "glgeMainImage") == -1)
    {
        //check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] File " << shaderFile << 
            " was inputed as an post-processing shader, but the uniform \"glgeMainImage\" for the input of the image is undefined" << "\n";
        }
    }
    //store a new shader in the post-processing stack
    glgePostProcessingShaders.push_back(shader);
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
Shader* glgeSetPostProsessingShader(std::string shaderSource)
{
    //create the new shader
    Shader* shader = new Shader(GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER, shaderSource);
    //check if the shader contains the main image
    if (glgeGetUniformVar(shader->getShader(), "glgeMainImage") == -1)
    {
        //check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] uniform \"glgeMainImage\" is undefined for post-processing shader" << "\n" << 
            "Shader source: " << "\n" << shaderSource << "\n";
        }
    }
    //store a new shader in the post-processing stack
    glgePostProcessingShaders.push_back(shader);
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

Shader* glgeSetPostProsessingShader(unsigned int s)
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
    }
    //store a new shader in the post-processing stack
    glgePostProcessingShaders.push_back(shader);
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

void glgeSetPostProsessingShader(Shader* shader)
{
    //store the shader
    glgePostProcessingShaders.push_back(shader);
    //check if the shader contains the main image
    if (glgeGetUniformVar(shader->getShader(), "glgeMainImage") == -1)
    {
        //check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] uniform \"glgeMainImage\" is undefined for post-processing shader passed by pointer" << "\n";
        }
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
}

Shader* glgeGetPostProcessingShader(int index)
{
    //return the searched shader
    return glgePostProcessingShaders[index];
}

int glgeGetIndexOfPostProcessingShader(Shader* shader)
{
    //find the index of the shader
    std::vector<Shader*>::iterator iter = std::find(glgePostProcessingShaders.begin(), glgePostProcessingShaders.end(), shader);
    //check if the element wasn't found
    if (iter == glgePostProcessingShaders.cend())
    {
        //quit the function with -1
        return -1;
    }
    else
    {
        //return the index
        return std::distance(glgePostProcessingShaders.begin(), iter);
    }
}

void glgeDeletePostProcessingShader(int index, bool del)
{
    //get a reference to the shader
    Shader* shader = glgePostProcessingShaders[index];
    //delte the shader from the post-processing stack
    glgePostProcessingShaders.erase(glgePostProcessingShaders.begin() + index);
    //check if the shader should be deleted
    if (del)
    {
        //if it should be deleted, delete it
        delete shader;
    }
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
    vec2 mem;
    //if the space variable is true(so it is greater than 0)
    if (space == 1)
    {
        //scale the pointer by the window size to transform it to screen space
        mem = vec2(pointerPos.x * glgeWindowSize.x, pointerPos.y * glgeWindowSize.y);
    }
    else
    {
        //add the half window size to the position
        mem += glgeWindowSize/vec2(2,2);
    }

    //use the glut function to warp the pointer to the specified position
    SDL_WarpMouseInWindow(glgeMainWindow, mem.x, mem.y);
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
    //return the main albedo texture
    return glgeFrameAlbedoMap;
}

unsigned int glgeGetMainNormalMap()
{
    //return the main normal map
    return glgeFrameNormalMap;
}

unsigned int glgeGetMainPositionMap()
{
    //return the main position map
    return glgeFramePositionMap;
}

unsigned int glgeGetMainRoughnessMap()
{
    //return the main roughness map
    return glgeFrameRoughnessMap;
}

unsigned int glgeGetLastFrame()
{
    //return a pointer to the last frame map
    return glgeFrameLastTick;
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
    //store the inputed function, nullpointers are catched before it is called
    glgeOnWindowResize = func;
}

void glgeSetFullscreenMode(bool isFullscreen)
{
    //store the inputed variable to say if should be in fullscreen mode
    glgeFullscreen = isFullscreen;

    //check if it should be fullscreen
    if (isFullscreen)
    {
        //store the current window size
        glgeTrueWindowSize = glgeWindowSize;
        //set the screen size to the complete screen
        SDL_SetWindowSize(glgeMainWindow, glgeMainDisplay.w, glgeMainDisplay.h);
        //use SDL to set the fullscreen
        SDL_SetWindowFullscreen(glgeMainWindow, SDL_WINDOW_FULLSCREEN);
        //resize everything
        glgeDefaultResizeFunc(glgeMainDisplay.w, glgeMainDisplay.h);
    }
    else
    {
        //use SDL to set the window to the original size
        SDL_SetWindowFullscreen(glgeMainWindow, 0);
        //set the screen size to the complete screen
        SDL_SetWindowSize(glgeMainWindow, glgeTrueWindowSize.x, glgeTrueWindowSize.y);
        //resize everything
        glgeDefaultResizeFunc(glgeTrueWindowSize.x, glgeTrueWindowSize.y);

    }
}

void glgeToggleFullscreen()
{
    //use glge to set the fullscreen to the inverse of the fullscreen
    glgeSetFullscreenMode(!glgeFullscreen);
}

bool glgeIsFullscreen()
{
    //output the current fullscreen mode
    return glgeFullscreen;
}

void glgeResizeWindow(int width, int height, bool force)
{
    //check if the window resize is allowed
    if (glgeAllowWindowResize || force)
    {
        //change the window size using freeglut
        SDL_SetWindowSize(glgeMainWindow, width, height);
        //if force is enabled, set the window size parameters
        if (force)
        {
            //instantly resize the parameters
            resizeWindow(width,height);
        }
    }
}

void glgeResizeWindow(vec2 size, bool force)
{
    //check if the window resize is allowed
    if (glgeAllowWindowResize || force)
    {
        //change the window size using freeglut
        SDL_SetWindowSize(glgeMainWindow, size.x, size.y);
        //if force is enabled, set the window size parameters
        if (force)
        {
            //instantly resize the parameters
            resizeWindow(size.x, size.y);
        }
    }
}

void glgeSetWindowResizable(bool resizable)
{
    //set the correct window resize mode
    SDL_SetWindowResizable(glgeMainWindow, SDL_bool(resizable));
    //store the inputed mode
    glgeAllowWindowResize = resizable;
}

void glgeToggleWindowResizable()
{
    //switch the state of the resize variable
    glgeAllowWindowResize = !glgeAllowWindowResize;
}

bool glgeIsWindowResizable()
{
    //output if the window can be resized
    return glgeAllowWindowResize;
}

void glgeSetWindowPosition(vec2 position, bool force)
{
    //check if the window movement is allowed
    if (glgeAllowWindowMovement || force)
    {
        //use glut to set the window position
        SDL_SetWindowPosition(glgeMainWindow, position.x,position.y);

        //if the movement should be forced, store the new position
        if (force)
        {
            //store the new window position
            glgeWindowPosition = position;
        }
    }
}

void glgeSetWindowPosition(int x, int y, bool force)
{
    //check if the window movement is allowed
    if (glgeAllowWindowMovement || force)
    {
        //use glut to set the window position
        SDL_SetWindowPosition(glgeMainWindow, x,y);

        //if the movement should be forced, store the new position
        if (force)
        {
            //store the new window position
            glgeWindowPosition = vec2(x,y);
        }
    }
}

vec2 glgeGetWindowPosition()
{
    //use glut to get the window position and return it
    return glgeWindowPosition;
}

void glgeSetWindowMoveable(bool allowMovement)
{
    //store if the window is allowed to move
    glgeAllowWindowMovement = allowMovement;
}

void glgeToggleWindowMoveable()
{
    //invert the allow movement
    glgeAllowWindowMovement = !glgeAllowWindowMovement;
}

bool glgeGetWindowMoveable()
{
    //return if the window movement is allowed
    return glgeAllowWindowMovement;
}

void glgeSetWindowIcon(const char* file)
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
    SDL_SetWindowIcon(glgeMainWindow,img);
}

unsigned int glgeGetAlbedoBuffer()
{
    //return the main albedo texture
    return glgeFrameAlbedoMap;
}

unsigned int glgeGetNormalBuffer()
{
    //return the main normal map
    return glgeFrameNormalMap;
}

unsigned int glgeGetPositionBuffer()
{
    //return the main position map
    return glgeFramePositionMap;
}

unsigned int glgeGetRoughnessBuffer()
{
    //return the main roughness map
    return glgeFrameRoughnessMap;
}

unsigned int glgeGetLighningBuffer()
{
    //return the lit image
    return glgeLightingImageOut;
}

bool glgeGetTransparencyPass()
{
    //return the pass status
    return glgeTransparentOpaquePass;
}

void glgeSetTransparencyCombineShader(Shader* shader)
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
    if (!glgeHasCustomTransparentCombineShader)
    {
        //if not, delete the old shader
        delete glgeTransparentCombineShader;
    }
    //store the shader
    glgeTransparentCombineShader = shader;
    //say that a custom shader is bound
    glgeHasCustomTransparentCombineShader = true;
}

Shader* glgeGetTransparencyCombineShader()
{
    //return the shader
    return glgeTransparentCombineShader;
}

bool glgeGetIfCustomTransparentCombineShaderIsBound()
{
    //return the boolean
    return glgeHasCustomTransparentCombineShader;
}

void glgeSetWindowBorderVisible(bool visible)
{
    //store the new window border state
    glgeHasWindowBorder = visible;
    //update the window border
    SDL_SetWindowBordered(glgeMainWindow, SDL_bool(visible));
}

bool glgeGetWindowBorderVisible()
{
    //return the current window borderd state
    return glgeHasWindowBorder;
}

void glgeSetWindowForceOpen(bool forceOpen)
{
    //store the new state of the variable
    glgeWindowForceOpen = forceOpen;
}

bool glgeGetWindowForceOpen()
{
    //return the current state of the variable
    return glgeWindowForceOpen;
}

void glgeCloseWindow(bool force)
{
    //check if this function should run
    if (glgeWindowForceOpen && !force)
    {
        //if not, stop this function
        return;
    }
    //else, say that the window should close
    glgeOPCloseWindow = true;
}

bool glgeGetWindowClosingInitiated()
{
    //return the current closing status
    return glgeOPCloseWindow;
}

void glgeStopWindowClosing(bool force)
{
    //check if this function should run
    if ((!glgeOPCloseWindow) || ((!force) && glgeOPCloseWindow))
    {
        //stop this function
        return;
    }
    //say that the window won't be closed
    glgeOPCloseWindow = false;
}

void glgeSetWindowAlwaysOnTop(bool onTop)
{
    //store the new state
    glgeWindowAlwaysOnTop = onTop;
    //update the window
    SDL_SetWindowAlwaysOnTop(glgeMainWindow, SDL_bool(onTop));
}

bool glgeGetWindowAlwaysOnTop()
{
    //return the current on top state
    return glgeWindowAlwaysOnTop;
}

void glgeSetWindowBrightness(float brightness)
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
    glgeWindowBrightness = brightness;
    //update the window brightness
    SDL_SetWindowBrightness(glgeMainWindow, brightness);
}

float glgeGetWindowBrightness()
{
    //return the current brightness
    return glgeWindowBrightness;
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
    //store the new show/hide state
    glgeWindowIsShown = show;
    //update the window
    if (show)
    {
        //show the window
        SDL_ShowWindow(glgeMainWindow);
    }
    else
    {
        //hide the window
        SDL_HideWindow(glgeMainWindow);
    }
}

bool glgeGetWindowShown()
{
    //return the current window show/hide state
    return glgeWindowIsShown;
}

void glgeMaximizeMinimizeWindow(bool maximized, bool force)
{
    //check if this function should execute
    if (!glgeAllowWindowResize && !force)
    {
        //stop this function
        return;
    }
    //store the new state
    glgeWindowMaximised = maximized;
    //maximize or minize the window
    if (maximized)
    {
        //maximize the window
        SDL_MaximizeWindow(glgeMainWindow);
    }
    else
    {
        //minize the window
        SDL_MinimizeWindow(glgeMainWindow);
    }
}

bool glgeGetWindowMaximized()
{
    //return the current maximized state
    return glgeWindowMaximised;
}

bool glgeGetWindowFocus()
{
    //get the input flags and maks out the input focus, then return it as an bool
    return (bool)(SDL_GetWindowFlags(glgeMainWindow) & SDL_WINDOW_INPUT_FOCUS);
}

bool glgeGetMouseFocus()
{
    //get the input flags and maks out the mouse focus, then return it as an bool
    return (bool)(SDL_GetWindowFlags(glgeMainWindow) & SDL_WINDOW_MOUSE_FOCUS);
}

void glgeSetMouseGrabMode(bool mode)
{
    //update the intern mode
    glgeMouseGrabMode = mode;
    //update the mode for the main window
    SDL_SetWindowMouseGrab(glgeMainWindow, SDL_bool(mode));
}

bool glgeGetMouseGrabMode()
{
    //return the intern mode
    return glgeMouseGrabMode;
}

void glgeFocusWindow(bool moveUp)
{
    //check if the window should move up
    if (moveUp)
    {
        //use the function that moves the window up
        SDL_RaiseWindow(glgeMainWindow);
    }
    else
    {
        //use the function that dosn't move the window up
        SDL_SetWindowInputFocus(glgeMainWindow);
    }
}