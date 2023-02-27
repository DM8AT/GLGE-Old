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

//include the CML dependencys
#include "CML/CMLVec2.h"

//include the default librarys
#include <iostream>
#include <fstream>
#include <cstring>

//include the OpenGL dependencys
#include <GL/glew.h>
#include <GL/freeglut.h>

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
        exit(1);
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
        exit(1);
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
        //stop the program
        exit(1);
    }
    //if no error occured, return the id of the uniform variable
    return ret;
}

//compile and add the shaders to this object
GLuint compileShader(const char* fileNameVS, const char* fileNameFS)
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
        exit(1);
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
        exit(1);
    }
    //add the shader program from the first file
    addShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);

    //read the second file
    if (!readFile(fileNameFS, fs))
    {
        //if the file can't be read, output an error message
        if (glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_OBJECT_COMPILE_SHADERS << std::endl;
        }
        //stop the program
        exit(1);
    }
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
        }
        //stop the program
        exit(1);
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
        exit(1);
    }

    //say open GL to use the shader program
    glUseProgram(shaderProgram);
    //return the shader program in the GLGE Object
    return shaderProgram;
}

//compile and add the shaders to this object
GLuint compileShader(std::string vs, std::string fs)
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
        exit(1);
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
        }
        //stop the program
        exit(1);
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
        exit(1);
    }

    //say open GL to use the shader program
    glUseProgram(shaderProgram);
    //return the shader program in the GLGE Object
    return shaderProgram;
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
        exit(1);
    }
    //if no error occured, create the window
    glutInitWindowSize(s.x,s.y);
    glutInitWindowPosition(p.x,p.y);
    glutCreateWindow(n);
    //say that the program should continue running after the window was closed
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    //initalise glew
    if (glewInit() != GLEW_OK)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_GLEW_INIT_FAILED, glewGetErrorString(glewInit()));
            std::cerr << GLGE_ERROR_STR_GLEW_INIT << std::endl;
        }
        //stop the program
        exit(1);
    }

    //say that an window was created
    glgeHasMainWindow = true;

    //store the size of the window
    glgeWindowSize = vec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

    //bind the window update function
    glutDisplayFunc(glgeDefaultDisplay);
    //bind the timer function
    glutTimerFunc(0, glgeDefaultTimer, 0);
    //bind the keyboard down and up function
    glutKeyboardFunc(glgeDefaultKeyFunc);
    glutKeyboardUpFunc(glgeDefaultKeyUpFunc);
    //bind the keyboard down and up function for special keys
    glutSpecialFunc(glgeDefaultSpecKeyFunc);
    glutSpecialUpFunc(glgeDefaultSpecKeyUpFunc);
    //bind the mouse update functions
    glutMouseFunc(glgeDefaultMouseFunc);
    glutPassiveMotionFunc(glgeDefaultPassiveMotionFunc);

    //say to cull backfasing triangles
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glgeUseCulling = true;

    //setup the depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    //setup transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enables Multisampling
	glEnable(GL_MULTISAMPLE);

    //create and bind the custom frame buffer
    glGenFramebuffers(1, &glgeMultFBO);
    //bind the new framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, glgeMultFBO);

    //generate the texture for the frame buffer
    glGenTextures(1, &glgeFrameBufferMultisampleTexture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, glgeFrameBufferMultisampleTexture);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, glgeSamples, GL_RGB, glgeWindowSize.x, glgeWindowSize.y, GL_TRUE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, glgeFrameBufferMultisampleTexture, 0);

    //generate the Render Buffer
    glGenRenderbuffers(1, &glgeMultRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, glgeMultRBO);
    //setup the storage for the render buffer
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, glgeSamples, GL_DEPTH24_STENCIL8, glgeWindowSize.x, glgeWindowSize.y);
    //attach an depth stencil
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, glgeMultRBO);

    //check if the framebuffer compiled correctly
    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //if the frame buffer compiled not correctly
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        //print an error
        std::cerr << GLGE_FATAL_ERROR_FRAMEBUFFER_NOT_COMPILED << fboStatus << std::endl;
        //stop the program
        exit(1);
    }

    //unbind the multi sample framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //unbind the multi sample render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    //create a second framebuffer for post processing

    //create and bind the custom frame buffer
    glGenFramebuffers(1, &glgeFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, glgeFBO);

    //generate the texture for the frame buffer
    glGenTextures(1, &glgeFrameBufferTexture);
    glBindTexture(GL_TEXTURE_2D, glgeFrameBufferTexture);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, glgeWindowSize.x, glgeWindowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //bind the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glgeFrameBufferTexture, 0);

    //generate the Render Buffer
    glGenRenderbuffers(1, &glgeRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, glgeRBO);
    //setup the storage for the render buffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, glgeWindowSize.x, glgeWindowSize.y);
    //attach an depth stencil
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, glgeRBO);
    //unbind the render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    //check if the framebuffer compiled correctly
    fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
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

    //generate the shaders for the default post processing
    glgePostProcessingShader = compileShader(GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER, GLGE_DEFAULT_POST_PROCESSING_FRAGMENT_SHADER);

    glUniform1i(glGetUniformLocation(glgePostProcessingShader, "screenTexture"), 0);
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