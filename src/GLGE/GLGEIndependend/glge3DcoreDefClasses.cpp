/**
 * @file glge3DcoreDefClasses.cpp
 * @author DM8AT
 * @brief define the graphics api independand classes for the 3D Core
 * @version 0.1
 * @date 2024-01-09
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license.
 */
#include "glge3DcoreDefClasses.h"

//include the GLGE dependencys
#include "glgeErrors.hpp"
#include "glgeVars.hpp"
#include "glgePrivDefines.hpp"
#include "glgeInternalFuncs.h"

//include needed CML librarys
#include "../CML/CMLQuaternion.h"

//include the default librarys
#include <iostream>
#include <cstring>
#include <math.h>
#include <sstream>
#include <map>
#include <algorithm>

/////////////
//TRANSFORM//
/////////////

//default constructor
Transform::Transform()
{
    //initalise the object, say that it exists
    this->scale = vec3(1,1,1);
}

//constructor using a position, rotation and a scale on 3 axis
Transform::Transform(vec3 pos, vec3 rot, vec3 scale)
{
    //set the values to the input
    this->pos = pos;
    //set the rotation to the rotation in radians
    this->rot.x = rot.x*GLGE_TO_RADIANS;
    this->rot.y = rot.y*GLGE_TO_RADIANS;
    this->rot.z = rot.z*GLGE_TO_RADIANS;
    this->scale = scale;
}

//constructor using a position, rotation and a uniform scale
Transform::Transform(vec3 pos, vec3 rot, float scale)
{
    //set position amd rotation to the inputs
    this->pos = pos;
    //set the rotation to the rotation in radians
    this->rot.x = rot.x*GLGE_TO_RADIANS;
    this->rot.y = rot.y*GLGE_TO_RADIANS;
    this->rot.z = rot.z*GLGE_TO_RADIANS;
    //set the scale to the input scale on all 3 axis
    this->scale = vec3(scale,scale,scale);
}

//get the transformation matrix for this point
mat4 Transform::getMatrix()
{
    //calculate the translation matrix
    mat4 transl = mat4(1, 0, 0, this->pos.x,
                       0, 1, 0, this->pos.y,
                       0, 0, 1, this->pos.z,
                       0, 0, 0, 1);
    
    //rotation on x axis
    mat4 rotXMat = mat4(1, 0,                     0,                     0,
                        0, std::cos(this->rot.x),-std::sin(this->rot.x), 0,
                        0, std::sin(this->rot.x), std::cos(this->rot.x), 0,
                        0, 0,                     0,                     1);

    //rotation on y axis
    mat4 rotYMat = mat4(std::cos(this->rot.y), 0, std::sin(this->rot.y), 0,
                        0,                     1,                     0, 0,
                       -std::sin(this->rot.y), 0, std::cos(this->rot.y), 0,
                        0,                     0,                     0, 1);

    //rotation on z axis
    mat4 rotZMat = mat4(std::cos(this->rot.z),-std::sin(this->rot.z), 0, 0,
                        std::sin(this->rot.z), std::cos(this->rot.z), 0, 0,
                        0,                     0,                     1, 0,
                        0,                     0,                     0, 1);

    //scaling matrix
    mat4 scaleMat = mat4(this->scale.x, 0, 0, 0,
                         0, this->scale.y, 0, 0,
                         0, 0, this->scale.z, 0,
                         0, 0, 0,             1);

    //return a multiplycation of the matrices
    return transl * scaleMat * ((rotXMat * rotYMat) * rotZMat);
}

mat4 Transform::getRotationMatrix()
{
    //pre-compute sine and cosine values
    float sx = std::sin(this->rot.x);
    float cx = std::cos(this->rot.x);
    float sy = std::sin(this->rot.y);
    float cy = std::cos(this->rot.y);
    float sz = std::sin(this->rot.z);
    float cz = std::cos(this->rot.z);
    //rotation on x axis
    mat4 rotXMat = mat4(1, 0, 0,   0,
                        0, cx,-sx, 0,
                        0, sx, cx, 0,
                        0, 0, 0,   1);

    //rotation on y axis
    mat4 rotYMat = mat4(cy, 0, sy, 0,
                        0,  1, 0,  0,
                       -sy, 0, cy, 0,
                        0,  0, 0,  1);

    //rotation on z axis
    mat4 rotZMat = mat4(cz,-sz, 0, 0,
                        sz, cz, 0, 0,
                        0,  0,  1, 0,
                        0,  0,  0, 1);

    //return the product of the matrices
    return (rotXMat * rotYMat) * rotZMat;
}

//////////
//Vertex//
//////////

//default constructor
Vertex::Vertex()
{
    //initalise the vertex
}

//constructor using vector
Vertex::Vertex(vec3 pos, vec4 color)
{
    //store the inputed position
    this->pos = pos;
    //set a white color
    this->color = color;
    //set the texture coordinate to 0,0
    this->texCoord = vec2(0,0);
}

//constructor using floates
Vertex::Vertex(float x, float y, float z, vec4 color)
{
    //store the inputed position
    this->pos = vec3(x,y,z);
    //set a white color
    this->color = color;
    //set the texture coordinate to 0,0
    this->texCoord = vec2(0,0);
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float a)
{
    //store the inputed position
    this->pos = vec3(x,y,z);
    //set a white color
    this->color = vec4(r,g,b,a);
    //set the texture coordinate to 0,0
    this->texCoord = vec2(0,0);
}

//constructor using vector
Vertex::Vertex(vec3 pos, vec2 texCoord)
{
    //store the inputed position
    this->pos = pos;
    //store the inputed tex coord
    this->texCoord = texCoord;
    //set the color transparent
    this->color = vec4(0,0,0,0);
}

//constructor using floates
Vertex::Vertex(float x, float y, float z, vec2 texCoord)
{
    //store the inputed position
    this->pos = vec3(x,y,z);
    //store the inputed tex coord
    this->texCoord = texCoord;
    //set the color transparent
    this->color = vec4(0,0,0,0);
}

Vertex::Vertex(float x, float y, float z, float tX, float tY)
{
    //store the inputed position
    this->pos = vec3(x,y,z);
    //store the inputed tex coord
    this->texCoord = vec2(tX, tY);
    //set the color transparent
    this->color = vec4(0,0,0,0);
}

Vertex::Vertex(vec3 p, vec3 n)
{
    //store the inputed values
    this->pos = p;
    this->normal = n;
}

Vertex::Vertex(vec3 p, vec2 t, vec3 n)
{
    //store the inputed values
    this->pos = p;
    this->texCoord = t;
    this->normal = n;
}

//a lot of floats
Vertex::Vertex(float x, float y, float z, float tx, float ty, float nx, float ny, float nz)
{
    //store the inputed position values
    this->pos = vec3(x,y,z);
    //store the inputed texture coordinats
    this->texCoord = vec2(tx, ty);
    //store and normalise the inputed normal
    this->normal = vec3(nx,ny,nz);
    this->normal.normalize();
}

//10 floats
Vertex::Vertex(float x, float y, float z, float r, float g, float b, float a, float nx, float ny, float nz)
{
    //store the inputed position values
    this->pos = vec3(x,y,z);
    //store the inputed color
    this->color = vec4(r,g,b,a);
    //store and normalise the inputed normal
    this->normal = vec3(nx,ny,nz);
    this->normal.normalize();
}

//6 floats + vec4
Vertex::Vertex(float x, float y, float z, vec4 color, float nx, float ny, float nz)
{
    //pass to another constructor
    *this = Vertex(x,y,z, color.x, color.y, color.z, color.w, nx,ny,nz);
}
