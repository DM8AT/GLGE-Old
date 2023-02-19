/**
 * @file GLGEShaderCore.cpp
 * @author DM8AT
 * @brief define the functions for the shader core of the GLGE library
 * @version 0.1
 * @date 2023-02-18
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license.
 */

#include "GLGEShaderCore.h"

//include GLGE dependencys
#include "GLGE/glgeFuncs.hpp"
#include "GLGE/glgeVars.hpp"
#include "GLGE/glgePrivDefines.hpp"

//include the default librarys
#include <iostream>
#include <math.h>

///////////
//CLASSES//
///////////

//SHADER

Shader::Shader()
{
    //init the object
}

Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile)
{
    //set the shader store variable to the compiled shader from the files
    this->shader = compileShader(vertexShaderFile, fragmentShaderFile);
}

Shader::Shader(std::string vertexShaderData, std::string fragmentShaderData)
{
    //set the shader store variable to the compiled shader from the strings
    this->shader = compileShader(vertexShaderData, fragmentShaderData);
}

Shader::Shader(const char* shaderFile, unsigned int type)
{
    //init an string to store the data from the file
    std::string data;

    //read the data form the file
    readFile(shaderFile, data);

    //if the inputed shader is an fragment shader
    if (type == GLGE_FRAGMENT_SHADER)
    {
        //set the shader store variable to the compilation of an empty vertex shader and the inputed data
        this->shader = compileShader(GLGE_EMPTY_VERTEX_SHADER, data);
    }
    //else, if the shader is an vertex shader
    else if (type == GLGE_VERTEX_SHADER)
    {
        //set the shader store variable to the copilation of the inputed file and an empty fragment shader
        this->shader = compileShader(data, GLGE_EMPTY_FRAGMENT_SHADER);
    }
    //if the type is invalide
    else
    {
        //set the stored shader to 0
        this->shader = 0;

        //if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            printf("[GLGE ERROR] Shader of type %d is not valide.\n", type);

            printf("             List of codes:\n             GLGE_VERTEX_SHADER: %d\n             GLGE_FRAGMENT_SHADER: %d\n", GLGE_VERTEX_SHADER, GLGE_FRAGMENT_SHADER);
        }
    }
}

Shader::Shader(std::string data, unsigned int type)
{
    //if the inputed shader is an fragment shader
    if (type == GLGE_FRAGMENT_SHADER)
    {
        //set the shader store variable to the compilation of an empty vertex shader and the inputed data
        this->shader = compileShader(GLGE_EMPTY_VERTEX_SHADER, data);
    }
    //else, if the shader is an vertex shader
    else if (type == GLGE_VERTEX_SHADER)
    {
        //set the shader store variable to the copilation of the inputed file and an empty fragment shader
        this->shader = compileShader(data, GLGE_EMPTY_FRAGMENT_SHADER);
    }
    //if the type is invalide
    else
    {
        //set the stored shader to 0
        this->shader = 0;

        //if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            printf("[GLGE ERROR] Shader of type %d is not valide.\n", type);

            printf("             List of codes:\n             GLGE_VERTEX_SHADER: %d\n             GLGE_FRAGMENT_SHADER: %d\n", GLGE_VERTEX_SHADER, GLGE_FRAGMENT_SHADER);
        }
    }
}

GLuint Shader::getShader()
{
    //return the stored shader
    return this->shader;
}

void Shader::deleteShader()
{
    //delete the shader program
    glDeleteProgram(this->shader);
    //set the stored shader to 0
    this->shader = 0;
}

void Shader::updateShader(const char* vertexShaderFile, const char* fragmentShaderFile)
{
    this->deleteShader();

    *this = Shader(vertexShaderFile, fragmentShaderFile);
}

void Shader::updateShader(std::string vertexShaderData, std::string fragmentShaderData)
{
    this->deleteShader();

    *this = Shader(vertexShaderData, fragmentShaderData);
}

void Shader::updateShader(const char* shaderFile, unsigned int type)
{
    this->deleteShader();

    *this = Shader(shaderFile, type);
}

void Shader::updateShader(std::string shaderData, unsigned int type)
{
    this->deleteShader();

    *this = Shader(shaderData, type);
}

/////////////
//FUNCTIONS//
/////////////

