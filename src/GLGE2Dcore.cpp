/**
 * @file GLGE2Dcore.cpp
 * @author DM8AT
 * @brief the defines for functions declared in GLGE2Dcore.h
 * @version 0.1
 * @date 2023-01-29
 * 
 * @copyright CC BY (see  https://creativecommons.org/licenses/by/4.0/)
 * 
 */

//include the main file
#include "GLGE2Dcore.h"
//include the GLGE dependencys
#include "GLGE/GLGEDefines.hpp"
#include "GLGE/glgePrivDefines.hpp"
#include "GLGE/glgeVars.hpp"

//the needed default C++ libs
#include <math.h>
#include <iostream>

//////////////
//Local Vars//
//////////////

Camera2D* mainCam = NULL;

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

    //update the object
    this->update();
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

    //update the object
    this->update();
}

void Object2D::draw()
{
    //bind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    //bind the shader
    glUseProgram(this->shader);

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

    //pass the move matrix to the shader
    glUniformMatrix3fv(moveMatLoc, 1, GL_FALSE, &this->moveMat.m[0][0]);

    //bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawElements(GL_TRIANGLES, this->mesh.indices.size()*2.f, GL_UNSIGNED_INT, 0);

    //unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //deactivate the sub elements
    //deactivate the position argument
    glDisableVertexAttribArray(0);
    //deactivate the color argument
    glDisableVertexAttribArray(1);
    //deactivate the texCoord argument
    glDisableVertexAttribArray(2);

    //unbind the shader
    glUseProgram(0);
}

//update the object
void Object2D::update()
{
    //update the move matrix
    this->recalculateMoveMatrix();
}

void Object2D::recalculateVertexBuffer(Mesh2D mesh)
{
    //check if a new mesh should be asigned
    if ((mesh.indices.size() != 0) || (mesh.vertices.size() != 0))
    {
        this->mesh = mesh;
    }

    //recalculate the vertex buffer
    this->updateVertexBuffer();
}

void Object2D::recalculateIndexBuffer(Mesh2D mesh)
{
    //check if a new mesh should be asigned
    if ((mesh.indices.size() != 0) || (mesh.vertices.size() != 0))
    {
        this->mesh = mesh;
    }

    //recalculate the vertex buffer
    this->updateIndexBuffer();
}

void Object2D::recalculateMeshBuffer(Mesh2D mesh)
{
    //check if a new mesh should be asigned
    if ((mesh.indices.size() != 0) || (mesh.vertices.size() != 0))
    {
        this->mesh = mesh;
    }

    //recalculate all buffers
    this->updateVertexBuffer();
    this->updateIndexBuffer();
}

void Object2D::setMesh(Mesh2D mesh)
{
    //store the inputed mesh
    this->mesh = mesh;
}

Mesh2D Object2D::getMesh()
{
    //return the mesh
    return this->mesh;
}

void Object2D::setShader(const char* p)
{
    //save the inputed path in an string
    std::string path = p;

    //add the suffixes
    std::string vs = p+std::string(".vs");
    std::string fs = p+std::string(".fs");

    //do the shader setup
    this->shaderSetup(vs.c_str(), fs.c_str());
}

void Object2D::setShader(GLuint shader)
{
    //store the inputed shader
    this->shader = shader;
    
    //get the new location for the move matrix
    this->moveMatLoc = glgeGetUniformVar(shader, glgeMoveMatrix);
}

GLuint Object2D::getShader()
{
    //return the shader
    return this->shader;    
}

void Object2D::setTexture(const char* file)
{
    //compile and store the texture
    this->texture = glgeTextureFromFile(file);
}

void Object2D::setTexture(GLuint texture)
{
    //store the inputed texture
    this->texture = texture;
}

GLuint Object2D::getTexture()
{
    //return the stored texture
    return this->texture;
}

void Object2D::setTransform(Transform2D transform)
{
    //store the inputed argument
    this->transf = transform;
}

Transform2D Object2D::getTransform()
{
    //store the inputed argument
    return this->transf;
}

void Object2D::move(vec2 deltaPos)
{
    //change the position
    this->transf.pos += deltaPos;
}

void Object2D::move(float deltaX, float deltaY)
{
    //change the position
    this->transf.pos += vec2(deltaX, deltaY);
}

void Object2D::move(float speedX, float speedY, float dir)
{
    //change the position
    this->transf.pos += vec2(speedX*std::sin(dir*GLGE_TO_RADIANS), speedY*std::cos(dir*GLGE_TO_RADIANS));
}

