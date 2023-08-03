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
#include "glgeErrors.hpp"
#include "glgeVars.hpp"
#include "glgeFuncs.hpp"
#include "GLGEDefines.hpp"
#include "glgePrivDefines.hpp"

//include needed CML librarys
#include "CML/CMLQuaternion.h"

//include the default librarys
#include <iostream>
#include <cstring>
#include <math.h>
#include <sstream>
#include <map>

/////////////////////
//PRIVATE FUNCTIONS//
/////////////////////

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

//store the default 3D shader
GLuint glgeShaderDefault;

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

mat4 Transform::getRotationMatrix()
{
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

    //return the product of the matrices
    return (rotXMat * rotYMat) * rotZMat;
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

//a lot of floats
Vertex::Vertex(float x, float y, float z, float tx, float ty, float nx, float ny, float nz)
{
    //store the inputed position values
    this->pos = vec3(x,y,z);
    //store the inputed texture coordinats
    this->texCoord = vec2(tx, ty);
    //store and normalise the inputed normal
    this->normal = vec3(nx,ny,nz);
    this->normal.normalize();
}

//10 floats
Vertex::Vertex(float x, float y, float z, float r, float g, float b, float a, float nx, float ny, float nz)
{
    //store the inputed position values
    this->pos = vec3(x,y,z);
    //store the inputed color
    this->color = vec4(r,g,b,a);
    //store and normalise the inputed normal
    this->normal = vec3(nx,ny,nz);
    this->normal.normalize();
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
                sscanf(line.c_str(), "f %d %d %d", &m[2], &m[1], &m[0]);
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
                sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &v[2], &t[2], &v[1], &t[1], &v[0], &t[0]);
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
                    sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v[2], &n[2], &v[1], &n[1], &v[0], &n[0]);
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
                    int v[3] = {v2-1, v1-1, v0-1};
                    int t[3] = {t2-1, t1-1, t0-1};
                    int n[3] = {n2-1, n1-1, n0-1};
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

void Mesh::recalculateNormals()
{
    for (int i = 0; i < (int)this->indices.size()/3; i++)
    {
        vec3 normal = this->vertices[indices[i+1]].pos.cross(this->vertices[indices[i]].pos);
        normal.normalize();

        if (normal == vec3(1/0.f,1/0.f,1/0.f))
        {
            normal = vec3(0,0,0);
        }
        
        for (int j = 0; j < 3; j++)
        {
            bool is0 = this->vertices[indices[i+j]].normal == vec3(0,0,0);            

            if (is0)
            {
                this->vertices[indices[i+j]].normal = normal;
            }
            else
            {
                this->vertices[indices[i+j]].normal.normalize();
                this->vertices[indices[i+j]].normal = (this->vertices[indices[i+j]].normal + normal);
                this->vertices[indices[i+j]].normal.normalize();
            }
        }
    }
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
    //get the uniforms
    this->getUniforms();

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
    //get the uniforms
    this->getUniforms();

    //THIS MAY CAUSE AN MEMORY ACCES ERROR, IF NO CAMERA IS BOUND!
    //update the object
    this->update();
}

//construct an object from an mesh
Object::Object(Mesh mesh, Transform transform, bool isStatic)
{
    //construct and save a mesh from the pointers
    this->mesh = mesh;

    //save the inputed transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;

    //calculate the buffers
    this->compileBuffers();

    //bind the default 3D shader
    this->shader = glgeShaderDefault;
    //get the uniforms
    this->getUniforms();

    //THIS MAY CAUSE AN MEMORY ACCES ERROR, IF NO CAMERA IS BOUND!
    //update the object
    this->update();
}

