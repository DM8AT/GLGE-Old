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
    this->lightDat.pos = pos;
    //store the inpted color
    this->lightDat.color.x = col.x;
    this->lightDat.color.y = col.y;
    this->lightDat.color.z = col.z;
    this->lightDat.color.w = intensity;
    //specify the light as a point light
    this->lightDat.type = GLGE_LIGHT_SOURCE_TYPE_POINT;

    //setup the shadow map
    this->setupShadowMap();
}

Light::Light(vec3 pos, vec3 dir, unsigned int type, vec3 col, float angle, float intensity)
{
    //store the type
    this->setType(type);
    //store the inputed position
    this->lightDat.pos = pos;
    //store the inpted color
    this->lightDat.color.x = col.x;
    this->lightDat.color.y = col.y;
    this->lightDat.color.z = col.z;
    this->lightDat.color.w = intensity;
    //store the direction
    this->lightDat.dir = dir;
    //store the angle
    this->lightDat.angle = angle;

    //setup the shadow map
    this->setupShadowMap();
}

Light::Light(float x, float y, float z, float r, float g, float b, float intensity)
{
    //store the inputed position
    this->lightDat.pos = vec3(x,y,z);
    //store the inpted color
    this->lightDat.color.x = r;
    this->lightDat.color.y = g;
    this->lightDat.color.z = b;
    this->lightDat.color.w = intensity;
    //specify the light as a point light
    this->lightDat.type = GLGE_LIGHT_SOURCE_TYPE_POINT;
}

//set the position of the light source
void Light::setPos(vec3 p)
{
    //apply the new position
    this->lightDat.pos = p;
    //say that the light needs to be updated
    this->needsUpdate = true;
}

//set the position of the light source
void Light::setPos(float x, float y, float z)
{
    //apply the new position
    this->lightDat.pos = vec3(x,y,z);
    //say that the light needs to be updated
    this->needsUpdate = true;
}

//move the light source
void Light::move(vec3 v)
{
    //change the position of the light source
    this->lightDat.pos += v;
}

//move the light source
void Light::move(float x, float y, float z)
{
    //change the position of the light source
    this->lightDat.pos += vec3(x,y,z);
    //say that the light needs to be updated
    this->needsUpdate = true;
}

vec3 Light::getPos()
{
    //return the light source position
    return this->lightDat.pos;
}

void Light::setColor(vec3 col)
{
    //set the color to the inputed one
    this->lightDat.color.x = col.x;
    this->lightDat.color.y = col.y;
    this->lightDat.color.z = col.z;
    //say that the light needs to be updated
    this->needsUpdate = true;
}

void Light::setColor(float r, float g, float b)
{
    //set the color to the inputed one
    this->lightDat.color.x = r;
    this->lightDat.color.y = g;
    this->lightDat.color.z = b;
    //say that the light needs to be updated
    this->needsUpdate = true;
}

void Light::pitchColor(vec3 col)
{
    //scale the color by the inpt
    this->lightDat.color.x *= col.x;
    this->lightDat.color.y *= col.y;
    this->lightDat.color.z *= col.z;
    //say that the light needs to be updated
    this->needsUpdate = true;
}

void Light::pitchColor(float r, float g, float b)
{
    //scale the color by the inpt
    this->pitchColor(vec3(r,g,b));
}

void Light::addColor(vec3 color)
{
    //add the input to the color
    this->lightDat.color.x += color.x;
    this->lightDat.color.y += color.y;
    this->lightDat.color.z += color.z;
    //say that the light needs to be updated
    this->needsUpdate = true;
}

void Light::addColor(float r, float g, float b)
{
    //add the input to the color
    this->addColor(vec3(r,g,b));
}

vec3 Light::getColor()
{
    //return the color of the light source
    return vec3(this->lightDat.color.x,this->lightDat.color.y,this->lightDat.color.z);
}

void Light::setIntensity(float intens)
{
    //store the inputed intensity
    this->lightDat.color.w = intens;
    //say that the light needs to be updated
    this->needsUpdate = true;
}

void Light::scaleIntensity(float s)
{
    //scale the stored intensity
    this->lightDat.color.w *= s;
    //say that the light needs to be updated
    this->needsUpdate = true;
}

void Light::addIntensity(float d)
{
    //add the input to the stored intensity
    this->lightDat.color.w += d;
    //say that the light needs to be updated
    this->needsUpdate = true;
}

float Light::getInsensity()
{
    //return the stored intensity
    return this->lightDat.color.w;
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
    this->lightDat.type = type;
    //say that the light needs to be updated
    this->needsUpdate = true;
}

int Light::getType()
{
    //return the light source type
    return this->lightDat.type;
}

void Light::setDir(vec3 dir)
{
    //store the direction
    this->lightDat.dir = dir;
    //say that the light needs to be updated
    this->needsUpdate = true;
}

vec3 Light::getDir()
{
    //check if this is a point light
    if (this->lightDat.type == GLGE_LIGHT_SOURCE_TYPE_POINT)
    {
        //if it is, return 0
        return vec3(0);
    }
    else
    {
        //else, return the direction
        return this->lightDat.dir;
    }
}

void Light::setAngle(float angle)
{
    //store the angle
    this->lightDat.angle = angle;
    //say that the light needs to be updated
    this->needsUpdate = true;
}

