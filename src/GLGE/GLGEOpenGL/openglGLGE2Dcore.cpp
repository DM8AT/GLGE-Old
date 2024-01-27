/**
 * @file openglGLGE2Dcore.cpp
 * @author DM8AT
 * @brief define the functions for the 2D GLGE core
 * @version 0.1
 * @date 2024-01-02
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

//check if glew is allready included
#ifndef _GLGE_GLEW_
//say that glew is now included
#define _GLGE_GLEW_
//include glew
#include <GL/glew.h>
//close the if for glew
#endif

//include the main file
#include "openglGLGE2Dcore.h"
//include the GLGE dependencys
#include "openglGLGEDefines.hpp"
#include "../GLGEInternal/glgePrivDefines.hpp"
#include "openglGLGEVars.hpp"

//the needed default C++ libs
#include <math.h>
#include <iostream>

/////////////////////
// LOCAL VARIABLES //
/////////////////////

//store the main 2D camera
Camera2D* mainCam;
//store the default location of the move matrix
int glgeDefaultMoveMatLoc;

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

    //set the base 2D shader
    this->shader = glgeWindows[this->windowID]->getDefault2DShader();
    //set the move matrix location
    this->moveMatLoc = glgeDefaultMoveMatLoc;
    //get the UUID
    this->id = glgeObjectUUID;
    //increase the object count
    glgeObjectUUID++;

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

    //set the base 2D shader
    this->shader = glgeWindows[this->windowID]->getDefault2DShader();
    //set the move matrix location
    this->moveMatLoc = glgeDefaultMoveMatLoc;
    //get the UUID
    this->id = glgeObjectUUID;
    //increase the object count
    glgeObjectUUID++;

    //update the object
    this->update();
}

Object2D::Object2D(Mesh2D mesh, Transform2D transform, bool isStatic)
{
    //cast to another constructor
    *this = Object2D(mesh.vertices, mesh.indices, transform, isStatic);
}

Object2D::Object2D(unsigned int preset, unsigned int resolution, vec4 color, Transform2D transform, bool isStatic)
{
    //generate the mesh
    Mesh2D mesh = Mesh2D(preset, resolution, color);
    //cast to another constructor
    *this = Object2D(mesh, transform, isStatic);
}

void Object2D::draw()
{
    //check if this is the transparent pass
    if (glgeTransparentOpaquePass)
    {
        //break the draw call
        return;
    }
    //check if the window is the same it was created in
    if (! (this->windowID == glgeCurrentWindowIndex))
    {
        //if not, return
        return;
    }
    //disable depth testing
    glDisable(GL_DEPTH_TEST);
    //bind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);

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
    //pass the move matrix to the shader
    glUniformMatrix3fv(moveMatLoc, 1, GL_FALSE, &this->moveMat.m[0][0]);

    //activate the first texture unit
    glActiveTexture(GL_TEXTURE0);
    //bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);

    //draw the object
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
    //store the new mesh
    this->mesh = mesh;

    //check if the mesh was initalised
    if (this->windowID == -1)
    {
        //create the buffers
        this->createBuffers();
        //stop this function
        return;
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

void Object2D::setShader(unsigned int shader)
{
    //store the inputed shader
    this->shader = shader;
    
    //get the new location for the move matrix
    this->moveMatLoc = glgeGetUniformVar(shader, glgeCamMatrix);
}

void Object2D::setShader(std::string vs, std::string fs)
{
    //compile the shader source code and store the shader
    this->shader = glgeCompileShader(vs, fs);
    
    //get the new location for the move matrix
    this->moveMatLoc = glgeGetUniformVar(shader, glgeCamMatrix);
}

unsigned int Object2D::getShader()
{
    //return the shader
    return this->shader;    
}

void Object2D::setTexture(const char* file)
{
    //compile and store the texture
    this->texture = glgeTextureFromFile(file);
}

void Object2D::setTexture(unsigned int texture)
{
    //store the inputed texture
    this->texture = texture;
}

void Object2D::deleteTexture()
{
    //delete the stored texture
    glDeleteTextures(1, &this->texture);
    //set the stored texture to 0
    this->texture = 0;
}

unsigned int Object2D::getTexture()
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

void Object2D::setAnchor(vec2 anchor)
{
    //store the inputed position
    this->anchor = anchor;
}

void Object2D::setAnchor(float x, float y)
{
    //store the inputs as an vec2
    this->anchor = vec2(x,y);
}

vec2 Object2D::getAnchor()
{
    //return the anchor
    return this->anchor;
}

void Object2D::setStatic(bool stat)
{
    this->isStatic = stat;
}

bool Object2D::getStatic()
{
    return this->isStatic;
}

void Object2D::createBuffers()
{
    //check if the window ID is -1
    if (this->windowID == -1)
    {
        //then store the current window ID in it
        this->windowID = glgeCurrentWindowIndex;
    }
    else
    {
        //check if the IDs are not the same
        if (! (this->windowID == glgeCurrentWindowIndex))
        {
            //if they are not the same, check if a warning should print
            if (glgeWarningOutput)
            {
                //print an warning
                printf("[GLGE WARNING] tried to setup an allready setup object in a different window\n");
            }
            //stop the function
            return;
        }
    }
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
    //check if the IDs are not the same
    if (! (this->windowID == glgeCurrentWindowIndex))
    {
        //if they are not the same, check if a warning should print
        if (glgeWarningOutput)
        {
            //print an warning
            printf("[GLGE WARNING] tried to update an object in a different window\n");
        }
        //stop the function
        return;
    }
    //check if an old buffer was loaded
    if (this->VBO != 0)
    {
        //delete the old buffer
        glDeleteBuffers(this->VBOLen, &this->VBO);
    }
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
    //check if the IDs are not the same
    if (! (this->windowID == glgeCurrentWindowIndex))
    {
        //if they are not the same, check if a warning should print
        if (glgeWarningOutput)
        {
            //print an warning
            printf("[GLGE WARNING] tried to update an object in a different window\n");
        }
        //stop the function
        return;
    }
    //check if an old buffer was loaded
    if (this->IBO != 0)
    {
        //delete the old buffer
        glDeleteBuffers(this->IBOLen, &this->IBO);
    }
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
    this->moveMatLoc = glgeGetUniformVar(this->shader, glgeCamMatrix);
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
    //correct the size of the object depending on the largest side of the window
    this->moveMat = mat3(1.f/glgeWindows[glgeCurrentWindowIndex]->getWindowAspect(), 0, 0,
                         0,                                                          1.f, 0,
                         0,                                                          0, 1.f) * this->moveMat;
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
    this->camMat = (rotMat * scaMat * movMat);
}

////////////
// BUTTON //
////////////

Button::Button()
{
    //say that the object exists
}

Button::Button(const char* texture, Transform2D transf)
{
    //get the size of the texture
    vec2 size = glgeGetTextureSize(texture);
    //calculate the normalized size
    if (size.x > size.y)
    {
        //divide by the x component
        size /= vec2(size.x);
    }
    else
    {
        //divide by the y component
        size /= vec2(size.y);
    }
    //cast to another constuctor
    *this = Button(size, vec4(0), transf);
    //store the texture
    this->setTexture(texture);
}

Button::Button(unsigned int width, unsigned int height, vec4 color, Transform2D transf)
{
    //cast to another constructor
    *this = Button(vec2(width, height), color, transf);
}

Button::Button(vec2 size, vec4 color, Transform2D transf)
{
    //calculate the half size (for performance)
    vec2 shalf = size / vec2(2);
    //generate the vertices
    std::vector<Vertex2D> vert = {Vertex2D( shalf.x, shalf.y, color), Vertex2D( shalf.x,-shalf.y, color),
                                  Vertex2D(-shalf.x, shalf.y, color), Vertex2D(-shalf.x,-shalf.y, color)};
    //load the texture coordinates
    vert[0].texCoord = vec2(1,0);
    vert[1].texCoord = vec2(1,1);
    vert[2].texCoord = vec2(0,0);
    vert[3].texCoord = vec2(0,1);
    //generate the indices
    std::vector<unsigned int> inds = {1,0,2, 1,2,3};

    //calculate the mesh
    this->mesh = Mesh2D(vert, inds);
    //save the transform
    this->transf = transf;
    //say that the object is static
    this->isStatic = true;

    //create the buffers
    this->createBuffers();

    //set the base 2D shader
    this->shader = glgeWindows[this->windowID]->getDefault2DShader();
    //set the move matrix location
    this->moveMatLoc = glgeDefaultMoveMatLoc;
    //get the UUID
    this->id = glgeObjectUUID;
    //increase the object count
    glgeObjectUUID++;
    //store that this is a circle
    this->isCircle = false;
    //store the width and height
    this->size = size;

    //update the object
    this->update();
}

Button::Button(float radius, unsigned int res, vec4 color, Transform2D transf)
{
    //generate the mesh from an preset
    Mesh2D mesh = Mesh2D(GLGE_PRESET_CIRCLE, res, color);
    //scale according to the radius
    for (int i = 0; i < (int)mesh.vertices.size(); i++)
    {
        //scale the position
        mesh.vertices[i].pos.scale(radius);
    }
    
    //store the mesh
    this->mesh = mesh;
    //save the transform
    this->transf = transf;
    //say that the object is static
    this->isStatic = true;

    //create the buffers
    this->createBuffers();

    //set the base 2D shader
    this->shader = glgeWindows[this->windowID]->getDefault2DShader();
    //set the move matrix location
    this->moveMatLoc = glgeDefaultMoveMatLoc;
    //get the UUID
    this->id = glgeObjectUUID;
    //increase the object count
    glgeObjectUUID++;
    //store that this is a circle
    this->isCircle = true;
    //store the radius
    this->size = vec2(radius);

    //update the object
    this->update();
}

void Button::update()
{
    //recalculate the move matrix
    this->recalculateMoveMatrix();

    //check if the mouse is inside the button
    
    //button can't be pressed if the window isn't focused
    if (!glgeWindows[this->windowID]->isFocused())
    {
        //say that the button isn't focused nor pressed
        this->clickLastTick = false;
        this->hoverLastTick = false;
        //stop the funcion
        return;
    }
    //store the mouse position
    vec2 p = (vec2(glgeMouse.pos.x, 1.f - glgeMouse.pos.y) / vec2(0.5)) - vec2(1);
    //apply the window aspect ratio to the mouse position
    p.x *= glgeWindows[this->windowID]->getWindowAspect();
    //check if the button is a circle
    if (this->isCircle)
    {
        //calculate the mouse position in object space
        p -= this->transf.pos;
        //check if the length is shorter than the radius
        if (p.length() <= (this->size.x * this->transf.size.x))
        {
            //call the onHover function

            //check for a click
            if (glgeMouse.leftButton)
            {
                //set the click start this tick to the inverse of click last tick
                this->clickThis = !this->clickLastTick;
                //say that the button was clicked this tick
                this->clickLastTick = true;
                //say that the click can't stop this tick
                this->clickStopThis = false;
            }
            else
            {
                //say that the click can't be this tick
                this->clickThis = false;
                //set the click end this tick to click last tick
                this->clickStopThis = this->clickLastTick;
                //say that the button was not clicked this tick
                this->clickLastTick = false;
            }

            //say that the hovering can't stop this tick
            this->hoveringStopedThis = false;
            //set the hovering start to the inverse of the hover last tick
            this->hoverThis = !this->hoverLastTick;
            //say that the button was hoverd this tick
            this->hoverLastTick = true;
        }
        else
        {
            //say that the button can't be hoverd this tick
            this->hoverThis = false;
            //set the button hover stop to the hover last tick
            this->hoveringStopedThis = this->hoverLastTick;
            //say that the button was not hoverd
            this->hoverLastTick = false;
            //say that the button cant be clicked
            this->clickThis = false; this->clickLastTick = false; this->clickStopThis = false;
        }
    }
    else
    {
        //calculate the half size
        vec2 hsize = (this->size.scale(this->transf.size)) / vec2(2);
        //calculat the mouse position in object space
        p -= this->transf.pos;
        //check if the mouse is hovering the button
        if ((p >= vec2(0)-hsize) && (p <= hsize))
        {
            //call the onHover function

            //check for a click
            if (glgeMouse.leftButton)
            {
                //set the click start this tick to the inverse of click last tick
                this->clickThis = !this->clickLastTick;
                //say that the button was clicked this tick
                this->clickLastTick = true;
                //say that the click can't stop this tick
                this->clickStopThis = false;
            }
            else
            {
                //say that the click can't be this tick
                this->clickThis = false;
                //set the click end this tick to click last tick
                this->clickStopThis = this->clickLastTick;
                //say that the button was not clicked this tick
                this->clickLastTick = false;
            }

            //say that the hovering can't stop this tick
            this->hoveringStopedThis = false;
            //set the hovering start to the inverse of the hover last tick
            this->hoverThis = !this->hoverLastTick;
            //say that the button was hoverd this tick
            this->hoverLastTick = true;
        }
        else
        {
            //say that the button can't be hoverd this tick
            this->hoverThis = false;
            //set the button hover stop to the hover last tick
            this->hoveringStopedThis = this->hoverLastTick;
            //say that the button was not hoverd
            this->hoverLastTick = false;
            //say that the button cant be clicked
            this->clickThis = false; this->clickLastTick = false; this->clickStopThis = false;
        }
    }
}

bool Button::isClicked()
{
    return this->clickLastTick;
}

bool Button::isHoverd()
{
    return this->hoverLastTick;
}

bool Button::clickThisTick()
{
    return this->clickThis;
}

bool Button::hoverThisTick()
{
    return this->hoverThis;
}

bool Button::clickStopThisTick()
{
    return this->clickStopThis;
}

bool Button::hoverStopThisTick()
{
    return this->hoveringStopedThis;
}

/////////////
//FUNCTIONS//
/////////////

void glgeInit2DCore()
{
    //check if GLGE has a window
    if (!glgeHasMainWindow)
    {
        //if not, check if errors should be printed
        if (glgeErrorOutput)
        {
            //if yes, print an error
            std::cerr << "[GLGE ERROR] initalising a GLGE core module before opening a window dosn't work\n";
        }
        //check if glge should stop upon an error
        if (glgeExitOnError)
        {
            //stop the program
            exit(1);
        }
        //else, stop the function
        return;
    }
    //get the move matrix location from the default shader
    glgeDefaultMoveMatLoc = glgeGetUniformVar(glgeWindows[glgeMainWindowIndex]->getDefault2DShader(), glgeCamMatrix);
}

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
