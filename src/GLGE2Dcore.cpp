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

///////////
//STRUCTS//
///////////

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