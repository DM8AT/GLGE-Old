/**
 * @file GLGE3Dcore.cpp
 * @author DM8AT
 * @brief in this file, all files to use the 3D functions for GLGE are defined
 * @version 0.1
 * @date 2022-12-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

//include the 3d core
#include "GLGE3Dcore.h"

//include the GLGE dependencys
#include "GLGE/glgeErrors.hpp"
#include "GLGE/glgeVars.hpp"
#include "GLGE/glgeFuncs.hpp"
#include "GLGE/GLGEDefines.hpp"
#include "GLGE/glgePrivDefines.hpp"

//include needed CML librarys
#include "GLGE/CML/CMLQuaternion.h"

//include freeglut
#include <GL/freeglut.h>

//include the default librarys
#include <iostream>
#include <cstring>
#include <math.h>

/////////////////////
//PRIVATE FUNCTIONS//
/////////////////////

vec3 rotateVector(float angle, vec3 v, vec3 target)
{
    Quaternion RotationQ = Quaternion(angle, v);

    Quaternion ConjugateQ = RotationQ.conjugate();

    Quaternion W = (RotationQ * target) * ConjugateQ;

    return vec3(W.x,W.y,W.z);
}

//////////////////////
//PRIAVATE VAIRABLES//
//////////////////////

//store the camera GLGE is using
Camera* glgeMainCamera;

////////////////////////////
//Decalrations for Structs//
////////////////////////////

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

///////////
//CLASSES//
///////////

////////
//MESH//
////////

//default constructor
Mesh::Mesh()
{
    //init the object
}

//pointer array constructor
Mesh::Mesh(Vertex* vertices, unsigned int* indices, unsigned int sizeVertices, unsigned int sizeIndices)
{
    //convert the pointer arrays to vectors and save them
    this->vertices = std::vector<Vertex>(vertices, vertices + (sizeVertices/sizeof(vertices[0])));
    this->indices = std::vector<unsigned int>(indices, indices + (sizeIndices/sizeof(indices[0])));
}

//vector constructor
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    //save the inputed vectors
    this->vertices = vertices;
    this->indices = indices;
}

//////////
//OBJECT//
//////////

//default constructor
Object::Object()
{
    //init the object
}

//constructor using pointer
Object::Object(Vertex* vertices, unsigned int* indices, unsigned int sizeVertices, unsigned int sizeIndices, Transform transform, bool isStatic)
{
    //construct and save a mesh from the pointers
    this->mesh = Mesh(vertices, indices, sizeVertices, sizeIndices);

    //save the inputed transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;

    //calculate the buffers
    this->compileBuffers();
}

//constructor using vectors
Object::Object(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Transform transform, bool isStatic)
{
    //construct and save a mesh from the pointers
    this->mesh = Mesh(vertices, indices);

    //save the inputed transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;

    //calculate the buffers
    this->compileBuffers();
}

//draw the object
void Object::draw()
{
    //bind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    //bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);

    //bind the shader
    glUseProgram(this->shader);

    //activate sub elements
    //say where the position vector is
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, pos));
    //say where the color is
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, color));
    //say where the texCoords are
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, texCoord));
    
    //pass the move matrix to the shader
    glUniformMatrix4fv(moveMatLoc, 1, GL_FALSE, &this->moveMat.m[0][0]);

    glDrawElements(GL_TRIANGLES, this->mesh.indices.size(), GL_UNSIGNED_INT, 0);

    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //deactivate the sub elements
    //deactivate the position argument
    glDisableVertexAttribArray(0);
    //deactivate the color argument
    glDisableVertexAttribArray(1);
    //deactivate the texCoord argument
    glDisableVertexAttribArray(2);

    //unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //unbind the shader
    glUseProgram(0);
}

void Object::update()
{
    //recalculate the move matrix
    this->recalculateMoveMatrix();
}

void Object::setShader(const char* vs, const char* fs)
{
    //shader setup
    this->shaderSetup(vs, fs);
}

void Object::setShader(const char* p)
{
    //save the inputed path in an string
    std::string path = p;

    //add the suffixes
    std::string vs = p+std::string(".vs");
    std::string fs = p+std::string(".fs");

    //do the shader setup
    this->shaderSetup(vs.c_str(), fs.c_str());
}

void Object::setShader(GLuint shader)
{
    //store the inputed shader
    this->shader = shader;
    
    //get the new location for the move matrix
    this->moveMatLoc = glgeGetUniformVar(shader, glgeMoveMatrix);
}

void Object::setTexture(const char* texture)
{
    //load and save the texture
    this->texture = glgeTextureFromFile(texture);
}

void Object::setTexture(GLuint texture)
{
    //store the ipnuted texture
    this->texture = texture;
}

GLuint Object::getTexture()
{
    //return the texture
    return this->texture;
}

//PRIV
void Object::compileBuffers()
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

void Object::shaderSetup(const char* vs, const char* fs)
{
    //compile and save the shader
    this->shader = glgeCompileShader(vs, fs);

    //save the location of the move matrix
    this->moveMatLoc = glgeGetUniformVar(this->shader, glgeMoveMatrix);
}

void Object::recalculateMoveMatrix()
{
    //set the move matrix to the product of 3 matrices
    this->moveMat = glgeMainCamera->getProjectionMatrix() * glgeMainCamera->getViewMatrix() * this->transf.getMatrix(); 
}

//////////
//CAMERA//
//////////

//PUBLIC

//default constructor
Camera::Camera()
{
    //initalise the object
}

//initalise with parameters
Camera::Camera(float FOV, float near, double far, Transform transform)
{
    //check for errors
    bool error = false;

    //check if the FOV is 0 or less
    if (FOV <= 0)
    {
        //output an error
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_CAMERA_FOV_LESS_EQUAL_ZERO, FOV);
        }
        //say that an error occured
        error = true;
    }
    //check if the near cliping plane is 0 or less
    if (near <= 0)
    {
        //output an error
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_CAMERA_NEAR_LESS_EQUAL_ZERO, near);
        }
        //say that an error occured
        error = true;
    }
    //check if the far cliping plane is 0 or less
    if (far <= 0)
    {
        //output an error
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_CAMERA_FAR_LESS_EQUAL_ZERO, far);
        }
        //say that an error occured
        error = true;
    }
    //check if the far cliping plane is less or equal to the near cliping plane
    if (far <= near)
    {
        //output an error
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_CAMERA_FAR_LESS_EQUAL_NEAR);
        }
        //say that an error occured
        error = true;
    }

    //if an error occured
    if (error)
    {
        //output where the error occured
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_CAMERA_CAMERA << std::endl;
        }
        //stop the script
        exit(1);
    }

    //if no error occured, store the variables
    this->fov = FOV*GLGE_TO_RADIANS;
    this->near = near;
    this->far = far;
    this->transf = transform;
}

//update the view matrix
void Camera::update()
{
    //recalculate the view matrix
    this->viewMatrix = this->calculateViewMatrix();
}

//update the projection matrix
void Camera::recalculateProjection()
{
    //recalculate the projection matrix
    this->projectionMatrix = this->calculateProjectionMatrix();
}

//get the view matrix
mat4 Camera::getViewMatrix()
{
    //return the view matrix
    return this->viewMatrix;
}

//get the projection matrix
mat4 Camera::getProjectionMatrix()
{
    //return the projection matrix
    return this->projectionMatrix;
}

//movement stuff

//apply a new transform to the camera
void Camera::applyTransform(Transform t)
{
    //set the transform to the inputed transform
    this->transf = t;
}

//set the position of the camera
void Camera::set(vec3 p)
{
    //apply the new position
    this->transf.pos = p;
}

//set the position of the camera
void Camera::set(float x, float y, float z)
{
    //apply the new position
    this->transf.pos = vec3(x,y,z);
}

//move the camera
void Camera::move(vec3 v)
{
    //change the position of the camera
    this->transf.pos += v;
}

//move the camera
void Camera::move(float x, float y, float z)
{
    //change the position of the camera
    this->transf.pos += vec3(x,y,z);
}

//set the rotation of the camera
void Camera::setRotation(vec2 r)
{
    //apply the new rotation
    this->transf.rot = vec3(r.x,r.y,0);
}

//set the rotation of the camera
void Camera::setRotation(float x, float y)
{
    //apply the new rotation
    this->transf.rot = vec3(x,y,0);
}

//rotate the camera
void Camera::rotate(vec2 r)
{
    //rotate the camera
    this->transf.rot += vec3(r.x,r.y,0);
}

//rotate the camera
void Camera::rotate(float x, float y)
{
    //rotate the camera
    this->transf.rot += vec3(x,y,0);
}

//set the field of view from the camera
void Camera::setFOV(float f)
{
    //apply the new fov
    this->fov = f*GLGE_TO_RADIANS;
}

//change the field of view
void Camera::sizeFOV(float df)
{
    //change the fov
    this->fov += (df*GLGE_TO_RADIANS);
}

//change the fov


//PRIVATE

mat4 Camera::calculateViewMatrix()
{
    //store a vector for the y axis
    vec3 yaxis = vec3(up.x,up.y,up.z);
    //start rotation things
    //calculate the view vector
    //setup the vector to face right
    vec3 view = vec3(right.x,right.y,right.z);
    //rotathe the vector around the y axis by the x rotation amount
    view = rotateVector(transf.rot.x,yaxis, view);
    //normaise the vector
    view.normalize();

    //rotate the view vector and calculate U
    //calculate the cross product of the yaxis and the view vector
    vec3 U = view.cross(yaxis);
    //normalise the U vector
    U.normalize();
    //rotate the view vector around U by the rotation on the y axis
    view = rotateVector(transf.rot.y, U, view);
    
    //setup the vectors
    //N is equal to the view vector
    vec3 N = view;
    //noralise N
    N.normalize();

    //V is the cross product of U and N. Both are normalised, so no normalistion needed
    vec3 V = U.cross(N);

    //setup the view matrix
    //IMPORTANT: the vectors are in reversd order!
    mat4 vRotMat = mat4(U.z,U.y,U.x,0,
                        V.z,V.y,V.x,0,
                        N.z,N.y,N.x,0,
                        0,  0,  0,  1);

    mat4 vTraMat = mat4(1,0,0,-transf.pos.x,
                        0,1,0,-transf.pos.y,
                        0,0,1,-transf.pos.z,
                        0,0,0,1);

    mat4 viewMat = vRotMat * vTraMat;

    //return the matrix
    return viewMat;
}

mat4 Camera::calculateProjectionMatrix()
{
    float ar = glgeWindowAspect;
    float zRange = far - near;
    float tHF = std::tan((fov/2.f));

    return mat4(1.f/(tHF*ar), 0.f, 0.f, 0.f,
                0.f, 1/tHF, 0.f, 0.f,
                0.f, 0.f, (-near - far)/zRange, (2.f*far * near) / zRange,
                0.f, 0.f, 1.f, 0.f);
}

/////////////
//FUNCTIONS//
/////////////

//bind a camera
void glgeBindCamera(Camera* camera)
{
    //check if the inputed parameter is valide
    if (camera == nullptr)
    {
        //output an error
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_INPUT_NULLPOINTER);
            //say where the error occured
            std::cerr << GLGE_ERROR_STR_BIND_MAIN_CAMERA << std::endl;
        }
        //stop the program
        exit(1);
    }
    //else bind the camera
    glgeMainCamera = camera;
}

//initalise for 3D
void glgeInit3DCore()
{
    //set the depth test correctly
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}