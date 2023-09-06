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


////////////////////
//Public functions//
////////////////////

//init function for glge
void glgeInit(int argc, char** argv)
{
    //error check
    bool error = false;
    //if argc = 0, the length of argv is 0 --> output error, but only if error output is enabled
    if (argc == 0)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_ARGC_OUT_OF_BOUNDS, argc);
        }
        error = true;
    }
    //if argv is the nullpointer, output an error if error output is enabled
    if (argv == nullptr)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_NULLPOINTER_INP_INIT);
        }
        error = true;
    }
    //if an error occoured, stop the function
    if (error)
    {
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_GLGE_INIT_ERROR << std::endl;
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
            std::cerr << GLGE_ERROR_STR_WINDOW_INPUT_ERROR << std::endl;
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
            std::cerr << GLGE_ERROR_STR_WINDOW_INPUT_ERROR << std::endl;
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
                            /* Change view port to the new window dimensions */
                            glgeDefaultResizeFunc(event.window.data1, event.window.data2);
                            /* Update window content */
                            glgeDefaultDisplay();
                            break;
                    }
            }
            //check if the window should close:
            if (event.type == SDL_QUIT)
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
            std::cerr << GLGE_ERROR_STR_SET_CLEAR_COLOR << std::endl;
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
            std::cerr << GLGE_ERROR_STR_SET_CLEAR_COLOR << std::endl;
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
            std::cerr << GLGE_ERROR_STR_SET_CLEAR_COLOR << std::endl;
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
    GLint texMode = GL_RGB;
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
            std::cerr << "[GLGE ERROR] the skybox texture was too larg: specified size was " << width << ", mixmal allowed size is " << GL_MAX_TEXTURE_BUFFER_SIZE << std::endl;
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
            std::cerr << "[GLGE ERROR] the skybox texture was too larg: specified size was " << height << ", mixmal allowed size is " << GL_MAX_TEXTURE_BUFFER_SIZE << std::endl;
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
            std::cerr << GLGE_ERROR_STR_BIND_DISPLAY_CALLBACK << std::endl;
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
            std::cerr << GLGE_ERROR_STR_BIND_MAIN_CALLBACK << std::endl;
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
            std::cerr << GLGE_ERROR_STR_SET_MAX_FPS << std::endl;
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
            std::cerr << GLGE_ERROR_STR_GLGE_SET_MOV_MAT_NAME << std::endl;
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
void glgeAddShader(GLuint shaderProgram, const char* shadertext, GLenum shaderType)
{
    //create a new shader with the inputed type
    GLuint shaderObj = glCreateShader(shaderType);

    //check if the shader object could be created
    if (shaderObj == 0)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            std::cout << GLGE_ERROR_COULD_NOT_CREATE_SHADER << shaderType << std::endl;
            //print where the error occured
            std::cerr << GLGE_ERROR_STR_OBJECT_ADD_SHADER << std::endl;
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
    GLint lengths[1];
    lengths[0] = strlen(shadertext);

    //set the shader source code
    glShaderSource(shaderObj, 1, p, lengths);

    //compile to the new shader object
    glCompileShader(shaderObj);

    //check for compiling errors
    GLint success;
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
            std::cerr << GLGE_ERROR_STR_OBJECT_ADD_SHADER << std::endl;
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
GLint glgeGetUniformVar(GLuint program, const char* name)
{
    //create the output variable
    GLint ret;
    //set the output variable to the uniform variable in the shader
    ret = glGetUniformLocation(program, name);
    //if the id is -1, output an error
    if (ret == -1)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_UNIFORM_VAR_NOT_FOUND, name);
            //say where the error occured
            std::cerr << GLGE_ERROR_STR_OBJECT_GET_UNIFORM_VARIABLE << std::endl;
        }
        //return 0
        return -1;
    }
    //if no error occured, return the id of the uniform variable
    return ret;
}

