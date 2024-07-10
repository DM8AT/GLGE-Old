/**
 * @file Controll.cs
 * @author DM8AT
 * @brief this file is an example on how to use compute shaders to controll particles
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 */
//version specifyer needs to at least support compute shaders
#version 450 core
//a size of 64 is requred to execute the correct amount of particles
layout (local_size_x = 64, local_size_y = 1, local_size_z = 1) in;
//pi is later used to correctly distribute the points
#define PI 3.1415926535897932384

//GLGE adds in default includes that are marked with "<" and ended with ">". 
//this here includes particle utilitys and for this also includes a particle structure and the particle buffer array
//as well as the particle buffer structure
#include <glgeParticlesUtil>

//n is the seed for the random number generator. It can be anything but should be something 
//individual for each particle
uint n = gl_GlobalInvocationID.x * gl_GlobalInvocationID.x;
/**
 * @brief integer hash copied from Hugo Elias
 */
float hash() 
{   
	n = (n<<13U)^n; 
    n = n*(n*n*15731U+789221U)+1376312589U;
    return float(n&uvec3(0x0fffffffU))/float(0x0fffffff);
}

//this function updates one particle
void tickParticle(inout BuffParticle part)
{
    //this specifys the rotation speed
    float rotScale = 0.1;
    //change the rotation around the x axis by a small random amount
    part.rx += 0.05 * hash() * rotScale;
    //change the rotation around the y axis by a small random amount
    part.ry += 0.1 * hash() * rotScale;
    //change the rotation around the z axis by a small random amount
    part.rz += 0.075 * hash() * rotScale;
}

/**
 * @brief calculate a random position inside a disk
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
 * @brief calculate a random position inside a donut
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
 * @brief initalise a particle
 */
void initParticle(inout BuffParticle part)
{
    //calculate a random size for the particle in range of 0.2 to 2.2
    vec3 scale = vec3(hash() * 2.f + 0.2f);
    //set the scale for the particle
    part.sx = scale.x; part.sy = scale.y; part.sz = scale.z;

    //calculate a random position inside a donut
    vec3 pos = posInDonut(2000.f, 5000.f);
    //store the position
    part.px = pos.x;part.py = pos.y;part.pz = pos.z;
    //set the y position to a random positino in range -50 to 50
    part.py = hash() * 100.f - 50.f;

    //set the lifetime to 100
    part.lifetime = 100.f;
}

void main()
{
    //get the current particle buffer from the particle array
    BuffParticle part = glgeParticles[gl_GlobalInvocationID.x];
    //check if the particle should die
    if (part.lifetime < 1.f)
    {
        //if it should, re-initalise the particle
        initParticle(part);
    }
    else
    {
        //if not, update the particle
        tickParticle(part);
    }
    //re-compute all the matrices for the particle buffer
    glgeComputeParticleMatrices(part);
    //store the particle in the buffer array
    glgeParticles[gl_GlobalInvocationID.x] = part;
}