/**
 * @file GLGEMaterialCore.cpp
 * @author DM8AT
 * @brief in this file, all things from GLGEMaterialCore.h are defined. 
 * @version 0.1
 * @date 2023-02-23
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

//include GLGEMaterialCore
#include "GLGEMaterialCore.h"

//include the library core
#include "GLGE.h"

//include the shared variables
#include "glgeVars.hpp"

Material::Material()
{
    //init the object
}

Material::Material(const char* image, const char* uniform, float roughness)
{
    //store the inputed roughness
    this->roughness = roughness;

    //load the inputed image to the image vector
    this->addImage(image, uniform);
}

Material::Material(vec4 color, float roughness)
{
    //store the inputed roughness
    this->roughness = roughness;

    //store the inputed base color
    this->color = color;
}

void Material::setDefaultUnifromName(const char* newName, unsigned int type)
{
    //store the new name for the specefied unifrom
    switch (type)
    {
        //case if the new name is for a normal map
        case GLGE_NORMAL_MAP:
            this->normalUniform = newName;
            break;

        //case if the new name is for a specular map
        case GLGE_SPECULAR_MAP:
            this->specularUniform = newName;
            break;

        //case if the new name is for a offset map
        case GLGE_OFFSET_MAP:
            this->offsetUniform = newName;
            break;

        //case if the new name is for a color
        case GLGE_COLOR:
            this->colorUniform = newName;
            break;
        
        //default case
        default:
            break;
    }
}

void Material::addImage(const char* image, unsigned int type)
{
    //add the new image to the image vector
    this->textures.push_back(glgeTextureFromFile(image));
    //store an empty image positoion
    this->imageLocs.push_back(0);
    
    //check if the type is the default image
    if (type == GLGE_IMAGE)
    {
        //store the default name for the image in the uniform vector
        this->uniformNames.push_back("image");
    }
    //else, check if the type is a normal map
    else if (type == GLGE_NORMAL_MAP)
    {
        //store an empty string in the unifrom
        this->uniformNames.push_back(NULL);

        //store the location of the normal map
        this->normalMapLoc = (unsigned int)this->uniformNames.size()-1;
    }
    //else, check if the type is a specular map
    else if (type == GLGE_SPECULAR_MAP)
    {
        //store an empty string in the unifrom
        this->uniformNames.push_back(NULL);

        //store the location of the specular map
        this->specularMapLoc = (unsigned int)this->uniformNames.size()-1;
    }
    //else, check if the type is a offset map
    else if (type == GLGE_OFFSET_MAP)
    {
        //store an empty string in the unifrom
        this->uniformNames.push_back(NULL);

        //store the location of the offset map
        this->offsetMapLoc = (unsigned int)this->uniformNames.size()-1;
    }
}

void Material::addImage(const char* image, const char* uniformName)
{
    //store the inputed image in the image vector
    this->textures.push_back(glgeTextureFromFile(image));
    //store an empty image positoion
    this->imageLocs.push_back(0);
    //store the inputed image uniform name
    this->uniformNames.push_back(uniformName);
}

void Material::applyShader(GLuint shader)
{
    //activate the inputed shader
    glUseProgram(shader);
    //keep track ot the amount of bound textures
    int bTs = 0;
    //store the state of the GLGE error output
    bool err = glgeErrorOutput;
    //disable the GLGE error output
    glgeErrorOutput = false;

    //get the location of the roughness variable
    this->roughnessLoc = glgeGetUniformVar(shader, "roughness");

    //get the location of the color
    this->colorLoc = glgeGetUniformVar(shader, this->colorUniform);

    //try get the location of the normal map
    if (this->normalMapLoc != -1)
    {
        //load the normal map location
        this->imageLocs[normalMapLoc] = glgeGetUniformVar(shader, this->normalUniform);
        //pass the texture to the shader
        bTs++;
    }

    //try get the location of the specular map
    if (this->specularMapLoc != -1)
    {
        //load the specular map location
        this->imageLocs[specularMapLoc] = glgeGetUniformVar(shader, this->specularUniform);
        //pass the texture to the shader
        bTs++;
    }

    //try get the offset map location
    if (this->offsetMapLoc != -1)
    {
        //load the location of the offset map
        this->imageLocs[offsetMapLoc] = glgeGetUniformVar(shader, this->offsetUniform);
        //pass the texture to the shader
        bTs++;
    }

    //load the rest of the image uniforms
    for (int i = 0; i < (int)this->textures.size(); i++)
    {
        //check if the image has an uniform
        if (this->uniformNames[i] == NULL)
        {
            continue;
        }
        else
        {
            //else, load the image uniform
            this->imageLocs[i] = glgeGetUniformVar(shader, this->uniformNames[i]);
            //update the amount of texture units
            bTs++;
        }
    }

    //get the location of the var to say how many pictures are used
    this->usedLoc = glgeGetUniformVar(shader, GLGE_USED_TEXTURES_VAR);

    //reset the error output
    glgeErrorOutput = err;

    //unbind the shader
    glUseProgram(0);
}

void Material::setUniformName(const char* name, unsigned int item)
{
    //load the unifrom name for the position specified in item
    this->uniformNames[item] = name;
}

void Material::applyMaterial()
{
    //pass the color of the material
    if (this->colorLoc != 0)
    {
        glUniform4f(this->colorLoc, this->color.x, this->color.y, this->color.z, this->color.w);
    }

    //pass the roughness to the material
    if (this->roughnessLoc != 0)
    {
        glUniform1f(this->roughnessLoc, roughness);
    }

    //pass the amount of textures to the shader
    glUniform1i(this->usedLoc, (int)this->textures.size());

    //store how much textures are bound
    int bTs = 0;

    //check if the normal map exists
    if (this->normalMapLoc != -1)
    {
        //bind the normal map to an texture sampler
        glActiveTexture(GL_TEXTURE0 + bTs);
        //add the bound texture
        this->boundTextures.push_back(GL_TEXTURE0 + bTs);
        //pass the normal map to the shader
        glUniform1i(this->textures[this->normalMapLoc], bTs);
        //change the amout of bound textures by 1
        bTs++;
    }

    //check if the specular map exists
    if (this->specularMapLoc != -1)
    {
        //bind the specular map to an texture sampler
        glActiveTexture(GL_TEXTURE0 + bTs);
        //pass the specular map to the shader
        glUniform1i(this->textures[this->specularMapLoc], bTs);
        //change the amout of bound textures by 1
        bTs++;
    }

    //check if the offset map exists
    if (this->offsetMapLoc != -1)
    {
        //bind the offset map to an texture sampler
        glActiveTexture(GL_TEXTURE0 + bTs);
        //add the bound texture
        this->boundTextures.push_back(GL_TEXTURE0 + bTs);
        //pass the offset map to the shader
        glUniform1i(this->textures[this->offsetMapLoc], bTs);
        //change the amout of bound textures by 1
        bTs++;
    }

    //check if there are textures to load
    if ((int)this->textures.size() == 0)
    {
        //if there are no textures to load, stop
        return;
    }

    //load the rest of the textures
    for (int i = 0; i < (int)this->textures.size(); i++)
    {
        //check if the texture could allready be bound
        if (i == this->normalMapLoc || i == this->specularMapLoc || i == this->offsetMapLoc)
        {
            //skip to the next iteration
            continue;
        }

        //else, activate the new texture
        glActiveTexture(GL_TEXTURE0 + bTs);
        //add the bound texture
        this->boundTextures.push_back(GL_TEXTURE0 + bTs);
        //pass the uniform texture
        glBindTexture(GL_TEXTURE_2D, this->textures[i]);
        //increase the amount of bound textures
        glUniform1i(this->imageLocs[i], bTs);
        bTs++;
    }
}

void Material::removeMaterial()
{
    //loop over all bound textures
    for (GLuint tex : this->boundTextures)
    {
        //activate the current used texture
        glActiveTexture(tex);
        //bind the default texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    //clear the bound textures
    this->boundTextures.clear();
}