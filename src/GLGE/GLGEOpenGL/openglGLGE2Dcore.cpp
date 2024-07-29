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
#include "../GLGEIndependend/glgePrivDefines.hpp"
#include "openglGLGEFuncs.hpp"
#include "openglGLGEVars.hpp"
#include "../GLGEIndependend/GLGEData.h"

//the needed default C++ libs
#include <math.h>
#include <iostream>
#include <algorithm>

/////////////////////
// LOCAL VARIABLES //
/////////////////////

/**
 * @brief store the main 2D camera
 */
Camera2D* mainCam;


///////////////////////
// PRIVATE FUNCTIONS //
///////////////////////

/**
 * @brief add a polygon / circle to an exisiting mesh
 * 
 * @param vertices the allready existing 2D vertices
 * @param indices the allready existing indices
 * @param numbers the number of points for the polygon / the resolution of the circle
 * @param col the color for the polygon / cirlce
 */
inline void addCircle(std::vector<Vertex2D>* vertices, std::vector<unsigned int>* indices, unsigned int numbers, vec4 col)
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
        vec2 p = vec2(std::sin(c_angle), std::cos(c_angle));
        //create the vertex
        Vertex2D vert = Vertex2D(p);
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
            //normal circle texturing
            vert.texCoord = vec2(p.x * 0.5 + 0.5, 1.f - (p.y * 0.5 + 0.5));            
        }
        //add the vertex
        vertices->push_back(vert);
    }

    //generate the indices
    for (int i = 0; i < (int)points - 2; i++)
    {
        //create a triangle
        indices->push_back(first_ind);
        indices->push_back(i + 2 + first_ind);
        indices->push_back(i + 1 + first_ind);
    }
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
    //create the buffers
    this->createBuffers();
}

//constructor with vectors
Mesh2D::Mesh2D(std::vector<Vertex2D> vertices, std::vector<unsigned int> indices)
{
    //save the inputed vectors
    this->vertices = vertices;
    this->indices = indices;
    //create the buffers
    this->createBuffers();
}

