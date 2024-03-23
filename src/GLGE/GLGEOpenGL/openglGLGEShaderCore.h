/**
 * @file openglGLGEShaderCore.h
 * @author DM8AT
 * @brief declare the shader core of GLGE
 * @version 0.1
 * @date 2024-01-02
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_SHADER_CORE_OGL_H_
#define _GLGE_SHADER_CORE_OGL_H_


//include the dependencys
//standart librarys
#include <string>
#include <map>
//CML
#include "../CML/CML.h"

//check if the material core is allready included
#ifndef _GLGE_MATERIAL_CORE_H_

//define the mode for setting custom values
#define GLGE_MODE_SET 0
//define the mode for adding custom values
#define GLGE_MODE_ADD 1
//define the mode for subtracting custom values
#define GLGE_MODE_SUBTRACT 2
//define the mode for multipliing custom values
#define GLGE_MODE_MULTIPLY 3
//define the mode for dividing custom values
#define GLGE_MODE_DIVIDE 4
//define the mode for a cross product
#define GLGE_MODE_CROSS 5
//define the mode for a logical and opperation
#define GLGE_MODE_AND 6
//define the mode for a logical or opperation
#define GLGE_MODE_OR 7
//define the mode for a logical not opperation
#define GLGE_MODE_NOT 8
//define the mode for a logical nand opperation
#define GLGE_MODE_NAND 9
//define the mode for a logical nor opperation
#define GLGE_MODE_NOR 10
//define the mode for a logical xor opperation
#define GLGE_MODE_XOR 11

//close the if-statement
#endif //line 20

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

/**
 * @brief a generic shader object, used to change the look of objects, lighing or the complete scene
 */
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
     * @brief Construct a new Shader that has a geometry shader
     * 
     * @param vertexShader the fiel for the vertex shader
     * @param geometryShader the fiel for the geometry shader
     * @param fragmentShader the fiel for the fragment shader
     */
    Shader(const char* vertexShader, const char* geometryShader, const char* fragmentShader);

    /**
     * @brief Construct a new Shader using an allready compiled shader
     * 
     * @param shader the compiled shader
     */
    Shader(unsigned int shader);

    /**
     * @brief set the shader to the current active shader
     */
    void applyShader();

    /**
     * @brief unbind the shader
     */
    void removeShader();

    /**
     * @brief Get the Shader
     * 
     * @return unsigned int the compiled OpenGL shader
     */
    unsigned int getShader();

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

    /**
     * @brief Set the value for a custom float uniform in the shader
     * 
     * @param name the name of the uniform in the shader
     * @param value the value for the float
     * @param mode say if the value should be set, added, subtracted, multiplied or divided | default: set
     */
    void setCustomFloat(std::string name, float value, unsigned int mode = GLGE_MODE_SET);

    /**
     * @brief Set the value for a custom int uniform in the shader
     * 
     * @param name the name of the uniform in the shader
     * @param value the value for the integer
     * @param mode say if the value should be set, added, subtracted, multiplied or divided | default: set
     */
    void setCustomInt(std::string name, int value, unsigned int mode = GLGE_MODE_SET);

    /**
     * @brief Set the value for a custom bool uniform in the shader
     * 
     * @param name the name of the uniform in the shader
     * @param value the value for the bool
     * @param mode say if the value should be set, or if an logical opperation should be performed | default: set
     */
    void setCustomBool(std::string name, bool value, unsigned int mode = GLGE_MODE_SET);

    /**
     * @brief Set the value for a custom vec2 uniform in the shader
     * 
     * @param name the name of the uniform in the shader
     * @param value the value for the vec2
     * @param mode say if the value should be set, added, subtracted, scaled, divided or crossed | default: set
     */
    void setCustomVec2(std::string name, vec2 value, unsigned int mode = GLGE_MODE_SET);

    /**
     * @brief Set the value for a custom vec3 uniform in the shader
     * 
     * @param name the name of the uniform in the shader
     * @param value the value for the vec3
     * @param mode say if the value should be set, added, subtracted, scaled, divided or crossed | default: set
     */
    void setCustomVec3(std::string name, vec3 value, unsigned int mode = GLGE_MODE_SET);

    /**
     * @brief Set the value for a custom vec4 uniform in the shader
     * 
     * @param name the name of the uniform in the shader
     * @param value the value for the vec4
     * @param mode say if the value should be set, added, subtracted, scaled, divided or crossed | default: set
     */
    void setCustomVec4(std::string name, vec4 value, unsigned int mode = GLGE_MODE_SET);

    /**
     * @brief Set the value for a custom mat2 uniform in the shader
     * 
     * @param name the name of the uniform in the shader
     * @param value the value for the mat2
     * @param mode say if the value should be set, added, subtracted or multiplied | default: set
     */
    void setCustomMat2(std::string name, mat2 value, unsigned int mode = GLGE_MODE_SET);

    /**
     * @brief Set the value for a custom mat3 uniform in the shader
     * 
     * @param name the name of the uniform in the shader
     * @param value the value for the mat3
     * @param mode say if the value should be set, added, subtracted or multiplied | default: set
     */
    void setCustomMat3(std::string name, mat3 value, unsigned int mode = GLGE_MODE_SET);

    /**
     * @brief Set the value for a custom mat4 uniform in the shader
     * 
     * @param name the name of the uniform in the shader
     * @param value the value for the mat4
     * @param mode say if the value should be set, added, subtracted or multiplied | default: set
     */
    void setCustomMat4(std::string name, mat4 value, unsigned int mode = GLGE_MODE_SET);

    /**
     * @brief Load a custom texture from an file
     * 
     * @param name the name of the sampler in the shader
     * @param file the texture file
     */
    void setCustomTexture(std::string name, char* file);

    /**
     * @brief Load a custom texture from an file
     * 
     * @param name the name of the sampler in the shader
     * @param file the texture file
     */
    void setCustomTexture(std::string name, std::string file);

    /**
     * @brief Load a custom texture from an texture pointer
     * 
     * @param name the name of the sampler in the shader
     * @param texture the OpenGL texture pointer
     */
    void setCustomTexture(std::string name, unsigned int texture);

    /**
     * @brief Get a float that is parsed to the shader
     * 
     * @param name the name of the uniform
     * @return float the value that is going to be parsed
     */
    float getFloatByName(std::string name);

    /**
     * @brief Get an integer that is parsed to the shader
     * 
     * @param name the name of the uniform
     * @return int the value that is going to be parsed
     */
    int getIntByName(std::string name);

    /**
     * @brief Get an boolean that is parsed to the shader
     * 
     * @param name the name of the uniform
     * @return bool the value that is going to be parsed
     */
    bool getBoolByName(std::string name);

    /**
     * @brief Get an vec2 that is parsed to the shader
     * 
     * @param name the name of the uniform
     * @return vec2 the value that is going to be parsed
     */
    vec2 getVec2ByName(std::string name);

    /**
     * @brief Get an vec3 that is parsed to the shader
     * 
     * @param name the name of the uniform
     * @return vec3 the value that is going to be parsed
     */
    vec3 getVec3ByName(std::string name);

    /**
     * @brief Get an vec4 that is parsed to the shader
     * 
     * @param name the name of the uniform
     * @return vec4 the value that is going to be parsed
     */
    vec4 getVec4ByName(std::string name);

    /**
     * @brief Get an mat2 that is parsed to the shader
     * 
     * @param name the name of the uniform
     * @return mat2 the value that is going to be parsed
     */
    mat2 getMat2ByName(std::string name);

    /**
     * @brief Get an mat3 that is parsed to the shader
     * 
     * @param name the name of the uniform
     * @return mat3 the value that is going to be parsed
     */
    mat3 getMat3ByName(std::string name);

    /**
     * @brief Get an mat4 that is parsed to the shader
     * 
     * @param name the name of the uniform
     * @return mat4 the value that is going to be parsed
     */
    mat4 getMat4ByName(std::string name);

    /**
     * @brief Get an OpenGL sampler that is parsed to the shader
     * 
     * @param name the name of the uniform
     * @return unsigned int the sampler that is going to be parsed
     */
    unsigned int getTextureByName(std::string name);

    /**
     * @brief Get the string descriping the name of the inputed mode
     * 
     * @param mode the mode to get the name from
     * @return std::string the string that describes the mode
     */
    std::string getModeString(unsigned int mode);

    /**
     * @brief resample all uniform variables
     */
    void recalculateUniforms();
    
