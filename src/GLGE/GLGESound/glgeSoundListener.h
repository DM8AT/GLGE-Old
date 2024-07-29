/**
 * @file glgeSoundListener.h
 * @author DM8AT
 * @brief define a class to controll the listener's position for position dependend audio
 * @version 0.1
 * @date 2024-07-11
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 */

#ifndef _GLGE_SND_LISTENER_H_
#define _GLGE_SND_LISTENER_H_

#include "../GLGEIndependend/glge3DcoreDefClasses.h"
#include "../GLGEIndependend/glge2DcoreDefClasses.h"
#include "../CML/CML.h"

/**
 * @brief a class to handle the position of one or more listeners
 */
class Listener
{
public:

    /**
     * @brief Default constructor
     */
    Listener() = default;

    /**
     * @brief Create a new listener
     * 
     * @param pos the position of the listener
     * @param rot the rotation of the listener
     */
    Listener(vec3 pos, vec3 rot = vec3(0));

    /**
     * @brief Crate a new listener
     * 
     * @param transform the transform for the listener (the scale is ignored)
     */
    Listener(Transform transform);

    /**
     * @brief Create a new listener
     * 
     * @param pos the 2D position for the listener
     * @param rot the angle for rotation in 2D
     */
    Listener(vec2 pos, float rot = 0);

    /**
     * @brief Create a new listener
     * 
     * @param transform the 2D transformation for the listener
     */
    Listener(Transform2D transform);

    /**
     * @brief Destroy the Listener
     */
    ~Listener();

    /**
     * @brief bind the listener to the OpenAL context
     */
    void bind();

    /**
     * @brief Set the position for the listener
     * 
     * @param pos the new position for the listener
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void setPos(vec3 pos, bool supressVel = false);

    /**
     * @brief Set the position for the listener
     * 
     * @param x the position on the x axis for the listener
     * @param y the position on the y axis for the listener
     * @param z the position on the z axis for the listener
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void setPos(float x, float y, float z, bool supressVel = false);

    /**
     * @brief Set the 2D position of the listener
     * 
     * @param pos the position of the listener in 2D space
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void setPos(vec2 pos, bool supressVel = false);

    /**
     * @brief Set the 2D position of the listener
     * 
     * @param x the position on the x axis for the listener
     * @param y the position on the y axis for the listener
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void setPos(float x, float y, bool supressVel = false);

    /**
     * @brief change the position of the listener
     * 
     * @param deltaPos the difference in position
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void move(vec3 deltaPos, bool supressVel = false);

    /**
     * @brief change the position of the listener
     * 
     * @param dx the difference in position of the listener on the x axis
     * @param dy the difference in position of the listener on the y axis
     * @param dz the difference in position of the listener on the z axis
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void move(float dx, float dy, float dz, bool supressVel = false);

    /**
     * @brief change the position of the listener in 2D space
     * 
     * @param deltaPos the difference in position
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void move(vec2 deltaPos, bool supressVel = false);

    /**
     * @brief change the position of the listener in 2D space
     * 
     * @param dx the difference in position of the listener on the x axis
     * @param dy the difference in position of the listener on the y axis
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void move(float dx, float dy, bool supressVel = false);

    /**
     * @brief Get the position of the listener
     * 
     * @return vec3 the listeners position
     */
    vec3 getPos();

    /**
     * @brief Get the 2D position of the listener
     * 
     * @return vec2 the position in 2D space
     */
    vec2 getPos2D();

    /**
     * @brief Set the rotation for the listener
     * 
     * @param rot the rotation of the listener
     */
    void setRot(vec3 rot);

    /**
     * @brief Set the rotation for the listener
     * 
     * @param rx the rotation around the x axis
     * @param ry the rotation around the y axis
     * @param rz the rotation around the z axis
     */
    void setRot(float rx, float ry, float rz);

    /**
     * @brief Set the rotation of the listener in 2D space
     * 
     * @param rot the new rotation in 2D space as angle
     */
    void setRot(float rot);

    /**
     * @brief rotate the listener
     * 
     * @param deltaRot the difference in rotation
     */
    void rotate(vec3 deltaRot);

    /**
     * @brief rotate the listener
     * 
     * @param drx the difference in rotation around the x axis
     * @param dry the difference in rotation around the y axis
     * @param drz the difference in rotation around the z axis
     */
    void rotate(float drx, float dry, float drz);

    /**
     * @brief rotate the listener in 2D space
     * 
     * @param delta the difference in angles for the listener
     */
    void rotate(float delta);

    /**
     * @brief Get the rotation of the listener
     * 
     * @return vec3 the listeners rotation
     */
    vec3 getRot();

    /**
     * @brief Get the 2D rotation of the listener
     * 
     * @return float the 2D rotation angle
     */
    float getRot2D();

    /**
     * @brief Set the transformation of the listener
     * 
     * @param transform the new transform for the listener (scale is ignored)
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void setTransform(Transform transform, bool supressVel = false);

    /**
     * @brief Set the 2D transformation of the listener
     * 
     * @param transform the 2D transformation for the listener (scale is ignored)
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void setTransform(Transform2D transform, bool supressVel = false);

    /**
     * @brief change the transformation of the listener
     * 
     * @param deltaTransform the difference in tranformations (scale is ignored)
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void changeTransform(Transform deltaTransform, bool supressVel = false);

    /**
     * @brief change the 2D transformation of the listener
     * 
     * @param deltaTransform the difference in tranformations (scale is ignored)
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void changeTransform(Transform2D deltaTransform, bool supressVel = false);

    /**
     * @brief Get the transformation of the listener (scale is always equal to 'vec3(1)')
     * 
     * @return Transform the transformation of the listener
     */
    Transform getTransform();

    /**
     * @brief Get the 2D transformation of the listener (scale is always equal to 'vec2(1)')
     * 
     * @return Transform2D the 2D transformation of the listener 
     */
    Transform2D getTransform2D();

protected:

    /**
     * @brief super constructor
     * 
     * @param pos the position of the listener
     * @param rot the rotation of the listener
     */
    void super(vec3 pos, vec3 rot);
private:

    /**
     * @brief recalculate the orientation
     */
    void updateOrientation();

    /**
     * @brief store the listener position
     */
    vec3 pos = vec3(0);
    /**
     * @brief store the velocity of the listener
     */
    vec3 vel = vec3(0);
    /**
     * @brief store the rotation of the listener to calculate the orientation
     */
    vec3 rot = vec3(0);
    /**
     * @brief store the orientation of the listener
     */
    float orientation[6] = {0,0,-1,0,1,0};
};

#endif