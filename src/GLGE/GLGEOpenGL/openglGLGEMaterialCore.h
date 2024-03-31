/**
 * @file openglGLGEMaterialCore.h
 * @author DM8AT
 * @brief declare the material core of GLGE
 * @version 0.1
 * @date 2024-01-02
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_MATERIAL_CORE_OGL_H_
#define _GLGE_MATERIAL_CORE_OGL_H_

//include the GLGE core
#include "openglGLGE.h"

//define things for identification

//define the roughness uniform
#define GLGE_ROUGHNESS_ID 0
//define the color uniform
#define GLGE_COLOR_ID 1
//define the texture coordinate uniform
#define GLGE_TEXTURE_COORDINATS_ID 2

//define the image type for an normal image
#define GLGE_IMAGE_ID 3
//define the image type for an normal map
#define GLGE_NORMAL_MAP_ID 4
//define the image type for an specular map
#define GLGE_SPECULAR_MAP_ID 5
//define the image type for an height map
#define GLGE_HEIGHT_MAP_ID 6
//define the image type for an metalic map
#define GLGE_METALIC_MAP_ID 7

//define the names for the default unifroms

//define the default name for a normal map
#define GLGE_NORMAL_MAP "glgeNormalMap"
//define the default name for a specular map
#define GLGE_SPECULAR_MAP "glgeSpecularMap"
//define the default name for a height map
#define GLGE_HEIGHT_MAP "glgeHeightMap"
//define the default name for a roughness map
#define GLGE_ROUGHNESS_MAP "glgeRoughnessMap"
//define the default name for a metalic map
#define GLGE_METALIC_MAP "glgeMetalicMap"
//define the default name for the color
#define GLGE_COLOR "glgeColor"
//define the name for the var to say the number of used textures
#define GLGE_USED_TEXTURES "glgeUsedTextures"
//define the name for the default texture
#define GLGE_TEXTURE "glgeTexture"

//define the suffix for activated materials
#define GLGE_SUFFIX_IS_ACTIVE_TEXTURE "IsActive"

//check if the shader core is allredy included
#ifndef _GLGE_SHADER_CORE_H_
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
#endif //line 64

//include the needed math librarys
#include "../CML/CML.h"

//include the needed things from the default library
#include <vector>
#include <map>
#include <string>

/**
 * @brief a material, that can be applied to an object to change its look
 */
class Material
{
public:
    /**
     * @brief Construct a new Material
     * 
     * default constructor
     */
    Material();

    /**
     * @brief Construct a new Material
     * 
     * @param image the image for the base texture
     * @param uniform the name of the uniform variable in the corresponding shader
     * @param roughness the roughness for the texture
     * @param metalstore the metalicness of the object
     */
    Material(const char* image, const char* uniform, float roughness, float metal = 0);

    /**
     * @brief Construct a new Material
     * 
     * @param texture a OpenGL pointer to the texture on the GPU
     * @param uniform the name of the uniform variable in the corresponding shader
     * @param roughness the roughness for the texture
     * @param metal store the metalicness of the object
     */
    Material(unsigned int texture, const char* uniform, float roughness, float metal = 0);

    /**
     * @brief Construct a new Material
     * 
     * @param color the base color of the material
     * @param roughness the roughness for the texture
     * @param metal store the metalicness of the object
     */
    Material(vec4 color, float roughness, float metal = 0);

    /**
     * @brief Set the Default Name for an uniform variable 
     * 
     * @param newName the new name for one of the default uniforms
     * @param type the type of the default uniform
     */
    void setDefaultUnifromName(const char* newName, unsigned int type);

    /**
     * @brief Set the Normal Map for a material
     * this function stores that the material has normal maps activated
     * @param image the image for the normal map
     * @param uniformName the uniform for the normal map in the shader
     */
    void setNormalMap(const char* image, const char* uniformName = GLGE_NORMAL_MAP);

    /**
     * @brief Set the Roughness Map for a material
     * this function stores that the material has roughness maps activated
     * @param image the image for the roughness maps
     * @param uniformName the uniform name for the roughness map
     */
    void setRoughnessMap(const char* image, const char* uniformName = GLGE_ROUGHNESS_MAP);

    /**
     * @brief Set the Height Map for a material
     * this function stores that the material has height maps activated
     * @param image the image for the height map
     * @param uniformName the uniform name for the height map
     */
    void setHeightMap(const char* image, const char* uniformName = GLGE_HEIGHT_MAP);

    /**
     * @brief Set the Metalic Map for a material
     * 
     * @param image the image for the metalic map
     * @param uniformName the uniform name for the metalic map
     */
    void setMetalicMap(const char* image, const char* uniformName = GLGE_METALIC_MAP);

    /**
     * @brief this function gets all needed data from the sader
     */
    void applyShader(unsigned int shader);

    /**
     * @brief apply the material for drawing
     * 
     * @return int the amount of bound textures
     */
    void applyMaterial();

    /**
     * @brief the material will no longer be used
     */
    void removeMaterial();

    /**
     * @brief Set if the object is effected by lighting
     * 
     * @param lit true: lighting takes effect | false: lighting is ignored
     */
    void setLit(bool lit);

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
     * @brief encode the object into some data
     * 
     * @param data the data to encode into
     */
    void encode(Data* data);
    /**
     * @brief decode the object from some data
     * 
     * @param data the data to decode from
     */
    void decode(Data data);

private:
    //store the positon of the roughness in the shader
    int roughnessLoc = -1;
    //store the position of the metalic in the shader
    int metalicLoc = -1;
    //store the positon of the color in the shader
    int colorLoc = -1;
    //store the position of the used textures uniform
    int usedLoc = -1;
    //store the position to say if the normal map is active
    int normIsActivLoc = -1;
    //store the position to say if the roughness map is active
    int roughIsActivLoc = -1;
    //store the position to say if the hight map is used
    int highIsActiveLoc = -1;
    //store the position of the lit parameter in the shader
    int litLoc = -1;
    //say if the material is effected by lighting
    bool lit = true;
    //store the color
    vec4 color = vec4(0,0,0,1);
    //store the roughness
    float roughness;
    //store the metal value
    float metal;
    //store the names for the normal map uniform
    const char* normalUniform = GLGE_NORMAL_MAP;
    //store the names for the roughness map uniform
    const char* roughnessUniform = GLGE_ROUGHNESS_MAP;
    //store the names for the specular map uniform
    const char* specularUniform = GLGE_SPECULAR_MAP;
    //store the names for the height map uniform
    const char* heightUniform = GLGE_HEIGHT_MAP;
    //store the names for the color uniform
    const char* colorUniform = GLGE_COLOR;
    //store the names for the color uniform
    const char* metalicUniform = GLGE_METALIC_MAP;
    //store the amount of bound textures
    int boundTextures;
    //store the position to pass if a texture is bound
    int hasTextureLoc;

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


#endif