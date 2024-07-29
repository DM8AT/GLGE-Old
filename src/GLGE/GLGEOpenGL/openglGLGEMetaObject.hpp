/**
 * @file openglGLGEMetaObject.hpp
 * @author DM8AT
 * @brief meta-objects are objects that don't have any mesh and are drawn without rasterisation. Everything to handle them is declared here. 
 * Meta-Objects are bound to a Meta-Object Handler and can't be drawn on they're own. 
 * @version 0.1
 * @date 2024-07-04
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_OGL_META_OBJECT_H_
#define _GLGE_OGL_META_OBJECT_H_

#include "openglGLGEWindow.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DRAW TYPES
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief say that this meta object handler should draw using ray tracing
 */
#define GLGE_META_DRAW_RAY_TRACING 0
/**
 * @brief say that this meta object handler should draw using ray marching. 
 * ray marching is slower than ray tracing, but gives a lot more freedom in regards of object deformation. 
 */
#define GLGE_META_DRAW_RAY_MARCHING 1

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIMITIVES
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief declare the identifyer to say that a Meta-Object is unused / hidden / uninitalised
 */
#define GLGE_META_PRIMITIVE_EMPTY 0
/**
 * @brief declare the identifyer for the cube primitive for Meta-Objects
 */
#define GLGE_META_PRIMITIVE_CUBE 1
/**
 * @brief declare the identifyer for the sphere primitive for Meta-Objects
 */
#define GLGE_META_PRIMITIVE_SPHERE 2

/**
 * @brief store all the data for a Meta-Object
 */
struct MetaObject
{
    /**
     * @brief store the position of the Meta-Object
     */
    vec3 pos = vec3(0);
    /**
     * @brief store if the Meta-Object should be updated
     */
    bool queuedUpdate = true;
    /**
     * @brief store the roatation of the Meta-Object
     */
    vec3 rot = vec3(0);
    /**
     * @brief store the scaling of the Meta-Object
     */
    vec3 scale = vec3(1);
    /**
     * @brief store the primitive of the Meta-Object
     */
    unsigned int primitive = GLGE_META_PRIMITIVE_EMPTY;

    /**
     * @brief Default constructor
     */
    MetaObject() = default;

    /**
     * @brief Construct a Meta-Object from some data
     * 
     * @param pos the position for the Meta-Object
     * @param primitive the primitive to use for the Meta-Object
     * @param rot the rotation for the Meta-Object
     * @param scale the scaling for the Meta-Object
     */
    MetaObject(vec3 pos, unsigned int primitive, vec3 rot = vec3(0), vec3 scale = vec3(1));

    /**
     * @brief queue this meta object to be updated by the handler
     */
    void queueForUpdate();
};

/**
 * @brief this class makes it possible to draw Meta-Objects. It can draw an infinite amount of Meta-Objects at once, but all 
 * of them must use the same drawing methode. 
 */
class MetaObjectHandler
{
public:

    /**
     * @brief Construct a new Meta-Object Handler
     */
    MetaObjectHandler();

    /**
     * @brief Destroy the Meta Object Handler
     */
    ~MetaObjectHandler();

    /**
     * @brief draw all the meta objects
     */
    void draw();

    /**
     * @brief loop over all Meta-Objects and update them, if an update is queued
     */
    void update();

    /**
     * @brief add an Meta-Object to the Meta-Object Handler
     * @param object the object to add
     */
    void addMetaObject(MetaObject* object);

    /**
     * @brief Set a custom shader to draw the Meta-Objects
     * 
     * @param shader the shader to draw the Meta-Objects
     * @param replace say if the old shader should be deleted, DEFAULT: true
     */
    void setCustomDrawShader(Shader* shader, bool replace = true);

    /**
     * @brief Set the custom shader to draw the Meta-Objects
     * 
     * @param shader the path to a file containing the shader
     * @param replace say if the old shader should be deleted, DEFAULT: true
     */
    void setCustomDrawShader(const char* shader, bool replace = true);

    /**
     * @brief Get the Shader that is used to draw the Meta-Objects
     * 
     * @return Shader* a pointer to the own shader
     */
    Shader* getShader();

    /**
     * @brief get if a custom draw shader is bound
     * 
     * @return true : a custom draw shader was bound | 
     * @return false : no custom draw shader was bound
     */
    bool hasCustomShader();

    /**
     * @brief Set the draw methode to use for drawing the Meta-Objects
     * 
     * @param drawMethode the identifyer of the new draw methode
     */
    void setDrawMethode(unsigned int drawMethode);

    /**
     * @brief Get the identifyer of the used draw methode
     * 
     * @return unsigned int the identifyer of the used draw methode
     */
    unsigned int getDrawMethode();

    /**
     * @brief queue a full update of the GPU-Side buffer
     * @warning this will update the whole buffer regadless if an update is nececerry. This may cause lag because of mass data transfear from CPU to GPU. 
     */
    void queueFullUpdate();

private:
    /**
     * @brief store the methode used to draw the meta objects
     */
    unsigned int drawMethode = GLGE_META_DRAW_RAY_TRACING;
    /**
     * @brief store all the meta objects
     */
    std::vector<MetaObject*> metaObjects = {};
    /**
     * @brief if a full update is queued, all objects will be updated regardless of the own update flag
     */
    bool queuedFullUpdate = false;
    /**
     * @brief store the amount of actualy bound Meta-Objects
     */
    unsigned int count = 0;
    /**
     * @brief store the shader used to draw the meta objects
     */
    Shader* drawShader = 0;
    /**
     * @brief store if a custom draw shader is bound
     */
    bool customShader = false;
    /**
     * @brief store the shader storage buffer object used to storing all the meta objects
     */
    unsigned int ssbo = 0;
    /**
     * @brief store the amount of MetaObjects that can fit into the own ssbo
     */
    int ssboSize = 0;
    /**
     * @brief store the identifyer the Meta-Object Hander was bound to
     */
    int windowID = -1;
};

#endif