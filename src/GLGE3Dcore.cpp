/**
 * @file GLGE3Dcore.cpp
 * @author DM8AT
 * @brief in this file, all files to use the 3D functions for GLGE are defined
 * @version 0.1
 * @date 2022-12-23
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
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
#include <sstream>
#include <map>

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

bool has_double_slash(std::string &str)
{
    //get the length of the stringt and decreese it by 2
    int length = str.length() - 2;
    //loop over every element in the string, but not the last one, since the length is decreesed by two instead of one
    for (int i = 0; i < length; i++)
    {
        //if the item i and the next item are boath a slash, return true
        if (str[i] == '/' && str[i + 1] == '/')
            return true;
    }
    //at this point, the loop hasn't return true, so return false
    return false;
}

//////////////////////
//PRIAVATE VAIRABLES//
//////////////////////

//store the camera GLGE is using
Camera* glgeMainCamera;

//store the default 3D shader
GLuint glgeShaderDefault;
//the position of the shader 
GLint glgeMoveMatDefaulLoc;

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

Mesh::Mesh(std::string data, int type)
{
    if (type != GLGE_OBJ)
    {
        std::cerr << "Currently, only .obj files are supported" << std::endl;
        return;
    }

    //seperate the data into the lines
    std::istringstream iss(data);

    //create an std::vector of 3D vectors for the vertices
    std::vector<vec3> verts;
    //create an vector to store the texture coordinates
    std::vector<vec2> tex;
    //create an vector to store the vertex normals
    std::vector<vec3> normals;

    //create an dictionary to store all created vectors
    std::map<std::string, std::pair<Vertex, unsigned int>> cverts;

    //loop over every line in the data
    for (std::string line; std::getline(iss, line); )
    {
        //check if the line starts with an v (v stands for Vertex)
        if (line[0] == 'v')
        {
            //if the line starts only with an v, it stores vertex position coordinates
            if (line[1] == ' ')
            {
                //create three floats to store the position
                float x,y,z;
                //load the position from the string into the floats
                sscanf(line.c_str(), "v %f %f %f", &x,&y,&z);
                //store the float as an new vertex
                verts.push_back(vec3(x,y,z));
            }
            //if the line starts with an vt it stores vertex texture coordinates
            else if (line[1] == 't')
            {
                //create two floats to store the texture coordinats
                float x,y;
                //load the texture coordinats from the line
                sscanf(line.c_str(), "vt %f %f", &x, &y);
                //store the texture coordinate
                tex.push_back(vec2(x,y));
            }
            //if the line starts with an vn, it stores vertex normals
            else if (line[1] == 'n')
            {
                //create three floats to store the normal
                float nx, ny, nz;
                //load the data form the line
                sscanf(line.c_str(), "vn %f %f %f", &nx, &ny, &nz);
                //store the normal
                normals.push_back(vec3(nx,ny,nz));
            }
        }
        //if the line starts with an f, it is a face. 
        else if (line[0] == 'f')
        {
            //count the amount of spaces in the line and save it in edge
            int edge = count_char(line, ' ');
            //if the edge builds no triangle, step to the next face
            if (edge != 3)
            {
                //jump to the next loop iteration
                continue;
            }
            //count the amount of slashes in the line and save it in count_slash
            int count_slash = count_char(line, '/');
            //if there are no slashes in th eline
            if (count_slash == 0)
            {
                //create 3 ints to store the face conections
                int m[3];
                //read the conections from the file
                sscanf(line.c_str(), "f %d %d %d", &m[0], &m[1], &m[2]);
                //create the three vertices
                for (int i = 0; i < 3; i++)
                {
                    //check if the vertex that should be created allready exists
                    if (cverts.count(std::to_string(m[i])))
                    {
                        //if the vertex allready exists, store the value in the face
                        this->indices.push_back(cverts[std::to_string(m[i])].second);
                    }
                    else
                    {
                        //if the vertex does not exist, create the vertex
                        this->vertices.push_back(Vertex(verts[m[i]]));
                        //store that the vertex exists
                        cverts[std::to_string(m[i])].first = Vertex(verts[m[i]]);
                        cverts[std::to_string(m[i])].second = (unsigned int)this->indices.size()-1;
                        //store the position of the vertex in the face
                        this->indices.push_back(cverts[std::to_string(m[i])].second);
                    }
                }
            }
            //else if there are equal slashes and spaces in the line
            else if (count_slash == edge)
            {
                //create 3 ints
                int v[3];
                //create 3 more ints
                int t[3];
                //read in the line using the 6 integers
                sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &v[0], &t[0], &v[1], &t[1], &v[2], &t[2]);
                //create the three vertices
                for (int i = 0; i < 3; i++)
                {
                    //check if the vertex that should be created allready exists
                    if (cverts.count(std::to_string(v[i])+std::string(",")+std::to_string(t[i])))
                    {
                        //if the vertex allready exists, store the value in the face
                        this->indices.push_back(cverts[std::to_string(v[i])+std::string("v,")+std::to_string(t[i])].second);
                    }
                    else
                    {
                        //if the vertex does not exist, create the vertex
                        this->vertices.push_back(Vertex(verts[v[i]], tex[t[i]]));
                        //store that the vertex exists
                        cverts[std::to_string(v[i])+std::string("v,")+std::to_string(t[i])].first = Vertex(verts[v[i]], tex[t[i]]);
                        cverts[std::to_string(v[i])+std::string("v,")+std::to_string(t[i])].second = (unsigned int)this->vertices.size()-1;
                        //store the position of the vertex in the face
                        this->indices.push_back(cverts[std::to_string(v[i])+std::string("v,")+std::to_string(t[i])].second);
                    }
                }
            }
            //else if the count of slashes is the double of the edge variable
            else if (count_slash == edge * 2)
            {
                //if has_double_slash return true
                if (has_double_slash(line))
                {
                    //create 3 intigers
                    int v[3];
                    //create 3 intigers
                    int n[3];
                    //read the data from the file
                    sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v[0], &n[0], &v[1], &n[1], &v[2], &n[2]);
                    //create the three vertices
                    for (int i = 0; i < 3; i++)
                    {
                        //check if the vertex that should be created allready exists
                        if (cverts.count(std::to_string(v[i])+std::string("v,/,")+std::to_string(n[i])))
                        {
                            //if the vertex allready exists, store the value in the face
                            this->indices.push_back(cverts[std::to_string(v[i])+std::string("v,/,")+std::to_string(n[i])].second);
                        }
                        else
                        {
                            //if the vertex does not exist, create the vertex
                            this->vertices.push_back(Vertex(verts[v[i]], normals[n[i]]));
                            //store that the vertex exists
                            cverts[std::to_string(v[i])+std::string("v,/,")+std::to_string(n[i])].first = Vertex(verts[v[i]], normals[n[i]]);
                            cverts[std::to_string(v[i])+std::string("v,/,")+std::to_string(n[i])].second = (unsigned int)this->vertices.size()-1;
                            //store the position of the vertex in the face
                            this->indices.push_back(cverts[std::to_string(v[i])+std::string("v,/,")+std::to_string(n[i])].second);
                        }
                    }
                }
                //else it has no double slahs, then create an face from pos, tex and normal
                else
                {
                    //create 3 intigers for the position
                    int v0, v1, v2;
                    //create 3 intigers for the texture
                    int t0, t1, t2;
                    //create 3 intigers for the normal
                    int n0, n1, n2;
                    //read the data from the file
                    sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n0, &v1, &t1, &n1, &v2, &t2, &n2);
                    //convert the integers to arrays
                    int v[3] = {v0-1, v1-1, v2-1};
                    int t[3] = {t0-1, t1-1, t2-1};
                    int n[3] = {n0-1, n1-1, n2-1};
                    //create the three vertices
                    for (int i = 0; i < 3; i++)
                    {
                        //check if the vertex that should be created allready exists
                        if (cverts.count(std::to_string(v[i])+std::string(",")+std::to_string(t[i])+std::string(",")+std::to_string(n[i])))
                        {
                            //if the vertex allready exists, store the value in the face
                            this->indices.push_back(cverts[(std::to_string(v[i])+std::string("v,")+std::to_string(t[i])+std::string("t,")+std::to_string(n[i]))].second);
                        }
                        else
                        {
                            //if the vertex does not exist, create the vertex
                            this->vertices.push_back(Vertex(verts[v[i]], tex[t[i]], normals[n[i]]));
                            //store that the vertex exists
                            cverts[std::to_string(v[i])+std::string("v,")+std::to_string(t[i])+std::string("t,")+std::to_string(n[i])].first = Vertex(verts[v[i]], normals[n[i]]);
                            cverts[std::to_string(v[i])+std::string("v,")+std::to_string(t[i])+std::string("t,")+std::to_string(n[i])].second = (unsigned int)this->vertices.size()-1;
                            //store the position of the vertex in the face
                            this->indices.push_back(cverts[std::to_string(v[i])+std::string("v,")+std::to_string(t[i])+std::string("t,")+std::to_string(n[i])].second);
                        }
                    }
                }
            }
        }
    }
}

Mesh::Mesh(const char* file, int type)
{
    std::string data;

    readFile(file, data);

    *this = Mesh(data,type);
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

    //bind the default 3D shader
    this->shader = glgeShaderDefault;
    //store the move matrix location
    this->moveMatLoc = glgeMoveMatDefaulLoc;

    //THIS MAY CAUSE AN MEMORY ACCES ERROR, IF NO CAMERA IS BOUND!
    //update the object
    this->update();
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

    //bind the default 3D shader
    this->shader = glgeShaderDefault;
    //store the move matrix location
    this->moveMatLoc = glgeMoveMatDefaulLoc;

    //THIS MAY CAUSE AN MEMORY ACCES ERROR, IF NO CAMERA IS BOUND!
    //update the object
    this->update();
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

void Object::setShader(std::string vs, std::string fs)
{
    //compile the shader source code and store the shader
    this->shader = glgeCompileShader(vs, fs);
    
    //get the new location for the move matrix
    this->moveMatLoc = glgeGetUniformVar(shader, glgeMoveMatrix);
}

GLuint Object::getShader()
{
    return this->shader;
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

//apply a new transform to the object
void Object::setTransform(Transform t)
{
    //set the transform to the inputed transform
    this->transf = t;
}

Transform Object::getTransform()
{
    //return the transform
    return this->transf;
}

//set the position of the object
void Object::setPos(vec3 p)
{
    //apply the new position
    this->transf.pos = p;
}

//set the position of the object
void Object::setPos(float x, float y, float z)
{
    //apply the new position
    this->transf.pos = vec3(x,y,z);
}

//move the object
void Object::move(vec3 v)
{
    //change the position of the object
    this->transf.pos += v;
}

//move the object
void Object::move(float x, float y, float z)
{
    //change the position of the object
    this->transf.pos += vec3(x,y,z);
}

vec3 Object::getPos()
{
    //return the object position
    return this->transf.pos;
}

//set the rotation of the object
void Object::setRotation(vec2 r)
{
    //apply the new rotation
    this->transf.rot = vec3(r.x,r.y,0);
}

//set the rotation of the object
void Object::setRotation(float x, float y)
{
    //apply the new rotation
    this->transf.rot = vec3(x,y,0);
}

//rotate the object
void Object::rotate(vec2 r)
{
    //rotate the object
    this->transf.rot += vec3(r.x,r.y,0);
}

//rotate the object
void Object::rotate(float x, float y)
{
    //rotate the object
    this->transf.rot += vec3(x,y,0);
}

vec3 Object::getRotation()
{
    //return the rotation
    return this->transf.rot;
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

Transform Camera::getTransform()
{
    //return the transform
    return this->transf;
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

vec3 Camera::getPos()
{
    //return the camera position
    return this->transf.pos;
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

vec3 Camera::getRotation()
{
    //return the rotation
    return this->transf.rot;
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

float Camera::getFOV()
{
    //return the fov
    return this->fov;
}

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
    glDepthFunc(GL_GREATER);
    //set the clear depth correctly
    glClearDepth(0.f);

    //load the default 3D shaders
    glgeShaderDefault = glgeCompileShader(GLGE_DEFAULT_3D_VERTEX, GLGE_DEFAULT_3D_FRAGMENT);
    //store the location of the move matrix in the default shader
    glgeMoveMatDefaulLoc = glgeGetUniformVar(glgeShaderDefault, glgeMoveMatrix);
}
