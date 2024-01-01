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
//check if glew is allready included
#ifndef _GLGE_GLEW_
//say that glew is now included
#define _GLGE_GLEW_
//include glew
#include <GL/glew.h>
//close the if for glew
#endif

//include the light core 
#include "GLGELightingCore.h"

//get access to the behind the scenes variables
#include "glgeVars.hpp"
//get the error strings for GLGE
#include "glgeErrors.hpp"
//get access to the glge functions
#include "glgeFuncs.hpp"

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
    
}



void glgeAddGlobalLighSource(Light* l)
{
    //add the light to the main window
    glgeWindows[glgeMainWindowIndex]->bindLight(l);
}