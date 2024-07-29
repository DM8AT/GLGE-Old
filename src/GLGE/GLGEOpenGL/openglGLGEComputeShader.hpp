/**
 * @file openglGLGEComputeShader.hpp
 * @author DM8AT
 * @brief 
 * @version 0.1
 * @date 2024-02-27
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_COMPUTE_SHADER_OGL_H_
#define _GLGE_COMPUTE_SHADER_OGL_H_

//include the base
#include "openglGLGE.h"
//include default shaders
#include "openglGLGEShaderCore.h"

/**
 * @brief say that the CPU should wait for the compute shader to finish
 */
#define GLGE_COMPUTE_MEMORY_BARRIER_ALL 0
/**
 * @brief say that the CPU should wait for the compute shader to finish acessing shader storage buffers
 */
#define GLGE_COMPUTE_MEMORY_BARRIER_STORAGE_BUFFER 1
/**
 * @brief say that the CPU should not wait
 */
#define GLGE_COMPUTE_MEMORY_BARRIER_NONE 2

/**
 * @brief a simple class to handle compute shaders
 */
class ComputeShader : public Shader
{
public:

    /**
     * @brief Default constructor
     */
    ComputeShader();

    /**
     * @brief Create a compute shader form an file
     * 
     * @param shaderFile the file containing the shader source code
     */
    ComputeShader(const char* shaderFile);

    /**
     * @brief Create a compute shader from raw source code
     * 
     * @param shaderCode the source code
     */
    ComputeShader(std::string shaderCode);

    /**
     * @brief run the compute shader
     * 
     * @param workgroups the amount of work groups to use
     * @param barrier specify the memory barrier to wait for the compute shader to finish
     */
    void dispatch(vec3 workgroups, unsigned int barrier = GLGE_COMPUTE_MEMORY_BARRIER_ALL);

    /**
     * @brief encode the data for the shader
     * 
     * @param data the data to encode into
     */
    void encode(Data* data);

    /**
     * @brief decode the object from some given data
     * 
     * @param data the data to decode from
     */
    void decode(Data* data);

    /**
     * @brief get the source code of the shader
     * 
     * @return std::string the source code of the shader
     */
    std::string getSRC();

private:
    //store the source code
    std::string src;

    /**
     * @brief super constructor for the object
     * 
     * @param src the shader source code
     */
    void super(std::string src);

    /**
     * @brief decode the memory barrier identifyer into one OpenGL is capable to read
     * 
     * @param barrier the memory barrier identifyer
     * @return unsigned int the OpenGL memory barrier identifyer or GL_ALL_BARRIER_BITS on error
     */
    static unsigned int decodeBarrier(unsigned int barrier);
};

#endif 