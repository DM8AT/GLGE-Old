/**
 * @file glgeFuncs.cpp
 * @author DM8AT
 * @brief define the behind-the-sceenes functions
 * @version 0.1
 * @date 2023-02-09
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

//include the GLGE dependencys
#include "glgeErrors.hpp"
#include "glgeVars.hpp"
#include "glgeDefaultFuncs.hpp"
#include "GLGEDefines.hpp"
#include "GLGE.h"

//include the CML dependencys
#include "CML/CMLVec2.h"

//include the default librarys
#include <iostream>
#include <fstream>
#include <cstring>

//include acess to images
#include "glgeImage.h"

//include the OpenGL dependencys
#include <GL/glew.h>

//read a file
bool readFile(const char* filename, std::string& output)
{
    //create a new ifstream object containing the file
    std::ifstream f(filename);

    //check if the file can be read
    bool ret = false;

    //check if the file is opend
    if(f.is_open())
    {
        //create a variable to store a single line
        std::string line;
        //loop over all lines in the file and store them
        while(getline(f, line))
        {
            //add the lines to the output
            output.append(line);
            //add a new line character to the output
            output.append("\n");
        }
        //close the file
        f.close();

        //say that the file could be opend
        ret = true;
    }
    else
    {
        //else print an error message
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_FILE_NOT_FOUND, filename);
        }
    }

    //return if the file could be read
    return ret;
}

//an copy of the shader code to make the shader handeling here easier

void addShader(GLuint shaderProgram, const char* shadertext, GLenum shaderType)
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
GLint getUniformVar(GLuint program, const char* name)
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
        //return -1
        return -1;
    }
    //if no error occured, return the id of the uniform variable
    return ret;
}

//compile and add the shaders to this object
GLuint compileShader(std::string vs, std::string fs, const char* fileVertexShader, const char* fileFragmentShader)
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
    addShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);

    //add the shader program from the second file
    addShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

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
            printf(GLGE_ERROR_OCCURED_IN_FILES, fileVertexShader, fileFragmentShader);
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
            printf(GLGE_ERROR_OCCURED_IN_FILES, fileVertexShader, fileFragmentShader);
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

//compile and add the shaders to this object
GLuint compileShader(const char* fileNameVS, const char* fileNameFS)
{
    //store the source of the vertex shader
    std::string vs;
    //store the source of the fragment shader
    std::string fs;

    //read the vertex shader
    readFile(fileNameVS, vs);
    //read the fragment shader
    readFile(fileNameFS, fs);

    //pass the info to another compilation function
    return compileShader(vs, fs, fileNameVS, fileNameFS);
}

void getLightingUniformsFromLightingPass()
{
    //reset the uniform list
    glgeLightColInLightingPass.clear();
    glgeLightIntInLightingPass.clear();
    glgeLightPosInLightingPass.clear();

    //set the prefix for the light position
    std::string prefixLightPos = std::string("glgeLightPos[");
    //set the prefix for the light color
    std::string prefixLightCol = std::string("glgeLightColor[");
    //set the prefix for the light intensity
    std::string prefixLightInt = std::string("glgeLightInt[");

    for (int i = 0; i < (int)glgeLights.size(); i++)
    {
        //get the uniform for the light color
        glgeLightColInLightingPass.push_back(getUniformVar(glgeLightingShader, (prefixLightCol + std::to_string(i) + std::string("]")).c_str()));
        //get the uniform for the light intensity
        glgeLightIntInLightingPass.push_back(getUniformVar(glgeLightingShader, (prefixLightInt + std::to_string(i) + std::string("]")).c_str()));
        //get the uniform for the light position
        glgeLightPosInLightingPass.push_back(getUniformVar(glgeLightingShader, (prefixLightPos + std::to_string(i) + std::string("]")).c_str()));
    }
}

void getDefaultUniformsFromPostProcessingShader()
{
    //store the glge error output
    bool glgeErr = glgeErrorOutput;
    //glgeSetErrorOutput(false);

    //get the main image variable from the shader
    glgeMainImageInPPS = glgeGetUniformVar(glgePostProcessingShader, "glgeMainImage");

    //disable the error output
    glgeSetErrorOutput(false);
    //get the position map from the post processing shader
    glgeAlbedoInPPS = glgeGetUniformVar(glgePostProcessingShader, "glgeAlbedoMap");
    //get the position map from the post processing shader
    glgeNormalInPPS = glgeGetUniformVar(glgePostProcessingShader, "glgeNormalMap");
    //get the position map from the post processing shader
    glgePositionInPPS = glgeGetUniformVar(glgePostProcessingShader, "glgePositionMap");
    //get the position map from the post processing shader
    glgeRoughnessInPPS = glgeGetUniformVar(glgePostProcessingShader, "glgeRoughnessMap");

    //reset the glge error output
    glgeSetErrorOutput(glgeErr);

    //if the main image wasn't found, print the output
    if ((glgeMainImageInPPS == -1) && glgeErrorOutput)
    {
        std::cerr << "[GLGE ERROR] couldn't find main image uniform in post processing shader" << std::endl;
    }
}

bool getUniformsForLightingShader()
{
    //let the error messages set for the albedo map, you want the albedo map in most cases
    glgeAlbedoInLightingPass = getUniformVar(glgeLightingShader, "glgeAlbedoMap");
    //next, turn the errors temporarely off
    //get if the errors are turned on
    bool error = glgeGetErrorOutput();
    //turn the errors of
    glgeSetErrorOutput(false);
    //get the unform for the normal map
    glgeNormalInLightingPass = getUniformVar(glgeLightingShader, "glgeNormalMap");
    //get the uniform for the position map
    glgePositionInLightingPass = getUniformVar(glgeLightingShader, "glgePositionMap");
    //get the uniform for the roughness map
    glgeRoughnessInLightingPass = getUniformVar(glgeLightingShader, "glgeRoughnessMap");

    //get the lighting uniforms
    getLightingUniformsFromLightingPass();
    
    //get the uniform for the amount of active lights
    glgeActiveLightInLightingPass = getUniformVar(glgeLightingShader, "glgeActiveLights");
    //get the uniform for the camera position
    glgeCamPosInLightingPass = getUniformVar(glgeLightingShader, "glgeCameraPos");
    //get the uniform for the far plane
    glgeFarPlaneInLightingPass = getUniformVar(glgeLightingShader, "glgeFarPlane");
    //get the camera rotation matrix in the lighting shader
    glgeRotInLightingPass = getUniformVar(glgeLightingShader, "glgeCameraRot");
    //get the camera projection matrix in the lighting shader
    glgeProjInLightingPass = getUniformVar(glgeLightingShader, "glgeProject");
    //reset the error output
    glgeSetErrorOutput(error);

    //if the albedo LightingPass is -1, then there is no albedo map, so return false
    return (glgeAlbedoInLightingPass != -1);
}

/**
 * @brief Create a Window
 * 
 * this function is only called internaly in GLGE
 * 
 * @param n the name of the window
 * @param s the size of the window
 * @param p the position of the window
 */
