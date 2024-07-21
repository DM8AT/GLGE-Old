/**
 * @file glgeSoundVars.cpp
 * @author DM8AT
 * @brief declare the default state for all the state variables defined in glgeSoundVars.hpp
 * @version 0.1
 * @date 2024-07-11
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 */

#include "glgeSoundVars.hpp"

/**
 * @brief store all temporary sounds
 */
std::vector<glgeTempSound> glgeTempSounds = {};

/**
 * @brief store the currently opend OpenAL device
 */
ALCdevice* glgeCurrentDevice = 0;

/**
 * @brief store the current OpenAL context
 */
ALCcontext* glgeCurrentContext = 0;

/**
 * @brief store a pointer to the currently bound listener or 0 if none is bound
 */
Listener* glgeCurrentListener = 0;

/**
 * @brief store the scaling of the effect
 */
float glgeDopplerFactor = 1.f;
/**
 * @brief store the speed of sound in the surrounding (default is 343)
 */
float glgeDopplerSoundVelocity = 343.f;