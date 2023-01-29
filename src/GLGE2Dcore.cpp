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

//OBJECT 2D

//default constructor
Object2D::Object2D()
{
    //init the object
}

//constructor using array pointers
Object2D::Object2D(Vertex2D* vertices, Triangle* faces, unsigned int sizeOfVertices, unsigned int sizeOfFaces, Transform2D transform, bool isStatic)
{
    //create an mesh from the array pointers
    this->mesh = Mesh2D(vertices, faces, sizeOfVertices, sizeOfFaces);

    //save the transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;

    //create the buffers
    this->createBuffers();
}

//constructor using vectors
Object2D::Object2D(std::vector<Vertex2D> vertices, std::vector<Triangle> faces, Transform2D transform, bool isStatic)
{
    //save an mesh created from the two vectors
    this->mesh = Mesh2D(vertices, faces);

    //save the transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;

    //create the buffers
    this->createBuffers();
}

void Object2D::draw()
{
    
}

void Object2D::createBuffers()
{
    //generate the vertex buffer for the object
    glGenBuffers(1, &this->VBO);
    //bind the vertex buffer object to store data
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    //store the mesh data in the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.vertices[0])*((int)mesh.vertices.size()), &(mesh.vertices[0]), GL_STATIC_DRAW);
    //unbind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //generate the index buffer
    glGenBuffers(1, &this->FBO);
    //bind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->FBO);
    //store the index information in the index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.faces[0])*((int)mesh.faces.size()), &(mesh.faces[0]), GL_STATIC_DRAW);
    //unbind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}