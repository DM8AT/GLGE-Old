/**
 * @file glge2DcoreDefClasses.cpp
 * @author DM8AT
 * @brief declare the graphics api independend functions for the 2D core
 * @version 0.1
 * @date 2024-01-23
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license.
 * 
 */

//include the base
#include "glge2DcoreDefClasses.h"

//include glge dependencys
#include "glgePrivDefines.hpp"
#include "glgeVars.hpp"

//the needed default C++ libs
#include <math.h>
#include <iostream>

///////////
//STRUCTS//
///////////

//TRANSFORM

//default constructor
Transform2D::Transform2D()
{
    //init the object
}

//constructor with vector
Transform2D::Transform2D(vec2 pos, float rot, vec2 scale)
{
    //save the inputed attributes
    this->pos = pos;
    this->rot = rot*GLGE_TO_RADIANS;
    this->size = scale;
}

//constructor with floats
Transform2D::Transform2D(float x, float y, float rot, vec2 scale)
{
    //save the inputed attributes and convert the floats to an vector
    this->pos = vec2(x,y);
    this->rot = rot*GLGE_TO_RADIANS;
    this->size = scale;
}

mat3 Transform2D::getMatrix()
{
    //move everything to the position
    mat3 moveMat(1,0,this->pos.x,
                 0,1,this->pos.y,
                 0,0,1);

    //rotate everything correctly
    mat3 rotaMat(std::cos(rot),-std::sin(rot),0,
                 std::sin(rot), std::cos(rot),0,
                 0,             0,            1);

    //scale everything correctly
    mat3 sizeMat(this->size.x, 0, 0,
                 0, this->size.y, 0,
                 0, 0,            1);

    //return the multiplied matrices
    return (moveMat * sizeMat) * rotaMat;
}

//VERTEX2D

//default constructor
Vertex2D::Vertex2D()
{
    //init the object
}

//constructor with vec
Vertex2D::Vertex2D(vec2 p, vec4 color)
{
    //set the position to the input
    this->pos = p;
    //store the inputed color
    this->color = color;
}

//constructor using floats
Vertex2D::Vertex2D(float x, float y, vec4 color)
{
    //set the position to the inputs
    this->pos = vec2(x,y);
    //store the inputed color
    this->color = color;
}

//vertex with a lot of arguments
Vertex2D::Vertex2D(float x, float y, float r, float g, float b, float a)
{
    //set the position to the inputs
    this->pos = vec2(x,y);
    //store the inputed color
    this->color = vec4(r,g,b,a);
}

//vertex with a lot of arguments and an position
Vertex2D::Vertex2D(vec2 pos, float r, float g, float b, float a)
{
    //set the position to the inputs
    this->pos = pos;
    //store the inputed color
    this->color = vec4(r,g,b,a);
}

//constructor with vec
Vertex2D::Vertex2D(vec2 p, vec2 texCoord)
{
    //set the position to the input
    this->pos = p;
    //store the inputed texture coordinate
    this->texCoord = texCoord;
}

//constructor using floats
Vertex2D::Vertex2D(float x, float y, vec2 texCoord)
{
    //set the position to the inputs
    this->pos = vec2(x,y);
    //store the inputed texture coordinate
    this->texCoord = texCoord;
}

//constructor with vector and floates
Vertex2D::Vertex2D(vec2 pos, float tX, float tY)
{
    //store the inputed position
    this->pos = pos;
    //store the inputed texture coordinate
    this->texCoord = vec2(tX,tY);
}

//constructor with a lot of floates
Vertex2D::Vertex2D(float x, float y, float tX, float tY)
{
    //store the inputed position
    this->pos = vec2(x,y);
    //store the inputed texture coordinate
    this->texCoord = vec2(tX,tY);
}
