/**
 * @file openglGLGEParticles.cpp
 * @author DM8AT
 * @brief declare a GPU-Driven particle system
 * @version 0.1
 * @date 2024-06-29
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#include "openglGLGEParticles.h"
#include "openglGLGEVars.hpp"

/**
 * @brief check if the particle system is bound to a window
 */
#define PARTICLESYS_WINDOW_CHECK if (this->windowID != glgeCurrentWindowIndex) {GLGE_THROW_ERROR("Unable to execute because particle system was called in wrong window or uninitalised")}

ParticleSystem::ParticleSystem(Mesh* mesh, unsigned int particleCount, Transform transform, unsigned int shape)
{
    this->super(mesh, particleCount, transform, shape);
}

ParticleSystem::~ParticleSystem()
{
    PARTICLESYS_WINDOW_CHECK
    //delete the ubo
    glDeleteBuffers(1, &this->ubo);
    //delete the ssbo
    glDeleteBuffers(1, &this->ssbo);
    //delete the mesh
    delete this->mesh;
}

void ParticleSystem::draw()
{
    PARTICLESYS_WINDOW_CHECK

    //check if this is the transparent pass
    if (glgeWindows[this->windowID]->isTranparentPass()) {return;}

    //disable blending
    if (!glgeWindows[this->windowID]->isTranparentPass())
    {
        //enable color blending
        glDisable(GL_BLEND);
    }

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    //set the depth function correct
    glDepthFunc(GL_GREATER);

    //bind the buffers
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->ubo);
    //wait untill all accesses for the shader storage buffer finish
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    //bind the shader storage buffer
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, this->ssbo);

    //apply the own shader
    this->shader->applyShader();
    //apply the material
    this->material->apply();

    //bind the mesh
    this->mesh->bind();
    
    //draw the elements instanced
    glDrawElementsInstanced(GL_TRIANGLES, this->mesh->indices.size(), GL_UNSIGNED_INT, 0, this->numParticles);

    //unbind the shader
    this->shader->removeShader();

    //unbind the material
    this->material->remove();

    //unbind the mesh
    this->mesh->unbind();

    //unbind the buffers
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ParticleSystem::update()
{
    PARTICLESYS_WINDOW_CHECK
    //get the transformation matrix
    this->data.modelMat = this->transf.getMatrix();
    //get the rotation matrix
    this->data.rotMat = this->transf.getRotationMatrix();
    //update the uniform buffer
    this->updateUBO();

    //check if a controll shader is boun
    if (this->controllShader)
    {
        //bind the shader storage object
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, this->ssbo);
        //wait untill all accesses for the shader storage buffer finish
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        //dispatch one instance of the shader per particle
        this->controllShader->dispatch(vec3(std::ceil(this->numParticles / 64.f), 1,1), GLGE_COMPUTE_MEMORY_BARRIER_NONE);
    }
    //check if material is bound
    if (this->material)
    {
        //update the material
        this->material->update(this->shader->getShader());
    }
}

void ParticleSystem::setMesh(Mesh* mesh)
{
    PARTICLESYS_WINDOW_CHECK
    //check if the mesh is a nullpointer
    if (!mesh)
    {
        //throw an error
        GLGE_THROW_ERROR("Nullpointer is not a valid mesh")
        return;
    }
    //store the mesh
    this->mesh = mesh;
    //initalise the mesh
    this->mesh->init();
}

Mesh* ParticleSystem::getMesh()
{
    PARTICLESYS_WINDOW_CHECK
    //return a pointer to the base mesh
    return this->mesh;
}

void ParticleSystem::setCount(unsigned int particleCount)
{
    PARTICLESYS_WINDOW_CHECK
    //store the new count
    this->numParticles = particleCount;
    //check if a buffer allready exists
    if (this->ssbo != 0)
    {
        //if it dose, delete the old buffer
        glDeleteBuffers(1, &this->ssbo);
    }
    //create the new buffer
    glCreateBuffers(1, &this->ssbo);
    //bind the buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);

    //set the amount of data
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Particle) * this->numParticles, 0, GL_DYNAMIC_DRAW);

    //unbind the buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

unsigned int ParticleSystem::getCount()
{
    PARTICLESYS_WINDOW_CHECK
    //return the amount of particles
    return this->numParticles;
}

void ParticleSystem::setPos(vec3 pos)
{
    PARTICLESYS_WINDOW_CHECK
    //store the new position
    this->transf.pos = pos;
}

void ParticleSystem::setPos(float x, float y, float z)
{
    //pas to another function
    this->setPos(vec3(x,y,z));
}

vec3 ParticleSystem::getPos()
{
    PARTICLESYS_WINDOW_CHECK
    //return the position
    return this->transf.pos;
}

void ParticleSystem::setRot(vec3 rot)
{
    PARTICLESYS_WINDOW_CHECK
    //store the new rotation
    this->transf.rot = rot;
}

void ParticleSystem::setRot(float x, float y, float z)
{
    //pass to another function
    this->setRot(vec3(x,y,z));
}

vec3 ParticleSystem::getRot()
{
    PARTICLESYS_WINDOW_CHECK
    //return the rotation
    return this->transf.rot;
}

void ParticleSystem::setScale(vec3 scale)
{
    PARTICLESYS_WINDOW_CHECK
    //store the new scale
    this->transf.scale = scale;
}

void ParticleSystem::setScale(float x, float y, float z)
{
    //pass to another function
    this->setScale(vec3(x,y,z));
}

vec3 ParticleSystem::getScale()
{
    PARTICLESYS_WINDOW_CHECK
    //return the scale
    return this->transf.scale;
}

void ParticleSystem::setTransform(Transform transform)
{
    PARTICLESYS_WINDOW_CHECK
    //store the new transform
    this->transf = transform;
}

