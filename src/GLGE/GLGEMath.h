/**
 * @file GLGEMath.h
 * @author DM8AT
 * @brief define a few math functions usefull for 3D and 2D game developement
 * @version 0.1
 * @date 2023-03-27
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_MATH_H_
#define _GLGE_MATH_H_

#include "CML/CML.h"

/**
 * @brief get a matrix to rotate a vector to an point
 * !! Function not garantied !!
 * @param eye the vector to rotate
 * @param center where to rotate the vector
 * @param up the local up vector (Default to y)
 * @return mat4 the matrix to rotate the vector to the point
 */
mat4 glgeLookAt(vec3 eye, vec3 center, vec3 up = vec3(0,1,0));

/**
 * @brief rotate a vector
 * this function works with quaternions, so it gives back the rotated vector instantly
 * @param angle the angle to rotate the vector
 * @param v the vector to rotate
 * @param target the target to rotate the vector to
 * @return vec3 the rotated vector
 */
vec3 glgeRotateVector(float angle, vec3 v, vec3 target);

/**
 * @brief linearly interpolate two floats
 * 
 * @param x the first float (p1)
 * @param y the second float (p2)
 * @param t where in betwean the value lies (0-1 is save)
 * @return float the interpolated float
 */
float glgeLerp(float x, float y, float t);

/**
 * @brief linearly interpolate two vec2's
 * 
 * @param x the first vector (p1)
 * @param y the second vector (p2)
 * @param t where in betwean the value lies (0-1 is save)
 * @return vec2 the interpolated vector
 */
vec2 glgeLerp(vec2 x, vec2 y, float t);

/**
 * @brief linearly interpolate two vec3's
 * 
 * @param x the first vector (p1)
 * @param y the second vector (p2)
 * @param t where in betwean the value lies (0-1 is save)
 * @return vec3 the interpolated vector
 */
vec3 glgeLerp(vec3 x, vec3 y, float t);

/**
 * @brief interpolate two values
 * this is not linear intperpolation, it is cosine interpolation
 * @param x the first float (p1)
 * @param y the second float (p2)
 * @param t where in betwean the value lies (0-1 is save)
 * @return float the interpolated float
 */
float glgeInterpolate(float x, float y, float t);

/**
 * @brief calculate a semi-random number
 * 
 * @param pos the position to sample the number
 * @return float the semi-random number (range: -1 to 1)
 */
float glgeNoise2D(vec2 pos);

/**
 * @brief smoothed noise, somewhat like perlin noise
 * 
 * @param pos the position to sample the noise
 * @return float the semi-random number (range: -1 to 1)
 */
float glgeInterpolatedNoise2D(vec2 pos);

/**
 * @brief smooth semi-random numbers
 * 
 * @param pos the position to sample the numbers
 * @return float the semi-random number (range: -1 to 1)
 */
float glgeValueNoise2D(vec2 pos);

/**
 * @brief smoothed out interpolated noise
 * 
 * @param pos the position to sample the noise
 * @return float the semi-random number (range: -1 to 1)
 */
float glgeSmoothedNoise2D(vec2 pos);

/**
 * @brief set the seed for the noise functions
 * 
 * @param seed the new seed for the functions
 */
void glgeSetSeed(unsigned long seed);

/**
 * @brief get the current seed from the noise functions
 * 
 * @return unsigned long the seed
 */
unsigned long glgeGetSeed();

/**
 * @brief define the number of octaves used by higher-level noise
 * 
 * @param numOctaves the new number of octaves
 */
void glgeSetNumOctaves(unsigned int numOctaves);

/**
 * @brief get the current number of octaves
 * 
 * @return int the current number of octaves
 */
int glgeGetNumOctaves();

/**
 * @brief set the persistance for the higher-level noise functions
 * a range from 0 to 1 is recomended, but values outside the range work too
 * @param persistance the new persistance
 */
void glgeSetPersistance(float persistance);

/**
 * @brief get the current persistance
 * 
 * @return float the current persistance
 */
float glgeGetPersistance();

/**
 * @brief clamp a float
 * 
 * @param v the value to clamp
 * @param min the minimal value, the value input should have
 * @param max the maximal value, the value input should have
 * @return float the clamped value
 */
float glgeClamp(float v, float min, float max);

/**
 * @brief clamp a vector
 * 
 * @param v the vector to clamp
 * @param min the minimal value, each value of the vector should have
 * @param max the maximal value, each value of the vector should have
 * @return vec2 the clamped vector
 */
vec2 glgeClamp(vec2 v, float min, float max);

/**
 * @brief clamp a vector
 * 
 * @param v the vector to clamp
 * @param min the minimal value, each value of the vector should have
 * @param max the maximal value, each value of the vector should have
 * @return vec3 the clamped vector
 */
vec3 glgeClamp(vec3 v, float min, float max);

/**
 * @brief clamp a vector
 * 
 * @param v the vector to clamp
 * @param min the minimal value, each value of the vector should have
 * @param max the maximal value, each value of the vector should have
 * @return vec4 the clamped vector
 */
vec4 glgeClamp(vec4 v, float min, float max);

/**
 * @brief convert an angle to radians
 * 
 * @param angle the angle in degrees
 * @return float the angle in radians
 */
float glgeToRadians(float angle);

/**
 * @brief convert an angle to degrees
 * 
 * @param angle the angle in radians
 * @return float the angle in degrees
 */
float glgeToDegrees(float angle);

/**
 * @brief get the bigger value of two numbers
 * 
 * @param value the value to compare
 * @param min the value to compare with
 * @return float the bigger value
 */
float glgeMax(float value, float min);

/**
 * @brief get the smaller value of two numbers
 * 
 * @param value the value to compare
 * @param min the value to compare with
 * @return float the bigger value
 */
float glgeMin(float value, float max);

#endif