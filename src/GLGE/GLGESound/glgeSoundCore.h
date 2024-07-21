/**
 * @file glgeSoundCore.h
 * @author DM8AT
 * @brief the main sound function collection for GLGE
 * @version 0.1
 * @date 2024-07-11
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 */

#ifndef _GLGE_SND_CORE_H_
#define _GLGE_SND_CORE_H_

#include <vector>
#include <string>
#include "glgeSoundListener.h"
#include "glgeSoundSpeaker.h"

/**
 * @brief get a vector containing the names of all audio devices
 * 
 * @return std::vector<std::string> all the device names
 */
std::vector<std::string> glgeGetAvailableAudioDevices();

/**
 * @brief open the conection to a specific audio device
 * 
 * @param device the name of the device, use 0 for the default device
 */
void glgeOpenDevice(const char* device = 0);

/**
 * @brief play a simple sound from a .wav file
 * 
 * @param file the path to the sound file
 * @param volume the volume for the sound
 * @param tickCallback a callback function that is called every time the sound is ticked
 * @param endCallback a callback function that is executed once the sound was stoppe
 */
void glgePlaySound(const char* file, float volume = 1.f, void (*tickCallback)() = 0, void (*endCallback)() = 0);

/**
 * @brief do cleanup and stuff for temporary sounds
 */
void glgeTickSounds();

/**
 * @brief set the scaling for the dopper effect
 * 
 * @param factor the new doppler effect factor, can't be less than 0
 */
void glgeSetDopplerEffectFactor(float factor);

/**
 * @brief get the current doppler effect factor
 * 
 * @return float the current doppler effect factor
 */
float glgeGetDopplerEffectFactor();

/**
 * @brief set the speed of sound that is used to calculate the doppler effect. The default is 343, the speed of sound in air
 * 
 * @param speed the new speed of sound in meters per second
 */
void glgeSetDopplerEffectSpeedOfSound(float speed);

/**
 * @brief get the speed of sound used to calculate the doppler effect
 * 
 * @return float the speed of sound
 */
float glgeGetDopplerEffectSpeedOfSound();

/**
 * @brief get the amount of unpositional sounds currently playing
 * 
 * @return unsigned int the amount of unpositional sounds playing
 */
unsigned int glgeGetCurrentSoundCount();

#endif