void Object2D::move(float speed)
{
    //change the position
    this->transf.pos += vec2(speed*std::sin(this->transf.rot*GLGE_TO_RADIANS), speed*std::cos(this->transf.rot*GLGE_TO_RADIANS));
}

void Object2D::setPos(vec2 pos)
{
    //set the position to the input
    this->transf.pos = pos;
}

void Object2D::setPos(float x, float y)
{
    //set the position to the input
    this->transf.pos = vec2(x,y);
}

vec2 Object2D::getPos()
{
    //return the position
    return this->transf.pos;
}

void Object2D::rotate(float angle)
{
    //change the rotation of the object
    this->transf.rot += (angle*GLGE_TO_RADIANS);
}

void Object2D::setRotation(float dir)
{
    //set the rotation of the object
    this->transf.rot = dir*GLGE_TO_RADIANS;
}

float Object2D::getRotation()
{
    //return the rotation in degrees
    return this->transf.rot * GLGE_TO_DEGREES;
}

void Object2D::scale(vec2 scale)
{
    //scale the object
    this->transf.size.scale(scale);
}

void Object2D::scale(float scaleX, float scaleY)
{
    //scale the object
    this->transf.size.scale(vec2(scaleX,scaleY));
}

void Object2D::scaleAdd(vec2 scale)
{
    //add the input to the size
    this->transf.size += scale;
}

void Object2D::scaleAdd(float scaleX, float scaleY)
{
    //add the input to the size
    this->transf.size += vec2(scaleX, scaleY);
}

void Object2D::setScale(vec2 scale)
{
    //set the size of the object
    this->transf.size = scale;
}

void Object2D::setScale(float x, float y)
{
    //set the size of the object
    this->transf.size = vec2(x,y);
}

vec2 Object2D::getScale()
{
    //return the scale
    return this->transf.size;
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

    //save the length of the Vertex Buffer
    this->IBOLen = mesh.vertices.size();

    //generate the index buffer
    glGenBuffers(1, &this->IBO);
    //bind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
    //store the index information in the index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.indices[0])*((int)mesh.indices.size()), &(mesh.indices[0]), GL_STATIC_DRAW);
    //unbind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //save the length of the Index Buffer
    this->IBOLen = mesh.indices.size();
}

void Object2D::updateVertexBuffer()
{
    //delete the old buffer
    glDeleteBuffers(this->VBOLen, &this->VBO);
    //generate the vertex buffer for the object
    glGenBuffers(1, &this->VBO);
    //bind the vertex buffer object to store data
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    //store the mesh data in the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.vertices[0])*((int)mesh.vertices.size()), &(mesh.vertices[0]), GL_STATIC_DRAW);
    //unbind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //save the length of the Vertex Buffer
    this->IBOLen = mesh.vertices.size();
}

void Object2D::updateIndexBuffer()
{
    //delete the old buffer
    glDeleteBuffers(this->IBOLen, &this->IBO);
    //generate the index buffer
    glGenBuffers(1, &this->IBO);
    //bind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
    //store the index information in the index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.indices[0])*((int)mesh.indices.size()), &(mesh.indices[0]), GL_STATIC_DRAW);
    //unbind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //save the length of the Index Buffer
    this->IBOLen = mesh.indices.size();
}

void Object2D::shaderSetup(const char* vs, const char* fs)
{
    //compile and save the shader
    this->shader = glgeCompileShader(vs, fs);

    //save the location of the move matrix
    this->moveMatLoc = glgeGetUniformVar(this->shader, glgeMoveMatrix);
}

//move matrix update
void Object2D::recalculateMoveMatrix()
{
    //get the move matrix from the transform
    this->moveMat = this->transf.getMatrix();

    //if the object is not static, get the camera matrix, if it is available
    if (!this->isStatic && (mainCam != NULL))
    {
        this->moveMat = mainCam->getMatrix() * this->moveMat;
    }

    //fix matrix bug
    this->moveMat.m[2][2] = -1;
    //fix deformation
    this->moveMat = mat3(1/glgeWindowAspect,0,0,
                         0,                 1,0,
                         0,                 0,1) * moveMat;
}

//////////
//CAMERA//
//////////

//default constructor
Camera2D::Camera2D()
{
    //init the object
}

//cam from transform
Camera2D::Camera2D(Transform2D transform)
{
    //store the inputed transform
    this->transf = transform;
}

