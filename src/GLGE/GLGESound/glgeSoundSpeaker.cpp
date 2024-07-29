/**
 * @file glgeSoundSpeaker.cpp
 * @author DM8AT
 * @brief declare the functionality of a class for positional sound sources defined in glgeSoundSpeaker.h
 * @version 0.1
 * @date 2024-07-11
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 */

#include "glgeSoundSpeaker.h"
#include "glgeSoundVars.hpp"
#include "../GLGEIndependend/glgePrivDefines.hpp"
#include <AL/alut.h>
#include <fstream>

Speaker::Speaker(const char* file, vec3 pos, vec3 rot)
{
    //pass to the super constructor
    this->super(file, pos, rot);
}

Speaker::Speaker(const char* file, Transform transform)
{
    //pass to the super constructor
    this->super(file, transform.pos, transform.rot);
}

Speaker::Speaker(const char* file, vec2 pos, float rot)
{
    //pass to the super constructor
    this->super(file, vec3(pos.x,0,pos.y),vec3(0,rot,0));
}

Speaker::Speaker(const char* file, Transform2D transform)
{
    //pass to the super construcotr
    this->super(file, vec3(transform.pos.x,0,transform.pos.y), vec3(0,transform.rot,0));
}

Speaker::~Speaker()
{
    //delete the buffer
    alDeleteBuffers(1, &this->buffer);
    //delete the source
    alDeleteSources(1, &this->source);
}

void Speaker::play(bool force)
{
    //check if the source is playing
    if (this->isPlaying() && !force)
    {
        //stop the function
        return;
    }
    //start the speaker
    alSourcePlay(this->source);
}

void Speaker::pause(bool pause)
{
    //check if the source should pause
    if (pause)
    {
        //pause the source
        alSourcePause(this->source);
    }
    else
    {
        //start the source
        alSourcePlay(this->source);
    }
}

bool Speaker::isPaused()
{
    //store the source state
    ALenum state;
    //get the source state
    alGetSourcei(this->source, AL_SOURCE_STATE, &state);
    //compare the state
    return (state == AL_PAUSED);
}

void Speaker::togglePause()
{
    //set teh paused state to not paused
    this->pause(!this->isPaused());
}

void Speaker::stop()
{
    //check if the source is not playing
    if (!this->isPlaying())
    {
        //stop the function
        return;
    }
    //stop the speaker
    alSourceStop(this->source);
}

bool Speaker::isPlaying()
{
    //store the source state
    ALenum state;
    //get the source state
    alGetSourcei(this->source, AL_SOURCE_STATE, &state);
    //compare the state
    return (state == AL_PLAYING) || (state == AL_PAUSED);
}

void Speaker::setGain(float gain)
{
    //store the new gain value
    this->gain = gain;
    //set the gain
    alSourcef(this->source, AL_GAIN, gain);
}

float Speaker::getGain()
{
    //return the gain value
    return this->gain;
}

void Speaker::setPitch(float pitch)
{
    //store the new pitch
    this->pitch = pitch;
    //set the pitch
    alSourcef(this->source, AL_PITCH, pitch);
}

float Speaker::getPitch()
{
    //return the pitch
    return this->pitch;
}

void Speaker::setPos(vec3 pos, bool supressVel)
{
    //check if the velocity should update
    if (supressVel)
    {
        //set the velocity to 0
        this->vel = vec3(0);
    }
    else
    {
        //update the velocity
        this->vel = this->pos - pos;
    }
    //store the new position
    this->pos = pos;
    //update the position
    alSource3f(this->source, AL_POSITION, this->pos.x,this->pos.y,this->pos.z);
    //update the velocity
    alSource3f(this->source, AL_VELOCITY, this->vel.x,this->vel.y,this->vel.z);
}

void Speaker::setPos(float x, float y, float z, bool supressVel)
{
    //pass to another function
    this->setPos(vec3(x,y,z), supressVel);
}

void Speaker::setPos(vec2 pos, bool supressVel)
{
    //pass to another function
    this->setPos(pos.x,0,pos.y,supressVel);
}

void Speaker::setPos(float x, float y, bool supressVel)
{
    //pass to another function
    this->setPos(x,0,y,supressVel);
}

void Speaker::move(vec3 deltaPos, bool supressVel)
{
    //change the position
    this->setPos(this->pos + deltaPos, supressVel);
}

void Speaker::move(float dx, float dy, float dz, bool supressVel)
{
    //pass to another function
    this->move(vec3(dx,dy,dz), supressVel);
}

