/**
 * @file openglGLGE3Dcore.cpp
 * @author DM8AT
 * @brief define the 3D core functions and classes of GLGE
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

//include the 3d core
#include "openglGLGE3Dcore.h"

//include the GLGE dependencys
#include "../GLGEIndependend/glgeErrors.hpp"
#include "../GLGEIndependend/glgeVars.hpp"
#include "openglGLGEFuncs.hpp"
#include "openglGLGEDefines.hpp"
#include "../GLGEIndependend/glgePrivDefines.hpp"
#include "openglGLGEVars.hpp"

//include needed CML librarys
#include "../CML/CMLQuaternion.h"

//include the default librarys
#include <iostream>
#include <cstring>
#include <math.h>
#include <sstream>
#include <map>

//////////
//OBJECT//
//////////

//default constructor
Object::Object()
{
    //init the object
}

//constructor using preset
Object::Object(unsigned int preset, vec4 color, unsigned int res, Transform transform, bool isTransparent, bool isStatic)
{
    //say that the window index is -1 (uninitalised)
    this->windowIndex = -1;
    //construct and save a mesh from the pointers
    this->mesh = Mesh(preset, color, res);

    //save the inputed transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;
    //save if the object is transparent
    this->isTransparent = isTransparent;

    //store the UUID
    this->objData.uuid = glgeObjectUUID;
    //increase the glge object uuid
    glgeObjectUUID++;
    //update the window model matrix ubo
    glgeWindows[glgeCurrentWindowIndex]->resizeModelSSBO();

    //calculate the buffers
    this->compileBuffers();

    //check if the object is transparent
    if (this->isTransparent)
    {
        //bind the transparent shader
        this->shader = Shader(glgeWindows[this->windowIndex]->getDefault3DTransparentShader());
    }
    else
    {
        //bind the opaque shader
        this->shader = Shader(glgeWindows[this->windowIndex]->getDefault3DShader());
    }
    //get the uniforms
    this->getUniforms();

    //THIS MAY CAUSE AN MEMORY ACCES ERROR, IF NO CAMERA IS BOUND!
    //update the object
    this->update();
}

//constructor using pointer
Object::Object(Vertex* vertices, unsigned int* indices, unsigned int sizeVertices, unsigned int sizeIndices, Transform transform, bool isTransparent, bool isStatic)
{
    //say that the window index is -1 (uninitalised)
    this->windowIndex = -1;
    //construct and save a mesh from the pointers
    this->mesh = Mesh(vertices, indices, sizeVertices, sizeIndices);

    //save the inputed transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;
    //save if the object is transparent
    this->isTransparent = isTransparent;

    //store the UUID
    this->objData.uuid = glgeObjectUUID;
    //increase the glge object uuid
    glgeObjectUUID++;
    //update the window model matrix ubo
    glgeWindows[glgeCurrentWindowIndex]->resizeModelSSBO();

    //calculate the buffers
    this->compileBuffers();

    //check if the object is transparent
    if (this->isTransparent)
    {
        //bind the transparent shader
        this->shader = Shader(glgeWindows[this->windowIndex]->getDefault3DTransparentShader());
    }
    else
    {
        //bind the opaque shader
        this->shader = Shader(glgeWindows[this->windowIndex]->getDefault3DShader());
    }
    //get the uniforms
    this->getUniforms();

    //THIS MAY CAUSE AN MEMORY ACCES ERROR, IF NO CAMERA IS BOUND!
    //update the object
    this->update();
}

//constructor using vectors
Object::Object(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Transform transform, bool isTransparent, bool isStatic)
{
    //say that the window index is -1 (uninitalised)
    this->windowIndex = -1;
    //construct and save a mesh from the pointers
    this->mesh = Mesh(vertices, indices);

    //save the inputed transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;
    //save if the object is transparent
    this->isTransparent = isTransparent;

    //store the UUID
    this->objData.uuid = glgeObjectUUID;
    //increase the glge object uuid
    glgeObjectUUID++;
    //update the window model matrix ubo
    glgeWindows[glgeCurrentWindowIndex]->resizeModelSSBO();

    //calculate the buffers
    this->compileBuffers();

    //check if the object is transparent
    if (this->isTransparent)
    {
        //bind the transparent shader
        this->shader = Shader(glgeWindows[this->windowIndex]->getDefault3DTransparentShader());
    }
    else
    {
        //bind the opaque shader
        this->shader = Shader(glgeWindows[this->windowIndex]->getDefault3DShader());
    }
    //get the uniforms
    this->getUniforms();

    //THIS MAY CAUSE AN MEMORY ACCES ERROR, IF NO CAMERA IS BOUND!
    //update the object
    this->update();
}

//construct an object from an mesh
Object::Object(Mesh mesh, Transform transform, bool isTransparent, bool isStatic)
{
    //say that the window index is -1 (uninitalised)
    this->windowIndex = -1;
    //construct and save a mesh from the pointers
    this->mesh = mesh;

    //save the inputed transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;
    //save if the object is transparent
    this->isTransparent = isTransparent;

    //store the UUID
    this->objData.uuid = glgeObjectUUID;
    //increase the glge object uuid
    glgeObjectUUID++;
    //update the window model matrix ubo
    glgeWindows[glgeCurrentWindowIndex]->resizeModelSSBO();

    //calculate the buffers
    this->compileBuffers();

    //check if the object is transparent
    if (this->isTransparent)
    {
        //bind the transparent shader
        this->shader = Shader(glgeWindows[this->windowIndex]->getDefault3DTransparentShader());
    }
    else
    {
        //bind the opaque shader
        this->shader = Shader(glgeWindows[this->windowIndex]->getDefault3DShader());
    }
    //get the uniforms
    this->getUniforms();

    //THIS MAY CAUSE AN MEMORY ACCES ERROR, IF NO CAMERA IS BOUND!
    //update the object
    this->update();
}

//create an object from an file
Object::Object(const char* file, int type, Transform transform, bool isTransparent, bool isStatic)
{
    //say that the window index is -1 (uninitalised)
    this->windowIndex = -1;
    //construct and save a mesh from the pointers
    this->mesh = Mesh(file, type);

    //save the inputed transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;
    //save if the object is transparent
    this->isTransparent = isTransparent;

    //store the UUID
    this->objData.uuid = glgeObjectUUID;
    //increase the glge object uuid
    glgeObjectUUID++;
    //update the window model matrix ubo
    glgeWindows[glgeCurrentWindowIndex]->resizeModelSSBO();

    //calculate the buffers
    this->compileBuffers();

    //check if the object is transparent
    if (this->isTransparent)
    {
        //bind the transparent shader
        this->shader = Shader(glgeWindows[this->windowIndex]->getDefault3DTransparentShader());
    }
    else
    {
        //bind the opaque shader
        this->shader = Shader(glgeWindows[this->windowIndex]->getDefault3DShader());
    }
    //get the uniforms
    this->getUniforms();

    //THIS MAY CAUSE AN MEMORY ACCES ERROR, IF NO CAMERA IS BOUND!
    //update the object
    this->update();
}

Object::~Object()
{
    //destroy the object
    this->destroy();
    //clear light vectors
    if (this->lightColLocs.data()){this->lightColLocs.clear();}
    if (this->lightDatLocs.data()){this->lightDatLocs.clear();}
    if (this->lightDirLocs.data()){this->lightDirLocs.clear();}
    if (this->lightPosLocs.data()){this->lightPosLocs.clear();}
}

void Object::destroy()
{
    //delete the VBO
    glDeleteBuffers(1, &this->VBO);
    //delete the IBO
    glDeleteBuffers(1, &this->IBO);
    //reset the window id
    this->windowIndex = -1;
    //clear the mesh
    this->mesh.vertices.clear();
    this->mesh.indices.clear();
}

//draw the object
void Object::draw()
{
    //check if the object belongs to the window
    if (glgeCurrentWindowIndex != this->windowIndex)
    {
        //check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] Can't draw an object in an window it was not created in.\n";
        }
        //check if the program should close on an error
        if (glgeExitOnError)
        {
            //close with an error
            exit(1);
        }
        //stop the function
        return;
    }
    //check for the shadow pass
    if (glgeShadowPass && (!this->isTransparent))
    {
        //call the shadow draw function
        this->shadowDraw();
        //stop this function
        return;
    }
    //check if the current object belongs to the current pass
    if ((!this->isTransparent) && glgeWindows[this->windowIndex]->isTranparentPass())
    {
        //abbort the draw, if not
        return;
    }
    //check for fully transparent objects
    if (this->fullyTransparent && !glgeWindows[this->windowIndex]->isTranparentPass())
    {
        //abbort the draw, if it is the opaque pass
        return;
    }

    //enable depth testing
    glEnable(GL_DEPTH_TEST);

    if (!glgeWindows[this->windowIndex]->isTranparentPass())
    {
        //enable color blending
        glDisable(GL_BLEND);
    }
    //bind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->ubo);

    //bind the transparent extra data
    if (this->isTransparent)
    {
        //pass the current pass
        this->shader.setCustomInt("glgePass", glgeWindows[this->windowIndex]->isTranparentPass());
    }

    //bind the shader
    this->shader.applyShader();

    int shadowCasters = 0;
    //loop over all the light sources
    for (int i = 0; i < (int)glgeWindows[this->windowIndex]->getLights().size(); i++)
    {
        if (glgeWindows[this->windowIndex]->getLights()[i]->getType() != GLGE_LIGHT_SOURCE_TYPE_DIRECTIONAL) { continue; }
        //pass the texture handler
        glUniform1i(glGetUniformLocation(this->shader.getShader(), ("glgeShadowMap[" + std::to_string(shadowCasters) + "]").c_str()), glgeWindows[this->windowIndex]->getLights()[i]->getShadowMap()+1);
        shadowCasters++;
    }
    glUniform1i(glGetUniformLocation(this->shader.getShader(), "glgeActiveShadowCasters"), shadowCasters);

    //Bind the material
    this->mat->apply();

    //always input the color argument
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
    //say where the normals are
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, normal));
    //check if this is the transparent pass
    if (this->isTransparent)
    {
        //pass the lights
        this->loadLights();
    }

    glDrawElements(GL_TRIANGLES, this->mesh.indices.size(), GL_UNSIGNED_INT, 0);

    //unbind the texture for the shadow map
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    //deactivate the sub elements
    //deactivate the position argument
    glDisableVertexAttribArray(0);
    //deactivate the color argument
    glDisableVertexAttribArray(1);
    //deactivate the texCoord argument
    glDisableVertexAttribArray(2);

    //unbind the shader
    this->shader.removeShader();

    //unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object::update()
{
    //check if the object belongs to the window
    if (glgeCurrentWindowIndex != this->windowIndex)
    {
        //check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] Can't update an object in an window it was not created in.\n";
        }
        //check if the program should close on an error
        if (glgeExitOnError)
        {
            //close with an error
            exit(1);
        }
        //stop the function
        return;
    }
    //recalculate the move matrix
    this->recalculateMatrices();

    //update the own UBO
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    //buffer the data
    glBufferData(GL_UNIFORM_BUFFER, sizeof(ObjectData), &this->objData, GL_DYNAMIC_DRAW);
    //check if a material is applied
    if (mat != NULL)
    {
        //update the material
        this->mat->update();
    }

    //push the screen resolution to the shader
    this->shader.setCustomVec2("glgeScreenResolution", glgeWindows[this->windowIndex]->getSize());
    //update the amount of active light sources
    this->shader.setCustomInt("glgeActiveLights", (int)glgeWindows[this->windowIndex]->getLights().size());

    //check if the object is transparent
    if (this->isTransparent)
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

void Object::setShader(unsigned int shader)
{
    //store the inputed shader
    this->shader = Shader(shader);
    
    //get all uniforms
    this->getUniforms();
}

void Object::setShader(std::string vs, std::string fs)
{
    //compile the shader source code and store the shader
    this->shader = Shader(vs, fs);
    
    //get all uniforms
    this->getUniforms();
}

void Object::setShader(std::string vs, const char* file)
{
    //compile and store the shader
    this->shader = Shader(vs, file);
    
    //get all uniforms
    this->getUniforms();
}

unsigned int Object::getShader()
{
    return this->shader.getShader();
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
void Object::setRotation(vec3 r)
{
    //apply the new rotation
    this->transf.rot = r;
}

//set the rotation of the object
void Object::setRotation(float x, float y, float z)
{
    //apply the new rotation
    this->transf.rot = vec3(x,y,z);
}

//rotate the object
void Object::rotate(vec3 r)
{
    //rotate the object
    this->transf.rot += r;
}

//rotate the object
void Object::rotate(float x, float y, float z)
{
    //rotate the object
    this->transf.rot += vec3(x,y,z);
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

void Object::setMaterial(Material* mat)
{
    //store the inputed material
    this->mat = mat;
}

Material* Object::getMaterial()
{
    //return a pointer to the material stored in the object
    return this->mat;
}

Shader* Object::getShaderP()
{
    //return a pointer to the shader stored in the object
    return &this->shader;
}

void Object::setTransparency(bool transparent)
{
    //store the inputed value
    this->isTransparent = transparent;
}

bool Object::getTransparency()
{
    //output if the object is transparent
    return this->isTransparent;
}

void Object::setFullyTransparent(bool transparent)
{
    //store the new input
    this->fullyTransparent = transparent;
}

bool Object::getFullyTransparent()
{
    //return the current state
    return this->fullyTransparent;
}

Data* Object::encode()
{
    //create a new data object
    Data* dat = new Data();

    //store the amount of vertives
    dat->writeLong(this->mesh.vertices.size());
    //loop over every vertex
    for (Vertex vert : this->mesh.vertices)
    {
        //store the vertex position
        dat->writeVec3(vert.pos);
        //store the vertex color
        dat->writeVec4(vert.color);
        //store the texture coordinate
        dat->writeVec2(vert.texCoord);
        //store the normal
        dat->writeVec3(vert.normal);
    }

    //store the amount of indices
    dat->writeLong(this->mesh.indices.size());
    //loop over every vertex
    for (unsigned int ind : this->mesh.indices)
    {
        //store the index
        dat->writeUInt(ind);
    }

    //store the transform
    //store the position
    dat->writeVec3(this->transf.pos);
    //store the rotation
    dat->writeVec3(this->transf.rot);
    //store the scale
    dat->writeVec3(this->transf.scale);

    //store if the object is transparent
    dat->writeBool(this->isTransparent);
    //store if the object is fully transparent
    dat->writeBool(this->fullyTransparent);
    //store if the object is static
    dat->writeBool(this->isStatic);

    //encode the material
    this->mat->encode(dat);

    //return the finished data
    return dat;
}

void Object::decode(Data dat)
{
    //create a new empty mesh
    Mesh m = Mesh();

    //store the amount of vertives
    long vert = dat.readLong();
    //loop over every vertex
    for (long i = 0; i < vert; i++)
    {
        //create a new vertex
        Vertex vert;
        //store the vertex position
        vert.pos = dat.readVec3();
        //store the vertex color
        vert.color = dat.readVec4();
        //store the texture coordinate
        vert.texCoord = dat.readVec2();
        //store the normal
        vert.normal = dat.readVec3();
        //store the vertex
        m.vertices.push_back(vert);
    }

    //store the amount of indices
    long ind = dat.readLong();
    //loop over every vertex
    for (long i = 0; i < ind; i++)
    {
        //store the index
        m.indices.push_back(dat.readUInt());
    }

    //say that the window index is -1 (uninitalised)
    this->windowIndex = -1;
    //construct and save a mesh from the pointers
    this->mesh = m;

    //store the transform
    //store the position
    this->transf.pos = dat.readVec3();
    //store the rotation
    this->transf.rot = dat.readVec3();
    //store the scale
    this->transf.scale = dat.readVec3();

    //save if the object is static
    this->isStatic = isStatic;
    //save if the object is transparent
    this->isTransparent = isTransparent;

    //store the UUID
    this->objData.uuid = glgeObjectUUID;
    //increase the glge object uuid
    glgeObjectUUID++;

    //calculate the buffers
    this->compileBuffers();

    //check if the object is transparent
    if (this->isTransparent)
    {
        //bind the transparent shader
        this->shader = Shader(glgeWindows[this->windowIndex]->getDefault3DTransparentShader());
    }
    else
    {
        //bind the opaque shader
        this->shader = Shader(glgeWindows[this->windowIndex]->getDefault3DShader());
    }
    //get the uniforms
    this->getUniforms();

    //THIS MAY CAUSE AN MEMORY ACCES ERROR, IF NO CAMERA IS BOUND!
    //update the object
    this->update();

    //store if the object is transparent
    this->isTransparent = dat.readBool();
    //store if the object is fully transparent
    this->fullyTransparent = dat.readBool();
    //store if the object is static
    this->isStatic = dat.readBool();

    //load the material
    //create a new material
    this->mat = new Material();
    //decode the material data
    this->mat->decode(dat);
}

//PRIV
void Object::compileBuffers()
{
    //check if the window index is -1 (uninitalised)
    if (this->windowIndex == -1)
    {
        //store the new window index
        this->windowIndex = glgeCurrentWindowIndex;
    }
    //make sure to bind the OpenGL context
    glgeWindows[this->windowIndex]->makeCurrent();
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
    this->objData.modelMat = this->transf.getMatrix();
    //recalculate the rotation matrix
    this->objData.rotMat = this->transf.getRotationMatrix();
}

void Object::getUniforms()
{
    //store the state of the GLGE error output
    bool outErrs = glgeErrorOutput;
    //deactivate GLGE errors
    glgeErrorOutput = false;

    //get the position of the uniform block index
    this->uboIndex = glGetUniformBlockIndex(this->shader.getShader(), "glgeObjectData");
    //create a new uniform buffer
    glGenBuffers(1, &this->ubo);
    //bind the buffer
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    //allocate enough size for the data
    glBufferData(GL_UNIFORM_BUFFER, sizeof(ObjectData), NULL, GL_DYNAMIC_DRAW);

    //add the screen resolution
    this->shader.setCustomVec2("glgeScreenResolution", glgeWindows[this->windowIndex]->getSize());

    //check if the object is transparent
    if (this->isTransparent)
    {
        //add the pass variable
        //pass the current pass
        this->shader.setCustomInt("glgePass", false);
        //add the depth and alpha buffer
        this->shader.setCustomTexture("glgeDepthAlbedoBuffer", glgeWindows[this->windowIndex]->getEIDATex());
    }

    //get all uniform positions
    this->shader.recalculateUniforms();
    //reset GLGE error output
    glgeErrorOutput = outErrs;
}

void Object::getLightUniforms()
{
    //set the prefix for the light position
    std::string prefixLightPos = std::string("glgeLightPos");
    //set the prefix for the light color
    std::string prefixLightCol = std::string("glgeLightColor");
    //set the prefix for the light intensity
    std::string prefixLightDat = std::string("glgeLightData");
    //set the prefix for the light direction
    std::string prefixLightDir = std::string("glgeLightDir");

    //store the state of the GLGE error output
    bool outErrs = glgeErrorOutput;
    //deactivate GLGE errors
    glgeErrorOutput = false;

    //clear the light position vector
    this->lightPosLocs.clear();
    //clear the light color vector
    this->lightColLocs.clear();
    //clear the light intensity vector
    this->lightDatLocs.clear();
    //clear the light direction vector
    this->lightDirLocs.clear();

    for (int i = 0; i < (int)glgeWindows[this->windowIndex]->getLights().size(); i++)
    {
        //calculate the name of an uniform for the light positions
        std::string uniform = prefixLightPos + std::string("[") + std::to_string(i) + std::string("]");
        //get the uniform for the light position
        this->lightPosLocs.push_back(glgeGetUniformVar(this->shader.getShader(), uniform.c_str()));

        //calculate the name of an uniform for the light color
        uniform = prefixLightCol + std::string("[") + std::to_string(i) + std::string("]");
        //get the uniform for the light color
        this->lightColLocs.push_back(glgeGetUniformVar(this->shader.getShader(), uniform.c_str()));

        //calculate the name of an uniform for the light intensity
        uniform = prefixLightDat + std::string("[") + std::to_string(i) + std::string("]");
        //get the uniform for the light intensity
        this->lightDatLocs.push_back(glgeGetUniformVar(this->shader.getShader(), uniform.c_str()));

        //calculate the name of an uniform for the light intensity
        uniform = prefixLightDir + std::string("[") + std::to_string(i) + std::string("]");
        //get the uniform for the light intensity
        this->lightDirLocs.push_back(glgeGetUniformVar(this->shader.getShader(), uniform.c_str()));
    }
    //get the uniform for the amount of used lights
    this->usedLigtsPos = glgeGetUniformVar(this->shader.getShader(), "glgeActiveLights");

    //reset GLGE error output
    glgeErrorOutput = outErrs;
}

void Object::loadLights()
{
    //get all lights
    std::vector<Light*> lights = glgeWindows[this->windowIndex]->getLights();
    //load all light positions to the shader
    for (int i = 0; i < (int)this->lightPosLocs.size(); i++)
    {
        //pass the light position
        glUniform3f(this->lightPosLocs[i], lights[i]->getPos().x, lights[i]->getPos().y, lights[i]->getPos().z);
        //pass the light color
        glUniform3f(this->lightColLocs[i], lights[i]->getColor().x, lights[i]->getColor().y, lights[i]->getColor().z);
        //pass the light data
        glUniform4f(this->lightDatLocs[i], 
            lights[i]->getType(), 
            std::cos(lights[i]->getIntenseAngle() * GLGE_TO_RADIANS), 
            std::cos(lights[i]->getAngle() * GLGE_TO_RADIANS),
            lights[i]->getInsensity());
        //pass the light direction
        glUniform3f(this->lightDirLocs[i], lights[i]->getDir().x, lights[i]->getDir().y, lights[i]->getDir().z);
    }

    //pass the amount of used lights to the shader
    glUniform1i(this->usedLigtsPos, (int)lights.size());
}

void Object::shadowDraw()
{
    //get the shadow shader from the window
    Shader* sShader = glgeWindows[glgeCurrentWindowIndex]->getShadowShader();
    //pass the light matrix
    sShader->setCustomMat4("glgeLightSpaceMat", glgeCurrentShadowCaster->getLightMat());
    //activate the shadow shader
    sShader->applyShader();

    //bind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->ubo);

    //always input the color argument
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
    //say where the normals are
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, normal));

    glDrawElements(GL_TRIANGLES, this->mesh.indices.size(), GL_UNSIGNED_INT, 0);

    //unbind the texture for the shadow map
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

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

    //remove the shadow shader
    sShader->removeShader();
}

//////////
//CAMERA//
//////////

//PUBLIC

//default constructor
Camera::Camera()
{
    //initalise the object
    //default for FOV: 90
    this->camData.fov = 90*GLGE_TO_RADIANS;
    //default for near plane: 0.1
    this->camData.near = 0.1;
    //default for far plane: 100
    this->camData.far = 100;
    //instantiate the own updates
    this->update();
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
            std::cerr << GLGE_ERROR_STR_CAMERA_CAMERA << "\n";
        }
        //stop the script
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }

    //if no error occured, store the variables
    this->camData.fov = FOV*GLGE_TO_RADIANS;
    this->camData.near = near;
    this->camData.far = far;
    this->transf = transform;
}

//update the view matrix
void Camera::update()
{
    //recalculate the view matrices
    this->calculateViewMatrix();
    //recalculate the projection
    this->recalculateProjection();
    //update the position
    this->camData.pos = this->transf.pos;
    //update the rotation
    this->camData.rot = this->transf.rot;
    //update the main camera matrix
    this->camData.camMat = this->camData.projMat * this->camData.rotMat * this->camData.transMat;
    //update the ubo
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    //store the data
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), &this->camData, GL_DYNAMIC_DRAW);
}

//update the projection matrix
void Camera::recalculateProjection()
{
    //recalculate the projection matrix
    this->camData.projMat = this->calculateProjectionMatrix();
}

mat4 Camera::getRotMat()
{
    //return the rotation matrix
    return this->camData.rotMat;
}

mat4 Camera::getTransformMat()
{
    //return the transformation matrix
    return this->camData.transMat;
}

//get the projection matrix
mat4 Camera::getProjectionMatrix()
{
    //return the projection matrix
    return this->camData.projMat;
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
    //just add the y component
    this->transf.pos.y += y;
    //add the difference of the x and z component scaled by the sine of the rotatin on the x-axis
    this->transf.pos.x += (x - z) * std::sin(this->transf.rot.x);
    //do the same as for x, but change the order of x and z and use cosine
    this->transf.pos.z += (z - x) * std::cos(this->transf.rot.x);
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
    this->camData.fov = f*GLGE_TO_RADIANS;
}

//change the field of view
void Camera::sizeFOV(float df)
{
    //change the fov
    this->camData.fov += (df*GLGE_TO_RADIANS);
}

float Camera::getFOV()
{
    //return the fov
    return this->camData.fov;
}

float Camera::getFarPlane()
{
    //return the far cliping plane
    return this->camData.far;
}

float* Camera::getRotMatPointer()
{
    //return a pointer to the rotation matrix
    return &this->camData.rotMat.m[0][0];
}

float* Camera::getProjectionMatrixPointer()
{
    //return a pointer to the rotation matrix
    return &this->camData.projMat.m[0][0];
}

float* Camera::getTransformMatPointer()
{
    //return a pointer to the transformation matrix
    return &this->camData.transMat.m[0][0];
}

void Camera::setWindowIndex(unsigned int windowIndex)
{
    //store the inputed window index
    this->windowIndex = windowIndex;

    //create a new uniform buffer
    glGenBuffers(1, &this->ubo);
    //bind the buffer
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    //allocate enough size for the data
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), NULL, GL_DYNAMIC_DRAW);
}

unsigned int Camera::getWindowIndex()
{
    //return the window index
    return this->windowIndex;
}

void Camera::readyForDraw()
{
    //bind the ubo
    glBindBufferBase(GL_UNIFORM_BUFFER,1,this->ubo);
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
    this->camData.rotMat = mat4(U.z,U.y,U.x,0,
                                V.z,V.y,V.x,0,
                                N.z,N.y,N.x,0,
                                0,  0,  0,  1);

    /*vec4 o = this->transf.getRotationMatrix() * vec4(1,1,1,1);
    this->rotMat = glgeLookAt(this->transf.pos, vec3(o.x,o.y,o.z), vec3(0,1,0));*/

    this->camData.transMat = mat4(1,0,0,-transf.pos.x,
                                  0,1,0,-transf.pos.y,
                                  0,0,1,-transf.pos.z,
                                  0,0,0,1);
}

mat4 Camera::calculateProjectionMatrix()
{
    float ar = 1;
    if (glgeHasMainWindow)
    {
        //check if a window is bound to this camera
        if (this->windowIndex != ((unsigned int)-1))
        {
            ar = glgeWindows[this->windowIndex]->getWindowAspect();
        }
    }
    float zRange = this->camData.far - this->camData.near;
    float tHF = std::tan((this->camData.fov/2.f));

    return mat4(1.f/(tHF*ar), 0.f, 0.f, 0.f,
                0.f, 1/tHF, 0.f, 0.f,
                0.f, 0.f, (-this->camData.near - this->camData.far)/zRange, (2.f*this->camData.far * this->camData.near) / zRange,
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
            std::cerr << GLGE_ERROR_STR_BIND_MAIN_CAMERA << "\n";
        }
        //stop the program
        if (glgeExitOnError)
        {
            //only exit the program if glge is tolled to exit on an error
            exit(1);
        };
    }
    //else bind the camera to the main window
    glgeWindows[glgeMainWindowIndex]->setCamera(camera);
}

//initalise for 3D
void glgeInit3DCore()
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
    //set the depth test correctly
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_GREATER);
    //set the clear depth correctly
    glClearDepth(0.f);
}
