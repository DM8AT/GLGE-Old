/**
 * @file openglGLGEMaterialCore.cpp
 * @author DM8AT
 * @brief define the material core of GLGE
 * @version 0.1
 * @date 2024-01-02
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */


//check if glew is allready included
#ifndef _GLGE_GLEW_
//say that glew is now included
#define _GLGE_GLEW_
//include glew
#include <GL/glew.h>
//close the if for glew
#endif

//include GLGEMaterialCore
#include "openglGLGEMaterialCore.h"
//incldue the variables
#include "openglGLGEVars.hpp"
//include the defines
#include "openglGLGEDefines.hpp"

Material::Material()
{
    //default constructor
}

Material::Material(vec4 color, float roughness, bool lit, float metalic)
{
    //store the color
    this->matData.r = color.x;
    this->matData.g = color.y;
    this->matData.b = color.z;
    this->matData.a = color.w;
    //store the rougness
    this->matData.roughness = roughness;
    //store if the object is lit
    this->matData.lit = lit;
    //store the metallic ness
    this->matData.metalic = metalic;
    //apply to the current window
    this->bindToWindow(glgeCurrentWindowIndex);
}

Material::Material(Texture* texture, float roughness, bool lit, float metalic)
{
    //store the texture
    this->ambientMap = texture;
    //store the roughness
    this->matData.roughness = roughness;
    //store if the object is lit
    this->matData.lit = lit;
    //store the metalicness
    this->matData.metalic = metalic;
    //apply to the current window
    this->bindToWindow(glgeCurrentWindowIndex);
}

Material::Material(const char* texture, float roughness, bool lit, float metalic)
{
    //create a new texture
    this->ambientMap = new Texture(texture);
    //store the roughness
    this->matData.roughness = roughness;
    //store if the object is lit
    this->matData.lit = lit;
    //store the metalicness
    this->matData.metalic = metalic;
    //apply to the current window
    this->bindToWindow(glgeCurrentWindowIndex);
}

Material::~Material()
{
    //delete the ambient texture
    delete this->ambientMap;
    //delete the normal map
    delete this->normalMap;
    //delete the roughness map
    delete this->roughnessMap;
    //delete the metalic map
    delete this->metalicMap;
}

void Material::update()
{
    //check if a update is qued
    if (!this->quedUpdate)
    {
        //if not, stop the function
        return;
    }
    //check if the correct window is active
    if (glgeCurrentWindowIndex != this->windowId)
    {
        //throw an error
        GLGE_THROW_ERROR("Can't update a material in an window it is not bound to")
    }
    //check if the ambient texture exists
    if (this->ambientMap != NULL)
    {
        //update the albdeo map
        this->matData.ambientTex = this->ambientMap->getHandler();
        //say that the ambient map exists
        this->matData.ambientTexActive = 1;
    }
    else
    {
        //update the albdeo map
        this->matData.ambientTex = 0;
        //say that no ambient map exists
        this->matData.ambientTexActive = 0;
    }
    //check if the normal texture exists
    if (this->normalMap != NULL)
    {
        //update the normal map
        this->matData.normalMapTex = this->normalMap->getHandler();
        //say that the normal map exists
        this->matData.normalMapActive = 1;
    }
    else
    {
        //update the normal map
        this->matData.normalMapTex = 0;
        //say that no normal map exists
        this->matData.normalMapActive = 0;
    }
    //check if the roughness texture exists
    if (this->roughnessMap != NULL)
    {
        //update the roughness map
        this->matData.roughnessMap = this->roughnessMap->getHandler();
        //say that the roughness map exists
        this->matData.roughnessMapActive = 1;
    }
    else
    {
        //update the roughness map
        this->matData.roughnessMap = 0;
        //say that no roughness map exists
        this->matData.roughnessMapActive = 0;
    }
    //check if the metalic texture exists
    if (this->metalicMap != NULL)
    {
        //update the metalic map
        this->matData.metalicMap = this->metalicMap->getHandler();
        //say that the metalic map exists
        this->matData.metalicMapActive = 1;
    }
    else
    {
        //update the metalic map
        this->matData.metalicMap = 0;
        //say that no metalic map exists
        this->matData.metalicMapActive = 0;
    }

    //bind the uniform buffer
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    //send the data to the gpu
    glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialData), &this->matData, GL_STATIC_DRAW);
    //say that the update is done
    this->quedUpdate = false;
}