Transform ParticleSystem::getTransform()
{
    PARTICLESYS_WINDOW_CHECK
    //return the own transform
    return this->transf;
}

void ParticleSystem::setShape(unsigned int shape)
{
    PARTICLESYS_WINDOW_CHECK
    //check if the shape is valid
    if (!this->isValidShape(shape))
    {
        //throw an error
        GLGE_THROW_ERROR("Shape identifyer " + std::to_string(shape) + " dosn't name a valid shape")
        return;
    }

    //check if the own shader is a nullpointer

    //store the new shape
    this->shape = shape;
}

unsigned int ParticleSystem::getShape()
{
    PARTICLESYS_WINDOW_CHECK
    //return the own shape
    return this->shape;
}

void ParticleSystem::setControllShader(ComputeShader* shader, bool rep)
{
    PARTICLESYS_WINDOW_CHECK
    //check if the shader isn't a nullpointer
    if (!shader)
    {
        //throw an error
        GLGE_THROW_ERROR("Nullpointer isn't a valid controll shader for a particle system")
        return;
    }

    //check if replacement is on
    if (rep && this->customControll)
    {
        //delete the old shader
        delete this->controllShader;
    }

    //store the new shader
    this->controllShader = shader;
    //say that a custom shader is bound
    this->customControll = true;
}

void ParticleSystem::setControllShader(const char* shader, bool rep)
{
    PARTICLESYS_WINDOW_CHECK
    //create the new controll shader and pass to another function
    this->setControllShader(new ComputeShader(shader), rep);
}

ComputeShader* ParticleSystem::getControllShader()
{
    PARTICLESYS_WINDOW_CHECK
    //return the own controll shader
    return this->controllShader;
}

void ParticleSystem::setMaterial(Material* material, bool rep)
{
    PARTICLESYS_WINDOW_CHECK
    //check if the material isn't 0
    if (!material)
    {
        //throw an error
        GLGE_THROW_ERROR("Nullpointer is no valid material")
        return;
    }

    //check if replacement is enalbed
    if (rep && this->customMaterial)
    {
        //delete the old material
        delete this->material;
    }

    //store the new material
    this->material = material;
    //say that a custom material is bound
    this->customMaterial = true;
    //update the material
    this->material->update(this->shader->getShader());
}

Material* ParticleSystem::getMaterial()
{
    PARTICLESYS_WINDOW_CHECK
    //return a pointer to the own material
    return this->material;
}

void ParticleSystem::setShader(Shader* shader, bool rep)
{
    PARTICLESYS_WINDOW_CHECK
    //check if the shader isn't a nullpointer
    if (!shader)
    {
        //throw an error
        GLGE_THROW_ERROR("Nullpointer isn't a valid shader for a particle system")
        return;
    }

    //check if replacement is enabled
    if (rep && this->customShader)
    {
        //delete the old shader
        delete this->shader;
    }

    //store the new shader
    this->shader = shader;
    //say that a custom shader is bound
    this->customShader = true;
    //check if a material is bound
    if (this->material)
    {
        //update the material with the shader
        this->material->update(this->shader->getShader(), true);
    }
}

Shader* ParticleSystem::getShader()
{
    PARTICLESYS_WINDOW_CHECK
    //return a pointer to the shader
    return this->shader;
}

void ParticleSystem::super(Mesh* mesh, unsigned int particleCount, Transform transform, unsigned int shape)
{
    //check if the shape is a valid shape id
    if (!this->isValidShape(shape))
    {
        //throw an error
        GLGE_THROW_ERROR("Tried to construct particle system with invalid shape of ID " + std::to_string(shape))
        return;
    }
    //check if the mesh is a nullpointer
    if (!mesh)
    {
        //throw an error
        GLGE_THROW_ERROR("Nullpointer is not a valid mesh")
        return;
    }
    //if everything is ok, store the shape
    this->shape = shape;

    //store the current window id to bind the particle system to the window
    this->windowID = glgeCurrentWindowIndex;

    //store the new mesh
    this->mesh = mesh;
    //initalise the mesh
    this->mesh->init();
    //store the particle count
    this->numParticles = particleCount;
    //store the transformation
    this->transf = transform;

    //set the amount of particles
    this->setCount(this->numParticles);

    //store the own object UUID
    this->data.uuid = glgeObjectUUID;
    //increase the amount of created objects
    glgeObjectUUID++;
    //get the default shader from the current window
    this->shader = new Shader(glgeWindows[this->windowID]->getDefault3DShader());
    this->customShader = true;

    //update the object
    this->update();
}

bool ParticleSystem::isValidShape(unsigned int shapeID)
{
    //switch over all possible states
    switch (shapeID)
    {
    //a box shape is valid
    case GLGE_PARTICLE_SPAWNER_SHAPE_BOX:
        return true;
        break;
    //a sphere shape is valid
    case GLGE_PARTICLE_SPAWNER_SHAPE_SPHERE:
        return true;
        break;
    //a plane shape is valid
    case GLGE_PARTICLE_SPAWNER_SHAPE_PLANE:
        return true;
        break;
    //a disc shape is valid
    case GLGE_PARTICLE_SPAWNER_SHAPE_DISC:
        return true;
        break;
    
    //everything else is not valid
    default:
        return false;
        break;
    }
}

void ParticleSystem::updateUBO()
{
    //check if a ubo exists
    if (this->ubo == 0)
    {
        //if not, create one
        glCreateBuffers(1, &this->ubo);
    }
    //bind the ubo
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    //store the data
    glBufferData(GL_UNIFORM_BUFFER, sizeof(this->data), &this->data, GL_STREAM_COPY);
    //unbind the buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}