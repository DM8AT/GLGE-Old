/**
 * @file GLGEMath.cpp
 * @author DM8AT
 * @brief define the math functoins for GLGEMath.h
 * the private noise functions and the cosine interpolation is from https://stackoverflow.com/questions/29711668/perlin-noise-generation
 * @version 0.1
 * @date 2023-03-27
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 */

#include "GLGEMath.h"
#include <math.h>
#include "glgePrivDefines.hpp"

//store the seed for the noise functions
unsigned long glgeSeed = 0;

mat4 glgeLookAt(vec3 eye, vec3 center, vec3 up)
{
    //create 3 vectors for the collums of the output matrix
    vec3 a,b,c;

    //get the difference from the eye to the center
    c = eye-center;
    //normalise that difference
    c.normalize();

    //store the up vector in b
    b = up;
    //a is a cross product from b and c
    a = b.cross(c);
    //recalculate the b vector
    b = c.cross(a);

    //normalise the length of a and b
    a.normalize();
    b.normalize();

    //create the output matrix
    mat4 out;

    //store the elements in the matrix
    out.m[0][0] = a.x;
    out.m[1][0] = a.y;
    out.m[2][0] = a.z;
    out.m[3][0] = -(a*eye);
    out.m[0][1] = b.x;
    out.m[1][1] = b.y;
    out.m[2][1] = b.z;
    out.m[3][1] = -(b*eye);
    out.m[0][2] = c.x;
    out.m[1][2] = c.y;
    out.m[2][2] = c.z;
    out.m[3][2] = -(c*eye);
    out.m[0][3] = 0;
    out.m[1][3] = 0;
    out.m[2][3] = 0;
    out.m[3][3] = 1.0f;

    //return the matrix
    return out;
}

vec3 glgeRotateVector(float angle, vec3 v, vec3 target)
{
    //create a rotation quaternion from the vector input and the angle
    Quaternion RotationQ = Quaternion(angle, v);

    //conjugate the rotation quaternion
    Quaternion ConjugateQ = RotationQ.conjugate();

    //do some quaternion magic, I don't understand this part compleatly
    Quaternion W = (RotationQ * target) * ConjugateQ;

    //return the vector component of the quaternions
    return vec3(W.x,W.y,W.z);
}

//thanks for https://stackoverflow.com/questions/29711668/perlin-noise-generation for help with the noise

  //store the numbers on X and on Y
  int numX = 512,
      numY = 512,
      //store the number of octaves
      numOctaves = 7;
  //store the persistance for heigher-level noise
  float persistence = 0.5;

  //define how much prime number trippels are in the list
  #define maxPrimeIndex 10
  //store the current prime index
  int primeIndex = 0;

  //store a lot of heigh prime numbers
  int primes[maxPrimeIndex][3] = {
    { 995615039, 600173719, 701464987 },
    { 831731269, 162318869, 136250887 },
    { 174329291, 946737083, 245679977 },
    { 362489573, 795918041, 350777237 },
    { 457025711, 880830799, 909678923 },
    { 787070341, 177340217, 593320781 },
    { 405493717, 291031019, 391950901 },
    { 458904767, 676625681, 424452397 },
    { 531736441, 939683957, 810651871 },
    { 997169939, 842027887, 423882827 }
  };

  //calculate simple noise
  float Noise(int i, int x, int y) {
    //do some magic
    int n = x + y * 57;
    n = (n << 13) ^ n;
    int a = primes[i][0], b = primes[i][1], c = primes[i][2];
    int t = (n * (n * n * a + b) + c) & 0x7fffffff;
    //output a semi-random number
    return 1.0 - (float)(t)/1073741824.0;
  }

  float SmoothedNoise(int i, int x, int y) {
    //calculate a lot of noise
    float corners = (Noise(i, x-1, y-1) + Noise(i, x+1, y-1) +
                      Noise(i, x-1, y+1) + Noise(i, x+1, y+1)) / 16,
          sides = (Noise(i, x-1, y) + Noise(i, x+1, y) + Noise(i, x, y-1) +
                    Noise(i, x, y+1)) / 8,
          center = Noise(i, x, y) / 4;
    //calculate the sum of a lot of noise
    return corners + sides + center;
  }

  float Interpolate(float a, float b, float x) {  // cosine interpolation
    //multiply x by pi and calculate the half of 1 minus cosine of ft(x * PI)
    float ft = x * GLGE_PI,
          f = (1 - cos(ft)) * 0.5;
    //return a times 1 minus f and add b times f (alias do some magic)
    return  a*(1-f) + b*f;
  }

  float InterpolatedNoise(int i, float x, float y) {
    //store the inputed x as an integer
    int integer_X = x;
    //store the part behind the dot of x
    float fractional_X = x - integer_X;
    //do the same for y
    int integer_Y = y;
    float fractional_Y = y - integer_Y;

    //calculate 4 somooth noise values and interpolate betwean them
    float v1 = SmoothedNoise(i, integer_X, integer_Y),
          v2 = SmoothedNoise(i, integer_X + 1, integer_Y),
          v3 = SmoothedNoise(i, integer_X, integer_Y + 1),
          v4 = SmoothedNoise(i, integer_X + 1, integer_Y + 1),
          i1 = Interpolate(v1, v2, fractional_X),
          i2 = Interpolate(v3, v4, fractional_X);
    //return an interpolation of the noise
    return Interpolate(i1, i2, fractional_Y);
  }

  float ValueNoise_2D(float x, float y) {
    //set the total to 0
    float total = 0,
          //set the frequency to 2 to the power of the number of octaves
          frequency = pow(2, numOctaves),
          //set the amplitude to 1
          amplitude = 1;
    //repeat num octave times
    for (int i = 0; i < numOctaves; ++i) {
      //divide the frequency by 2
      frequency /= 2;
      //multypile the amplitude by the persitance
      amplitude *= persistence;
      //add a interpolated noise to the total
      total += InterpolatedNoise((primeIndex + i) % maxPrimeIndex,
          x / frequency, y / frequency) * amplitude;
    }
    //return the total divided by the freuqency
    return total / frequency;
  }