void createWindow(const char* n, vec2 s, vec2 p)
{
    //check for errors in the window creation
    bool error = false;
    if (glgeHasMainWindow)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_TWO_WINDOWS);
        }
        error = true;
    }
    //if an error occured, return
    if (error)
    {
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_WINDOW_SUB_FUNC<< std::endl;
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }
    //if no error occured, create the window

    //initalise SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        //if sdl throws an value different than 0, print an error
        std::cerr << "[FATAL ERROR] GLGE failed to initalise SDL, SDL error: " << SDL_GetError() << std::endl;
        //exit with an error
        exit(1);
    }

    //initalise to OpenGL 3.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    //say that doublebuffering should be used
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //set the depth accuarcy
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    //store the SDL Window
    SDL_Window* window;
    //create an window using SDL2
    window = SDL_CreateWindow(n, p.x, p.y, s.x, s.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    //disable VSync
    SDL_GL_SetSwapInterval(0);

    //check if the window could be created
    if (!window)
    {
        //if the window couldn't be created, throw an fatal error
        std::cerr << "[FATAL ERROR] GLGE couldn't create the SDL main window. SDL error: " << SDL_GetError() << std::endl;
        //exit with an error
        exit(1);
    }

    //store the SDL Context
    SDL_GLContext context;
    //get the OpenGL context from the window
    context = SDL_GL_CreateContext(window);

    //check if the context is valide
    if (context == NULL)
    {
        //if the context is not valide, print an fatal error
        std::cerr << "[FATAL ERROR] GLGE could not create a OpenGL Context to the SDL Window. SDL Error: " << SDL_GetError() << std::endl;
        //close SDL
        SDL_Quit();
        //exit with an error
        exit(1);
    }

    //initalise glew
    if (glewInit() != GLEW_OK)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_GLEW_INIT_FAILED, glewGetErrorString(glewInit()));
            std::cerr << GLGE_ERROR_STR_GLEW_INIT << std::endl;
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //say that an window was created
    glgeHasMainWindow = true;

    //store the window pointer
    glgeMainWindow = window;

    //store the size of the window
    glgeWindowSize = s;
    //store the position of the window
    glgeWindowPosition = p;

    //say to cull backfasing triangles
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glgeUseCulling = true;

    //disable clamping in the vertex shader
    glClampColorARB(GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE);
    //disable clamping in the fragment shader
    glClampColorARB(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE);

    //setup the depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    //setup transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //create and bind the custom frame buffer
    glGenFramebuffers(1, &glgeFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, glgeFBO);

    //generate the Render Buffer
    glGenRenderbuffers(1, &glgeRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, glgeRBO);
    //setup the storage for the render buffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, glgeWindowSize.x, glgeWindowSize.y);
    //attach an depth stencil
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, glgeRBO);

    //generate the texture for the frame buffer
    glGenTextures(1, &glgeFrameAlbedoMap);
    glBindTexture(GL_TEXTURE_2D, glgeFrameAlbedoMap);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, glgeWindowSize.x, glgeWindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glgeFrameAlbedoMap, 0);
    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //generate a texture to store the normals
    glGenTextures(1, &glgeFrameNormalMap);
    glBindTexture(GL_TEXTURE_2D, glgeFrameNormalMap);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, glgeWindowSize.x, glgeWindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, glgeFrameNormalMap, 0);
    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //generate a texture to store the fragment position
    glGenTextures(1, &glgeFramePositionMap);
    glBindTexture(GL_TEXTURE_2D, glgeFramePositionMap);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, glgeWindowSize.x, glgeWindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, glgeFramePositionMap, 0);
    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //generate a texture to store the fragment roughness
    glGenTextures(1, &glgeFrameRoughnessMap);
    glBindTexture(GL_TEXTURE_2D, glgeFrameRoughnessMap);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, glgeWindowSize.x, glgeWindowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, glgeFrameRoughnessMap, 0);

    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //unbind the render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    //specify the number of used color buffers
    glDrawBuffers(glgeLenUsedColorBuffers, glgeUsedColorBuffers);

    //check if the framebuffer compiled correctly
    GLuint fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //if the frame buffer compiled not correctly
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        //print an error
        std::cerr << GLGE_FATAL_ERROR_FRAMEBUFFER_NOT_COMPILED << fboStatus << std::endl;
        //stop the program
        exit(1);
    }

    //create and bind the second custom frame buffer
    glGenFramebuffers(1, &glgeFBOLastTick);
    glBindFramebuffer(GL_FRAMEBUFFER, glgeFBOLastTick);

    //generate the second Render Buffer
    glGenRenderbuffers(1, &glgeRBOLastTick);
    glBindRenderbuffer(GL_RENDERBUFFER, glgeRBOLastTick);
    //setup the storage for the render buffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, glgeWindowSize.x, glgeWindowSize.y);
    //attach an depth stencil
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, glgeRBOLastTick);

    //generate a texture to store the last tick image
    glGenTextures(1, &glgeFrameLastTick);
    glBindTexture(GL_TEXTURE_2D, glgeFrameLastTick);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, glgeWindowSize.x, glgeWindowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glgeFrameLastTick, 0);

    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //if the frame buffer compiled not correctly
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        //print an error
        std::cerr << GLGE_FATAL_ERROR_FRAMEBUFFER_NOT_COMPILED << fboStatus << std::endl;
        //stop the program
        exit(1);
    }

    //create and bind the thired custom frame buffer
    glGenFramebuffers(1, &glgeLightingFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, glgeLightingFBO);

    //generate the second Render Buffer
    glGenRenderbuffers(1, &glgeLightingRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, glgeLightingRBO);
    //setup the storage for the render buffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, glgeWindowSize.x, glgeWindowSize.y);
    //attach an depth stencil
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, glgeLightingRBO);

    //generate a texture to store the lighting pass output
    glGenTextures(1, &glgeLightingImageOut);
    glBindTexture(GL_TEXTURE_2D, glgeLightingImageOut);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, glgeWindowSize.x, glgeWindowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glgeLightingImageOut, 0);

    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //if the frame buffer compiled not correctly
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        //print an error
        std::cerr << GLGE_FATAL_ERROR_FRAMEBUFFER_NOT_COMPILED << fboStatus << std::endl;
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
    glGenVertexArrays(1, &glgeScreenVAO);
    glGenBuffers(1, &glgeScreenVBO);
    //bind the array buffer
    glBindBuffer(GL_ARRAY_BUFFER, glgeScreenVBO);
    //load the data into the array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
    //unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //generate the shaders for the default lighting shader
    glgeLightingShader = compileShader(GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER, GLGE_DEFAULT_LIGHTING_SHADER, "GLGE:BuildInPostProcessingVertexShader", "GLGE:BuildInPostProcessingFragmentShader");

    //get the uniforms form the shader
    bool albedo = getUniformsForLightingShader();

    //check if the albedo map was found
    if (!albedo)
    {
        std::cerr << "Could not find albedo map in default lighthing shader" << std::endl;
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //get the uniform for the screen texture in the lighting shader
    glUniform1i(glGetUniformLocation(glgeLightingShader, "screenTexture"), 0);


    //generate the shaders for the default lighting shader
    glgePostProcessingShader = compileShader(GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER, GLGE_DEFAULT_POST_PROCESSING_FRAGMENT_SHADER, "GLGE:BuildInPostProcessingVertexShader", "GLGE:BuildInPostProcessingFragmentShader");

    //get the uniform for the screen texture in the post processing shader
    glUniform1i(glGetUniformLocation(glgePostProcessingShader, "glgeMainImage"), 0);

    //load the default lighting shader
    glgeSetLightingShader(GLGE_DEFAULT_LIGHTING_SHADER);

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
    glGenVertexArrays(1, &glgeSkyboxBuffer);
    glGenBuffers(1, &glgeSkyboxBuffer);
    //bind the array buffer
    glBindBuffer(GL_ARRAY_BUFFER, glgeSkyboxBuffer);
    //load the data into the array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVerts), &skyboxVerts, GL_STATIC_DRAW);
    //unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    //generate the shaders for the default lighting shader
    glgeSkyboxShader = glgeCompileShader(GLGE_DEFAULT_VERTEX_SKYBOX, GLGE_DEFAULT_FRAGMENT_SKYBOX);

    //get the uniforms
    //get the rotation uniform
    glgeSkyboxRotation = getUniformVar(glgeSkyboxShader, "glgeRot");
    //get the projection uniform
    glgeSkyboxProject = getUniformVar(glgeSkyboxShader, "glgeProject");
    //get the sampler
    glgeSkyboxSampler = getUniformVar(glgeSkyboxShader, "glgeSkybox");
}

