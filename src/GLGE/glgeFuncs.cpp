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
//check if glew is allready included
#ifndef _GLGE_GLEW_
//say that glew is now included
#define _GLGE_GLEW_
//include glew
#include <GL/glew.h>
//close the if for glew
#endif

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

void addShader(unsigned int shaderProgram, const char* shadertext, GLenum shaderType)
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
int getUniformVar(unsigned int program, const char* name)
{
    //create the output variable
    int ret;
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
            std::cerr << GLGE_ERROR_STR_OBJECT_GET_UNIFORM_VARIABLE << "\n";
        }
        //return -1
        return -1;
    }
    //if no error occured, return the id of the uniform variable
    return ret;
}

//compile and add the shaders to this object
unsigned int compileShader(std::string vs, std::string fs, const char* fileVertexShader, const char* fileFragmentShader)
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
    addShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);

    //add the shader program from the second file
    addShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

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
unsigned int compileShader(const char* fileNameVS, const char* fileNameFS)
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

/**
 * @brief Create a Window
 * 
 * this function is only called internaly in GLGE
 * 
 * @param n the name of the window
 * @param s the size of the window
 * @param p the position of the window
 */
GLGEWindow* createWindow(const char* n, vec2 s, vec2 p)
{
    //check if an window is allready created
    if (!glgeHasMainWindow)
    {
        //store the main window index
        glgeMainWindowIndex = (int)glgeWindows.size();
        //say that an window was created
        glgeHasMainWindow = true;
    }
    //create the window pointer
    GLGEWindow* wptr = new GLGEWindow(n, s, p, glgeWindowFlags);
    //start the window
    wptr->start();
    //make the window the current window
    wptr->makeCurrent();
    //return the window
    return wptr;
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
            std::cerr << "[GLGE WARNING] failed to create SDL_Surface from fiel " << file << "\n";
        }

        //return 0
        return NULL;
    }
    //if the surface construction was sucessfull, return the surface
    return surface;
}