float glgeLerp(float x, float y, float t)
{
    //return the interpolated input
    return x + t * (y - x);
}

vec2 glgeLerp(vec2 x, vec2 y, float t)
{
    //it is like for floats
    return x*t + y*(1-t);
}

vec3 glgeLerp(vec3 x, vec3 y, float t)
{
    //it is like for floats
    return x*t + y*(1-t);
}

float glgeInterpolate(float x, float y, float t)
{
    //run the interpolation function (this function is private)
    return Interpolate(x, y, t);
}

float glgeNoise2D(vec2 pos)
{
    //return the noise of the position with the stored seed
    return Noise(glgeSeed, pos.x, pos.y);
}

float glgeInterpolatedNoise2D(vec2 pos)
{
    //return the interpolated noise value of the inputed position and the stored seed
    return InterpolatedNoise(glgeSeed, pos.x, pos.y);
}

float glgeValueNoise2D(vec2 pos)
{
    //return the value noise value of the inputed position
    return ValueNoise_2D(pos.x, pos.y);
}

float glgeSmoothedNoise2D(vec2 pos)
{
    //return the smoothed noise value of the seed and the inputed position
    return SmoothedNoise(glgeSeed, pos.x, pos.y);
}

void glgeSetSeed(unsigned long seed)
{
    //store the inputed seed
    glgeSeed = seed;
}

unsigned long glgeGetSeed()
{
    //return the stored seed
    return glgeSeed;
}

void glgeSetNumOctaves(unsigned int n)
{
    //store the inputed number of octaves
    numOctaves = n;
}

int glgeGetNumOctaves()
{
    //get the stored number of octaves
    return numOctaves;
}

void glgeSetPersistance(float p)
{
    //store the inputed persistance
    persistence = p;
}

float glgeGetPersistance()
{
    //return the stored bersistance
    return persistence;
}

float glgeClamp(float v, float min, float max)
{
    //check if the value is smaler than the minimal value
    if (v < min)
    {
        //if it is, then the value is the minimum
        v = min;
    }
    //else, if the value is greater than the maximum
    else if (v > max)
    {
        //then the value is the maximum
        v = max;
    }
    //return the value
    return v;
}

vec2 glgeClamp(vec2 v, float min, float max)
{
    //simply clamp the x and y value independandly
    return vec2(glgeClamp(v.x, min, max), glgeClamp(v.y, min, max));
}

vec3 glgeClamp(vec3 v, float min, float max)
{
    //clamp the x,y and z axes independandly
    return vec3(glgeClamp(v.x, min, max), glgeClamp(v.y, min, max), glgeClamp(v.z, min, max));
}

vec4 glgeClamp(vec4 v, float min, float max)
{
    //clamp the x,y,z and w axes independandly
    return vec4(glgeClamp(v.x, min, max), glgeClamp(v.y, min, max), glgeClamp(v.z, min, max), glgeClamp(v.w, min, max));
}

//convert the angle a to radians
float glgeToRadians(float a)
{
    //return the angle in radians
    return a * GLGE_TO_RADIANS;
}

//convert the angle a to degrees
float glgeToDegrees(float a)
{
    //return the angle in degrees
    return a * GLGE_TO_DEGREES;
}

float glgeMax(float value, float min)
{
    //if the value is greater min, return the value, else min
    return (value >= min) ? value : min;
}

float glgeMin(float value, float max)
{
    //if the value is less than max, return the value, else max
    return (max >= value) ? max : value;
}