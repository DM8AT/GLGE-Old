/**
 * @file openglGLGEParticles.h
 * @author DM8AT
 * @brief defines a compute shader and instancing based particle system
 * @version 0.1
 * @date 2024-06-29
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 */

#ifndef _OGL_GLGE_PARTICLES_H_
#define _OGL_GLGE_PARTICLES_H_

#include "openglGLGE3Dcore.h"
#include "openglGLGEComputeShader.hpp"

/**
 * @brief let the particles spawn in a box around the origin
 */
#define GLGE_PARTICLE_SPAWNER_SHAPE_BOX 0
/**
 * @brief let the particles spawn in a sphere around the origin
 */
#define GLGE_PARTICLE_SPAWNER_SHAPE_SPHERE 1
/**
 * @brief let the particles spawn on a plane around the origin. The Z-Component of the size is ignored
 */
#define GLGE_PARTICLE_SPAWNER_SHAPE_PLANE 2
/**
 * @brief let the particles spawn on a disc around the origin. The Z-Component of the size is ignored
 */
#define GLGE_PARTICLE_SPAWNER_SHAPE_DISC 3

/**
 * @brief store the data for a single particle
 */
struct Particle
{
    /**
     * @brief store the position of the particle relative to it's emitter
     */
    vec3 pos = vec3(0);
    /**
     * @brief store the rotation of the particle relative to it's emitter
     */
    vec3 rot = vec3(0);
    /**
     * @brief store the size of the particle relative to it's emitter
     */
    vec3 size = vec3(1);
    /**
     * @brief store the velocity of the particle
     */
    vec3 velocity = vec3(0);
    /**
     * @brief store the lifetime of the particle. Anly lifetime lower than 0 is hidden
     */
    float lifetime = 100.f;
    /**
     * @brief store the model matrix
     */
    mat4 modelMat = mat4();
    /**
     * @brief store the model rotation matrix
     */
    mat4 rotMat = mat4();
};

/**
 * @brief a particle spawner
 */
class ParticleSystem
{
public:
    /**
     * @brief Default constructor
     */
    ParticleSystem() = default;

    /**
     * @brief Create a new particle system
     * 
     * @param mesh the mesh for the particles
     * @param particleCount the amount of particles to spawn
     * @param transform the transform of the particle system
     * @param shape the shape of the particle system
     */
    ParticleSystem(Mesh* mesh, unsigned int particleCount = 128, Transform transform = Transform(), unsigned int shape = GLGE_PARTICLE_SPAWNER_SHAPE_BOX);

    /**
     * @brief Destroy the Particle System
     */
    ~ParticleSystem();

    /**
     * @brief draw all the particles
     */
    void draw();

    /**
     * @brief update all the particles
     */
    void update();

    /**
     * @brief Set the mesh for the particles
     * 
     * @param mesh a pointer to the base mesh
     */
    void setMesh(Mesh* mesh);

    /**
     * @brief Get the mesh from the particles
     * 
     * @return Mesh* the pointer to the mesh
     */
    Mesh* getMesh();

    /**
     * @brief Set the amount of particles to spawn
     * @warning This resizes internal arrays on the GPU-Side. This is done by dealocating and realocating them. This will reset all particles and may cause lag. It is not recomended to use this function every tick. 
     * @param particleCount the new particle count
     */
    void setCount(unsigned int particleCount);

    /**
     * @brief Get the amount of particles
     * 
     * @return unsigned int the amount of particles
     */
    unsigned int getCount();

    /**
     * @brief Set the position of the particle emitter
     * 
     * @param pos the new position of the emitter
     */
    void setPos(vec3 pos);

    /**
     * @brief Set the position of the particle emitter
     * 
     * @param x the x component of the new position
     * @param y the y component of the new position
     * @param z the z component of the new position
     */
    void setPos(float x, float y, float z);

    /**
     * @brief Get the position of the particle emitter
     * 
     * @return vec3 the position in world space
     */
    vec3 getPos();

    /**
     * @brief Set the rotation of the particle emitter
     * 
     * @param rot the new rotation of the particle emitter
     */
    void setRot(vec3 rot);

    /**
     * @brief Set the rotation of the particle emitter
     * 
     * @param x the rotation around the x axis
     * @param y the rotation around the y axis
     * @param z the rotation around the z axis
     */
    void setRot(float x, float y, float z);

    /**
     * @brief Get the rotation of the particle emitter
     * 
     * @return vec3 the rotation of the particle emitter
     */
    vec3 getRot();