//constructor with presets
Mesh2D::Mesh2D(unsigned int preset, unsigned int resolution, vec4 color)
{
    switch (preset)
    {
    case GLGE_PRESET_EMPTY:
        {
            //clear the indices
            this->indices.clear();
            //clear the vertices
            this->vertices.clear();
        }
        break;
    
    case GLGE_PRESET_SQUARE:
        {
            //load a preset of a square
            //store the vertices
            std::vector<Vertex2D> vertices;
            //case for texture coordinates
            if (color.w == -1)
            {
                //create the triangle
                //set the vertices for the triangle
                vertices = {Vertex2D( 1, 1, 1,  0),
                            Vertex2D( 1,-1, 1,  1),
                            Vertex2D(-1, 1, 0,  0),
                            Vertex2D(-1,-1, 0,  1)};
            }
            //specal case
            else if (color.w == -2)
            {
                //create the triangle
                //set the vertices for the triangle
                vertices = {Vertex2D( 1, 1, 1,0,0,1),
                            Vertex2D( 1,-1, 0,1,0,1),
                            Vertex2D(-1, 1, 0,1,0,1),
                            Vertex2D(-1,-1, 0,0,1,1)};
            }
            else
            {
                //create the triangle
                //set the vertices for the triangle
                vertices = {Vertex2D( 1, 1, color),
                            Vertex2D( 1,-1, color),
                            Vertex2D(-1, 1, color),
                            Vertex2D(-1,-1, color)};
            }
            //bind the vertices to an triangle1
            std::vector<unsigned int> indices = {1,0,2, 1,2,3};

            //store the indices
            this->vertices = vertices;
            this->indices = indices;
        }
        break;

    case GLGE_PRESET_TRIANGLE:
        {
            //store the vertices
            std::vector<Vertex2D> vertices;
            //case for texture coordinates
            if (color.w == -1)
            {
                //create the triangle
                //set the vertices for the triangle
                vertices = {Vertex2D(0,1,   0.5,0),
                            Vertex2D(-1,-1, 0,  1),
                            Vertex2D( 1,-1, 1,  1)};
            }
            //specal case
            else if (color.w == -2)
            {
                //create the triangle
                //set the vertices for the triangle
                vertices = {Vertex2D(0,1,   1,0,0,1),
                            Vertex2D(-1,-1, 0,1,0,1),
                            Vertex2D( 1,-1, 0,0,1,1)};
            }
            else
            {
                //create the triangle
                //set the vertices for the triangle
                vertices = {Vertex2D(0,1,   color),
                            Vertex2D(-1,-1, color),
                            Vertex2D( 1,-1, color)};
            }
            //bind the vertices to an triangle1
            std::vector<unsigned int> indices = {0,1,2};

            //store the indices
            this->vertices = vertices;
            this->indices = indices;
        }
        break;

    case GLGE_PRESET_POLYGON:
        {
            //default the resolution to 5
            if (resolution == 0)
            {
                resolution = 5;
            }
            //generate a polygon
            addCircle(&this->vertices, &this->indices, resolution, color);
        }
        break;

    case GLGE_PRESET_CIRCLE:
        {
            //generate a circle
            addCircle(&this->vertices, &this->indices, resolution, color);
        }
        break;

    case GLGE_PRESET_ARROW:
        {
            //create the vertices
            std::vector<Vertex2D> verts = {Vertex2D(-0.17, -1), Vertex2D(0.17, -1 ), Vertex2D(-0.17,0.5),
                                           Vertex2D( 0.17,0.5), Vertex2D(-0.48,0.5), Vertex2D( 0.48,0.5),
                                           Vertex2D(0,1)};


            //loop over all vertices
            for (int i = 0; i < (int)verts.size(); i++)
            {
                //check if color should be added
                if (color.w == -1)
                {
                    //if not, add texcoords
                    verts[i].texCoord = vec2(verts[i].pos.x * 0.5 + 0.5, 1.f - (verts[i].pos.y * 0.5 + 0.5));
                    //remove the color
                    verts[i].color = vec4(0);
                }
                else
                {
                    //add color
                    verts[i].color = color;
                }

                //exclude the first two vertices
                if (i < 2)
                { continue; }

                //move the position by the resolution
                verts[i].pos += vec2(0, resolution);
            }

            //create the indices
            std::vector<unsigned int> inds = {0,1,2, 2,1,3, 4,5,6};

            //load the mesh
            this->vertices = verts;
            this->indices = inds;
        }
        break;

    default:
        {
            //check if an error should print
            if (glgeErrorOutput)
            {
                //print an error message
                std::cerr << "[GLGE ERROR] Invalid preset used to construct a 2D object\n";
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
    //create the buffers
    this->createBuffers();
}

Mesh2D::~Mesh2D()
{
    //delete the mesh
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->IBO);
}

void Mesh2D::bind()
{
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
}

void Mesh2D::unbind()
{
    //unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //deactivate the sub elements
    //deactivate the position argument
    glDisableVertexAttribArray(0);
    //deactivate the color argument
    glDisableVertexAttribArray(1);
    //deactivate the texCoord argument
    glDisableVertexAttribArray(2);
}

void Mesh2D::createBuffers()
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices[0])*((int)this->vertices.size()), &(this->vertices[0]), GL_STATIC_DRAW);
    //unbind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //generate the index buffer
    glGenBuffers(1, &this->IBO);
    //bind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
    //store the index information in the index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices[0])*((int)this->indices.size()), &(this->indices[0]), GL_STATIC_DRAW);
    //unbind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh2D::updateVertexBuffer()
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
    //bind the vertex buffer object to store data
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    //store the mesh data in the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices[0])*((int)this->vertices.size()), &(this->vertices[0]), GL_STATIC_DRAW);
    //unbind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Mesh2D::updateIndexBuffer()
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
    //bind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
    //store the index information in the index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices[0])*((int)this->indices.size()), &(this->indices[0]), GL_STATIC_DRAW);
    //unbind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh2D::recalculateBuffers()
{
    //recalculate the VBO
    this->updateVertexBuffer();
    //recalculate the IBO
    this->updateIndexBuffer();
}

