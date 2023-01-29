/**
 * @file GLGE2Dcore.cpp
 * @author DM8AT
 * @brief the defines for functions declared in GLGE2Dcore.h
 * @version 0.1
 * @date 2023-01-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */

//include the main file
#include "GLGE2Dcore.h"
//include the GLGE dependencys
#include "GLGE/GLGEDefines.h"
#include "GLGE/glgePrivDefines.h"

//the needed default C++ libs
#include <math.h>

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
    return moveMat * sizeMat * rotaMat;
}

//VERTEX2D

//default constructor
Vertex2D::Vertex2D()
{
    //init the object
}

//constructor with vec
Vertex2D::Vertex2D(vec2 p)
{
    //set the position to the input
    this->pos = p;
}

//constructor using floats
Vertex2D::Vertex2D(float x, float y)
{
    //set the position to the inputs
    this->pos = vec2(x,y);
}

///////////
//CLASSES//
///////////

//MESH2D

//default constructor
Mesh2D::Mesh2D()
{
    //init the object
}

//constructor with pointer array
Mesh2D::Mesh2D(Vertex2D* vertices, Triangle* indices, unsigned int sizeOfVertices, unsigned int sizeOfIndices)
{
    //convert the pointer arrays to vectors and save them
    this->vertices = std::vector<Vertex2D>(vertices, vertices + sizeOfVertices);
    this->faces = std::vector<Triangle>(indices, indices + sizeOfIndices);
}

//constructor with vectors
Mesh2D::Mesh2D(std::vector<Vertex2D> vertices, std::vector<Triangle> indices)
{
    //save the inputed vectors
    this->vertices = vertices;
    this->faces = indices;
}