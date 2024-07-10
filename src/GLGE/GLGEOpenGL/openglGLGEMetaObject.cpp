/**
 * @file openglGLGEMetaObject.cpp
 * @author DM8AT
 * @brief implement the Meta-Object Handler class and the Meta-Object struct functions declared in file openglGLGEMetaObject.hpp
 * @version 0.1
 * @date 2024-07-04
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#include "openglGLGEMetaObject.hpp"
#include "openglGLGEVars.hpp"
#include "../GLGEIndependend/glgePrivDefines.hpp"

/**
 * @brief check if the Meta-Object Handler was bound to a window
 */
#define MOH_CHECK_WINDOW if (this->windowID != glgeCurrentWindowIndex) {GLGE_THROW_ERROR("Can't execute OpenGL-Dependent functions on a Meta-Object Handler in a window it was not created in") return;}

MetaObject::MetaObject(vec3 pos, unsigned int primitive, vec3 rot, vec3 scale)
{
    //just pass the inputs to the own data
    this->pos = pos;
    this->primitive = primitive;
    this->rot = rot;
    this->scale = scale;
}

void MetaObject::queueForUpdate()
{
    //say that a update is queued
    this->queuedUpdate = true;
}






MetaObjectHandler::MetaObjectHandler()
{
    //store the window ID
    this->windowID = glgeCurrentWindowIndex;
    //create a new buffer
    glCreateBuffers(1, &this->ssbo);
}

MetaObjectHandler::~MetaObjectHandler()
{
    //check if it was bound to a window
    if (this->windowID != -1)
    {
        //delete the GPU-Side data vector
        glDeleteBuffers(1, &this->ssbo);
    }
    //delete the own Meta-Object vector
    this->metaObjects.clear();
}

void MetaObjectHandler::draw()
{
    //check if it was bound to a window
    MOH_CHECK_WINDOW

    //only draw durring solid pass
    if (glgeWindows[glgeCurrentWindowIndex]->isTranparentPass()) {return;}

    //bind the SSBO to index 0
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->ssbo);

    //enable blending
    glEnable(GL_BLEND);
    //set the blend function for color attachment 0
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    //bind the fullscreen quad
    glgeWindows[glgeCurrentWindowIndex]->bindScreenRect();

    //bind the own shader
    this->drawShader->applyShader();

    glUniform2f(glGetUniformLocation(this->drawShader->getShader(), "glgeWindowSize"), glgeWindows[glgeCurrentWindowIndex]->getSize().x,glgeWindows[glgeCurrentWindowIndex]->getSize().y);

    //draw the fullscreen quad
    glDrawArrays(GL_TRIANGLES,0,6);
    //check if debug data gathering is enabled
    if (glgeGatherDebugInfo)
    {
        //increment the amount of draw passes
        glgeDrawCallCountT++;
        //increment the amount of drawn triangles by 2 (the amount of triangels in a rectangle)
        glgeTriangleCountT += 2;
    }

    //remove the shader
    this->drawShader->removeShader();

    //unbind the fullscreen quad
    glgeWindows[glgeCurrentWindowIndex]->unbindScreenRect();

    //unbind the ssbo
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void MetaObjectHandler::update()
{
    //check if it was bound to a window
    MOH_CHECK_WINDOW
    //bind the ssbo
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
    //iterate over all objects
    for (int i = 0; i < (int)this->metaObjects.size(); i++)
    {
        //get if the current object should update
        if (!(this->metaObjects[i]->queuedUpdate || this->queuedFullUpdate))
        {
            //jump to the next execution
            continue;
        }

        //else, copy the data to the GPU
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, i*sizeof(MetaObject), sizeof(MetaObject), this->metaObjects[i]);
        //say that the update is done
        this->metaObjects[i]->queuedUpdate = false;
        //unqueue any full update
        this->queuedFullUpdate = false;
    }
    //unbind the ssbo
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void MetaObjectHandler::addMetaObject(MetaObject* object)
{
    //check if it was bound to a window
    MOH_CHECK_WINDOW
    //check if the Meta-Object is a nullpointer
    if (!object)
    {
        //throw an error
        GLGE_THROW_ERROR("Nullpointer is not a valid Meta-Object")
        return;
    }

    //store the new Meta-Object
    this->metaObjects.push_back(object);

    //bind the buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
    //store the new data
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(MetaObject) * this->metaObjects.size(), this->metaObjects.data(), GL_STATIC_DRAW);
    //unbind the buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void MetaObjectHandler::setCustomDrawShader(Shader* shader, bool replace)
{
    //check if the new shader is a nullpointer
    if (!shader)
    {
        //throw an error
        GLGE_THROW_ERROR("Nullpointer is not a valid drawing shader for Meta-Objects")
        return;
    }

    //check if the old shader should be replaced
    if (this->customShader && replace)
    {
        //delete the old shader
        delete this->drawShader;
    }

    //store the new shader
    this->drawShader = shader;
    //say that a custom shader is bound
    this->customShader = true;
}

void MetaObjectHandler::setCustomDrawShader(const char* shader, bool replace)
{
    //pass to another function
    this->setCustomDrawShader(new Shader(GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER, shader), replace);
}

Shader* MetaObjectHandler::getShader()
{
    //return a pointer to the own shader
    return this->drawShader;
}

bool MetaObjectHandler::hasCustomShader()
{
    //return if a custom draw shader is bound
    return this->customShader;
}

void MetaObjectHandler::setDrawMethode(unsigned int drawMethode)
{
    //store the new draw methode
    this->drawMethode = drawMethode;
}

unsigned int MetaObjectHandler::getDrawMethode()
{
    //return the draw methode
    return this->drawMethode;
}

void MetaObjectHandler::queueFullUpdate()
{
    //say that a full update is queued
    this->queuedFullUpdate = true;
}