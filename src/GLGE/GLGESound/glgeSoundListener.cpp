/**
 * @file glgeSoundListener.cpp
 * @author DM8AT
 * @brief declare the functions for the sound listener class for positional audio defined in glgeSoundListener.h
 * @version 0.1
 * @date 2024-07-11
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 */

#include "glgeSoundListener.h"
#include "glgeSoundVars.hpp"
#include <math.h>

Listener::Listener(vec3 pos, vec3 rot)
{
    //pass to the super constructor
    this->super(pos, rot);
}

Listener::Listener(Transform transform)
{
    //pass to the super constructor
    this->super(transform.pos, transform.rot);
}


Listener::Listener(vec2 pos, float rot)
{
    //pass to the super constructor
    this->super(vec3(pos.x,0,pos.y), vec3(0,rot,0));
}

Listener::Listener(Transform2D transform)
{
    //pass to the super constructor
    this->super(vec3(transform.pos.x,0,transform.pos.y), vec3(0,transform.rot,0));
}

Listener::~Listener()
{
    if (glgeCurrentListener == this)
    {
        glgeCurrentListener = 0;
    }
}

void Listener::super(vec3 pos, vec3 rot)
{
    //store the position
    this->pos = pos;
    //store the rotation
    this->rot = rot;

    //bind the listener
    this->bind();
}

void Listener::bind()
{
    //pass the position
    alListener3f(AL_POSITION, this->pos.x,this->pos.y,this->pos.z);
    //pass the velocity
    alListener3f(AL_VELOCITY, this->vel.x,this->vel.y,this->vel.z);
    //pass the orientation
    alListenerfv(AL_ORIENTATION, this->orientation);
    //store a pointer to this listener to update it automaticaly
    glgeCurrentListener = this;
}

void Listener::setPos(vec3 pos, bool supressVel)
{
    //check if velocity sould be calculated
    if (supressVel)
    {
        //zero the velocity
        this->vel = vec3(0);
    }
    else
    {
        //if not, calculate the velocity
        this->vel = this->pos - pos;
    }
    //store the new position
    this->pos = pos;
}

void Listener::setPos(float x, float y, float z, bool supressVel)
{
    //pass to another function
    this->setPos(vec3(x,y,z),supressVel);
}

void Listener::setPos(vec2 pos, bool supressVel)
{
    //pass to another function
    this->setPos(vec3(pos.x,0,pos.y), supressVel);
}

void Listener::setPos(float x, float y, bool supressVel)
{
    //pass to another function
    this->setPos(x,0,y,supressVel);
}

void Listener::move(vec3 deltaPos, bool supressVel)
{
    //check if velocity should be stored
    if (supressVel)
    {
        //zero the velocity
        this->vel = vec3(0);
    }
    else
    {
        //update the velocity
        this->vel = deltaPos;
    }
    //update the position
    this->pos += deltaPos;
}

void Listener::move(float dx, float dy, float dz, bool supressVel)
{
    //pass to another function
    this->move(vec3(dx,dy,dz), supressVel);
}

void Listener::move(vec2 deltaPos, bool supressVel)
{
    //pass to another function
    this->move(vec3(deltaPos.x,0,deltaPos.y), supressVel);
}

void Listener::move(float dx, float dy, bool supressVel)
{
    //pass to another function
    this->move(dx,0,dy,supressVel);
}

vec3 Listener::getPos()
{
    //return the position
    return this->pos;
}

vec2 Listener::getPos2D()
{
    return vec2(this->pos.x,this->pos.z);
}

void Listener::setRot(vec3 rot)
{
    //pass to another function
    this->setRot(rot.x, rot.y, rot.z);
}

void Listener::setRot(float rx, float ry, float rz)
{
    //store the values modded by 2 Pi
    this->rot.x = std::fmod(rx, 2*M_PI);
    this->rot.y = std::fmod(ry, 2*M_PI);
    this->rot.z = std::fmod(rz, 2*M_PI);
    //update the orientation
    this->updateOrientation();
}

void Listener::setRot(float rot)
{
    //pass to another function
    this->setRot(0,rot,0);
}

void Listener::rotate(vec3 deltaRot)
{
    //update the rotation
    this->setRot(this->rot + deltaRot);
}

void Listener::rotate(float drx, float dry, float drz)
{
    //pass to another function
    this->rotate(vec3(drx,dry,drz));
}

void Listener::rotate(float delta)
{
    //pass to another function
    this->rotate(0,delta,0);
}

vec3 Listener::getRot()
{
    //return the rotation
    return this->rot;
}

float Listener::getRot2D()
{
    //return only the roattion around y
    return this->rot.y;
}

void Listener::setTransform(Transform transform, bool supressVel)
{
    //set the position
    this->setPos(transform.pos, supressVel);
    //set the rotation
    this->setRot(transform.rot);
}

void Listener::setTransform(Transform2D transform, bool supressVel)
{
    //set the position
    this->setPos(transform.pos, supressVel);
    //set the rotation
    this->setRot(transform.rot);    
}

void Listener::changeTransform(Transform deltaTransform, bool supressVel)
{
    //update the position
    this->move(deltaTransform.pos, supressVel);
    //update the rotation
    this->rotate(deltaTransform.rot);
}

void Listener::changeTransform(Transform2D deltaTransform, bool supressVel)
{
    //update the position
    this->move(deltaTransform.pos, supressVel);
    //update the rotation
    this->rotate(deltaTransform.rot);
}

Transform Listener::getTransform()
{
    //return a transform of the position and rotation with a scale of 1
    return Transform(this->pos, this->rot, vec3(1));
}

Transform2D Listener::getTransform2D()
{
    //return a 2D transform of the position and rotation with a scale of 1
    return Transform2D(vec2(this->pos.x, this->pos.z), this->rot.y, vec2(1));
}

void Listener::updateOrientation()
{
    //store the orientation
    this->orientation[0] = std::sin(this->rot.y);
    this->orientation[2] = -std::cos(this->rot.y);
}