Camera2D::Camera2D(vec2 pos, float rot, vec2 size)
{
    //store the inputed values
    this->transf = Transform2D(pos, rot, size);
}

Camera2D::Camera2D(float x, float y, float rot, vec2 size)
{
    //store the inputed values
    this->transf = Transform2D(x,y, rot, size);
}

Camera2D::Camera2D(vec2 pos, float sX, float sY, float rot)
{
    //store the inputed values
    this->transf = Transform2D(pos, rot, vec2(sX,sY));
}

Camera2D::Camera2D(float x, float y, float sX, float sY, float rot)
{
    //store the inputed values
    this->transf = Transform2D(x,y, rot, vec2(sX,sY));
}

void Camera2D::update()
{
    //recaluclate the camera matrix
    this->recalculateMatrix();
}

mat3 Camera2D::getMatrix()
{
    //return the current cam matrix
    return this->camMat;
}

void Camera2D::setTransform(Transform2D transform)
{
    //store the inputed argument
    this->transf = transform;
}

Transform2D Camera2D::getTransform()
{
    //store the inputed argument
    return this->transf;
}

void Camera2D::move(vec2 deltaPos)
{
    //change the position
    this->transf.pos += deltaPos;
}

void Camera2D::move(float deltaX, float deltaY)
{
    //change the position
    this->transf.pos += vec2(deltaX, deltaY);
}

void Camera2D::move(float speedX, float speedY, float dir)
{
    //change the position
    this->transf.pos += vec2(speedX*std::sin(dir*GLGE_TO_RADIANS), speedY*std::cos(dir*GLGE_TO_RADIANS));
}

void Camera2D::move(float speed)
{
    //change the position
    this->transf.pos += vec2(speed*std::sin(this->transf.rot*GLGE_TO_RADIANS), speed*std::cos(this->transf.rot*GLGE_TO_RADIANS));
}

void Camera2D::setPos(vec2 pos)
{
    //set the position to the input
    this->transf.pos = pos;
}

void Camera2D::setPos(float x, float y)
{
    //set the position to the input
    this->transf.pos = vec2(x,y);
}

vec2 Camera2D::getPos()
{
    //return the position
    return this->transf.pos;
}

void Camera2D::rotate(float angle)
{
    //change the rotation of the object
    this->transf.rot += (angle*GLGE_TO_RADIANS);
}

void Camera2D::setRotation(float dir)
{
    //set the rotation of the object
    this->transf.rot = dir*GLGE_TO_RADIANS;
}

float Camera2D::getRotation()
{
    //return the rotation in degrees
    return this->transf.rot * GLGE_TO_DEGREES;
}

void Camera2D::scale(vec2 scale)
{
    //scale the object
    this->transf.size.scale(scale);
}

void Camera2D::scale(float scaleX, float scaleY)
{
    //scale the object
    this->transf.size.scale(vec2(scaleX,scaleY));
}

void Camera2D::scaleAdd(vec2 scale)
{
    //add the input to the size
    this->transf.size += scale;
}

void Camera2D::scaleAdd(float scaleX, float scaleY)
{
    //add the input to the size
    this->transf.size += vec2(scaleX, scaleY);
}

void Camera2D::setScale(vec2 scale)
{
    //set the size of the object
    this->transf.size = scale;
}

void Camera2D::setScale(float x, float y)
{
    //set the size of the object
    this->transf.size = vec2(x,y);
}

vec2 Camera2D::getScale()
{
    //return the scale
    return this->transf.size;
}

void Camera2D::recalculateMatrix()
{
    //calculate the rotation matrix
    mat3 rotMat = mat3(std::cos(this->transf.rot),-std::sin(this->transf.rot),0,
                       std::sin(this->transf.rot), std::cos(this->transf.rot),0,
                       0,                          0,                         1);
    //calculate the scale matrix
    mat3 scaMat = mat3(this->transf.size.x,0,0,
                       0,this->transf.size.y,0,
                       0,0,1);

    //calculate the move matrix
    mat3 movMat = mat3(1,0,this->transf.pos.x,
                       0,1,this->transf.pos.y,
                       0,0,1);

    //set the cam mat to an product of the matrices
    this->camMat = rotMat * scaMat * movMat;
}

/////////////
//FUNCTIONS//
/////////////

void glgeBindMain2DCamera(Camera2D* camera)
{
    //store the inputed pointer
    mainCam = camera;
}

Camera2D* glgeGetMain2DCamera()
{
    //return the stored camera
    return mainCam;
}