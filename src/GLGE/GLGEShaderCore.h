/**
 * @file GLGEShaderCore.h
 * @author DM8AT
 * @brief in this file, the functions for the shader core of the library are defined. 
 * @version 0.1
 * @date 2023-02-18
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 */

//check for double include
#ifndef _GLGE_SHADER_CORE_H_
#define _GLGE_SHADER_CORE_H_

//include the dependencys
//standart librarys
#include <string>

//include the OpenGL dependencys
#include <GL/glew.h>

///////////
//DEFINES//
///////////

//define GLGE_VERTEX_SHADER to 1, it is needed in an constructor of the Shader class
#define GLGE_VERTEX_SHADER 1
//define GLGE_FRAGMENT_SHADER to 2, it is needed in an constructor of the Shader class
#define GLGE_FRAGMENT_SHADER 2
//define GLGE_GEOMETRY_SHADER to 3, it is needed in an additional function of the shader class

///////////
//CLASSES//
///////////

class Shader
{
public:
    /**
     * @brief Construct a new Shader.
     * default constructor
     */
    Shader();

    /**
     * @brief Construct a new Shader from two files
     * 
     * @param vertexShaderFile the file for the vertex shader
     * @param fragmentShaderFile the file for the fragment shader
     */
    Shader(const char* vertexShaderFile, const char* fragmentShaderFile);

    /**
     * @brief Construct a new Shader from two strings
     * 
     * @param vertexShaderData the data for the vertex shader as an string
     * @param fragmentShaderData the data for the fragment shader as an string
     */
    Shader(std::string vertexShaderData, std::string fragmentShaderData);

    /**
     * @brief Construct a new Shader from an file and an preset
     * 
     * @param shaderFile the file for the shader
     * @param type the type of the inputed shader
     */
    Shader(const char* shaderFile, unsigned int type);

    /**
     * @brief Construct a new Shader from shader data and an preset
     * 
     * @param shaderData the data for the shader
     * @param type the type of the shader
     */
    Shader(std::string shaderData, unsigned int type);

    /**
     * @brief Construct a new Shader from shader data and an preset
     * 
     * @param vertexShaderData the data from the vertex shader
     * @param fragmentShaderFile the path to the fragment shader
     */
    Shader(std::string vertexShaderData, const char* fragmentShaderFile);

    /**
     * @brief Construct a new Shader from shader data and an preset
     * 
     * @param vertexShaderFile the path to the vertex shader
     * @param fragmentShaderData the data of the fragment shader
     */
    Shader(const char* vertexShaderFile, std::string fragmentShaderData);

    /**
     * @brief Get the Shader
     * 
     * @return GLuint the compiled OpenGL shader
     */
    GLuint getShader();

    /**
     * @brief delete the shader
     */
    void deleteShader();

    /**
     * @brief adds an geometry shader to the bound shader
     * 
     * @param source the source code of the shader
     */
    void addGeometryShader(std::string source);

    /**
     * @brief adds an geometry shader to the bound shader
     * 
     * @param source the file that contains the shader
     */
    void addGeometryShader(const char* file);

    /**
     * @brief change the currently stored shader
     * 
     * @param vertexShaderFile the new vertex shader file
     * @param fragmentShaderFile the new fragment shader file
     */
    void updateShader(const char* vertexShaderFile, const char* fragmentShaderFile);

    /**
     * @brief change the currently stored shader
     * 
     * @param vertexShaderData the new data for the vertex shader
     * @param fragmentShaderData the new data for the fragment shader
     */
    void updateShader(std::string vertexShaderData, std::string fragmentShaderData);

    /**
     * @brief change the currently stored shader
     * 
     * @param shaderFile the new file for the shader
     * @param type the type of the new shader
     */
    void updateShader(const char* shaderFile, unsigned int type);

    /**
     * @brief change the currently stored shader
     * 
     * @param shaderData the data for the new shader
     * @param type the type of the new shader
     */
    void updateShader(std::string shaderData, unsigned int type);

protected:
    //store the address of the OpenGL shader
    GLuint shader = 0;
    //store the shader source code
};

/////////////
//FUNCTIONS//
/////////////

/**
 * @brief create an shader with an kernal (edge detection, blur, usw)
 * 
 * @param kernal the kernal for the shader
 * @param size the size of the kernal
 * @return Shader the shader as an instance of the Shader class
 */
Shader glgeCreateKernalShader(float* kernal, int size);

/**
 * @brief create an shader with an kernal (edge detection, blur, usw)
 * 
 * @param kernal the kernal for the shader
 * @param size the size of the kernal pointer (sizeof(kernal))
 * @return Shader Shader the shader as an instance of the Shader class
 */
Shader glgeCreateKernalShader(float* kernal, unsigned long size);

#endif