Shader glgeCreateKernalShader(float* kernal, int size)
{
    std::string header("#version 300 es\nprecision mediump float;out vec4 FragColor;in vec2 texCoords;uniform sampler2D screenTexture;");
    std::string width = std::to_string(glgeWindowSize.x);
    std::string height = std::to_string(glgeWindowSize.y);

    std::string body1 = std::string("const float offset_x = 1.f / ")+width+(";const float offset_y = 1.f / ") + height + std::string(";"); 

    std::string offs = std::string("vec2 offsets["+std::to_string(size*size)+"] = vec2[](");

    std::string mult = std::to_string((float)size/2.f - 0.5f)+std::string("*");

    int x = 0;
    int y = 0;

    for (int i = 0; i < size*size; i++)
    {
        if (x > size-1)
        {
            x = 0;
            y++;
        }

        offs += std::string("vec2(")+mult;

        if (x < (float)size/2.f - 0.5f)
        {
            offs += std::string("-offset_x,");
        }
        else if (x == (float)size/2.f - 0.5f)
        {
            offs += std::string("0.f,");
        }
        else
        {
            offs += std::string("offset_x,");
        }

        offs += mult;

        if (y < ((float)size/2.f - 0.5f))
        {
            offs += std::string("offset_y");
        }
        else if (y == (float)size/2.f - 0.5f)
        {
            offs += std::string("0.f");
        }
        else
        {
           offs += std::string("-offset_y");
        }

        if (x == size-1 && y == size-1)
        {
            offs += std::string("));");
        }
        else
        {
            offs += std::string("),");
        }

        x++;
    }

    std::string kern = std::string("float kernel["+std::to_string(size*size)+"] = float[](");

    for (int i = 0; i < size*size; i++)
    {
        if (i == size*size - 1)
        {
            kern += std::to_string(kernal[i])+std::string(");");
        }
        else
        {
            kern += std::to_string(kernal[i])+std::string(",");
        }
    }

    std::string body2 = std::string("void main(){vec3 color = vec3(0.0f);for(int i = 0; i < "+std::to_string(size*size)+"; i++){color += vec3(texture(screenTexture, texCoords.st + offsets[i])) * kernel[i];}FragColor = vec4(color, 1.0f);}");

    std::string shader = header + body1 + offs + kern + body2;

    return Shader(shader, GLGE_FRAGMENT_SHADER);
}

Shader glgeCreateKernalShader(float* kernal, unsigned long size)
{
    size = std::sqrt((float)size / (float)sizeof(float));

    std::string header("#version 300 es\nprecision mediump float;out vec4 FragColor;in vec2 texCoords;uniform sampler2D screenTexture;");
    std::string width = std::to_string(glgeWindowSize.x);
    std::string height = std::to_string(glgeWindowSize.y);

    std::string body1 = std::string("const float offset_x = 1.f / ")+width+(";const float offset_y = 1.f / ") + height + std::string(";"); 

    std::string offs = std::string("vec2 offsets["+std::to_string(size*size)+"] = vec2[](");

    std::string mult = std::to_string((float)size/2.f - 0.5f)+std::string("*");

    long unsigned int x = 0;
    long unsigned int y = 0;

    for (long unsigned int i = 0; i < (size*size); i++)
    {
        if (x > size-1)
        {
            x = 0;
            y++;
        }

        offs += std::string("vec2(")+mult;

        if (x < (float)size/2.f - 0.5f)
        {
            offs += std::string("-offset_x,");
        }
        else if (x == (float)size/2.f - 0.5f)
        {
            offs += std::string("0.f,");
        }
        else
        {
            offs += std::string("offset_x,");
        }

        offs += mult;

        if (y < ((float)size/2.f - 0.5f))
        {
            offs += std::string("offset_y");
        }
        else if (y == (float)size/2.f - 0.5f)
        {
            offs += std::string("0.f");
        }
        else
        {
           offs += std::string("-offset_y");
        }

        if (x == size-1 && y == size-1)
        {
            offs += std::string("));");
        }
        else
        {
            offs += std::string("),");
        }

        x++;
    }

    std::string kern = std::string("float kernel["+std::to_string(size*size)+"] = float[](");

    for (long unsigned int i = 0; i < size*size; i++)
    {
        if (i == size*size - 1)
        {
            kern += std::to_string(kernal[i])+std::string(");");
        }
        else
        {
            kern += std::to_string(kernal[i])+std::string(",");
        }
    }

    std::string body2 = std::string("void main(){vec3 color = vec3(0.0f);for(int i = 0; i < "+std::to_string((int) size*size)+"; i+=1){color += vec3(texture(screenTexture, texCoords.st + offsets[i])) * kernel[i];}FragColor = vec4(color, 1.0f);}");

    std::string shader = header + body1 + offs + kern + body2;

    return Shader(shader, GLGE_FRAGMENT_SHADER);
}