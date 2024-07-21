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

///////////////////////
// PRIVATE FUNCTIONS //
///////////////////////

/**
 * @brief check if a string contains a double shalsh. This function is used when creating an object from an .obj file
 * 
 * @param str the string to check
 * @return true : the string contains a double slash somewhere | 
 * @return false : the sting dosn't contain a double slash
 */
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

/**
 * @brief add a vertex that lies on the unit sphere
 * 
 * @param p the un-normalised vertex position
 * @param color the color of the vertex
 * @param vertices the allready existing vertices to add to
 * @return unsigned int the index if the vertex allready exists
 */
unsigned int addVertexOnUintSphere(Vertex p, vec4 color, std::vector<Vertex>* vertices)
{
    //calculate the distance from the middle of the sphere
    float length = std::sqrt(p.pos.x * p.pos.x + p.pos.y * p.pos.y + p.pos.z * p.pos.z);
    //store the position of the vertex
    vec3 pos = vec3(p.pos.x/length, p.pos.y/length, p.pos.z/length);
    //check if the vertex exists
    for (int i = 0; i < (int)vertices->size(); i++)
    {
        //if it exists, return the index of the vertex
        if ((*vertices)[i].pos == pos)
        {
            return i;
        }
    }

    //use the normalised position to generate the vertex
    Vertex vert = Vertex(pos);

    //check if the color should be stored
    if (color.w == -1)
    {
        //calculate the uv coordinates
        float u = std::atan2(pos.x, pos.z) / (2.f*M_PI) + 0.5;
        float v = pos.y * 0.5 + 0.5;
        //store the uv coordinates
        vert.texCoord = vec2(u,v);
        //set the color to black
        vert.color = vec4(0,0,0,1);
    }
    else
    {
        //store the color
        vert.color = color;
    }
    //the normal is just the position of the vertex
    vert.normal = vert.pos;
    //store the vertex
    vertices->push_back(vert);
    //return the index of the vertex
    return (unsigned int) vertices->size()-1;
}

/**
 * @brief add a polygon / circle to an allready existing mesh
 * 
 * @param vertices the vertices of the allready existing mesh
 * @param indices the indices of the allready existing mesh
 * @param numbers the number of points for the polygon / the resolution of the circle
 * @param col the color for the polygon / circle. Use vec4(-1) for texture coordinates
 * @param pos a position offset for all vertices
 * @param flip say if the circle / polygon should be flipped in some way
 * @param fill say if the circle / polygon should be filled using triangulation
 * @param sidewaysNormals say if the normals should point sideways rather than upwards
 * @param texOffset a texture coordinate offset
 * @param textureMode the mode of the texture coordinates
 */