//compile and add the shaders to this object
GLuint glgeCompileShader(const char* fileNameVS, const char* fileNameFS)
{
    //create a new shader program
    GLuint shaderProgram = glCreateProgram();

    //check if the shader could be created
    if (shaderProgram == 0)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_COULD_NOT_CREATE_SHADER);
            //say where the error occured
            std::cerr << GLGE_ERROR_STR_OBJECT_COMPILE_SHADERS << std::endl;
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
            std::cerr << GLGE_ERROR_STR_OBJECT_COMPILE_SHADERS << std::endl;
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
            std::cerr << GLGE_ERROR_STR_OBJECT_COMPILE_SHADERS << std::endl;
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
    GLint success = 0;
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

GLuint glgeCompileShader(std::string fileDataVertex, std::string fileDataFragment)
{
    //create a new shader program
    GLuint shaderProgram = glCreateProgram();

    //check if the shader could be created
    if (shaderProgram == 0)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_COULD_NOT_CREATE_SHADER);
            //say where the error occured
            std::cerr << GLGE_ERROR_STR_OBJECT_COMPILE_SHADERS << std::endl;
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
    GLint success = 0;
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

GLuint glgeTextureFromFile(const char* name, vec2* sP)
{
    GLuint texture;
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
        std::cout << "Failed to load texture file: " << name << std::endl;
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
        std::cout << "Failed to load texture file: " << name << std::endl;
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
            std::cerr << GLGE_ERROR_STR_OBJECT_COMPILE_SHADERS << std::endl;
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

void glgeSetLightingShader(GLuint shader)
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

void glgeSetPostProcessingShader(const char* shaderFile)
{
    //create strings for the shader
    std::string data;

    //read the files
    if (!readFile(shaderFile, data))
    {
        //output an error message
        if (glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_OBJECT_COMPILE_SHADERS << std::endl;
        }
        //stop the script
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //compile the shader and save it
    glgePostProcessingShader = glgeCompileShader(GLGE_EMPTY_VERTEX_SHADER, data);

    //get the default uniforms
    getDefaultUniformsFromPostProcessingShader();
}

void glgeSetPostProcessingShader(std::string shader)
{
    //compile the shader and save it
    glgePostProcessingShader = glgeCompileShader(GLGE_EMPTY_VERTEX_SHADER, shader);

    //get the default uniforms
    getDefaultUniformsFromPostProcessingShader();
}

void glgeSetPostProcessingShader(GLuint shader)
{
    //store the inputed shader as the Lighting shader
    glgePostProcessingShader = shader;

    //get the default uniforms
    getDefaultUniformsFromPostProcessingShader();
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
            std::cerr << "[GLGE ERROR] couldn't create SDL system coursor, SDL error: " << SDL_GetError() << std::endl;
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
    //return the screen size
    return glgeWindowSize;
}

float glgeGetCurrentElapsedTime()
{
    return SDL_GetTicks();
}

GLuint glgeGetMainAlbedoMap()
{
    //return the main albedo texture
    return glgeFrameAlbedoMap;
}

GLuint glgeGetMainNormalMap()
{
    //return the main normal map
    return glgeFrameNormalMap;
}

GLuint glgeGetMainPositionMap()
{
    //return the main position map
    return glgeFramePositionMap;
}

GLuint glgeGetMainRoughnessMap()
{
    //return the main roughness map
    return glgeFrameRoughnessMap;
}

GLuint glgeGetLastFrame()
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

void glgeUseDownUpSampeling(bool shouldUse)
{
    //store the inputed state
    glgeDownUpSampeling = shouldUse;
}

void glgeSwapDownUpSampeling()
{
    //swap the current state of use
    glgeDownUpSampeling = !glgeDownUpSampeling;
}

bool glgeGetDownUpSampeling()
{
    //output the current state of use
    return glgeDownUpSampeling;
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
            std::cerr << "[GLGE ERROR] failed to create SDL_Surface for image to change the window icon, SDL error: " << SDL_GetError() << std::endl;
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