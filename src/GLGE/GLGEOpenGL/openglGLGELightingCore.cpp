/**
 * @file openglGLGELightingCore.cpp
 * @author DM8AT
 * @brief define the lighting core of GLGE
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

//include the light core 
#include "openglGLGELightingCore.h"
//include the internal variables
#include "openglGLGEVars.hpp"
//get the error strings for GLGE
#include "../GLGEIndependend/glgeErrors.hpp"
//get access to the glge functions
#include "openglGLGEFuncs.hpp"

//include the 4D matrices
#include "../CML/CMLMat4.h"

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
    //specify the light as a point light
    this->lightType = GLGE_LIGHT_SOURCE_TYPE_POINT;

    //setup the shadow map
    this->setupShadowMap();
}

Light::Light(vec3 pos, vec3 dir, unsigned int type, vec3 color, float angle, float intensity)
{
    //store the type
    this->setType(type);
    //store the inputed position
    this->pos = pos;
    //store the inpted color
    this->color = color;
    //store the inputed intensity
    this->lightIntensity = intensity;
    //store the direction
    this->dir = dir;
    //store the angle
    this->angle = angle;

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
    //specify the light as a point light
    this->lightType = GLGE_LIGHT_SOURCE_TYPE_POINT;

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

void Light::setType(unsigned int type)
{
    //check if the light soure type is safe
    if (type > 2)
    {
        //check if a warning should be printed
        if (glgeWarningOutput)
        {
            //print a warning
            printf("[GLGE WARNING] %d dose not correspont to any valid light source type\n", type);
        }
        //stop the function
        return;
    }
    //now, everything is safe to store
    this->lightType = type;
}

int Light::getType()
{
    //return the light source type
    return this->lightType;
}

void Light::setDir(vec3 dir)
{
    //store the direction
    this->dir = dir;
}

vec3 Light::getDir()
{
    //check if this is a point light
    if (this->lightType == GLGE_LIGHT_SOURCE_TYPE_POINT)
    {
        //if it is, return 0
        return vec3(0);
    }
    else
    {
        //else, return the direction
        return this->dir;
    }
}

void Light::setAngle(float angle)
{
    //store the angle
    this->angle = angle;
}

float Light::getAngle()
{
    //check if this is a spot light
    if (this->lightType == GLGE_LIGHT_SOURCE_TYPE_SPOT)
    {
        //return the angle
        return this->angle;
    }
    else
    {
        //else, return 0
        return 0;
    }
}

void Light::setIntenseAngle(float angle)
{
    //store the angle
    this->intAngle = angle;
}

float Light::getIntenseAngle()
{
    //check if this is a spot light
    if (this->lightType == GLGE_LIGHT_SOURCE_TYPE_SPOT)
    {
        //return the intense angle if it is not -1, if it is, return the angle
        return (this->intAngle == -1) ? this->angle : this->intAngle;
    }
    else
    {
        //else, return 0
        return 0;
    }
}

void Light::encode(Data* data)
{
    //store the light type
    data->writeInt(this->lightType);
    //store the position
    data->writeVec3(this->pos);
    //store the direction
    data->writeVec3(this->dir);
    //store the color
    data->writeVec3(this->color);
    //store the intensity
    data->writeFloat(this->lightIntensity);
    //store the angle
    data->writeFloat(this->angle);
    //store the intense angle
    data->writeFloat(this->intAngle);
}
void Light::decode(Data data)
{
    //store the light type
    this->lightType = data.readInt();
    //store the position
    this->pos = data.readVec3();
    //store the direction
    this->dir = data.readVec3();
    //store the color
    this->color = data.readVec3();
    //store the intensity
    this->lightIntensity = data.readFloat();
    //store the angle
    this->angle = data.readFloat();
    //store the intense angle
    this->intAngle = data.readFloat();
}

void Light::setupShadowMap()
{
    //shadows are currently not implemented
}



void glgeAddGlobalLighSource(Light* l)
{
    //add the light to the main window
    glgeWindows[glgeMainWindowIndex]->bindLight(l);
}