//convert an error code into an string
char* getGLErrorString(unsigned int error)
{
    //check for all errors OpenGL has and return theyre name
    if (error == GL_NO_ERROR)
    {
        return (char*)"GL_NO_ERROR";
    }
    else if (error == GL_INVALID_ENUM)
    {
        return (char*)"GL_INVALIDE_ENUM";
    }
    else if (error == GL_INVALID_VALUE)
    {
        return (char*)"GL_INVALIDE_VALUE";
    }
    else if (error == GL_INVALID_OPERATION)
    {
        return (char*)"GL_INVALIDE_OPERATION";
    }
    else if (error == GL_STACK_OVERFLOW)
    {
        return (char*)"GL_STACK_OVERFLOW";
    }
    else if (error == GL_STACK_UNDERFLOW)
    {
        return (char*)"GL_STACK_UNDERFLOW";
    }
    else if (error == GL_OUT_OF_MEMORY)
    {
        return (char*)"GL_OUT_OF_MEMORY";
    }
    else if (error == GL_TABLE_TOO_LARGE)
    {
        return (char*)"GL_TABLE_TOO_LARGE";
    }
    else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
    {
        return (char*)"GL_INVALID_FRAMEBUFFER_OPERATION";
    }
    else
    {
        //if the error dosn't match any known error code, output an standart error
        return (char*)"GLGE_UNKNOWN_GL_ERROR";
    }
}