void Material::bindToWindow(unsigned int windowId)
{
    //store the window id
    this->windowId = windowId;
    //create a new buffer
    glCreateBuffers(1, &this->ubo);
    //say that an update should be done
    this->quedUpdate = true;
    //update the object
    this->update();
}

void Material::setColor(vec4 color)
{
    //store the new color
    this->matData.r = color.x;
    this->matData.g = color.y;
    this->matData.b = color.z;
    this->matData.a = color.w;
    //say that an update is qued
    this->quedUpdate = true;
}

void Material::setColor(float r, float g, float b, float a)
{
    //cast to another function
    this->setColor(vec4(r,g,b,a));
}

vec4 Material::getColor()
{
    //return the color
    return vec4(
        this->matData.r,
        this->matData.g,
        this->matData.b,
        this->matData.a
    );
}

void Material::setRoughness(float roughness)
{
    //store the new roughness
    this->matData.roughness = roughness;
    //say that an update is qued
    this->quedUpdate = true;
}

float Material::getRoughness()
{
    //return the roughenss
    return this->matData.roughness;
}

void Material::setMetalic(float metalic)
{
    //store the new metallic value
    this->matData.metalic = metalic;
    //say that an update is qued
    this->quedUpdate = true;
}

float Material::getMetalic()
{
    //return the metalic value
    return this->matData.metalic;
}

void Material::setLit(bool lit)
{
    //store the new lit value
    this->matData.lit = lit;
    //say that an update is qued
    this->quedUpdate = true;
}

bool Material::isLit()
{
    //return the lit value
    return this->matData.lit;
}

void Material::setAmbientTexture(Texture* texture)
{
    //store the new ambient texture
    this->ambientMap = texture;
    //say that an update is qued
    this->quedUpdate = true;
}

void Material::setAmbientTexture(const char* texture)
{
    //create a new texture and cast to another function
    this->setAmbientTexture(new Texture(texture));
}

Texture* Material::getAmbientTexture()
{
    //return the ambient texture
    return this->ambientMap;
}

void Material::setNormalMap(Texture* normalMap)
{
    //store the new normal map
    this->normalMap = normalMap;
    //say that an update is qued
    this->quedUpdate = true;
}

void Material::setNormalMap(const char* texture)
{
    //create a new texutre and cast to another function
    this->setNormalMap(new Texture(texture));
}

Texture* Material::getNormalMap()
{
    //return the normal map
    return this->normalMap;
}

void Material::setRoughnessMap(Texture* roughnessMap)
{
    //store the new roughenss map
    this->roughnessMap = roughnessMap;
    //say that an update is qued
    this->quedUpdate = true;
}

void Material::setRoughnessMap(const char* texture)
{
    //create a new texture and cast to another function
    this->setRoughnessMap(new Texture(texture));
}

Texture* Material::getRoughnessMap()
{
    //return the rougness map
    return this->roughnessMap;
}

void Material::setMetalicMap(Texture* metalicMap)
{
    //store the new metalic map
    this->metalicMap = metalicMap;
    //say that an update is qued
    this->quedUpdate = true;
}

void Material::setMetalicMap(const char* texture)
{
    //create a new texture and cast to another function
    this->setMetalicMap(new Texture(texture));
}

Texture* Material::getMetalicMap()
{
    //return the metalic map
    return this->metalicMap;
}

void Material::apply()
{
    //bind the ubo
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, this->ubo);
}

void Material::encode(Data* data)
{
    //write all of the own data
    data->writeBytes((uint8_t*)&this->matData, sizeof(matData));
}

void Material::decode(Data data)
{
    //read the material data
    this->matData = *(MaterialData*)data.readBytes(sizeof(matData));
}