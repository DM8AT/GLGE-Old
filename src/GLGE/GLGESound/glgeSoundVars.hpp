/**
 * @file glgeSoundVars.hpp
 * @author DM8AT
 * @brief define all the shared variables that store the state of the GLGE sound system
 * @version 0.1
 * @date 2024-07-11
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 */

#ifndef _GLGE_SND_VARS_H_
#define _GLGE_SND_VARS_H_

#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
#include "glgeSoundListener.h"

/**
 * @brief store the structure of a one-time played sound
 */
struct glgeTempSound
{
    /**
     * @brief store the source of the sound
     */
    unsigned int source;
    /**
     * @brief store the sound buffer
     */
    unsigned int buffer;
    void (*tickCallback)() = 0;
    void (*endCallback)() = 0;
};

/**
 * @brief store all temporary sounds
 */
extern std::vector<glgeTempSound> glgeTempSounds;

/**
 * @brief store the currently opend OpenAL device
 */
extern ALCdevice* glgeCurrentDevice;

/**
 * @brief store the current OpenAL context
 */
extern ALCcontext* glgeCurrentContext;

/**
 * @brief store a pointer to the currently bound listener or 0 if none is bound
 */
extern Listener* glgeCurrentListener;

/**
 * @brief store the scaling of the effect
 */
extern float glgeDopplerFactor;
/**
 * @brief store the speed of sound in the surrounding (default is 343)
 */
extern float glgeDopplerSoundVelocity;

#endif