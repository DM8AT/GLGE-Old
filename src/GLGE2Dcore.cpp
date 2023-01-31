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
Mesh2D::Mesh2D(Vertex2D* vertices, unsigned int* indices, unsigned int sizeOfVertices, unsigned int sizeOfIndices)
{
    //convert the pointer arrays to vectors and save them
    this->vertices = std::vector<Vertex2D>(vertices, vertices + (sizeOfVertices/sizeof(vertices[0])));
    this->indices = std::vector<unsigned int>(indices, indices + (sizeOfIndices/sizeof(indices[0])));
}

//constructor with vectors
Mesh2D::Mesh2D(std::vector<Vertex2D> vertices, std::vector<unsigned int> indices)
{
    //save the inputed vectors
    this->vertices = vertices;
    this->indices = indices;
}

//OBJECT 2D

//default constructor
Object2D::Object2D()
{
    //init the object
}

//constructor using array pointers
Object2D::Object2D(Vertex2D* vertices, unsigned int* indices, unsigned int sizeOfVertices, unsigned int sizeOfFaces, Transform2D transform, bool isStatic)
{
    //create an mesh from the array pointers
    this->mesh = Mesh2D(vertices, indices, sizeOfVertices, sizeOfFaces);

    //save the transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;

    //create the buffers
    this->createBuffers();
}

//constructor using vectors
Object2D::Object2D(std::vector<Vertex2D> vertices, std::vector<unsigned int> indices, Transform2D transform, bool isStatic)
{
    //save an mesh created from the two vectors
    this->mesh = Mesh2D(vertices, indices);

    //save the transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;

    //create the buffers
    this->createBuffers();
}

void Object2D::draw()
{    
    //bind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    //activate sub elements
    //say where the position vector is
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), 0);
    //say where the color is
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(struct Vertex2D, color));
    //say where texture coordinates are
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(struct Vertex2D, texCoord));

    //bind the shader
    glUseProgram(this->shader);

    glDrawElements(GL_TRIANGLES, this->mesh.indices.size()*2.f, GL_UNSIGNED_INT, 0);

    //unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //unbind the shader
    glUseProgram(0);
}

void Object2D::setShader(const char* p)
{
    //save the inputed path in an string
    std::string path = p;

    //add the suffixes
    std::string vs = p+std::string(".vs");
    std::string fs = p+std::string(".fs");

    //compile and save the shader
    this->shader = glgeCompileShader(vs.c_str(), fs.c_str());
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
    glGenBuffers(1, &this->IBO);
    //bind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
    //store the index information in the index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.indices[0])*((int)mesh.indices.size()), &(mesh.indices[0]), GL_STATIC_DRAW);
    //unbind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}