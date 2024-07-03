/**
 * @file ParticleControllShader.cpp
 * @author DM8AT
 * @brief implement a simple particle system to demonstrate the particle capabilitys
 * @version 0.1
 * @date 2024-07-03
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#version 450 core
layout (local_size_x = 64, local_size_y = 1, local_size_z = 1) in;
#define PI 3.1415926535897932384

#include <glgeParticlesUtil>

/**
 * Utilitys to generate pseudo-random numbers
*/
//start seed for the random numbers
uint n = gl_GlobalInvocationID.x * gl_GlobalInvocationID.x;
float hash() 
{   // integer hash copied from Hugo Elias
	n = (n<<13U)^n; 
    n = n*(n*n*15731U+789221U)+1376312589U;
    return float(n&uvec3(0x0fffffffU))/float(0x0fffffff);
}

/**
 * Calculate a simple position in a disk shape
 */
vec3 randomDiskPoint(vec3 rand, vec3 n)
{
    float r = rand.x * 0.5 + 0.5;
    float angle = (rand.y + 1.0) * PI;
    float sr = sqrt(r);
    vec2 p = vec2(sr * cos(angle), sr * sin(angle));
    vec3 tangent = normalize(rand);
    vec3 bitangent = cross(tangent, n);
    tangent = cross(bitangent, n);
    return tangent * p.x + bitangent * p.y;
}

/**
 * Calculate a simple position in a 2D donut shape
 */
vec3 posInDonut(float minR, float maxR)
{
    for (int i = 0; i < 16; i++)
    {
        vec3 randPos = vec3(hash(),hash(),hash());
        randPos = randPos * 2.f - 1.f;
        vec3 pos = randomDiskPoint(randPos, vec3(0,1,0)) * maxR;
        if (length(pos) > minR)
        {
            return pos;
        }
    }

    return vec3(0);
}

/**
 * Spawn a particle
 */
BuffParticle spawn()
{
    //create a new particle
    BuffParticle particle;
    //set the position to a random position in a 2D donut
    vec3 pos = posInDonut(250.f, 2500.f);
    particle.px = pos.x;
    particle.pz = pos.z;
    //set the y position in a random range from -30 to 30
    particle.py = (hash() * 60.f) - 30.f;
    //set the scale to a range between 0.1 and 1
    float scale = 0.1 + hash() * 0.9;
    particle.sx = scale; particle.sy = scale; particle.sz = scale;
    //set the remaining lifetime to 1000
    particle.lifetime = 1000.f;
    //return the particle
    return particle;
}

/**
 * Tick a particle and continue its lifecycle
 */
void tick(inout BuffParticle particle)
{
    //rotate the particle a random amount around all axis
    particle.rx += 0.005 * hash();
    particle.ry += 0.01 * hash();
    particle.rz += 0.0075 * hash();

    //decrease the lifetime by 1
    particle.lifetime -= 1.f;
}

//start of the shader
void main()
{
    //get the particle this shader is assigned to handle
    BuffParticle own = glgeParticles[gl_GlobalInvocationID.x];

    //check if the particle is dead
    if (own.lifetime < 1.f)
    {
        //if it is dead, spawn a new one
        own = spawn();
    }
    else
    {
        //if it is alive, tick its lifecycle
        tick(own);
    }

    //recalculate all the matrices for the particle
    glgeComputeParticleMatrices(own);
    //store the patricle in the particle stack at its own index
    glgeParticles[gl_GlobalInvocationID.x] = own;
}