inline void addCircle(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, unsigned int numbers, vec4 col, vec3 pos = vec3(0), bool flip = false,
                      bool fill = true, bool sidewaysNormals = false, vec2 texOffset = vec2(0,0), unsigned int textureMode = 0)
{
    //store the amount of points
    unsigned int points = (numbers == 0) ? 32 : numbers;

    //calculate the angle
    double angle = (2.0*M_PI) / (double)points;

    //store the first index
    unsigned int first_ind = (unsigned int)vertices->size();

    //generate the vertices
    for (int i = 0; i < (int)points; i++)
    {
        //calculate the current angle
        double c_angle = angle * i;
        //store the calculated position
        vec3 p = vec3(std::sin(c_angle), 0, std::cos(c_angle)) + pos;
        //create the vertex
        Vertex vert = Vertex(p);
        //check if color should be added
        if (col.w != -1)
        {
            //add the color
            vert.color = col;
        }
        else
        {
            //clear the color
            vert.color = vec4(0);
            //check the texture mode
            if (textureMode == 0)
            {
                //normal circle texturing
                vert.texCoord = vec2(p.x * 0.5 + 0.5, p.z * 0.5 + 0.5) + texOffset;
            }
            else if (textureMode == 1)
            {
                //calculate u
                float u = (float)i*2 / (float)points;
                //check if u is larger than 1
                if (u > 1.f)
                {
                    //then set u to 2 - u
                    u = 2.f - u;
                }
                //1D circle wrapping
                vert.texCoord = vec2(u, 0) + texOffset;
            }
            
        }
        //check if the normals should be sideways
        if (sidewaysNormals)
        {
            //add a sideways normal
            vert.normal = vert.pos;
            vert.normal.normalize();
        }
        else
        {
            //add the normal vector
            vert.normal = vec3(0, flip ? -1 : 1, 0);
        }
        //add the vertex
        vertices->push_back(vert);
    }

    //check if indices should generate
    if (!fill)
    {
        //if not, stop the function
        return;
    }

    //generate the indices
    for (int i = 0; i < (int)points - 2; i++)
    {
        //create a triangle
        indices->push_back(first_ind);
        if (flip)
        {
            indices->push_back(i + 1 + first_ind);
            indices->push_back(i + 2 + first_ind);
        }
        else
        {
            indices->push_back(i + 2 + first_ind);
            indices->push_back(i + 1 + first_ind);
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

//constructor using preset
Object::Object(unsigned int preset, vec4 color, unsigned int res, Transform transform, bool isTransparent, bool isStatic)
{
    //say that the window index is the one from the current window
    this->windowIndex = glgeCurrentWindowIndex;
    //construct and save a mesh from the pointers
    this->mesh = new Mesh(preset, color, res);
    //initalise the mesh
    this->mesh->init();

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
    //say that the window index is the one from the current window
    this->windowIndex = glgeCurrentWindowIndex;
    //construct and save a mesh from the pointers
    this->mesh = new Mesh(vertices, indices, sizeVertices, sizeIndices);

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
    //say that the window index is the one from the current window
    this->windowIndex = glgeCurrentWindowIndex;
    //construct and save a mesh from the pointers
    this->mesh = new Mesh(vertices, indices);
    //initalise the mesh
    this->mesh->init();

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
Object::Object(Mesh* mesh, Transform transform, bool isTransparent, bool isStatic)
{
    //say that the window index is the one from the current window
    this->windowIndex = glgeCurrentWindowIndex;
    //construct and save a mesh from the pointers
    this->mesh = mesh;
    //initalise the mesh
    this->mesh->init();

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
    //say that the window index is the one from the current window
    this->windowIndex = glgeCurrentWindowIndex;
    //construct and save a mesh from the pointers
    this->mesh = new Mesh(file, type);

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
    //delete the mesh
    delete this->mesh;
    //reset the window id
    this->windowIndex = -1;
    //clear the mesh
    this->mesh->vertices.clear();
    this->mesh->indices.clear();
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
    //set the depth function correct
    glDepthFunc(GL_GREATER);

    if (!glgeWindows[this->windowIndex]->isTranparentPass())
    {
        //disable color blending
        glDisable(GL_BLEND);
    }

    //initalise the mesh (this will only run once)
    this->mesh->init();
    //bind the mesh
    this->mesh->bind();

    //bind the object buffer
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->ubo);

    //bind the transparent extra data
    if (this->isTransparent)
    {
        //pass the current pass
        this->shader.setCustomInt("glgePass", glgeWindows[this->windowIndex]->isTranparentPass());
    }

    //bind the shader
    this->shader.applyShader();

    //Bind the material
    this->mat->apply();

    //draw the object
    glDrawElements(GL_TRIANGLES, this->mesh->indices.size(), GL_UNSIGNED_INT, 0);
    //check if debug data gathering is enabled
    if (glgeGatherDebugInfo)
    {
        //increment the amount of draw passes
        glgeDrawCallCountT++;
        //increment the amount of triangles draw by the own triangle count
        glgeTriangleCountT += this->mesh->indices.size() / 3;
    }

    //unbind the material
    this->mat->remove();

    //unbind the shader
    this->shader.removeShader();
    //unbind the mesh
    this->mesh->unbind();
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
        this->mat->update(this->shader.getShader());
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
    this->transf.rot = vec3(std::fmod(r.x, 2*M_PI),std::fmod(r.y, 2*M_PI),std::fmod(r.z, 2*M_PI));
}

//set the rotation of the object
void Object::setRotation(float x, float y, float z)
{
    //pass to another function
    this->setRotation(vec3(x,y,z));
}

//rotate the object
void Object::rotate(vec3 r)
{
    //rotate the object
    this->setRotation(this->transf.rot + r);
}

//rotate the object
void Object::rotate(float x, float y, float z)
{
    //pass to another function
    this->rotate(vec3(x,y,z));
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
    this->mesh->recalculateNormals();
}

void Object::setOnlyMesh(Mesh* mesh)
{

    //check if the mesh is a nullpointer
    if (!mesh)
    {
        //throw an error
        GLGE_THROW_ERROR("Nullpointer is not a valid mesh")
        return;
    }
    //store the inputed mesh
    this->mesh = mesh;
}

void Object::setMesh(Mesh* m)
{
    //set the buffer
    this->setOnlyMesh(m);
    //update the mesh
    this->mesh->update();
}

Mesh* Object::getMesh()
{
    //return the stored mesh
    return this->mesh;
}

void Object::setMaterial(Material* mat)
{
    //store the inputed material
    this->mat = mat;
    //update it to bind it to the window and apply the shader
    this->mat->update(this->shader.getShader(),true);
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
    dat->writeLong(this->mesh->vertices.size());
    //loop over every vertex
    for (Vertex vert : this->mesh->vertices)
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
    dat->writeLong(this->mesh->indices.size());
    //loop over every vertex
    for (unsigned int ind : this->mesh->indices)
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
    Mesh m;

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

    //say that the window index is the one from the current window
    this->windowIndex = glgeCurrentWindowIndex;
    //construct and save a mesh from the pointers
    this->mesh = new Mesh(m.vertices, m.indices);

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

    //initalse the mesh
    this->mesh->init();

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

void Object::shaderSetup(const char* vs, const char* fs)
{
    //compile and save the shader
    this->shader = glgeCompileShader(vs, fs);

    //get all uniforms
    this->getUniforms();
    //check if a material is bound
    if (this->mat)
    {
        //update the material with the shader
        mat->update(this->shader.getShader(), true);
    }
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
            std::cos(lights[i]->getIntenseAngle()), 
            std::cos(lights[i]->getAngle()),
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

    //initalise the mesh (this will only run once)
    this->mesh->init();
    //bind the mesh
    this->mesh->bind();
    //bind the buffers
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->ubo);

    glDrawElements(GL_TRIANGLES, this->mesh->indices.size(), GL_UNSIGNED_INT, 0);
    //check if debug data gathering is enabled
    if (glgeGatherDebugInfo)
    {
        //increment the amount of draw passes
        glgeDrawCallCountT++;
        //increment the amount of triangles draw by the own triangle count
        glgeTriangleCountT += this->mesh->indices.size() / 3;
    }

    //unbind the texture for the shadow map
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    //unbind the mesh
    this->mesh->unbind();

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
    this->camData.fov = FOV;
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
void Camera::setPos(vec3 p)
{
    //apply the new position
    this->transf.pos = p;
}

//set the position of the camera
void Camera::setPos(float x, float y, float z)
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
    this->transf.pos.x += (x - z) * std::sin(this->transf.rot.y);
    //do the same as for x, but change the order of x and z and use cosine
    this->transf.pos.z += (z - x) * std::cos(this->transf.rot.y);
}

vec3 Camera::getPos()
{
    //return the camera position
    return this->transf.pos;
}

//set the rotation of the camera
void Camera::setRot(vec3 r)
{
    //apply the new rotation
    this->transf.rot = vec3(std::fmod(r.x, 2*M_PI),std::fmod(r.y, 2*M_PI),std::fmod(r.z, 2*M_PI));
}

//set the rotation of the camera
void Camera::setRot(float x, float y, float z)
{
    //pass to another function
    this->setRot(vec3(x,y,z));
}

//rotate the camera
void Camera::rotate(vec3 r)
{
    //rotate the camera
    this->setRot(this->transf.rot + r);
}

//rotate the camera
void Camera::rotate(float x, float y, float z)
{
    //pass to another function
    this->rotate(vec3(x,y,z));
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
    this->camData.fov = f;
}

//change the field of view
void Camera::sizeFOV(float df)
{
    //change the fov
    this->camData.fov += df;
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

mat4 Camera::getViewMatrix()
{
    return this->camData.transMat * this->camData.rotMat;
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
    view = glgeRotateVector(transf.rot.y,yaxis, view);
    //normaise the vector
    view.normalize();

    //rotate the view vector and calculate U
    //calculate the cross product of the yaxis and the view vector
    vec3 U = view.cross(yaxis);
    //normalise the U vector
    U.normalize();
    //rotate the view vector around U by the rotation on the y axis
    view = glgeRotateVector(transf.rot.x, U, view);
    
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


////////
//MESH//
////////

//default constructor
Mesh::Mesh()
{
    //init the object
}

Mesh::Mesh(unsigned int preset, vec4 color, unsigned int resolution)
{
    //pass to the super constructor
    this->superPres(preset, color, resolution);
}

//pointer array constructor
Mesh::Mesh(Vertex* vertices, unsigned int* indices, unsigned int sizeVertices, unsigned int sizeIndices)
{
    //convert the pointer arrays to vectors and save them
    this->vertices = std::vector<Vertex>(vertices, vertices + (sizeVertices/sizeof(vertices[0])));
    this->indices = std::vector<unsigned int>(indices, indices + (sizeIndices/sizeof(indices[0])));
    //pass to the super constructor
    this->superVec(this->vertices, this->indices);
}

//vector constructor
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    //pass to the super constructor
    this->superVec(vertices, indices);
}

Mesh::Mesh(std::string data, int type)
{
    //pass to the super constructor
    this->superFile(data, type);
}

Mesh::Mesh(const char* file, int type)
{
    //store teh data of the file
    std::string data;
    //read the file
    readFile(file, data);
    //pass to the super constructor
    this->superFile(data, type);
}

void Mesh::superVec(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    //store the inputed vertices
    this->vertices = vertices;
    //store the inputed indices
    this->indices = indices;
}

void Mesh::superPres(unsigned int preset, vec4 color, unsigned int resolution)
{

    //store the indices for the object
    std::vector<unsigned int> indices = {};
    //store the vertices for the object
    std::vector<Vertex> vertices = {};
    
    //check the type of used preset
    switch (preset)
    {
    //check for empty objects
    case GLGE_PRESET_EMPTY:
        {
            //clear the indices
            indices.clear();
            //clear the vertices
            vertices.clear();

            //store the vertices
            this->vertices = vertices;
            //store the indices
            this->indices = indices;
        }
        break;

    //generate a cube
    case GLGE_PRESET_CUBE:
        {
            //set the indices for the cube
            indices = {
                //front and back
                0,2,1,
                1,2,3,
                4,5,6,
                5,7,6,
                //top and bottom
                0+8,1+8,4+8,
                1+8,5+8,4+8,
                2+8,6+8,3+8,
                3+8,6+8,7+8,
                //left and right
                0+16,4+16,2+16,
                2+16,4+16,6+16,
                1+16,3+16,5+16,
                3+16,7+16,5+16
            };

            //check if texture or color should be used
            if (color.w == -1)
            {
                //use texture coordinates

                //load the vertices from an table
                vertices = {
                                 //pos   //tex   //normal
                         //front and back
                         Vertex(-1,-1, 1,  0,0,  0, 0, 1),
                         Vertex( 1,-1, 1,  1,0,  0, 0, 1),
                         Vertex(-1, 1, 1,  0,1,  0, 0, 1),
                         Vertex( 1, 1, 1,  1,1,  0, 0, 1),

                         Vertex(-1,-1,-1,  1,0,  0, 0,-1),
                         Vertex( 1,-1,-1,  0,0,  0, 0,-1),
                         Vertex(-1, 1,-1,  1,1,  0, 0,-1),
                         Vertex( 1, 1,-1,  0,1,  0, 0,-1),

                         //top and bottom
                         Vertex(-1,-1, 1,  0,1,  0,-1, 0),
                         Vertex( 1,-1, 1,  1,1,  0,-1, 0),
                         Vertex(-1, 1, 1,  0,0,  0, 1, 0),
                         Vertex( 1, 1, 1,  1,0,  0, 1, 0),

                         Vertex(-1,-1,-1,  0,0,  0,-1, 0),
                         Vertex( 1,-1,-1,  1,0,  0,-1, 0),
                         Vertex(-1, 1,-1,  0,1,  0, 1, 0),
                         Vertex( 1, 1,-1,  1,1,  0, 1, 0),

                         //left and right
                         Vertex(-1,-1, 1,  1,0, -1, 0, 0),
                         Vertex( 1,-1, 1,  0,0,  1, 0, 0),
                         Vertex(-1, 1, 1,  1,1, -1, 0, 0),
                         Vertex( 1, 1, 1,  0,1,  1, 0, 0),

                         Vertex(-1,-1,-1,  0,0, -1, 0, 0),
                         Vertex( 1,-1,-1,  1,0,  1, 0, 0),
                         Vertex(-1, 1,-1,  0,1, -1, 0, 0),
                         Vertex( 1, 1,-1,  1,1,  1, 0, 0)
                };
            }
            else
            {
                //use color

                //load the vertices
                //load the vertices from an table
                vertices = {
                                 //pos   //color   //normal
                         //front and back
                         Vertex(-1,-1, 1,  color,  0, 0, 1),
                         Vertex( 1,-1, 1,  color,  0, 0, 1),
                         Vertex(-1, 1, 1,  color,  0, 0, 1),
                         Vertex( 1, 1, 1,  color,  0, 0, 1),

                         Vertex(-1,-1,-1,  color,  0, 0,-1),
                         Vertex( 1,-1,-1,  color,  0, 0,-1),
                         Vertex(-1, 1,-1,  color,  0, 0,-1),
                         Vertex( 1, 1,-1,  color,  0, 0,-1),

                         //top and bottom
                         Vertex(-1,-1, 1,  color,  0,-1, 0),
                         Vertex( 1,-1, 1,  color,  0,-1, 0),
                         Vertex(-1, 1, 1,  color,  0, 1, 0),
                         Vertex( 1, 1, 1,  color,  0, 1, 0),

                         Vertex(-1,-1,-1,  color,  0,-1, 0),
                         Vertex( 1,-1,-1,  color,  0,-1, 0),
                         Vertex(-1, 1,-1,  color,  0, 1, 0),
                         Vertex( 1, 1,-1,  color,  0, 1, 0),

                         //left and right
                         Vertex(-1,-1, 1,  color, -1, 0, 0),
                         Vertex( 1,-1, 1,  color,  1, 0, 0),
                         Vertex(-1, 1, 1,  color, -1, 0, 0),
                         Vertex( 1, 1, 1,  color,  1, 0, 0),

                         Vertex(-1,-1,-1,  color, -1, 0, 0),
                         Vertex( 1,-1,-1,  color,  1, 0, 0),
                         Vertex(-1, 1,-1,  color, -1, 0, 0),
                         Vertex( 1, 1,-1,  color,  1, 0, 0)
                };
            }
            //store the vertices
            this->vertices = vertices;
            //store the indices
            this->indices = indices;
        }
        break;

    //generate a circle
    case GLGE_PRESET_CIRCLE:
        {
            //clear the indices and vertices
            this->vertices.clear();
            this->indices.clear();
            //add a circle
            addCircle(&this->vertices, &this->indices, resolution, color);
        }
        break;
    
    //generate a plane
    case GLGE_PRESET_PLANE:
        {
            //generate the indices
            indices = {0,1,2,
                       2,1,3};

            //check if color should be used
            if (color.w == -1)
            {
                //use texture coordinates

                vertices = {
                    Vertex( 1,0, 1,  1,1,  0,1,0), Vertex(-1,0, 1,  0,1,  0,1,0),
                    Vertex( 1,0,-1,  1,0,  0,1,0), Vertex(-1,0,-1,  0,0,  0,1,0)
                };
            }
            else
            {
                //use the color parameter

                vertices = {
                    Vertex( 1,0, 1,  color,  0,1,0), Vertex(-1,0, 1,  color,  0,1,0),
                    Vertex( 1,0,-1,  color,  0,1,0), Vertex(-1,0,-1,  color,  0,1,0)
                };
            }
            //store the vertices
            this->vertices = vertices;
            //store the indices
            this->indices = indices;
        }
        break;

    //generate a uv sphere
    case GLGE_PRESET_SPHERE:
        {
            //generate the list of vertices
            std::vector<Vertex> vertices;
            //generate the list of indices
            std::vector<unsigned int> indices;

            //thanks to Pickachuxxxx for the math. Source : https://gist.github.com/Pikachuxxxx/5c4c490a7d7679824e0e18af42918efc

            //check for 0 to replace it with the default
            if (resolution == 0)
            {
                //use 16 as default
                resolution = 16;
            }

            int latitudes = resolution;
            int longitudes = resolution;

            float radius = 1.f;

            // Temporary vertex

            float deltaLatitude = M_PI / latitudes;
            float deltaLongitude = 2 * M_PI / longitudes;
            float latitudeAngle;
            float longitudeAngle;

            // Compute all vertices first except normals
            for (int i = 0; i <= latitudes; i++)
            {
                latitudeAngle = M_PI / 2 - i * deltaLatitude; /* Starting -pi/2 to pi/2 */
                float xy = radius * cosf(latitudeAngle);    /* r * cos(phi) */
                float z = radius * sinf(latitudeAngle);     /* r * sin(phi )*/

                /*
                * We add (latitudes + 1) vertices per longitude because of equator,
                * the North pole and South pole are not counted here, as they overlap.
                * The first and last vertices have same position and normal, but
                * different tex coords.
                */
                for (int j = 0; j <= longitudes; ++j)
                {
                    longitudeAngle = j * deltaLongitude;

                    Vertex vertex;
                    vertex.pos.x = xy * cosf(longitudeAngle);       /* x = r * cos(phi) * cos(theta)  */
                    vertex.pos.z = xy * sinf(longitudeAngle);       /* y = r * cos(phi) * sin(theta) */
                    vertex.pos.y = z;                               /* z = r * sin(phi) */
                    if (color.w == -1)
                    {
                        vertex.texCoord.x = (float)j/longitudes;        /* s */
                        vertex.texCoord.y = (float)i/latitudes;         /* t */
                    }
                    else
                    {
                        vertex.color = color;
                    }

                    //compute vertex normal
                    vertex.normal = vertex.pos;
                    vertex.normal.normalize();
                    //store the vertex
                    vertices.push_back(vertex);
                }
            }

            /*
            *  Indices
            *  k1--k1+1
            *  |  / |
            *  | /  |
            *  k2--k2+1
            */
            unsigned int k1, k2;
            for(int i = 0; i < latitudes; ++i)
            {
                k1 = i * (longitudes + 1);
                k2 = k1 + longitudes + 1;
                // 2 Triangles per latitude block excluding the first and last longitudes blocks
                for(int j = 0; j < longitudes; ++j, ++k1, ++k2)
                {
                    if (i != 0)
                    {
                        indices.push_back(k1);
                        indices.push_back(k2);
                        indices.push_back(k1 + 1);
                    }

                    if (i != (latitudes - 1))
                    {
                        indices.push_back(k1 + 1);
                        indices.push_back(k2);
                        indices.push_back(k2 + 1);
                    }
                }
            }

            //store the final lists
            this->vertices = vertices;
            this->indices = indices;
        }
        break;

    case GLGE_PRESET_ICOSPHERE:
        {
            //check for 0 to replace it with the default
            if (resolution == 0)
            {
                //use 1 as default
                resolution = 1;
            }

            // create 20 triangles of the icosahedron
            std::vector<Vertex> vertices;

            // create 12 vertices of a icosahedron
            float t = (1.0 + std::sqrt(5.0)) / 2.0;

            addVertexOnUintSphere(Vertex(-1,  t,  0), color, &vertices);
            addVertexOnUintSphere(Vertex( 1,  t,  0), color, &vertices);
            addVertexOnUintSphere(Vertex(-1, -t,  0), color, &vertices);
            addVertexOnUintSphere(Vertex( 1, -t,  0), color, &vertices);

            addVertexOnUintSphere(Vertex( 0, -1,  t), color, &vertices);
            addVertexOnUintSphere(Vertex( 0,  1,  t), color, &vertices);
            addVertexOnUintSphere(Vertex( 0, -1, -t), color, &vertices);
            addVertexOnUintSphere(Vertex( 0,  1, -t), color, &vertices);

            addVertexOnUintSphere(Vertex( t,  0, -1), color, &vertices);
            addVertexOnUintSphere(Vertex( t,  0,  1), color, &vertices);
            addVertexOnUintSphere(Vertex(-t,  0, -1), color, &vertices);
            addVertexOnUintSphere(Vertex(-t,  0,  1), color, &vertices);

            //add the indices
            std::vector<unsigned int> indices;

            // 5 faces around point 0
            indices.push_back(11); indices.push_back(0); indices.push_back(5);
            indices.push_back(5); indices.push_back(0); indices.push_back(1);
            indices.push_back(1); indices.push_back(0); indices.push_back(7);
            indices.push_back(7); indices.push_back(0); indices.push_back(10);
            indices.push_back(10); indices.push_back(0); indices.push_back(11);

            // 5 adjacent faces
            indices.push_back(5); indices.push_back(1); indices.push_back(9);
            indices.push_back(11); indices.push_back(5); indices.push_back(4);
            indices.push_back(10); indices.push_back(11); indices.push_back(2);
            indices.push_back(7); indices.push_back(10); indices.push_back(6);
            indices.push_back(1); indices.push_back(7); indices.push_back(8);

            // 5 faces around point 3
            indices.push_back(9); indices.push_back(3); indices.push_back(4);
            indices.push_back(4); indices.push_back(3); indices.push_back(2);
            indices.push_back(2); indices.push_back(3); indices.push_back(6);
            indices.push_back(6); indices.push_back(3); indices.push_back(8);
            indices.push_back(8); indices.push_back(3); indices.push_back(9);

            // 5 adjacent faces
            indices.push_back(9); indices.push_back(4); indices.push_back(5);
            indices.push_back(4); indices.push_back(2); indices.push_back(11);
            indices.push_back(2); indices.push_back(6); indices.push_back(10);
            indices.push_back(6); indices.push_back(8); indices.push_back(7);
            indices.push_back(8); indices.push_back(9); indices.push_back(1);

            // refine triangles
            for (int i = 0; i < (int)resolution; i++)
            {
                std::vector<unsigned int> newIndices;

                for (int t = 0; t < (int)indices.size(); t += 3)
                {
                    Vertex a = vertices[indices[t]];
                    Vertex b = vertices[indices[t+1]];
                    Vertex c = vertices[indices[t+2]];

                    vec3 m0 = (a.pos + b.pos) / vec3(2);
                    vec3 m1 = (b.pos + c.pos) / vec3(2);
                    vec3 m2 = (c.pos + a.pos) / vec3(2);

                    unsigned int m0i = addVertexOnUintSphere(m0, color, &vertices);
                    unsigned int m1i = addVertexOnUintSphere(m1, color, &vertices);
                    unsigned int m2i = addVertexOnUintSphere(m2, color, &vertices);

                    newIndices.push_back(indices[t  ]);newIndices.push_back(m0i);newIndices.push_back(m2i);
                    newIndices.push_back(indices[t+1]);newIndices.push_back(m1i);newIndices.push_back(m0i);
                    newIndices.push_back(indices[t+2]);newIndices.push_back(m2i);newIndices.push_back(m1i);
                    newIndices.push_back(m0i);newIndices.push_back(m1i);newIndices.push_back(m2i);
                }
                indices.clear();
                indices = newIndices;
            }

            //store the generated icosphere
            this->vertices = vertices;
            this->indices = indices;
        }
        break;

    //add a cone
    case GLGE_PRESET_CONE:
        {
            //first, add a circle
            addCircle(&this->vertices, &this->indices, resolution, color, vec3(0,-1,0), true, true, true);
            //add one vertex on top
            //create the vertex at the position
            Vertex vert = Vertex(vec3(0,1,0), vec2(0.5,1), vec3(0,1,0));
            //check if color should be added
            if (color.w != -1)
            {
                //add the color
                vert.color = color;
            }
            //store the index of this vertex
            unsigned int ver_ind = (int)this->vertices.size();
            //add the vertex
            this->vertices.push_back(vert);
            //loop over all vertices
            for (int i = 0; i < (int)ver_ind - 1; i++)
            {
                //add a triangle
                this->indices.push_back(ver_ind);
                this->indices.push_back(i+1);
                this->indices.push_back(i);
            }
            //add the last triangle
            this->indices.push_back(ver_ind);
            this->indices.push_back(0);
            this->indices.push_back(ver_ind-1);
        }
        break;

    case GLGE_PRESET_CYLINDER:
        {
            //first, add a circle
            addCircle(&this->vertices, &this->indices, resolution, color, vec3(0,-1,0), true, true, true);
            //store the amount of vertices per circle
            int circAmount = (int)this->vertices.size();
            //then, add a second circle
            addCircle(&this->vertices, &this->indices, resolution, color, vec3(0,1,0), false, true, true);
            //add a circle of vertices for texture coordinates
            addCircle(&this->vertices, &this->indices, resolution, color, vec3(0,-1,0), false, false, true, vec2(0), 1);
            //add an unfilled circle
            addCircle(&this->vertices, &this->indices, resolution, color, vec3(0,1,0), false, false, true, vec2(0,1), 1);

            //start to fill the edges
            for (int i = (int)this->vertices.size() - circAmount*2; i < (int)this->vertices.size()-(circAmount+1); i++)
            {
                //add a quad
                //add the first tirangle
                this->indices.push_back(i+1);
                this->indices.push_back(i);
                this->indices.push_back(i + circAmount);
                //add the second triangle
                this->indices.push_back(i+1+circAmount);
                this->indices.push_back(i+1);
                this->indices.push_back(i+circAmount);
            }
            int i = (int)this->vertices.size() - circAmount*2;
            //add the last quad
            //add the first tirangle
            this->indices.push_back(i);
            this->indices.push_back(i+(circAmount-1));
            this->indices.push_back(i + circAmount);
            //add the second triangle
            this->indices.push_back(i+(circAmount-1));
            this->indices.push_back(i+(circAmount*2 - 1));
            this->indices.push_back(i+circAmount);
        }
        break;
    
    //default / error case
    default:
        {
            //check if an error should print
            if (glgeErrorOutput)
            {
                //print an error message
                std::cerr << "[GLGE ERROR] Invalid preset used to construct an 3D object\n";
            }
            //check if error should stop the program
            if (glgeExitOnError)
            {
                //close the program with an error
                exit(1);
            }
            //stop the function
            return;
        }
        break;
    }
}

void Mesh::superFile(std::string data, int type)
{
    if (type != GLGE_OBJ)
    {
        std::cerr << "Currently, only .obj files are supported" << "\n";
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

Mesh::~Mesh()
{
    //clear the vertices
    this->vertices.clear();
    //clear the indices
    this->indices.clear();
    //delete the VBO
    glDeleteBuffers(1, &this->VBO);
    //delete the IBO
    glDeleteBuffers(1, &this->IBO);
}

void Mesh::recalculateNormals()
{
    for (int i = 0; i < (int)this->indices.size()/3; i++)
    {
        vec3 edge1 = this->vertices[indices[i+1]].pos - this->vertices[indices[i]].pos;
        vec3 edge2 = this->vertices[indices[i+1]].pos - this->vertices[indices[i+2]].pos;
        
        vec3 normal = edge1.cross(edge2);
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

void Mesh::applyTransform(Transform transform)
{
    //get the transformation matrix
    mat4 transfMat = transform.getMatrix();
    //get the rotation matrix
    mat4 rotMat = transform.getRotationMatrix();

    //loop over all vertices
    for (int i = 0; i < (int)this->vertices.size(); i++)
    {
        //calculate a 4d position vector
        vec4 temp = transfMat * vec4(this->vertices[i].pos,1);
        //store the 3d part of the vector
        this->vertices[i].pos = vec3(temp.x, temp.y, temp.z);
        //calculate a 4d normal vector
        temp = rotMat * vec4(this->vertices[i].normal, 1);
        //store the 3d part of the vector
        this->vertices[i].normal = vec3(temp.x, temp.y, temp.z);
    }
}

Mesh Mesh::join(Mesh mesh)
{
    //create a list of vertices and copy the own vertices into it
    std::vector<Vertex> newVerts = this->vertices;
    //copy the other mesh vertices into it too
    newVerts.insert(newVerts.end(), mesh.vertices.begin(), mesh.vertices.end());
    
    //create a list of indices and copy the own indices into it
    std::vector<unsigned int> newInds = this->indices;
    //loop over the vertices of the other mesh
    for (int i = 0; i < (int)mesh.indices.size(); i++)
    {
        //add a single index from the other mesh to the new mesh and move it up by the amount of own indices
        newInds.push_back(mesh.indices[i] + (int)this->vertices.size());
    }
    //return a new mesh from the new indices and vertices
    return Mesh(newVerts, newInds);
}

void Mesh::joinThis(Mesh mesh)
{
    //join the mesh
    Mesh m = this->join(mesh);
    //copy the vertices
    this->vertices = m.vertices;
    //copy the indices
    this->indices = m.indices;
}

void Mesh::operator+=(Mesh mesh)
{
    //pass to the joinThis function
    this->joinThis(mesh);
}

Mesh Mesh::operator+(Mesh mesh)
{
    //pass to the join function
    return this->join(mesh);
}

void Mesh::update()
{
    //check if this mesh is allready initalised
    if ((this->VBO == 0) || (this->IBO == 0))
    {
        //throw an error
        GLGE_THROW_ERROR("You must initalise a mesh before updating it")
        return;
    }

    //bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    //store the mesh data
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(this->vertices[0]), this->vertices.data(), GL_STATIC_DRAW);
    //unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //bind the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
    //store the mesh data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(this->indices[0]), this->indices.data(), GL_STATIC_DRAW);
    //unbind the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::init()
{
    //check if this mesh is allready initalised
    if (!((this->VBO == 0) || (this->IBO == 0))) { return; }

    //create the new VBO buffer
    glCreateBuffers(1, &this->VBO);
    //bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    //store the mesh data
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(this->vertices[0]), this->vertices.data(), GL_STATIC_DRAW);
    //unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //create the IBO buffer
    glCreateBuffers(1, &this->IBO);
    //bind the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
    //store the mesh data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(this->indices[0]), this->indices.data(), GL_STATIC_DRAW);
    //unbind the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //store the window id
    this->windowID = glgeCurrentWindowIndex;
}

void Mesh::bind()
{
    //check if the window id is the window the mesh was created in
    if (this->windowID != glgeCurrentWindowIndex)
    {
        //throw an error
        GLGE_THROW_ERROR("Can't bind a mesh in a window it was not created in")
        return;
    }
    //bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    //bind the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);

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


}

void Mesh::unbind()
{
    //check if the window id is the window the mesh was created in
    if (this->windowID != glgeCurrentWindowIndex)
    {
        //throw an error
        GLGE_THROW_ERROR("Can't bind a mesh in a window it was not created in")
        return;
    }
    //deactivate the sub elements
    //deactivate the position argument
    glDisableVertexAttribArray(0);
    //deactivate the color argument
    glDisableVertexAttribArray(1);
    //deactivate the texCoord argument
    glDisableVertexAttribArray(2);
    //deactivate the normal argument
    glDisableVertexAttribArray(3);

    //unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //nubind the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}