    /**
     * @brief Set the scale of the particle emitter
     * 
     * @param scale the scale of the particle emitter
     */
    void setScale(vec3 scale);

    /**
     * @brief Set the scale of the particle emitter
     * 
     * @param x the scaling along the x-axis
     * @param y the scaling along the y-axis
     * @param z the scaling along the z-axis
     */
    void setScale(float x, float y, float z);

    /**
     * @brief Get the scale of the particle emitter
     * 
     * @return vec3 the scale of the particle emitter
     */
    vec3 getScale();

    /**
     * @brief Set the transformation of the particle emitter
     * 
     * @param transform the new transformation of the particle emitter
     */
    void setTransform(Transform transform);

    /**
     * @brief Get the transformation of the particle emitter
     * 
     * @return Transform the transformation of the particle emitter
     */
    Transform getTransform();

    /**
     * @brief Set the shape of the particle emitter
     * 
     * @param shape the new shape of the particle emitter
     */
    void setShape(unsigned int shape);

    /**
     * @brief Get the current shape of the particle emitter
     * 
     * @return unsigned int the identifier of the shape of the particle emitter
     */
    unsigned int getShape();

    /**
     * @brief Set the controll compute shader for the particles
     * 
     * @param shader a pointer to the compute shader object
     * @param replace say if the old shader should be deleted. Default is true
     */
    void setControllShader(ComputeShader* shader, bool replace = true);

    /**
     * @brief Set the controll compute shader for the particles
     * 
     * @param shader the path to a file
     * @param replace say if the old shader should be deleted. Default is true
     */
    void setControllShader(const char* shader, bool replace = true);

    /**
     * @brief Get the controll compute shader used to controll the particles
     * 
     * @return ComputeShader* a pointer to the controll shader
     */
    ComputeShader* getControllShader();

    /**
     * @brief Set the material for the particles
     * 
     * @param material a pointer to the material object
     * @param replace say if the old material should be deleted. Default is true
     */
    void setMaterial(Material* material, bool replace = true);

    /**
     * @brief Get the material from the particles
     * 
     * @return Material* a pointer to the own material
     */
    Material* getMaterial();

    /**
     * @brief Set the shader used by the particles
     * 
     * @param shader a pointer to the shader
     * @param replace say if the old shader should be deleted. Default is true
     */
    void setShader(Shader* shader, bool replace = true);

    /**
     * @brief Get the shader used by the particles
     * 
     * @return Shader* a pointer to the own shader
     */
    Shader* getShader();

protected:
    /**
     * @brief create the new particle system
     * 
     * @param mesh the mesh for the particles
     * @param particleCount the amount of particles to spawn
     * @param transform the transform of the particle system
     * @param shape the shape of the particle system
     */
    void super(Mesh* mesh, unsigned int particleCount, Transform transform, unsigned int shape);

    /**
     * @brief check if a shape is a valid shape identifyer
     * 
     * @param shape the shape identifyer
     * @return true : the shape identifyer is valid | 
     * @return false : the shape identifyer is not valid
     */
    bool isValidShape(unsigned int shape);

    /**
     * @brief update the GPU side of the data
     */
    void updateUBO();
private:
    /**
     * @brief store the data for the GPU
     */
    ObjectData data = ObjectData{};
    /**
     * @brief store the uniform buffer object
     */
    unsigned int ubo = 0;
    /**
     * @brief store the own mesh
     */
    Mesh* mesh = 0;
    /**
     * @brief store the amount of particles to draw
     */
    unsigned int numParticles = 0;
    /**
     * @brief draw the transform of the emitter
     */
    Transform transf = Transform();
    /**
     * @brief store the shape of the particle emitter
     */
    unsigned int shape = GLGE_PARTICLE_SPAWNER_SHAPE_BOX;
    /**
     * @brief store the shader storage buffer used to store the particle data
     */
    unsigned int ssbo = 0;
    /**
     * @brief store the compute shader used to update the particles
     */
    ComputeShader* controllShader = 0;
    /**
     * @brief store if a custom controll shader is bound
     */
    bool customControll = false;
    /**
     * @brief store a pointer to the material used by the particles
     */
    Material* material = 0;
    /**
     * @brief 
     */
    bool customMaterial = false;
    /**
     * @brief store a pointer to the shader used by the particles
     */
    Shader* shader = 0;
    /**
     * @brief 
     */
    bool customShader = false;
    /**
     * @brief store the index of the window the element is bound to
     */
    int windowID = -1;
};

#endif