int count_char(std::string &str, char ch)
{
    //begin the counting at 0
    int c = 0;
    //get the length of the str
    int length = str.length();
    //loop over every item in the string
    for (int i = 0; i < length; i++)
    {
        //if the item of the string is equal to the specified letter, increase c by one
        if (str[i] == ch)
            c++;
    }
    //return c, that contains the amount of ch in str
    return c;
}

void resizeWindow(int width, int height)
{
    //update the internal parameters
    //recalculate the window aspect ratio
    glgeWindowAspect = ((float)width/(float)height);
    //updated the stored window size
    glgeWindowSize = vec2(width,height);

    //check if the window is in fullscreen mode
    if (!glgeFullscreen)
    {
        //if it is not currently fullscreen mode, store the window size in the normal window size variable too
        glgeNormalWindowSize = glgeWindowSize;
    }
    
    //update all the renderbuffer sizes

    //update the window size of the frame buffer
    glBindRenderbuffer(GL_RENDERBUFFER, glgeRBO);
    //setup the storage for the render buffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, glgeWindowSize.x, glgeWindowSize.y);

    //update the window size of the second frame buffer to store the last tick
    glBindRenderbuffer(GL_RENDERBUFFER, glgeRBOLastTick);
    //setup the storage for the render buffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, glgeWindowSize.x, glgeWindowSize.y);

    //update the window size of the thierd frame buffer to store the last tick
    glBindRenderbuffer(GL_RENDERBUFFER, glgeLightingRBO);
    //setup the storage for the render buffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, glgeWindowSize.x, glgeWindowSize.y);

    //unbind the renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //update the render texture parameters
    glBindTexture(GL_TEXTURE_2D, glgeFrameAlbedoMap);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, glgeWindowSize.x, glgeWindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);

    //update all the texture sizes
    //update the normal texture parameters
    glBindTexture(GL_TEXTURE_2D, glgeFrameNormalMap);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, glgeWindowSize.x, glgeWindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);

    //update the position texture parameters
    glBindTexture(GL_TEXTURE_2D, glgeFramePositionMap);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, glgeWindowSize.x, glgeWindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);

    //update the roughness texture parameters
    glBindTexture(GL_TEXTURE_2D, glgeFrameRoughnessMap);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, glgeWindowSize.x, glgeWindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);

    //update the last tick texture
    glBindTexture(GL_TEXTURE_2D, glgeFrameLastTick);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, glgeWindowSize.x, glgeWindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);

    //update the main image texture
    glBindTexture(GL_TEXTURE_2D, glgeMainImageInPPS);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, glgeWindowSize.x, glgeWindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);

    //update the lighting texture
    glBindTexture(GL_TEXTURE_2D, glgeLightingImageOut);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, glgeWindowSize.x, glgeWindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);

    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //update the OpenGL viewport
    glViewport(0,0, width,height);
}