void Speaker::move(vec2 deltaPos, bool supressVel)
{
    //pass to another function
    this->move(deltaPos.x,0,deltaPos.y,supressVel);
}

void Speaker::move(float dx, float dy, bool supressVel)
{
    //pass to another function
    this->move(dx,0,dy,supressVel);
}

vec3 Speaker::getPos()
{
    //return the position
    return this->pos;
}

vec2 Speaker::getPos2D()
{
    //return only x and z
    return vec2(this->pos.x,this->pos.z);
}

void Speaker::setRot(vec3 rot)
{
    //pass to another function
    this->setRot(rot.x,rot.y,rot.z);
}

void Speaker::setRot(float rx, float ry, float rz)
{
    //store the moded position
    this->rot.x = std::fmod(std::abs(rx), 2*M_PI);
    this->rot.y = std::fmod(std::abs(ry), 2*M_PI);
    this->rot.z = std::fmod(std::abs(rz), 2*M_PI);
    //store the orientation
    this->dir.x = std::sin(this->rot.y);
    this->dir.z = -std::cos(this->rot.y);
    //update the direction
    alSource3f(this->source, AL_DIRECTION, this->dir.x,this->dir.y,this->dir.z);
}

void Speaker::setRot(float rot)
{
    //pass to another function
    this->setRot(0,rot,0);
}

void Speaker::rotate(vec3 deltaRot)
{
    //change the rotation value
    this->setRot(this->rot + deltaRot);
}

void Speaker::rotate(float drx, float dry, float drz)
{
    //pass to a different function
    this->rotate(vec3(drx,dry,drz));
}

void Speaker::rotate(float delta)
{
    //pass to another function
    this->rotate(0,delta,0);
}

vec3 Speaker::getRot()
{
    //return the rotation
    return this->rot;
}

float Speaker::getRot2D()
{
    //return the rotation aroud the y axis
    return this->rot.y;
}

void Speaker::setTransform(Transform transform, bool supressVel)
{
    //set the position
    this->setPos(transform.pos, supressVel);
    //set the rotation
    this->setRot(transform.rot);
}

void Speaker::setTransform(Transform2D transform, bool supressVel)
{
    //set the position
    this->setPos(transform.pos, supressVel);
    //set the rotation
    this->setRot(transform.rot);
}

void Speaker::changeTransform(Transform deltaTransform, bool supressVel)
{
    //change the position
    this->move(deltaTransform.pos, supressVel);
    //change the rotation
    this->rotate(deltaTransform.rot);
}

void Speaker::changeTansform(Transform2D deltaTransform, bool supressVel)
{
    //change the position
    this->move(deltaTransform.pos, supressVel);
    //change the rotation
    this->rotate(deltaTransform.rot);
}

Transform Speaker::getTransform()
{
    //return a transform of the position, rotation and a scale of 1
    return Transform(this->pos, this->rot, vec3(1));
}

Transform2D Speaker::getTransform2D()
{
    //return a transform of the position, rotation and a scale of 1
    return Transform2D(vec2(this->pos.x,this->pos.z), this->rot.y, vec2(1));
}

void Speaker::super(const char* file, vec3 pos, vec3 rot)
{
    //store the file
    std::fstream path;
    //open the file
    path.open(file);
    //check if the path is valid
    if (!path)
    {
        //throw an error
        GLGE_THROW_ERROR(std::string("Failed to open file '") + file + "'")
        return;
    }
    path.close();

    //create a new source
    alGenSources(1, &this->source);
    //create a new buffer
    alGenBuffers(1, &this->buffer);

    //data from the file
    void* data;
    //requency of the audio data
    float freq;
    //store the format
    ALenum format;
    //store the size
    ALsizei size;

    //load the audio file
    data = alutLoadMemoryFromFile(file,&format,&size,&freq);
    //get errors
    ALenum err = alutGetError();
    //check the error state
    if ((err != AL_NO_ERROR) || !data)
    {
        //throw an error
        GLGE_THROW_ERROR(std::string("Something went wrong while loading the .wav file. ALUT error: ") + std::string(alutGetErrorString(err)));
        return;
    }
    //store the data in the buffer
    alBufferData(this->buffer,format,data,size,freq);
    //bind the buffer to the sound source
    alSourcei(this->source, AL_BUFFER, this->buffer);
    //free the data
    free(data);

    //set the position
    this->setPos(pos);
    //set the rotation
    this->setRot(rot);

}