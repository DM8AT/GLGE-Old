/**
 * @file GLGEMaterialCore.h
 * @author DM8AT
 * @brief this file specifies all the things that can be done with materials. 
 * 
 * Materials are what can be applied to 3D objects to make the look realistic. This is needed, because GLGE uses
 * PBR (Physically Based Rendering). 
 * This file will be included using GLGE3DCore.h
 * 
 * @version 0.1
 * @date 2023-02-23
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

//check for double includes
#ifndef _GLGE_MATERIAL_CORE_H_
#define _GLGE_MATERIAL_CORE_H_

//define things for identification

//define the roughness uniform
#define GLGE_ROUGHNESS 0
//define the color uniform
#define GLGE_COLOR 1
//define the texture coordinate uniform
#define GLGE_TEXTURE_COORDINATS 2

//define the image type for an normal image
#define GLGE_IMAGE 3
//define the image type for an normal map
#define GLGE_NORMAL_MAP 4
//define the image type for an specular map
#define GLGE_SPECULAR_MAP 5
//define the image type for an offset map
#define GLGE_OFFSET_MAP 6

//define the names for the default unifroms

//define the default name for a normal map
#define GLGE_NORMAL_MAP_NAME "normalMap"
//define the default name for a specular map
#define GLGE_SPECULAR_MAP_NAME "specularMap"
//define the default name for a offset map
#define GLGE_OFFSET_MAP_NAME "offsetMap"
//define the default name for the color
#define GLGE_COLOR_NAME "col"

//include the needed math librarys
#include "CML/CMLVec4.h"

//include OpenGL
#include <GL/glew.h>

//include the needed things from the default library
#include <vector>

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
     */
    Material(const char* image, const char* uniform, float roughness);

    /**
     * @brief Construct a new Material
     * 
     * @param color the base color of the material
     * @param roughness the roughness for the texture
     */
    Material(vec4 color, float roughness);

    /**
     * @brief Set the Default Name for an uniform variable 
     * 
     * @param newName the new name for one of the default uniforms
     * @param type the type of the default uniform
     */
    void setDefaultUnifromName(const char* newName, unsigned int type);

    /**
     * @brief add an image for an default uniform
     * 
     * @param image the path to the image
     * @param type the type of the default uniform
     */
    void addImage(const char* image, unsigned int type = GLGE_IMAGE);

    /**
     * @brief add an image to a new uniform
     * 
     * @param image the path to the image
     * @param uniformName the name of the uniform
     */
    void addImage(const char* image, const char* uniformName);

    /**
     * @brief this function gets all needed data from the sader
     */
    void applyShader(GLuint shader);

    /**
     * @brief Set the name for a default unifrom
     * 
     * @param name the name of the uniform 
     * @param item the item in an list
     */
    void setUniformName(const char* name, unsigned int item = 0);

    /**
     * @brief apply the material for drawing
     */
    void applyMaterial();

    /**
     * @brief the material will no longer be used
     */
    void removeMaterial();

private:
    //store the positon of the roughness in the shader
    GLuint roughnessLoc = 0;
    //store the positon of the color in the shader
    GLuint colorLoc = 0;
    //store all images for the Material
    std::vector<GLuint> textures;
    //store the positons of the images
    std::vector<GLuint> imageLocs;
    //store the names for the uniforms
    std::vector<const char*> uniformNames;
    //store the location of the normal map in the image vector
    int normalMapLoc = -1;
    //store the location of the specular map in the image vector
    int specularMapLoc = -1;
    //store the location of the offset map in the image vector
    int offsetMapLoc = -1;
    //store the color
    vec4 color = vec4(1,1,1,1);
    //store the roughness
    float roughness;
    //store the names for the normal map uniform
    const char* normalUniform = GLGE_NORMAL_MAP_NAME;
    //store the names for the specular map uniform
    const char* specularUniform = GLGE_SPECULAR_MAP_NAME;
    //store the names for the offset map uniform
    const char* offsetUniform = GLGE_OFFSET_MAP_NAME;
    //store the names for the color uniform
    const char* colorUniform = GLGE_COLOR_NAME;
    //store the bound textures
    std::vector<GLenum> boundTextures;
};

//end the file
#endif