void Mesh2D::setData(std::vector<Vertex2D> vertices, std::vector<unsigned int> indices)
{
    //store the new vercites
    this->vertices = vertices;
    //store the new indices
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
    this->mesh = new Mesh2D(vertices, indices, sizeOfVertices, sizeOfFaces);
    //save the current window
    this->windowID = glgeCurrentWindowIndex;

    //save the transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;

    //set the base 2D shader
    this->shader = new Shader(glgeWindows[this->windowID]->getDefault2DShader());
    //store the default move matrix
    this->shader->setCustomMat3("glgeCamMat", mat3());
    //recalculate the uniforms
    this->shader->recalculateUniforms();
    //get the UUID
    this->id = glgeObjectUUID;

    //update the object
    this->update();
}

//constructor using vectors
Object2D::Object2D(std::vector<Vertex2D> vertices, std::vector<unsigned int> indices, Transform2D transform, bool isStatic)
{
    //save an mesh created from the two vectors
    this->mesh = new Mesh2D(vertices, indices);
    //save the current window
    this->windowID = glgeCurrentWindowIndex;

    //save the transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;

    //set the base 2D shader
    this->shader = new Shader(glgeWindows[this->windowID]->getDefault2DShader());
    //store the default move matrix
    this->shader->setCustomMat3("glgeCamMat", mat3());
    //recalculate the uniforms
    this->shader->recalculateUniforms();
    //get the UUID
    this->id = glgeObjectUUID;
    //increase the object count
    glgeObjectUUID++;

    //update the object
    this->update();
}

Object2D::Object2D(Mesh2D* mesh, Transform2D transform, bool isStatic)
{
    //save an mesh
    this->mesh = mesh;
    //save the current window
    this->windowID = glgeCurrentWindowIndex;

    //save the transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;

    //set the base 2D shader
    this->shader = new Shader(glgeWindows[this->windowID]->getDefault2DShader());
    //store the default move matrix
    this->shader->setCustomMat3("glgeCamMat", mat3());
    //recalculate the uniforms
    this->shader->recalculateUniforms();
    //get the UUID
    this->id = glgeObjectUUID;
    //increase the object count
    glgeObjectUUID++;

    //update the object
    this->update();
}

Object2D::Object2D(unsigned int preset, vec4 color, unsigned int resolution, Transform2D transform, bool isStatic)
{
    //save an mesh
    this->mesh = new Mesh2D(preset, resolution, color);
    //save the current window
    this->windowID = glgeCurrentWindowIndex;

    //save the transform
    this->transf = transform;

    //save if the object is static
    this->isStatic = isStatic;

    //set the base 2D shader
    this->shader = new Shader(glgeWindows[this->windowID]->getDefault2DShader());
    //store the default move matrix
    this->shader->setCustomMat3("glgeCamMat", mat3());
    //recalculate the uniforms
    this->shader->recalculateUniforms();
    //get the UUID
    this->id = glgeObjectUUID;
    //increase the object count
    glgeObjectUUID++;

    //update the object
    this->update();
}

Object2D::~Object2D()
{
    //delete the mesh
    delete this->mesh;
    //delete the texture
    delete this->texture;
    //delete the shader
    delete this->shader;
}

void Object2D::draw()
{
    //check if this is the transparent pass
    if (glgeWindows[this->windowID]->isTranparentPass())
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
    //enable color blending
    glEnable(GL_BLEND);
    //setup the blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //bind the mesh
    this->mesh->bind();

    //apply the shader
    this->shader->applyShader();

    //check if a texture exists
    if (this->texture)
    {
        //bind the texture to unit 0
        this->texture->bind(0, GLGE_TEXTURE_BIND_TEXTURE_UNIT, true);
    }

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

    //unbind the mesh
    this->mesh->unbind();
    //unbind the shader
    this->shader->removeShader();
    //check if a texture exists
    if (this->texture)
    {
        //unbind the texture
        this->texture->unbind();
    }
}

//update the object
void Object2D::update()
{
    //update the move matrix
    this->recalculateMoveMatrix();
    //store the matrix
    mat3 gpuMat = this->moveMat;
    //check if a camera is bound and if this is not static
    if (mainCam && !this->isStatic)
    {
        //multiply the mat with the camera mat
        gpuMat *= mainCam->getMatrix();
    }
    //push the matrix to the gpu
    this->shader->setCustomMat3("glgeCamMat", gpuMat);
}

