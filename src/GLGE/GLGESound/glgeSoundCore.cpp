/**
 * @file glgeSoundCore.cpp
 * @author DM8AT
 * @brief implement the main sound functions declared in glgeSoundCore.h
 * @version 0.1
 * @date 2024-07-11
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 */

#include "glgeSoundCore.h"
#include "glgeSoundVars.hpp"
#include "../GLGEIndependend/glgePrivDefines.hpp"
#include <cstring>
#include <fstream>
#include <AL/alut.h>

std::vector<std::string> glgeGetAvailableAudioDevices()
{
    //check if the enumeration extension exists
    if (!alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"))
    {
        //if not, throw an error
        GLGE_THROW_ERROR("Can't enumerate all available devices because enumeration extension is not present");
        return {};
    }

    //store the vector with the device names
    std::vector<std::string> devices;
    //store the current device to read
    const ALchar* device = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    //store the next device to read
    const ALchar* next = device + 1;
    //store the length of the device
    size_t len = 0;

    //loop over all available devices
    while (device && *device != '\0' && next && *next != '\0')
    {
        //read the device name
        devices.push_back(std::string(device));
        //read the length of the device name
        len = strlen(device);
        //increment the device pointer
        device += len+1;
        //increment the next pointer
        next += len+2;
    }
    
    for (std::string dev : devices)
    {
        printf("Found device %s\n", dev.c_str());
    }

    return devices;
}

void glgeOpenDevice(const char* device)
{
    //check if a device was allready opend
    if (glgeCurrentDevice)
    {
        //close the device
    }

    //open the specified device
    glgeCurrentDevice = alcOpenDevice(device);

    //reset the error context
    alGetError();

    //check if the device opening was successfull
    if (!glgeCurrentDevice)
    {
        //throw an error
        GLGE_THROW_ERROR("Failed to open sound device")
        return;
    }

    //create a context
    glgeCurrentContext = alcCreateContext(glgeCurrentDevice, NULL);
    //check if the context was created successfully
    if (!alcMakeContextCurrent(glgeCurrentContext))
    {
        //throw an error
        GLGE_THROW_ERROR("Faild to bind sound context")
        return;
    }

    //get the current error state
    ALenum error = alGetError();
    //check the error state
    if (error != AL_NO_ERROR)
    {
        //throw an error
        GLGE_THROW_ERROR(std::string("OpenAL error occured while opening device and creating context. OpenAL error: ") + std::to_string(error))
        return;
    }

    //inialise ALUT
    if (!alutInit(0,0))
    {
        //throw an error
        GLGE_THROW_ERROR("Failed to initalise ALUT")
        return;
    }
    //set the doppler factor
    alDopplerFactor(glgeDopplerFactor);
    //set the dopler velocity
    alDopplerVelocity(glgeDopplerSoundVelocity);
}

void glgePlaySound(const char* file, float volume, void (*tickCallback)(), void (*endCallback)())
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
    //store the sound
    glgeTempSound sound;
    //store the sound callbacks
    sound.endCallback = endCallback;
    sound.tickCallback = tickCallback;

    //store the size of the sound
	ALsizei size;
    //store the sound frequency
    ALfloat freq;
    //store the sound format
	ALenum format;
    //store the sound data
    ALvoid* data;

    //create the source
    alGenSources(1, &sound.source);
    //disable attenuation
    alSourcei (sound.source, AL_SOURCE_RELATIVE, AL_TRUE);
    //set the volume for the sound
    alSourcef(sound.source, AL_GAIN, volume);

    //create a buffer for the sound
    alGenBuffers(1, &sound.buffer);
    //load the wav file
    data = alutLoadMemoryFromFile(file, &format, &size, &freq);
    //get the alut error state
    ALenum err = alutGetError();
    //check the error state
    if ((err != AL_NO_ERROR) || !data)
    {
        //throw an error
        GLGE_THROW_ERROR(std::string("Something went wrong while loading the .wav file. ALUT error: ") + std::string(alutGetErrorString(err)));
        return;
    }
    //load the data to OpenAL
    alBufferData(sound.buffer, format, data, size, freq);
    //bind the buffer to the source
    alSourcei(sound.source, AL_BUFFER, sound.buffer);
    //free the sound data
    free(data);

    //play the source
    alSourcePlay(sound.buffer);
    //store the sound
    glgeTempSounds.push_back(sound);
}

void glgeTickSounds()
{
    //iterate over all sounds
    for (size_t i = 0; i < glgeTempSounds.size(); ++i)
    {
        //store the state
        ALenum state;
        //get the state
        alGetSourcei(glgeTempSounds[i].source, AL_SOURCE_STATE, &state);
        //check the sound still plays
        if (state != AL_PLAYING)
        {
            //delete the source
            alDeleteSources(1, &glgeTempSounds[i].source);
            //delete the buffer
            alDeleteBuffers(1, &glgeTempSounds[i].buffer);
            //check if a end function exists
            if (glgeTempSounds[i].endCallback)
            {
                //call the function
                (*glgeTempSounds[i].endCallback)();
            }
            //erase the sound from the sound buffer
            glgeTempSounds.erase(glgeTempSounds.begin() + i);
            //reset the error state
            alGetError();
        }
        //check if a tick function exists
        if (glgeTempSounds[i].tickCallback)
        {
            //call the function
            (*glgeTempSounds[i].tickCallback)();
        }
    }

    //check if a listener is bound
    if (glgeCurrentListener)
    {
        //re-bind the listener to update it
        glgeCurrentListener->bind();
    }

    //store the OpenAL errors
    ALenum err = alGetError();
    //check if an error occured
    if (err != AL_NO_ERROR)
    {
        //throw an error
        GLGE_THROW_ERROR(std::string("An OpenAL error occured. Error code: " + std::to_string(err)))
    }
}


void glgeSetDopplerEffectFactor(float factor)
{
    //check if the factor is in range
    if (factor < 0)
    {
        //if not, throw an error
        GLGE_THROW_ERROR("The value '" + std::to_string(factor) + "' is less than 0, wich is not allowed as parameter for 'glgeSetDopplerEffectFactor'.")
        return;
    }
    //store the new doppler effect factor
    glgeDopplerFactor = factor;
    //pass the factor
    alDopplerFactor(factor);
}

float glgeGetDopplerEffectFactor()
{
    //return the doppler factor
    return glgeDopplerFactor;
}

void glgeSetDopplerEffectSpeedOfSound(float speed)
{
    //check if the value is in range
    if (speed < 0)
    {
        //if not, throw an error
        GLGE_THROW_ERROR("The value '" + std::to_string(speed) + "' is less than 0, wich is not allowed as parameter for 'glgeSetDopplerEffectSpeedOfSound'.")
        return;
    }
    //store the speed of sound
    glgeDopplerSoundVelocity = speed;
    //pass the speed
    alDopplerVelocity(speed);
}

float glgeGetDopplerEffectSpeedOfSound()
{
    //return the speed
    return glgeDopplerSoundVelocity;
}

unsigned int glgeGetCurrentSoundCount()
{
    //return the size of the unpositional sound vector
    return (unsigned int)glgeTempSounds.size();
}