float Light::getAngle()
{
    //check if this is a spot light
    if (this->lightDat.type == GLGE_LIGHT_SOURCE_TYPE_SPOT)
    {
        //return the angle
        return this->lightDat.angle;
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
    this->lightDat.intAngle = angle;
    //say that the light needs to be updated
    this->needsUpdate = true;
}

float Light::getIntenseAngle()
{
    //check if this is a spot light
    if (this->lightDat.type == GLGE_LIGHT_SOURCE_TYPE_SPOT)
    {
        //return the intense angle if it is not -1, if it is, return the angle
        return (this->lightDat.intAngle == -1) ? this->lightDat.angle : this->lightDat.intAngle;
    }
    else
    {
        //else, return 0
        return 0;
    }
}

Data* Light::encode()
{
    //create a new data
    Data* data = new Data();
    //add the own data
    data->writeBytes((uint8_t*)&this->lightDat, sizeof(this->lightDat));
    //return the data
    return data;
}
void Light::decode(Data data)
{
    //read the own data
    this->lightDat = *(LightData*)data.readBytes(sizeof(this->lightDat));
    //say that the light needs to be updated
    this->needsUpdate = true;
}

mat4 Light::getLightMat()
{
    //return the light space matrix
    return mat4(this->lightSpaceMat);
}

void Light::makeCurrentShadowCaster()
{
    //store that this is the current shadow caster
    glgeCurrentShadowCaster = this;
    //update the light space matrix
    this->updateLightMat();
    //resize the viewport
    glViewport(0,0,1024,1024);
    //bind the shadow fbo
    glBindFramebuffer(GL_FRAMEBUFFER, this->shadowFBO);
    //clear the buffer
    glClear(GL_DEPTH_BUFFER_BIT);
}

int Light::getShadowMap()
{
    //return the shadow handler
    return this->texUnit;
}

void Light::bind()
{
    //activate the next texture unit
    glActiveTexture(GL_TEXTURE0 + glgeTextureUnit);
    //store the texture unit
    this->texUnit = glgeTextureUnit;
    //bind the texture
    glBindTexture(GL_TEXTURE_2D, this->shadowDepth);
    //increase the amount of texture units
    glgeTextureUnit++;
}

void Light::unbind()
{
    //activate the own texture unit
    glActiveTexture(GL_TEXTURE0 + this->texUnit);
    //reset the unit binding
    this->texUnit = 0;
    //bind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    //decrease the amount of texture units
    glgeTextureUnit--;
}

LightData Light::getLightData()
{
    //return the light data
    return this->lightDat;
}

bool Light::shouldUpdate()
{
    //return if an update is qued
    return this->needsUpdate;
}

void Light::updateDone()
{
    //say that the update is done
    this->needsUpdate = false;
}

void Light::queueUpdate()
{
    //say that an update is needed
    this->needsUpdate = true;
}

void Light::setupShadowMap()
{
    //create the texture for depth
    glGenTextures(1, &this->shadowDepth);
    //bind the texture
    glBindTexture(GL_TEXTURE_2D, this->shadowDepth);
    //set the size of the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    //set simple text parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    //create a new framebuffer
    glGenFramebuffers(1, &this->shadowFBO);
    //bind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->shadowFBO);
    //add a depth attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->shadowDepth, 0);

    //check the framebuffer status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //check if the framebuffer is compleate
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        //throw an error
        GLGE_THROW_ERROR("Failed to create framebuffer for shadow mapping")
    }

    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    //unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //say that the light needs to be updated
    this->needsUpdate = true;
}

void Light::updateLightMat()
{
    //store a vector for the y axis
    vec3 yaxis = vec3(0,1,0);
    //start rotation things
    //calculate the view vector
    //setup the vector to face right
    vec3 view = vec3(1,0,0);
    //rotathe the vector around the y axis by the x rotation amount
    view = glgeRotateVector(this->lightDat.dir.x,yaxis, view);
    //normaise the vector
    view.normalize();

    //rotate the view vector and calculate U
    //calculate the cross product of the yaxis and the view vector
    vec3 U = view.cross(yaxis);
    //normalise the U vector
    U.normalize();
    //rotate the view vector around U by the rotation on the y axis
    view = glgeRotateVector(this->lightDat.dir.y, U, view);
    
    //setup the vectors
    //N is equal to the view vector
    vec3 N = view;
    //noralise N
    N.normalize();

    //V is the cross product of U and N. Both are normalised, so no normalistion needed
    vec3 V = U.cross(N);
    V = glgeRotateVector(this->lightDat.dir.z, U, V);
    V.normalize();

    //setup the view matrix
    //IMPORTANT: the vectors are in reversd order!
    mat4 rotMat =  mat4(U.z,U.y,U.x,0,
                        V.z,V.y,V.x,0,
                        N.z,N.y,N.x,0,
                        0,  0,  0,  1);

    mat4 transfMat = mat4(1,0,0,-this->lightDat.pos.x,
                          0,1,0,-this->lightDat.pos.y,
                          0,0,1,-this->lightDat.pos.z,
                          0,0,0,1);

    //store the porjection matrix
    mat4 projMat;

    //check if this is a spotlight
    if (this->lightDat.type == GLGE_LIGHT_SOURCE_TYPE_SPOT)
    {
        //store the far plane
        float far = 1000.0;
        //store the near plane
        float near = 0.1;
        //calculate the z-range
        float zRange = far - near;
        //use the full angle of the light as the field of view
        float tHF = std::tan((this->lightDat.angle));
        //build a perspective projection matrix
        projMat =  mat4(1.f/tHF, 0.f, 0.f, 0.f,
                        0.f, 1/tHF, 0.f, 0.f,
                        0.f, 0.f, (-near - far)/zRange, (2.f*far * near) / zRange,
                        0.f, 0.f, 1.f, 0.f);

        glActiveTexture(GL_TEXTURE7);
    }

    //store the light space matrix
    this->lightSpaceMat = (projMat * rotMat * transfMat);
}

void glgeAddGlobalLighSource(Light* l)
{
    //say that the light need an update
    l->queueUpdate();
    //add the light to the main window
    glgeWindows[glgeMainWindowIndex]->bindLight(l);
}