void Object2D::setMesh(Mesh2D* mesh)
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

Mesh2D* Object2D::getMesh()
{
    //return the mesh
    return this->mesh;
}

void Object2D::setShader(Shader* shader)
{
    //check if the shader exists
    if (!shader)
    {
        //throw an error
        GLGE_THROW_ERROR("Nullpointer is not a valid shader")
        return;
    }
    //delete the old shader
    delete this->shader;
    //load the shader
    this->shader = shader;
    //set the move matrix uniform
    this->shader->setCustomMat3("glgeCamMat", mat3());
    //recalculate the uniforms
    this->shader->recalculateUniforms();
}

void Object2D::setShader(unsigned int shader)
{
    //pass to another function
    this->setShader(new Shader(shader));
}

void Object2D::setShader(std::string vs, std::string fs)
{
    //pass to another function
    this->setShader(new Shader(vs,fs));
}

Shader* Object2D::getShader()
{
    //return the shader
    return this->shader;    
}

void Object2D::setTexture(const char* file)
{
    //compile and store the texture
    this->texture = new Texture(file);
}

void Object2D::setTexture(Texture* texture)
{
    //check if the inputed texture exists
    if (!texture)
    {
        //throw an error
        GLGE_THROW_ERROR("Nullpointer is not a valid texture")
        return;
    }
    //store the texture
    this->texture = texture;
}

Texture* Object2D::getTexture()
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
    this->transf.pos += vec2(speedX*std::sin(dir), speedY*std::cos(dir));
}

void Object2D::move(float speed)
{
    //change the position
    this->transf.pos += vec2(speed*std::sin(this->transf.rot), speed*std::cos(this->transf.rot));
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
    this->setRot(this->transf.rot + (angle));
}

void Object2D::setRot(float dir)
{
    //set the rotation of the object
    this->transf.rot = std::fmod(dir,2*M_PI);
}

