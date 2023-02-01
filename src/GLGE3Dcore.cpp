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
Vertex::Vertex(vec3 pos)
{
    //store the inputed position
    this->pos = pos;
}

//constructor using floates
Vertex::Vertex(float x, float y, float z)
{
    //store the inputed position
    this->pos = vec3(x,y,z);
}

///////////
//CLASSES//
///////////

//default constructor
Face::Face()
{
    //say that it exists
}

//constructor using colors
Face::Face(unsigned int indices[3], vec4 colors[3], vec3 normal)
{
    //store the inputed indices
    this->indices[0] = indices[0];
    this->indices[1] = indices[1];
    this->indices[2] = indices[2];

    //store the inputed colors
    this->colors[0] = colors[0];
    this->colors[1] = colors[1];
    this->colors[2] = colors[2];
    //say that colors should be used
    this->useColor = 1.f;

    //store the inputed normal
    this->normal = normal;
}

//constructor using texcoords
Face::Face(unsigned int indices[3], vec2 texCoords[3], vec3 normal)
{
    //store the inputed indices
    this->indices[0] = indices[0];
    this->indices[1] = indices[1];
    this->indices[2] = indices[2];

    //store the inputed colors
    this->texCoords[0] = texCoords[0];
    this->texCoords[1] = texCoords[1];
    this->texCoords[2] = texCoords[2];
    //say that colors should be used
    this->useColor = 0.f;

    //store the inputed normal
    this->normal = normal;
}

//constructor using single numbers
Face::Face(unsigned int a, unsigned int b, unsigned int c, vec4 ca, vec4 cb, vec4 cc, vec3 normal)
{
    //store the inputed indices
    this->indices[0] = a;
    this->indices[1] = b;
    this->indices[2] = c;

    //store the inputed colors
    this->colors[0] = ca;
    this->colors[1] = cb;
    this->colors[2] = cc;
    //say that colors should be used
    this->useColor = 1.f;

    //store the inputed normal
    this->normal = normal;
}

//constructor using single numbers for a texture
Face::Face(unsigned int a, unsigned int b, unsigned int c, vec2 ta, vec2 tb, vec2 tc, vec3 normal)
{
    //store the inputed indices
    this->indices[0] = a;
    this->indices[1] = b;
    this->indices[2] = c;

    //store the inputed colors
    this->texCoords[0] = ta;
    this->texCoords[1] = tb;
    this->texCoords[2] = tc;
    //say that colors should be used
    this->useColor = 0.f;

    //store the inputed normal
    this->normal = normal;
}

////////
//MESH//
////////



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