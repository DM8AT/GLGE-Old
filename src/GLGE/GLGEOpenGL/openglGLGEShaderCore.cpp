/**
 * @file openglGLGEShaderCore.cpp
 * @author DM8AT
 * @brief define the opengl dependand shader core of GLGE
 * @version 0.1
 * @date 2024-01-02
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#include "openglGLGEShaderCore.h"

//include GLGE dependencys
#include "openglGLGEVars.hpp"
#include "openglGLGEFuncs.hpp"
#include "../GLGEInternal/glgePrivDefines.hpp"
#include "../GLGEInternal/glgeErrors.hpp"
#include "../GLGEMath.h"

//include the default librarys
#include <iostream>
#include <math.h>
#include <algorithm>

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
    this->shader = glgeCompileShader(vertexShaderFile, fragmentShaderFile);
}

Shader::Shader(std::string vertexShaderData, std::string fragmentShaderData)
{
    //set the shader store variable to the compiled shader from the strings
    this->shader = glgeCompileShader(vertexShaderData, fragmentShaderData);
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
        this->shader = glgeCompileShader(GLGE_EMPTY_VERTEX_SHADER, data);
    }
    //else, if the shader is an vertex shader
    else if (type == GLGE_VERTEX_SHADER)
    {
        //set the shader store variable to the copilation of the inputed file and an empty fragment shader
        this->shader = glgeCompileShader(data, GLGE_EMPTY_FRAGMENT_SHADER);
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
        this->shader = glgeCompileShader(GLGE_EMPTY_VERTEX_SHADER, data);
    }
    //else, if the shader is an vertex shader
    else if (type == GLGE_VERTEX_SHADER)
    {
        //set the shader store variable to the copilation of the inputed file and an empty fragment shader
        this->shader = glgeCompileShader(data, GLGE_EMPTY_FRAGMENT_SHADER);
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

Shader::Shader(std::string data, const char* f)
{
    std::string d;
    readFile(f, d);

    *this = Shader(data, d);
}

Shader::Shader(const char* f, std::string data)
{
    std::string d;
    readFile(f, d);

    *this = Shader(d, data);
}

Shader::Shader(const char* vs, const char* gs, const char* fs)
{
    //set this to a new shader with vertex and fragment shader
    *this = Shader(vs, fs);
    //add the geometry shader
    this->addGeometryShader(gs);
}

Shader::Shader(unsigned int shader)
{
    //store the inputed shader
    this->shader = shader;
}

unsigned int Shader::getShader()
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

void Shader::addGeometryShader(std::string source)
{
    //pre-compile the geometry shader
    std::string gData = precompileShaderSource(source);
    //add the shader program from the second file
    addShader(this->shader, gData.c_str(), GL_GEOMETRY_SHADER);

    //create an variable to check for success
    int success = 0;
    //setup an error log
    GLchar ErrorLog[1024] = {0};

    //link the shader program
    glLinkProgram(this->shader);

    //get the program iv from the shader
    glGetProgramiv(this->shader, GL_LINK_STATUS, &success);
    //check if the program linking was no success
    if (success == 0)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            //get the error from open gl and output it with an custom message
            glGetProgramInfoLog(this->shader, sizeof(ErrorLog), NULL, ErrorLog);
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
    glValidateProgram(this->shader);
    //get the program iv again
    glGetProgramiv(this->shader, GL_VALIDATE_STATUS, &success);
    //check for success
    if (!success)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            //get the error from open gl and output it with an custom message
            glGetProgramInfoLog(this->shader, sizeof(ErrorLog), NULL, ErrorLog);
            printf(GLGE_ERROR_SHADER_VALIDATE_ERROR, ErrorLog);
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //get all uniforms
    this->recalculateUniforms();
}

void Shader::addGeometryShader(const char* f)
{
    //store the shader source code 
    std::string source;

    //read the fiel
    readFile(f, source);

    //cast to another function
    this->addGeometryShader(source);
}

void Shader::updateShader(const char* vertexShaderFile, const char* fragmentShaderFile)
{
    //delete the current shader
    this->deleteShader();
    //recalculate the shader object
    this->shader = Shader(vertexShaderFile, fragmentShaderFile).getShader();
    //recalculate all uniforms
    this->recalculateUniforms();
}

void Shader::updateShader(std::string vertexShaderData, std::string fragmentShaderData)
{
    //delete the current shader
    this->deleteShader();
    //recalculate the shader object
    this->shader = Shader(vertexShaderData, fragmentShaderData).getShader();
    //recalculate all uniforms
    this->recalculateUniforms();
}

void Shader::updateShader(const char* shaderFile, unsigned int type)
{
    //delete the current shader
    this->deleteShader();
    //recalculate the shader object
    this->shader = Shader(shaderFile, type).getShader();
    //recalculate all uniforms
    this->recalculateUniforms();
}

void Shader::updateShader(std::string shaderData, unsigned int type)
{
    //delete the current shader
    this->deleteShader();
    //recalculate the shader object
    this->shader = Shader(shaderData, type).getShader();
    //recalculate all uniforms
    this->recalculateUniforms();
}

void Shader::setCustomFloat(std::string name, float value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->floats[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the floats
        this->floats[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the floats
        this->floats[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the floats
        this->floats[name] *= value;
        //stop this script
        return;
    }
    //divide the value
    if (mode == GLGE_MODE_DIVIDE)
    {
        //divide the floats
        this->floats[name] /= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"float\"" << "\n";
    }
}

void Shader::setCustomInt(std::string name, int value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->integers[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the ints
        this->integers[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the ints
        this->integers[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the ints
        this->integers[name] *= value;
        //stop this script
        return;
    }
    //divide the value
    if (mode == GLGE_MODE_DIVIDE)
    {
        //divide the ints
        this->integers[name] /= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"integer\"" << "\n";
    }
}

void Shader::setCustomBool(std::string name, bool value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->booleans[name] = value;
        //stop this script
        return;
    }
    //and the value
    if (mode == GLGE_MODE_AND)
    {
        //and the bools
        this->booleans[name] = this->booleans[name] && value;
        //stop this script
        return;
    }
    //or the value
    if (mode == GLGE_MODE_OR)
    {
        //or the bools
        this->booleans[name] = this->booleans[name] || value;
        //stop this script
        return;
    }
    //not the value
    if (mode == GLGE_MODE_NOT)
    {
        //not the bool
        this->booleans[name] = !value;
        //stop this script
        return;
    }
    //nand the value
    if (mode == GLGE_MODE_NAND)
    {
        //nand the bools
        this->booleans[name] = !(value && this->booleans[name]);
        //stop this script
        return;
    }
    //nor the value
    if (mode == GLGE_MODE_NOR)
    {
        //nor the bools
        this->booleans[name] = !(value || this->booleans[name]);
        //stop this script
        return;
    }
    //xor the value
    if (mode == GLGE_MODE_XOR)
    {
        //xor the bools
        this->booleans[name] = (value ? !this->booleans[name] : this->booleans[name]);
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"boolean\"" << "\n";
    }
}

void Shader::setCustomVec2(std::string name, vec2 value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->vec2s[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the vec2s
        this->vec2s[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the vec2s
        this->vec2s[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the vec2s
        this->vec2s[name] = this->vec2s[name].scale(value);
        //stop this script
        return;
    }
    //divide the value
    if (mode == GLGE_MODE_DIVIDE)
    {
        //divide the vec2s
        this->vec2s[name] /= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"vec2\"" << "\n";
    }
}

void Shader::setCustomVec3(std::string name, vec3 value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->vec3s[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the vec3s
        this->vec3s[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the vec3s
        this->vec3s[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the vec3s
        this->vec3s[name] = this->vec3s[name].scale(value);
        //stop this script
        return;
    }
    //divide the value
    if (mode == GLGE_MODE_DIVIDE)
    {
        //divide the vec3s
        this->vec3s[name] /= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"vec3\"" << "\n";
    }
}

void Shader::setCustomVec4(std::string name, vec4 value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->vec4s[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the vec4s
        this->vec4s[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the vec4s
        this->vec4s[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the vec4s
        this->vec4s[name] = this->vec4s[name].scale(value);
        //stop this script
        return;
    }
    //divide the value
    if (mode == GLGE_MODE_DIVIDE)
    {
        //divide the vec4s
        this->vec4s[name] /= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"vec4\"" << "\n";
    }
}

void Shader::setCustomMat2(std::string name, mat2 value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->mat2s[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the mat2s
        this->mat2s[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the mat2s
        this->mat2s[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the mat2s
        this->mat2s[name] *= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"mat2\"" << "\n";
    }
}

void Shader::setCustomMat3(std::string name, mat3 value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->mat3s[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the mat3s
        this->mat3s[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the mat3s
        this->mat3s[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the mat3s
        this->mat3s[name] *= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"mat3\"" << "\n";
    }
}

void Shader::setCustomMat4(std::string name, mat4 value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->mat4s[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the mat4s
        this->mat4s[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the mat4s
        this->mat4s[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the mat4s
        this->mat4s[name] *= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"mat4\"" << "\n";
    }
}

void Shader::setCustomTexture(std::string name, char* file)
{
    //load the file into the map
    this->customTextures[name] = glgeTextureFromFile(file);
}

void Shader::setCustomTexture(std::string name, std::string file)
{
    //load the file into the map
    this->customTextures[name] = glgeTextureFromFile(file.c_str());
}

void Shader::setCustomTexture(std::string name, unsigned int texture)
{
    //store the inputed texture pointer
    this->customTextures[name] = texture;
}

float Shader::getFloatByName(std::string name)
{
    //return the wanted element from the map
    return this->floats[name];
}

int Shader::getIntByName(std::string name)
{
    //return the wanted element from the map
    return this->integers[name];
}

bool Shader::getBoolByName(std::string name)
{
    //return the wanted element from the map
    return this->booleans[name];
}

vec2 Shader::getVec2ByName(std::string name)
{
    //return the wanted element from the map
    return this->vec2s[name];
}

vec3 Shader::getVec3ByName(std::string name)
{
    //return the wanted element from the map
    return this->vec3s[name];
}

vec4 Shader::getVec4ByName(std::string name)
{
    //return the wanted element from the map
    return this->vec4s[name];
}

mat2 Shader::getMat2ByName(std::string name)
{
    //return the wanted element from the map
    return this->mat2s[name];
}

mat3 Shader::getMat3ByName(std::string name)
{
    //return the wanted element from the map
    return this->mat3s[name];
}

mat4 Shader::getMat4ByName(std::string name)
{
    //return the wanted element from the map
    return this->mat4s[name];
}

unsigned int Shader::getTextureByName(std::string name)
{
    //return the wanted element from the map
    return this->customTextures[name];
}

std::string Shader::getModeString(unsigned int mode)
{
    //check if the mode is set
    if (mode == GLGE_MODE_SET)
    {
        //return the mode string
        return std::string("set");
    }
    //check if the mode is add
    else if (mode == GLGE_MODE_ADD)
    {
        //return the mode string
        return std::string("add");
    }
    //check if the mode is subtract
    else if (mode == GLGE_MODE_SUBTRACT)
    {
        //return the mode string
        return std::string("subtract");
    }
    //check if the mode is multiply
    else if (mode == GLGE_MODE_MULTIPLY)
    {
        //return the mode string
        return std::string("multiply");
    }
    //check if the mode is divide
    else if (mode == GLGE_MODE_DIVIDE)
    {
        //return the mode string
        return std::string("divide");
    }
    //check if the mode is cross
    else if (mode == GLGE_MODE_CROSS)
    {
        //return the mode string
        return std::string("cross");
    }
    //check if the mode is and
    else if (mode == GLGE_MODE_AND)
    {
        //return the mode string
        return std::string("and");
    }
    //check if the mode is or
    else if (mode == GLGE_MODE_OR)
    {
        //return the mode string
        return std::string("or");
    }
    //check if the mode is not
    else if (mode == GLGE_MODE_NOT)
    {
        //return the mode string
        return std::string("not");
    }
    //check if the mode is nand
    else if (mode == GLGE_MODE_NAND)
    {
        //return the mode string
        return std::string("nand");
    }
    //check if the mode is nor
    else if (mode == GLGE_MODE_NOR)
    {
        //return the mode string
        return std::string("nor");
    }
    //check if the mode is xor
    else if (mode == GLGE_MODE_XOR)
    {
        //return the mode string
        return std::string("cor");
    }
    else
    {
        //if the mode can't be found, return an error
        return (std::string("mode ") + std::to_string(mode) + std::string(" dose not exist"));
    }
}

void Shader::applyShader()
{
    //bind the current shader
    glUseProgram(this->shader);
    //pass all uniforms

    //pass the costom floats
    //store the current float
    std::map<std::string, unsigned int>::iterator curr;
    //loop over the floats
    for (curr = this->floatLocs.begin(); curr != this->floatLocs.end(); curr++)
    {
        //pass the float
        glUniform1f(curr->second, this->floats[curr->first]);
    }
    //pass the costom ints
    //loop over the bools
    for (curr = this->intLocs.begin(); curr != this->intLocs.end(); curr++)
    {
        //pass the ints
        glUniform1i(curr->second, this->integers[curr->first]);
    }
    //pass the costom bools
    //loop over the bools
    for (curr = this->boolLocs.begin(); curr != this->boolLocs.end(); curr++)
    {
        //pass the bools
        glUniform1i(curr->second, this->booleans[curr->first]);
    }
    //pass the costom vec2s
    //loop over the vec2s
    for (curr = this->vec2Locs.begin(); curr != this->vec2Locs.end(); curr++)
    {
        //pass the vec2s
        glUniform2f(curr->second, this->vec2s[curr->first].x, this->vec2s[curr->first].y);
    }
    //pass the costom vec3s
    //loop over the vec3s
    for (curr = this->vec3Locs.begin(); curr != this->vec3Locs.end(); curr++)
    {
        //pass the vec3s
        glUniform3f(curr->second, this->vec3s[curr->first].x, this->vec3s[curr->first].y, this->vec3s[curr->first].z);
    }
    //pass the costom vec4s
    //loop over the vec4s
    for (curr = this->vec4Locs.begin(); curr != this->vec4Locs.end(); curr++)
    {
        //pass the vec4s
        glUniform4f(curr->second, this->vec4s[curr->first].x, this->vec4s[curr->first].y, this->vec4s[curr->first].z, this->vec4s[curr->first].w);
    }
    //pass the costom mat2s
    //loop over the mat2s
    for (curr = this->mat2Locs.begin(); curr != this->mat2Locs.end(); curr++)
    {
        //pass the mat2s
        glUniformMatrix2fv(curr->second, 1, GL_FALSE, this->mat2s[curr->first].m[0]);
    }
    //pass the costom mat3s
    //loop over the mat3s
    for (curr = this->mat3Locs.begin(); curr != this->mat3Locs.end(); curr++)
    {
        //pass the mat3s
        glUniformMatrix3fv(curr->second, 1, GL_FALSE, this->mat3s[curr->first].m[0]);
    }
    //pass the costom mat4s
    //loop over the mat4s
    for (curr = this->mat4Locs.begin(); curr != this->mat4Locs.end(); curr++)
    {
        //pass the mat4s
        glUniformMatrix4fv(curr->second, 1, GL_FALSE, this->mat4s[curr->first].m[0]);
    }
    //pass the costom textures
    //loop over the textures
    for (curr = this->customTextureLocs.begin(); curr != this->customTextureLocs.end(); curr++)
    {
        //increment the amount of bound textures
        this->boundTextures++;
        //activate the current texture unit
        glActiveTexture(GL_TEXTURE0 + this->boundTextures);
        //bind the texture
        glBindTexture(GL_TEXTURE_2D, this->customTextures[curr->first]);
        //pass the textures
        glUniform1i(curr->second, this->boundTextures);
    }
}

void Shader::removeShader()
{
    //loop over all bound texture units and bind 0
    for (int i = this->boundTextures; i > 0; i--)
    {
        //bind the current texture unit
        glActiveTexture(GL_TEXTURE0 + i);
        //bind the texture to 0
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    //set the texture unit to 0
    glActiveTexture(GL_TEXTURE0);
    //unbind the shader
    glUseProgram(0);
    //reset the bound texture
    this->boundTextures = 0;
}

void Shader::recalculateUniforms()
{
    //bind the shader
    glUseProgram(this->shader);
    //load the floats
    //define a variable for the current element
    std::map<std::string, float>::iterator currF;
    //loop over the elements
    for (currF = this->floats.begin(); currF != this->floats.end(); currF++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->floatLocs[currF->first] = glgeGetUniformVar(shader, currF->first.c_str());
    }
    //load the integers
    //define a variable for the current element
    std::map<std::string, int>::iterator currI;
    //loop over the elements
    for (currI = this->integers.begin(); currI != this->integers.end(); currI++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->intLocs[currI->first] = glgeGetUniformVar(shader, currI->first.c_str());
    }
    //load the booleans
    //define a variable for the current element
    std::map<std::string, bool>::iterator currB;
    //loop over the elements
    for (currB = this->booleans.begin(); currB != this->booleans.end(); currB++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->boolLocs[currB->first] = glgeGetUniformVar(shader, currB->first.c_str());
    }
    //load the vec2s
    //define a variable for the current element
    std::map<std::string, vec2>::iterator currV2;
    //loop over the elements
    for (currV2 = this->vec2s.begin(); currV2 != this->vec2s.end(); currV2++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->vec2Locs[currV2->first] = glgeGetUniformVar(shader, currV2->first.c_str());
    }
    //load the vec3s
    //define a variable for the current element
    std::map<std::string, vec3>::iterator currV3;
    //loop over the elements
    for (currV3 = this->vec3s.begin(); currV3 != this->vec3s.end(); currV3++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->vec3Locs[currV3->first] = glgeGetUniformVar(shader, currV3->first.c_str());
    }
    //load the vec4s
    //define a variable for the current element
    std::map<std::string, vec4>::iterator currV4;
    //loop over the elements
    for (currV4 = this->vec4s.begin(); currV4 != this->vec4s.end(); currV4++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->vec4Locs[currV4->first] = glgeGetUniformVar(shader, currV4->first.c_str());
    }
    //load the mat2s
    //define a variable for the current element
    std::map<std::string, mat2>::iterator currM2;
    //loop over the elements
    for (currM2 = this->mat2s.begin(); currM2 != this->mat2s.end(); currM2++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->mat2Locs[currM2->first] = glgeGetUniformVar(shader, currM2->first.c_str());
    }
    //load the mat3s
    //define a variable for the current element
    std::map<std::string, mat3>::iterator currM3;
    //loop over the elements
    for (currM3 = this->mat3s.begin(); currM3 != this->mat3s.end(); currM3++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->mat3Locs[currM3->first] = glgeGetUniformVar(shader, currM3->first.c_str());
    }
    //load the mat4s
    //define a variable for the current element
    std::map<std::string, mat4>::iterator currM4;
    //loop over the elements
    for (currM4 = this->mat4s.begin(); currM4 != this->mat4s.end(); currM4++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->mat4Locs[currM4->first] = glgeGetUniformVar(shader, currM4->first.c_str());
    }
    //load the textures
    //define a variable for the current element
    std::map<std::string, unsigned int>::iterator currTex;
    //loop over the elements
    for (currTex = this->customTextures.begin(); currTex != this->customTextures.end(); currTex++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->customTextureLocs[currTex->first] = glgeGetUniformVar(shader, currTex->first.c_str());
    }
    //unbind the shader
    glUseProgram(0);
}

/////////////
//FUNCTIONS//
/////////////

Shader glgeCreateKernalShader(float* kernal, int size)
{
    std::string header("#version 300 es\nprecision highp float;out vec4 FragColor;in vec2 texCoords;uniform sampler2D glgeMainImage;");
    std::string width = std::to_string(glgeWindows[glgeCurrentWindowIndex]->getSize().x);
    std::string height = std::to_string(glgeWindows[glgeCurrentWindowIndex]->getSize().y);

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

    std::string body2 = std::string("void main(){vec3 color = vec3(0.0f);for(int i = 0; i < "+std::to_string(size*size)+"; i++){color += vec3(texture(glgeMainImage, texCoords.st + offsets[i])) * kernel[i];}FragColor = vec4(color, 1.0f);}");

    std::string shader = header + body1 + offs + kern + body2;

    return Shader(shader, GLGE_FRAGMENT_SHADER);
}

Shader glgeCreateKernalShader(float* kernal, unsigned long size)
{
    size = std::sqrt((float)size / (float)sizeof(float));

    std::string header("#version 300 es\nprecision highp float;out vec4 FragColor;in vec2 texCoords;uniform sampler2D glgeMainImage;uniform vec2 glgeWindowSize;void main(){");

    std::string body1 = std::string("float offset_x = 1.f / glgeWindowSize.x;float offset_y = 1.f / glgeWindowSize.y;"); 

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

    std::string body2 = std::string("vec3 color = vec3(0.0f);for(int i = 0; i < "+std::to_string((int) size*size)+"; i+=1){color += vec3(texture(glgeMainImage, texCoords.st + offsets[i])) * kernel[i];}FragColor = vec4(color, 1.0f);}");

    std::string shader = header + body1 + offs + kern + body2;

    return Shader(shader, GLGE_FRAGMENT_SHADER);
}

Shader glgeCreateGausionBlureShader(int radius)
{
    //calculate the width and height (diameter)
    const int d = radius*2 +1;
    //create the cernal
    float kernel[d*d] = {};

    //implementation from: https://www.tutorialspoint.com/gaussian-filter-generation-in-cplusplus
    double sigma = 1.0;
    double p, q = 2.0 * sigma * sigma;
    double sum = 0.0;
    for (int x = -radius; x <= radius; x++)
    {
        for (int y = -radius; y <= radius; y++)
        {
            p = sqrt(x * x + y * y);
            kernel[(x+radius)*(radius+1) + y+radius] = (exp(-(p * p) / q)) / (GLGE_PI * q);
            sum += kernel[(x+radius)*(radius+1) + y+radius];
        }
    }
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            kernel[i*3 + j] /= sum;
        }
    }
    
    //compile the shader as an cernal shader
    Shader shader = glgeCreateKernalShader(kernel, (unsigned long) d*d * sizeof(float));
    //return the shader
    return shader;
}

int glgeAddCustomPostProcessingFunc(Shader (*func)(unsigned int))
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't add a post processing function to a main window, if no main window exists\n");
        }
        //stop the function
        return -1;
    }

    //pass the function
    return glgeWindows[glgeMainWindowIndex]->addPostProcessingFunction(func);
}

void glgeDeleteCusotmPostProcessingFunc(int index)
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't delete a post processing function of an main window, if no main window exists\n");
        }
        //stop the function
        return;
    }

    //pass the function
    return glgeWindows[glgeMainWindowIndex]->removePostProcessingFunction(index);
}

int glgeGetIndexOfCustomPostProcessingFunc(Shader (*func)(unsigned int))
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't get the index of an post processing function from main window, if no main window exists\n");
        }
        //stop the function
        return -1;
    }

    //pass the function
    return glgeWindows[glgeMainWindowIndex]->getPostProcessingFuncIndex(func);
}

void glgeDeleteCusotmPostProcessingFunc(Shader (*func)(unsigned int))
{
    //check if a window is bound
    if (!glgeHasMainWindow)
    {
        //if not, check if an warning should be printed
        if (glgeWarningOutput)
        {
            //if it should, print a warning
            printf("[GLGE WARNING] can't delete a post processing function from an main window, if no main window exists\n");
        }
        //stop the function
        return;
    }

    //pass the function
    return glgeWindows[glgeMainWindowIndex]->removePostProcessingFunction(func);
}