//create an object from an file
Object::Object(const char* file, int type, Transform transform, bool isStatic)
{
    //construct and save a mesh from the pointers
    this->mesh = Mesh(file, type);

    //save the inputed transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;

    //calculate the buffers
    this->compileBuffers();

    //bind the default 3D shader
    this->shader = glgeShaderDefault;
    //get the uniforms
    this->getUniforms();

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

    //store how much texture units are used
    int textures;

    //check if the shadow pass is drawn
    if (!glgeIsShadowPass)
    {
        //only do shader binding if if it is not the shadow pass
        //bind the shader
        glUseProgram(this->shader);

        //Bind the material
        textures = this->mat.applyMaterial();

        //bind all light uniforms
        //this->loadLights();
    }

    //always input the color argument
    //activate sub elements
    //say where the position vector is
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, pos));
    if (!glgeIsShadowPass)
    {
        //say where the color is
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, color));
        //say where the texCoords are
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, texCoord));
        //say where the normals are
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, normal));
    }

    //always pass the move matrix to the shader
    glUniformMatrix4fv(camMatLoc, 1, GL_FALSE, &this->camMat.m[0][0]);
    //other uniforms are only passed if it is not the shadow pass
    if (!glgeIsShadowPass)
    {
        //pass the model matrix to the shader, if it has one
        if (modelMatLoc != 0)
        {
            glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, &this->modelMat.m[0][0]);
        }
        //pas the rotation matrix to the shader, if it has one
        if (this->rotMatLoc != 0)
        {
            glUniformMatrix4fv(rotMatLoc, 1, GL_FALSE, &this->rotMat.m[0][0]);
        }
        //pass the camera position to the shader, if it has one
        if (this->camPosLoc != 0)
        {
            glUniform3f(this->camPosLoc, glgeMainCamera->getPos().x, glgeMainCamera->getPos().y, glgeMainCamera->getPos().z);
        }
        //pass the camera position to the shader, if it has one
        if (this->camRotLoc != 0)
        {
            glUniform3f(this->camRotLoc, glgeMainCamera->getRotation().x, glgeMainCamera->getRotation().y, glgeMainCamera->getRotation().z);
        }
        //pass the far plane to the shader, if it exists
        if (this->farPlaneLoc != 0)
        {
            glUniform1f(this->farPlaneLoc, glgeMainCamera->getFarPlane());
        }

        if (glgeLights.size() != 0)
        {
            //bind the shadow map
            glgeLights[glgeLights.size()-1]->bindShadowMapTexture(textures + 1);
            //pass the sampler to the shader
            //glUniform1i(this->shadowMapLoc, textures + 1); // Error bei JuNi
        }
    }
    //if it is not the shadow pass
    else
    {
        //pass the model matrix to the shader
        glUniformMatrix4fv(glgeModelMatShadowLoc, 1, GL_FALSE, &this->modelMat.m[0][0]);
    }

    glDrawElements(GL_TRIANGLES, this->mesh.indices.size(), GL_UNSIGNED_INT, 0);

    //these things are only bound if it is not the shadow pass
    if (!glgeIsShadowPass)
    {
        //unbind the texture for the shadow map
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        //unbind the material
        this->mat.removeMaterial();

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

    //unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object::update()
{
    //recalculate the move matrix
    this->recalculateMatrices();

    //check if a light source was changed
    if (this->lightPosLocs.size() != glgeLights.size())
    {
        //update the light uniforms
        this->getLightUniforms();
    }
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
    
    //get all uniforms
    this->getUniforms();
}

void Object::setShader(std::string vs, std::string fs)
{
    //compile the shader source code and store the shader
    this->shader = glgeCompileShader(vs, fs);
    
    //get all uniforms
    this->getUniforms();
}

void Object::setShader(std::string vs, const char* file)
{
    //create a variable to store the fragment shader
    std::string fs;
    //load the fragment shader
    readFile(file, fs);
    //compile the shader source code and store the shader
    this->shader = glgeCompileShader(vs, fs);
    
    //get all uniforms
    this->getUniforms();
}

GLuint Object::getShader()
{
    return this->shader;
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

//set the size of the object
void Object::setSize(vec3 s)
{
    //store the inputed vector
    this->transf.scale = s;
}

//set the size of the object
void Object::setSize(float x, float y, float z)
{
    //convert the floats to an vector and store it
    this->transf.scale = vec3(x,y,z);
}

//set the size of the object
void Object::setSize(float s)
{
    //convert the float to an vector and store it
    this->transf.scale = vec3(s,s,s);
}

//change the size of the object
void Object::scale(vec3 s)
{
    //multiply each size of the size by the input
    this->transf.scale.scale(s);
}

//change the size of the object
void Object::scale(float x, float y, float z)
{
    //scale the size by the inputs
    this->transf.scale.scale(vec3(x,y,z));
}

//change the size of the object
void Object::scale(float s)
{
    //scale the size by the input
    this->transf.scale.scale(vec3(s,s,s));
}

//add something to the size of the Object
void Object::addScale(vec3 s)
{
    //add the input to the size of the object
    this->transf.scale += s;
}

//add something to the size of the Object
void Object::addScale(float x, float y, float z)
{
    //add the input to the size of the object
    this->transf.scale += vec3(x,y,z);
}

//add something to the size of the Object
void Object::addScale(float s)
{
    //add the input to the size of the object
    this->transf.scale += vec3(s,s,s);
}

//get the scale from the Object
vec3 Object::getScale()
{
    //return the stored scale
    return this->transf.scale;
}

void Object::recalculateNormals()
{
    //recalculate the nromals of the mesh
    this->mesh.recalculateNormals();
}

void Object::setOnlyMesh(Mesh mesh)
{
    //store the inputed mesh
    this->mesh = mesh;
}

void Object::recalculateBuffers()
{
    //delete the old buffers
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->IBO);

    //recompile the buffers
    this->compileBuffers();
}

void Object::setMesh(Mesh m)
{
    //set the buffer
    this->setOnlyMesh(m);
    //recalculate the buffers
    this->recalculateBuffers();
}

Mesh Object::getMesh()
{
    //return the stored mesh
    return this->mesh;
}

void Object::setMaterial(Material mat)
{
    //store the inputed material
    this->mat = mat;
    //update the material of the object
    this->mat.applyShader(this->shader);
}

Material Object::getMaterial()
{
    //return the material stored in the object
    return this->mat;
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

    //get all uniforms
    this->getUniforms();
}

void Object::recalculateMatrices()
{
    //save the model matrix
    this->modelMat = this->transf.getMatrix();
    //set the move matrix to the product of 3 matrices
    this->camMat = glgeMainCamera->getProjectionMatrix() * glgeMainCamera->getRotMat() * glgeMainCamera->getTransformMat();
    //recalculate the rotation matrix
    this->rotMat = this->transf.getRotationMatrix();
}

void Object::getUniforms()
{
    //store the state of the GLGE error output
    bool outErrs = glgeErrorOutput;
    //deactivate GLGE errors
    glgeErrorOutput = false;
    //save the location of the move matrix
    this->camMatLoc = glgeGetUniformVar(this->shader, glgeCamMatrix);
    //save the location of the model matrix
    this->modelMatLoc = glgeGetUniformVar(this->shader, "modelMat");
    //save the location of the camera position argument
    this->camPosLoc = glgeGetUniformVar(this->shader, "cameraPos");
    //save the location of the camera rotation argument
    this->camRotLoc = glgeGetUniformVar(this->shader, "cameraLook");
    //store the location of the rotation matrix
    this->rotMatLoc = glgeGetUniformVar(this->shader, "rotMat");
    //store the location for the shadow map
    this->shadowMapLoc = glgeGetUniformVar(this->shader, "shadowMap");
    //store the location of the far plane
    this->farPlaneLoc = glgeGetUniformVar(this->shader, "farPlane");
    //store the uniform for the shadow map
    this->shadowMapSamplerLoc = glgeGetUniformVar(this->shader, "shadowMap");
    //reset GLGE error output
    glgeErrorOutput = outErrs;

    //recalculate the uniforms of the material
    this->mat.applyShader(this->shader);

    //recalculate all light unifomrs
    this->getLightUniforms();
}

void Object::getLightUniforms()
{
    //set the prefix for the light position
    std::string prefixLightPos = std::string("lightPos");
    //set the prefix for the light color
    std::string prefixLightCol = std::string("lightColor");
    //set the prefix for the light intensity
    std::string prefixLightInt = std::string("lightInt");

    //store the state of the GLGE error output
    bool outErrs = glgeErrorOutput;
    //deactivate GLGE errors
    glgeErrorOutput = false;

    //clear the light position vector
    this->lightPosLocs.clear();
    //clear the light color vector
    this->lightColLocs.clear();
    //clear the light intensity vector
    this->lightIntLocs.clear();

    for (int i = 0; i < (int)glgeLights.size(); i++)
    {
        //calculate the name of an uniform for the light positions
        std::string uniform = prefixLightPos + std::string("[") + std::to_string(i) + std::string("]");
        //get the uniform for the light position
        this->lightPosLocs.push_back(glgeGetUniformVar(this->shader, uniform.c_str()));

        //calculate the name of an uniform for the light color
        uniform = prefixLightCol + std::string("[") + std::to_string(i) + std::string("]");
        //get the uniform for the light color
        this->lightColLocs.push_back(glgeGetUniformVar(this->shader, uniform.c_str()));

        //calculate the name of an uniform for the light intensity
        uniform = prefixLightInt + std::string("[") + std::to_string(i) + std::string("]");
        //get the uniform for the light intensity
        this->lightIntLocs.push_back(glgeGetUniformVar(this->shader, uniform.c_str()));
    }

    //get the uniform for the amount of used lights
    this->usedLigtsPos = glgeGetUniformVar(this->shader, "activeLights");

    //reset GLGE error output
    glgeErrorOutput = outErrs;
}

void Object::loadLights()
{
    //load all light positions to the shader
    for (int i = 0; i < (int)this->lightPosLocs.size(); i++)
    {
        //pass the light position if it is not 0
        if (this->lightPosLocs[i] != 0)
        {
            glUniform3f(this->lightPosLocs[i], glgeLights[i]->getPos().x, glgeLights[i]->getPos().y, glgeLights[i]->getPos().z);
        }
    }

    //load all light colors to the shader
    for (int i = 0; i < (int)this->lightPosLocs.size(); i++)
    {
        //pass the light color if it is not 0
        if (this->lightColLocs[i] != 0)
        {
            glUniform3f(this->lightColLocs[i], glgeLights[i]->getColor().x, glgeLights[i]->getColor().y, glgeLights[i]->getColor().z);
        }
    }

    //load all light intensitys to the shader
    for (int i = 0; i < (int)this->lightPosLocs.size(); i++)
    {
        //pass the light intensity if it is not 0
        if (this->lightIntLocs[i] != 0)
        {
            glUniform1f(this->lightIntLocs[i], glgeLights[i]->getInsensity());
        }
    }

    //pass the amount of used lights to the shader if it is not 0
    if (this->usedLigtsPos != 0)
    {
        glUniform1f(this->usedLigtsPos, (float)glgeLights.size());
    }
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
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
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
    //recalculate the view matrices
    this->calculateViewMatrix();
}

//update the projection matrix
void Camera::recalculateProjection()
{
    //recalculate the projection matrix
    this->projectionMatrix = this->calculateProjectionMatrix();
}

mat4 Camera::getRotMat()
{
    //return the rotation matrix
    return this->rotMat;
}

mat4 Camera::getTransformMat()
{
    //return the transformation matrix
    return this->transMat;
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
void Camera::rotate(float x, float y, float z)
{
    //rotate the camera
    this->transf.rot += vec3(x,y,z);
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

float Camera::getFarPlane()
{
    //return the far cliping plane
    return this->far;
}

//PRIVATE

void Camera::calculateViewMatrix()
{
    //store a vector for the y axis
    vec3 yaxis = vec3(up.x,up.y,up.z);
    //start rotation things
    //calculate the view vector
    //setup the vector to face right
    vec3 view = vec3(right.x,right.y,right.z);
    //rotathe the vector around the y axis by the x rotation amount
    view = glgeRotateVector(transf.rot.x,yaxis, view);
    //normaise the vector
    view.normalize();

    //rotate the view vector and calculate U
    //calculate the cross product of the yaxis and the view vector
    vec3 U = view.cross(yaxis);
    //normalise the U vector
    U.normalize();
    //rotate the view vector around U by the rotation on the y axis
    view = glgeRotateVector(transf.rot.y, U, view);
    
    //setup the vectors
    //N is equal to the view vector
    vec3 N = view;
    //noralise N
    N.normalize();

    //V is the cross product of U and N. Both are normalised, so no normalistion needed
    vec3 V = U.cross(N);
    V = glgeRotateVector(transf.rot.z, U, V);
    V.normalize();

    //setup the view matrix
    //IMPORTANT: the vectors are in reversd order!
    this->rotMat = mat4(U.z,U.y,U.x,0,
                        V.z,V.y,V.x,0,
                        N.z,N.y,N.x,0,
                        0,  0,  0,  1);

    /*vec4 o = this->transf.getRotationMatrix() * vec4(1,1,1,1);
    this->rotMat = glgeLookAt(this->transf.pos, vec3(o.x,o.y,o.z), vec3(0,1,0));*/

    this->transMat = mat4(1,0,0,-transf.pos.x,
                          0,1,0,-transf.pos.y,
                          0,0,1,-transf.pos.z,
                          0,0,0,1);

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
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
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
}
