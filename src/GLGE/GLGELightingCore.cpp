/**
 * @file GLGELightingCore.cpp
 * @author DM8AT
 * @brief define the functionality of the lights declared in GLGELightCore.h
 * @version 0.1
 * @date 2023-02-26
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

//include the light core 
#include "GLGELightingCore.h"

//get access to the behind the scenes variables
#include "glgeVars.hpp"
//get the error strings for GLGE
#include "glgeErrors.hpp"

//include the defalt library
#include <iostream>

shadowCubeMapFBO::shadowCubeMapFBO()
{
    //init the object
    this->fbo = 0;
}

shadowCubeMapFBO::~shadowCubeMapFBO()
{
    //delete the object

    //if the frame buffer is not 0, delete it
    if (this->fbo != 0)
    {
        glDeleteFramebuffers(1, &this->fbo);
    }

    //if the shadow map is not 0, delete it
    if (this->shadowCubeMap != 0)
    {
        glDeleteTextures(1, &this->shadowCubeMap);
    }

    //if a depth object exists, delete it
    if (this->depth != 0)
    {
        glDeleteTextures(1, &this->depth);
    }
}

bool shadowCubeMapFBO::init(unsigned int size)
{
    //brake, if the size is 0
    if (size == 0)
    {
        return false;
    }

    //store the inputed size
    this->size = size;

    //Create the FBO
    glGenFramebuffers(1, &this->fbo);
    //bind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    //Create the depth buffer
    glGenTextures(1, &this->depth);
    //bind the texture
    glBindTexture(GL_TEXTURE_2D, this->depth);
    //set the texture to an depth buffer
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->size, this->size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    //set the texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //Create the cube map
    glGenTextures(1, &this->shadowCubeMap);
    //bind the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->shadowCubeMap);
    //set the cube map texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //loop over all sides of the cube map
    for (uint i = 0 ; i < 6 ; i++)
    {
        //create the side texture
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, this->size, this->size, 0, GL_RED, GL_FLOAT, NULL);
    }

    //set the framebuffer to an depth buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depth, 0);

    //Disable writes to the color buffer
    glDrawBuffer(GL_NONE);

    //Disable reads from the color buffer
    glReadBuffer(GL_NONE);

    //get if there was an error in the creation of the frame buffer
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    //if there was an error, print an error and return false
    if (Status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("[GLGE ERROR] shadow FB error, status: 0x%x\n", Status);
        //delete the frame buffer
        //glDeleteFramebuffers(1,&this->fbo);
        //reset the FBO to stop other actions
        this->fbo = 0;
        //return that the initalisation failed
        return false;
    }

    //unbind the frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //unbind the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void shadowCubeMapFBO::bindForWriting(GLenum cubeFace)
{
    //bind the frame buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
    
    // set the width/height of the shadow map
    //glViewport(0, 0, this->size, this->size);
    //set the texture for the framebuffer
    //glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cubeFace, this->shadowCubeMap, 0);

    //reset the color attachment
    //glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void shadowCubeMapFBO::bindForReading(GLenum TextureUnit)
{
    //stop this script, if the FBO is 0
    if (this->fbo == 0)
    {
        return;
    }

    //activate the requested texture unit
    glActiveTexture(TextureUnit);
    //load in the shadow map
    glBindTexture(GL_TEXTURE_2D, this->shadowCubeMap);
}


Light::Light()
{
    //init the object
}

Light::Light(vec3 pos, vec3 col, float intensity)
{
    //store the inputed position
    this->pos = pos;
    //store the inpted color
    this->color = col;
    //store the inputed intensity
    this->lightIntensity = intensity;

    //init the shadow map
    this->shadowMap.init(glgeShadowMapResolution);
}

Light::Light(float x, float y, float z, float r, float g, float b, float intensity)
{
    //store the inputed position
    this->pos = vec3(x,y,z);
    //store the inpted color
    this->color = vec3(r,g,b);
    //store the inputed intensity
    this->lightIntensity = intensity;

    //init the shadow map
    this->shadowMap.init(glgeShadowMapResolution);
}

//set the position of the light source
void Light::setPos(vec3 p)
{
    //apply the new position
    this->pos = p;
}

//set the position of the light source
void Light::setPos(float x, float y, float z)
{
    //apply the new position
    this->pos = vec3(x,y,z);
}

//move the light source
void Light::move(vec3 v)
{
    //change the position of the light source
    this->pos += v;
}

//move the light source
void Light::move(float x, float y, float z)
{
    //change the position of the light source
    this->pos += vec3(x,y,z);
}

vec3 Light::getPos()
{
    //return the light source position
    return this->pos;
}

void Light::setColor(vec3 col)
{
    //set the color to the inputed one
    this->color = col;
}

void Light::setColor(float r, float g, float b)
{
    //set the color to the inputed one
    this->color = vec3(r,g,b);
}

void Light::pitchColor(vec3 col)
{
    //scale the color by the inpt
    this->color.scale(col);
}

void Light::pitchColor(float r, float g, float b)
{
    //scale the color by the inpt
    this->color.scale(vec3(r,g,b));
}

void Light::addColor(vec3 color)
{
    //add the input to the color
    this->color += color;
}

void Light::addColor(float r, float g, float b)
{
    //add the input to the color
    this->color += vec3(r,g,b);
}

vec3 Light::getColor()
{
    //return the color of the light source
    return this->color;
}

void Light::setIntensity(float intens)
{
    //store the inputed intensity
    this->lightIntensity = intens;
}

void Light::scaleIntensity(float s)
{
    //scale the stored intensity
    this->lightIntensity *= s;
}

void Light::addIntensity(float d)
{
    //add the input to the stored intensity
    this->lightIntensity += d;
}

float Light::getInsensity()
{
    //return the stored intensity
    return this->lightIntensity;
}



void glgeAddGlobalLighSource(Light* l)
{
    //store the inputed light source, if it isn't NULL
    if (l != nullptr)
    {
        glgeLights.push_back(l);
    }

    //if the amount of bound light sources is bigger than the maximum supported light sources
    if ((int)glgeLights.size() > GLGE_LIGHT_SOURCE_MAX)
    {
        //write a warning if warning printing is enabled
        if (glgeWarningOutput)
        {
            std::cout << GLGE_WARNING_MORE_LIGHTS_THAN_DEFAULT_SHADER_SUPPORT << (int)glgeLights.size() << std::endl;
        }
    }
}