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

Material::Material()
{
    //init the object
}

Material::Material(const char* image, float roughness)
{
    //store the inputed roughness
    this->roughness = roughness;

    //load the inputed image to the image vector
    this->addImage(image);
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
    //get the location of the color
    this->colorLoc = glgeGetUniformVar(shader, this->colorUniform);

    //try get the location of the normal map
    if (this->normalMapLoc != 1)
    {
        //load the normal map location
        this->imageLocs[normalMapLoc] = glgeGetUniformVar(shader, this->normalUniform);
    }

    //try get the location of the specular map
    if (this->specularMapLoc != -1)
    {
        //load the specular map location
        this->imageLocs[specularMapLoc] = glgeGetUniformVar(shader, this->specularUniform);
    }

    //try get the offset map location
    if (this->offsetMapLoc != -1)
    {
        //load the location of the offset map
        this->imageLocs[offsetMapLoc] = glgeGetUniformVar(shader, this->offsetUniform);
    }

    //load the rest of the image uniforms
    for (int i = 0; i < (int)this->textures.size(); i++)
    {
        //check if the image has an uniform
        printf("B: %d\n", this->uniformNames[i] == NULL);
        if (this->uniformNames[i] == NULL)
        {
            continue;
        }
        else
        {
            //else, load the image uniform
            this->imageLocs[i] = glgeGetUniformVar(shader, this->uniformNames[i]);
        }
    }
}

void Material::setUniformName(const char* name, unsigned int item)
{
    //load the unifrom name for the position specified in item
    this->uniformNames[item] = name;
}

void Material::applyMaterial()
{
    //pass the color of the material
    glUniform4f(this->colorLoc, this->color.x, this->color.y, this->color.z, this->color.w);

    //store how much textures are bound
    int boundTextures = 0;

    //check if the normal map exists
    if (this->normalMapLoc != -1)
    {
        //bind the normal map to an texture sampler
        glActiveTexture(GL_TEXTURE0 + boundTextures);
        //add the bound texture
        this->boundTextures.push_back(GL_TEXTURE0 + boundTextures);
        //pass the normal map to the shader
        glUniform1i(this->textures[this->normalMapLoc], boundTextures);
        //change the amout of bound textures by 1
        boundTextures++;
    }

    //check if the specular map exists
    if (this->specularMapLoc != -1)
    {
        //bind the specular map to an texture sampler
        glActiveTexture(GL_TEXTURE0 + boundTextures);
        //add the bound texture
        this->boundTextures.push_back(GL_TEXTURE0 + boundTextures);
        //pass the specular map to the shader
        glUniform1i(this->textures[this->specularMapLoc], boundTextures);
        //change the amout of bound textures by 1
        boundTextures++;
    }

    //check if the offset map exists
    if (this->offsetMapLoc != -1)
    {
        //bind the offset map to an texture sampler
        glActiveTexture(GL_TEXTURE0 + boundTextures);
        //add the bound texture
        this->boundTextures.push_back(GL_TEXTURE0 + boundTextures);
        //pass the offset map to the shader
        glUniform1i(this->textures[this->offsetMapLoc], boundTextures);
        //change the amout of bound textures by 1
        boundTextures++;
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
        glActiveTexture(GL_TEXTURE0 + boundTextures);
        //add the bound texture
        this->boundTextures.push_back(GL_TEXTURE0 + boundTextures);
        //pass the texture to the shader
        glUniform1i(this->textures[i], boundTextures);
        //increase the amount of bound textures
        boundTextures++;
    }
}

void Material::removeMaterial()
{
    //loop over all created textures
    for (GLenum texture : this->boundTextures)
    {
        //unbind the texture
        glBindTexture(0, texture-GL_TEXTURE0);
        //deactivate the texture
        //gl
    }
    //clear the vector of bound textures, they are all unbound
    this->boundTextures.clear();
}