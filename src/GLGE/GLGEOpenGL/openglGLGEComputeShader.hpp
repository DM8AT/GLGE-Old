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
     */
    void dispatch(vec3 workgroups);
};

#endif 