SDL_Surface* loadImage(const char* file)
{
    //check if the file exists
    //start an ifstream with the file
    std::fstream f;
    //open the inputed file
    f.open(file);
    //check if the file is good
    bool exists = f.good();
    //close the file
    f.close();
    //if the file dosn't exist
    if (!exists)
    {
        //print a warning message
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_FILE_NOT_FOUND, file);
        }
        //check if the program should close
        if (glgeExitOnError)
        {
            //if it should, close with an error
            exit(1);
        }
    }

    //tutorial to the image loading function: https://www.silbinarywolf.com/post/124379907558/loading-png-files-with-stbimage-and-sdl2
    //store the width of the image
    int w;
    //store the height of the image
    int h;
    //store how
    int bytesPerPixel;

    //load the inputed file
    unsigned char* data = glgeLoad(file, &w, &h, &bytesPerPixel);

    //Store the pitch of the image
	int pitch;
    //Calculate pitch
    pitch = w * bytesPerPixel;
    pitch = (pitch + 3) & ~3;
    
    // Setup relevance bitmask
	int Rmask, Gmask, Bmask, Amask;
    #if SDL_BYTEORDER == SDL_LIL_ENDIAN
        Rmask = 0x000000FF;
        Gmask = 0x0000FF00;
        Bmask = 0x00FF0000;
        Amask = (bytesPerPixel == 4) ? 0xFF000000 : 0;
    #else
        int s = (bytesPerPixel == 4) ? 0 : 8;
        Rmask = 0xFF000000 >> s;
        Gmask = 0x00FF0000 >> s;
        Bmask = 0x0000FF00 >> s;
        Amask = 0x000000FF >> s;
    #endif
    
    //load the image to an SDL Surface
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(data, w, h, bytesPerPixel*8, pitch, Rmask, Gmask, Bmask, Amask);

    //check if the surface loaded successfull
    if (!surface)
    {
        //clear the data
        glgeImageFree(data);

        //check if GLGE should print an warning
        if (glgeWarningOutput)
        {
            //print an error
            std::cerr << "[GLGE WARNING] failed to create SDL_Surface from fiel " << file << std::endl;
        }

        //return 0
        return NULL;
    }
    //if the surface construction was sucessfull, return the surface
    return surface;
}