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

//include the 4D matrices
#include "CML/CMLMat4.h"

//include the defalt library
#include <iostream>


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

    //setup the shadow map
    this->setupShadowMap();
}

Light::Light(float x, float y, float z, float r, float g, float b, float intensity)
{
    //store the inputed position
    this->pos = vec3(x,y,z);
    //store the inpted color
    this->color = vec3(r,g,b);
    //store the inputed intensity
    this->lightIntensity = intensity;

    //setup the shadow map
    this->setupShadowMap();
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

void Light::bindShadowMap()
{
    //bind the stored shadow map frambuffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->shadowFBO);
}

void Light::bindShadowMapTexture(int samp)
{
    //activate the sampler unit
    glActiveTexture(GL_TEXTURE0 + samp);
    //bind the shadow map texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->shadowMap);
}

void Light::setupShadowMap()
{
    //generate the frame buffer for the shadow map
    glGenFramebuffers(1, &this->shadowFBO);
    //bind the frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->shadowFBO);

    //generate the shadow map texture
    glGenTextures(1, &this->shadowMap);
    //bind the shadow map texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->shadowMap);

    //load the faces for the shadow map
    for (int  i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, glgeShadowMapResolution, glgeShadowMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    //set the texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //stop the texture from looping arround
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //store the texture as the texture for the frame buffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->shadowMap, 0);
    //disable calls to the color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    //unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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