protected:
    //store the address of the OpenGL shader
    unsigned int shader = 0;
    //store how many textures are bound
    int boundTextures = 0;

    //custom values

    //store the custom values (floats)
    std::map<std::string, float> floats;
    //store the custom values (integers)
    std::map<std::string, int> integers;
    //store the custom values (booleans)
    std::map<std::string, bool> booleans;
    //store the custom values (vec2)
    std::map<std::string, vec2> vec2s;
    //store the custom values (vec3)
    std::map<std::string, vec3> vec3s;
    //store the custom values (vec4)
    std::map<std::string, vec4> vec4s;
    //store the custom values (mat2)
    std::map<std::string, mat2> mat2s;
    //store the custom values (mat3)
    std::map<std::string, mat3> mat3s;
    //store the custom values (mat4)
    std::map<std::string, mat4> mat4s;
    //store a custom texture
    std::map<std::string, unsigned int> customTextures;

    //store the locations of the custom floats
    std::map<std::string, unsigned int> floatLocs;
    //store the locations of the custom integers
    std::map<std::string, unsigned int> intLocs;
    //store the locations of the custom booleans
    std::map<std::string, unsigned int> boolLocs;
    //store the locations of the custom vec2s
    std::map<std::string, unsigned int> vec2Locs;
    //store the locations of the custom vec3s
    std::map<std::string, unsigned int> vec3Locs;
    //store the locations of the custom vec4s
    std::map<std::string, unsigned int> vec4Locs;
    //store the locations of the custom mat2s
    std::map<std::string, unsigned int> mat2Locs;
    //store the locations of the custom mat3s
    std::map<std::string, unsigned int> mat3Locs;
    //store the locations of the custom mat4s
    std::map<std::string, unsigned int> mat4Locs;
    //store the locations of the custom textures
    std::map<std::string, unsigned int> customTextureLocs;
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

/**
 * @brief create a shader for an gausian blure
 * 
 * @param radius the blure radius
 * @return Shader the complete shader object
 */
Shader glgeCreateGausionBlureShader(int radius);

/**
 * @brief add a custom post-processing function
 * 
 * @param func a function pointer to the functin
 * @return int the index of execution of the function
 */
int glgeAddCustomPostProcessingFunc(Shader (*func)(unsigned int));

/**
 * @brief delete a custom post processing function
 * 
 * @param index the index of execution of the function
 */
void glgeDeleteCusotmPostProcessingFunc(int index);

/**
 * @brief delete a custom post processing function
 * 
 * @param func the function that should be removed
 */
void glgeDeleteCusotmPostProcessingFunc(Shader (*func)(unsigned int));

/**
 * @brief get the index of execution of an custom post-processing function
 * 
 * @param func the function pointer
 * @return int the index of execution
 */
int glgeGetIndexOfCustomPostProcessingFunc(Shader (*func)(unsigned int));


#endif