float Object2D::getRot()
{
    //return the rotation in degrees
    return this->transf.rot;
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

Data* Object2D::encode()
{
    //store the data while it is being set up
    Data* dat = new Data();

    //early encoding hook
    this->encodeHookEarly(dat);
    
    /*
     * Store the vertices
     */
    //Store the amount of vertices
    dat->writeLong(this->mesh->vertices.size());
    //loop over all the vertices
    for (Vertex2D vert : this->mesh->vertices)
    {
        //store the vertex position
        dat->writeVec2(vert.pos);
        //store the vertex color;
        dat->writeVec4(vert.color);
        //store the vertex texture coordinate
        dat->writeVec2(vert.texCoord);
    }
    
    /*
     * Store the indices
     */
    //store the amount of indices
    dat->writeLong(this->mesh->indices.size());
    //loop over all the indices
    for (unsigned int ind : this->mesh->indices)
    {
        //store the index
        dat->writeUInt(ind);
    }

    /*
     * Store the transform
     */
    //store the position
    dat->writeVec2(this->transf.pos);
    //store the rotation
    dat->writeFloat(this->transf.rot);
    //store the scaling
    dat->writeVec2(this->transf.size);

    /*
     * Store object attributes
     */
    //store the object UUID
    dat->writeUInt(this->id);
    //store the window id
    dat->writeInt(this->windowID);
    //store if the object is static
    dat->writeBool(this->isStatic);
    //store the anchor position
    dat->writeVec2(this->anchor);

    //late hook
    this->encodeHook(dat);

    //return the finished object data
    return dat;
}

void Object2D::decode(Data dat)
{
    //call early decode hook
    this->decodeHookEarly(&dat);

    //store all vectices
    std::vector<Vertex2D> vertices;
    //store all indices
    std::vector<unsigned int> indices;
    /*
     * get the vertices
     */
    //get the amount of vertices
    long vs = dat.readLong();
    //loop over all the vertices
    for (long i = 0; i < vs; i++)
    {
        //create a new vertex
        Vertex2D v;
        //get the vertex position
        v.pos = dat.readVec2();
        //get the vertex color;
        v.color = dat.readVec4();
        //get the vertex texture coordinate
        v.texCoord = dat.readVec2();
        //store the vertex
        vertices.push_back(v);
    }
    
    /*
     * get the indices
     */
    //get the amount of indices
    long is = dat.readLong();
    //loop over all the indices
    for (long i = 0; i < is; i++)
    {
        //store the index
        indices.push_back(dat.readUInt());
    }

    //store the mesh
    this->mesh = new Mesh2D(vertices, indices);

    /*
     * get the transform
     */
    //store the position
    this->transf.pos = dat.readVec2();
    //store the rotation
    this->transf.rot = dat.readFloat();
    //store the scaling
    this->transf.size = dat.readVec2();

    /*
     * Store object attributes
     */
    //store the object UUID
    this->id = dat.readUInt();
    //store the window id
    this->windowID = dat.readInt();
    //store if the object is static
    this->isStatic = dat.readBool();
    //store the anchor position
    this->anchor = dat.readVec2();

    //late deocde hook
    this->decodeHook(&dat);
}

void Object2D::encodeHook(Data*)
{
    //override code here
}

void Object2D::encodeHookEarly(Data*)
{
    //override code here
}

void Object2D::decodeHook(Data*)
{
    //override code here
}

void Object2D::decodeHookEarly(Data*)
{
    //override code here
}

void Object2D::superConstructor()
{
    //save the current window
    this->windowID = glgeCurrentWindowIndex;
    //set the base 2D shader
    this->shader = new Shader(glgeWindows[this->windowID]->getDefault2DShader());
    //set the shader
    this->shader->setCustomMat3("glgeCamMat", mat3());
    //recalculate the shader uniforms
    this->shader->recalculateUniforms();
    
    //get the UUID
    this->id = glgeObjectUUID;
    //increase the object count
    glgeObjectUUID++;

    //store the transform
    this->transf = transf;
    //update the object
    this->update();
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
    this->transf.pos += vec2(speedX*std::sin(dir), speedY*std::cos(dir));
}

void Camera2D::move(float speed)
{
    //change the position
    this->transf.pos += vec2(speed*std::sin(this->transf.rot), speed*std::cos(this->transf.rot));
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
    this->setRotation(this->transf.rot + angle);
}

void Camera2D::setRotation(float dir)
{
    //set the rotation of the object
    this->transf.rot = std::fmod(dir,2*M_PI);
}

float Camera2D::getRotation()
{
    //return the rotation in degrees
    return this->transf.rot;
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
    //calculate the half size (for performance)
    vec2 shalf = size / vec2(2);
    //generate the vertices
    std::vector<Vertex2D> vert = {Vertex2D( shalf.x, shalf.y, vec4(0,0,0,1)), Vertex2D( shalf.x,-shalf.y, vec4(0,0,0,1)),
                                  Vertex2D(-shalf.x, shalf.y, vec4(0,0,0,1)), Vertex2D(-shalf.x,-shalf.y, vec4(0,0,0,1))};
    //load the texture coordinates
    vert[0].texCoord = vec2(1,0);
    vert[1].texCoord = vec2(1,1);
    vert[2].texCoord = vec2(0,0);
    vert[3].texCoord = vec2(0,1);
    //generate the indices
    std::vector<unsigned int> inds = {1,0,2, 1,2,3};

    //calculate the mesh
    this->mesh = new Mesh2D(vert, inds);
    //save the transform
    this->transf = transf;
    //say that the object is static
    this->isStatic = true;

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
    //store the texture
    this->setTexture(texture);
}

Button::Button(unsigned int width, unsigned int height, vec4 color, Transform2D transf)
{
    //calculate the half size (for performance)
    vec2 shalf = vec2(width, height) / vec2(2);
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
    this->mesh = new Mesh2D(vert, inds);
    //save the transform
    this->transf = transf;
    //say that the object is static
    this->isStatic = true;
    //call the super constructor
    this->superConstructor();
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
    this->mesh = new Mesh2D(vert, inds);
    //save the transform
    this->transf = transf;
    //say that the object is static
    this->isStatic = true;
    //call the super constructor
    this->superConstructor();
}

Button::Button(float radius, unsigned int res, vec4 color, Transform2D transf)
{
    //generate the mesh from an preset
    Mesh2D* mesh = new Mesh2D(GLGE_PRESET_CIRCLE, res, color);
    //scale according to the radius
    for (int i = 0; i < (int)mesh->vertices.size(); i++)
    {
        //scale the position
        mesh->vertices[i].pos.scale(radius);
    }
    
    //store the mesh
    this->mesh = mesh;
    //save the transform
    this->transf = transf;
    //say that the object is static
    this->isStatic = true;
    //call the super constructor
    this->superConstructor();
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
    //calculat the mouse position in object space
    p -= this->transf.pos;
    //check if the button is a circle
    if (this->isCircle)
    {
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

void Button::encodeHook(Data* data)
{
    //store the size
    data->writeVec2(this->size);
    //store if this is a circle
    data->writeBool(this->isCircle);
}

void Button::decodeHook(Data* data)
{
    //read the size
    this->size = data->readVec2();
    //read if this is a circle
    this->isCircle = data->readBool();
}

Text::Text() : Object2D()
{}

Text::Text(const char* text, const char* font, vec4 color, int fontSize, Transform2D transf, bool dynMesh)
{
    //store the text
    this->text = std::string(text);
    //store the font file
    this->font = std::string(font);
    //store the color
    this->color = color;
    //store the font size
    this->fontSize = fontSize;
    //generate the texture for the text
    this->updateTexture();
    //store dynamic meshing
    this->dynamicMesh = dynMesh;

    //save the transform
    this->transf = transf;
    //say that the object is static
    this->isStatic = true;
    //call the super constructor
    this->superConstructor();
}

std::string Text::getText()
{
    //return the text
    return this->text;
}

std::string Text::getFont()
{
    //return the font
    return this->font;
}

void Text::setText(std::string text)
{
    //store the new text
    this->text = text;
    //update the texture
    this->updateTexture();
}

void Text::setFont(std::string font)
{
    //store the new text
    this->font = font;
    //update the texture
    this->updateTexture();
}

unsigned int Text::getFontSize()
{
    //return the font size
    return this->fontSize;
}

void Text::setFontSize(unsigned int size)
{
    //store the new text
    this->fontSize = size;
    //update the texture
    this->updateTexture();
}

vec4 Text::getTextColor()
{
    //return the text color
    return this->color;
}

void Text::setTextColor(vec4 col)
{
    //store the new text
    this->color = col;
    //update the texture
    this->updateTexture();
}

Texture* Text::getTextTexture()
{
    //return the texture
    return this->texture;
}

bool Text::getDynamicMeshing()
{
    //return the dynamic meshing
    return this->dynamicMesh;
}

void Text::setDynamicMeshing(bool dynMesh)
{
    //store the new dynamic meshing state
    this->dynamicMesh = dynMesh;    
}

void Text::encodeHook(Data* data)
{
    //encode the stored text
    data->writeString(this->text);
    //store the path to the font
    data->writeString(this->font);
    //store the font size
    data->writeInt(this->fontSize);
    //store the text color
    data->writeVec4(this->color);
    //store the text size
    data->writeVec2(this->texSize);
    //store if dynamic meshing is enabbled
    data->writeBool(this->dynamicMesh);
}

void Text::decodeHook(Data* data)
{
    //decode the stored text
    this->text = data->readString();
    //store the path to the font
    this->font = data->readString();
    //store the font size
    this->fontSize = data->readInt();
    //store the text color
    this->color = data->readVec4();
    //store the text size
    this->texSize = data->readVec2();
    //store if dynamic meshing is enabbled
    this->dynamicMesh = data->readBool();

    //update the text
    this->updateTexture();
}

void Text::updateTexture()
{
    //open the requested font
    TTF_Font *f = TTF_OpenFont(this->font.c_str(), fontSize);
    //check if the font opening was sucessfull
    if (!f)
    {
        GLGE_THROW_ERROR((std::string("Failed to open font file ") + font + std::string(" SDL Error : " + std::string(TTF_GetError()))).c_str())
    }
    //create the color for the surface
    SDL_Color c = SDL_Color{(uint8_t)(this->color.x * 255), (uint8_t)(this->color.y * 255), (uint8_t)(this->color.z * 255), (uint8_t)(this->color.w * 255)};
    //create the text surface
    SDL_Surface* surf = TTF_RenderText_Blended(f, this->text.c_str(), c);
    //check if the surface was created successfully
    if (!surf)
    {
        GLGE_THROW_ERROR((std::string("Failed to render text message to surface") + std::string(" SDL Error : " + std::string(SDL_GetError()))).c_str())
    }
    this->texSize = vec2(surf->w, surf->h);
    //delete the old texture
    delete this->texture;
    //convert SDL Texture to OpenGL texture
    this->texture = new Texture(sdlSurfaceToOpenGLTexture(surf), GLGE_TEX_RGB32);
    //check if dynamic meshing is activated
    if (this->dynamicMesh)
    {
        //calculate the y size
        vec2 s = vec2(this->texSize.x / this->texSize.y, 1);
        //generate the vertices
        std::vector<Vertex2D> vert = {Vertex2D(s.x, 0, 0,0,0,1), Vertex2D(s.x, -s.y, 0,0,0,1),
                                    Vertex2D(0,   0, 0,0,0,1), Vertex2D(0,   -s.y, 0,0,0,1)};
        //load the texture coordinates
        vert[0].texCoord = vec2(1,0);
        vert[1].texCoord = vec2(1,1);
        vert[2].texCoord = vec2(0,0);
        vert[3].texCoord = vec2(0,1);
        //generate the indices
        std::vector<unsigned int> inds = {1,0,2, 1,2,3};
        //check if a mesh exists
        if (!mesh)
        {
            //create a new mesh
            this->mesh = new Mesh2D(vert, inds);
        }
        else
        {
            //update the mesh
            this->mesh->setData(vert, inds);
            //recalculate the buffers
            this->mesh->recalculateBuffers();
        }
    }
}

TextInput::TextInput()
{}

TextInput::TextInput(const char* text, const char* font, vec4 color, int fontSize, Transform2D transf)
{
    //store the text
    this->text = std::string(text);
    //load the cursor position
    this->cursourPos = this->text.length();
    //check if the text is empty
    if (this->text.empty())
    {
        //set the text to an space
        this->text = " ";
        //store that the text is empty
        this->empty = true;
    }
    //store the font file
    this->font = std::string(font);
    //store the color
    this->color = color;
    //store the font size
    this->fontSize = fontSize;
    //generate the texture for the text
    this->updateTexture();
    //store dynamic meshing
    this->dynamicMesh = true;

    //save the transform
    this->transf = transf;
    //say that the object is static
    this->isStatic = true;

    //call the default constructor
    this->superConstructor();
}

void TextInput::update()
{
    //recalculate the move matrix
    this->recalculateMoveMatrix();
    
    //store the mouse position
    vec2 p = (vec2(glgeMouse.pos.x, 1.f - glgeMouse.pos.y) / vec2(0.5)) - vec2(1);
    //apply the window aspect ratio to the mouse position
    p.x *= glgeWindows[this->windowID]->getWindowAspect();
    //check if the world scale should be influenced
    if (!this->isStatic)
    {
        //scale according to the camera scale
        p /= mainCam->getScale();
    }
    //calculate the size
    vec2 size = this->texSize.scale(this->transf.size) / (glgeWindows[this->windowID]->getSize().scale(vec2(0.125)));
    //calculat the mouse position in object space
    p -= this->transf.pos;
    if (!this->isStatic)
    {
        p -= mainCam->getPos();
    }
    p.y *= -1.0;

    //check if the mouse is hovering the button
    if ((p >= vec2(0)) && (p <= size))
    {
        //check for a click
        if (glgeMouse.leftButton && !this->focused)
        {
            //select this box
            this->focused = true;
            //check if the text is empty
            if (this->empty)
            {
                //set the text to the cursor
                this->text = "|";
            }
            else
            {
                //change the cursor to an space
                this->text.insert(this->cursourPos, "|");
            }
            //check if an on enter function is bound
            if (this->onEnterFunc != NULL)
            {
                //call the function
                (*this->onEnterFunc)();
            }
            //update the texture
            this->updateTexture();
        }
    }
    else
    {
        //check for a click
        if (glgeMouse.leftButton && this->focused)
        {
            //unfocus
            this->focused = false;
            //change the cursor to an space
            this->text.erase(this->cursourPos, 1);
            //check if the text is empty
            if (this->text.empty())
            {
                //set the text to an space
                this->text = " ";
                //store that the text is empty
                this->empty = true;
            }
            //check if an on exit function is bound
            if (this->onExitFunc != NULL)
            {
                //call the function
                (*this->onExitFunc)();
            }
            //update the texture
            this->updateTexture();
        }
    }

    //check if the object is focused
    if (!this->focused)
    {
        //if not, return 
        return;
    }

    //store the cursor movement
    int deltaCourser = 0;

    //check if the left arrow key was pressed
    if (glgeKeysThisTick.arrowLeft)
    {
        //move the cursor left
        deltaCourser -= 1;
    }
    //check if the left arrow key was pressed
    if (glgeKeysThisTick.arrowRight)
    {
        //move the cursor left
        deltaCourser += 1;
    }

    //stop if nothing was updated
    if ((deltaCourser == 0) && glgeTypedThisTick.empty() && !glgeKeysThisTick.backspace)
    { return; }

    //store the old courser pos
    unsigned int oldCP = this->cursourPos;
    //move the coursor
    this->cursourPos += deltaCourser;

    //clamp the cursor position
    glgeClamp(&this->cursourPos, 0, this->text.length()-1);

    //move the visual courser
    std::swap(this->text[oldCP], this->text[this->cursourPos]);

    //check for backspace
    if (glgeKeysThisTick.backspace)
    {
        //check if the text is empty
        if (!text.empty() && (this->cursourPos > 0))
        {
            //check for the length of 1
            if (text.length() == 1)
            {
                //set the string to an space
                this->text = " ";
            }
            else
            {
                //delete the last character of the string
                this->text.erase(this->cursourPos - 1, 1);
                //move the cursor back
                this->cursourPos -= 1;
            }
        }
    }
    else
    {
        //store the new text
        this->text.insert(this->cursourPos, glgeTypedThisTick);
        //move the cursor
        this->cursourPos += glgeTypedThisTick.length();
    }
    
    //check if an on type function is set
    if (this->onTypeFunc != NULL)
    {
        //call the function
        (*this->onTypeFunc)();
    }

    //update the texture
    this->updateTexture();
}

bool TextInput::isFocused()
{
    return this->focused;
}

bool TextInput::isEmpty()
{
    return this->empty;
}

void TextInput::setOnTypeFunction(void (*func)())
{
    //store the inputed function
    this->onTypeFunc = func;
}

void (*TextInput::getOnTypeFunc())()
{
    //return the function
    return this->onTypeFunc;
}

void TextInput::setOnEnterFunction(void (*func)())
{
    //store the inpted function
    this->onEnterFunc = func;
}

void (*TextInput::getOnEnterFunc())()
{
    //return the function
    return this->onEnterFunc;
}

void TextInput::setOnExitFunction(void (*func)())
{
    //store the inpted function
    this->onExitFunc = func;
}

void (*TextInput::getOnExitFunc())()
{
    //return the function
    return this->onExitFunc;
}

void TextInput::encodeHook(Data* data)
{
    //encode the stored text
    data->writeString(this->text);
    //store the path to the font
    data->writeString(this->font);
    //store the font size
    data->writeInt(this->fontSize);
    //store the text color
    data->writeVec4(this->color);
    //store the text size
    data->writeVec2(this->texSize);
    //store if dynamic meshing is enabbled
    data->writeBool(this->dynamicMesh);
    //store the cursor position
    data->writeInt(this->cursourPos);
}

void TextInput::decodeHook(Data* data)
{
    //decode the stored text
    this->text = data->readString();
    //store the path to the font
    this->font = data->readString();
    //store the font size
    this->fontSize = data->readInt();
    //store the text color
    this->color = data->readVec4();
    //store the text size
    this->texSize = data->readVec2();
    //store if dynamic meshing is enabbled
    this->dynamicMesh = data->readBool();
    //store the cursor position
    this->cursourPos = data->readInt();

    //update the text
    this->updateTexture();
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
