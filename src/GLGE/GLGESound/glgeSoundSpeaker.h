/**
 * @file glgeSoundSpeaker.h
 * @author DM8AT
 * @brief define a class to handle a positional
 * @version 0.1
 * @date 2024-07-11
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 */

#ifndef _GLGE_SND_SPEAKER_H_
#define _GLGE_SND_SPEAKER_H_

#include "../GLGEIndependend/glge3DcoreDefClasses.h"
#include "../GLGEIndependend/glge2DcoreDefClasses.h"
#include "../CML/CML.h"

/**
 * @brief a sound source that can be positioned and rotated
 */
class Speaker
{
public:

    /**
     * @brief Default constructor
     */
    Speaker() = default;

    /**
     * @brief Cretate a new speaker
     * 
     * @param file the file to play
     * @param pos the position of the speaker
     * @param rot the rotation of the speaker
     */
    Speaker(const char* file, vec3 pos, vec3 rot = vec3(0));

    /**
     * @brief Create a new speaker
     * 
     * @param file the file to play
     * @param transform the 3D transformation of the sound source (scale is ignored)
     */
    Speaker(const char* file, Transform transform = Transform());

    /**
     * @brief Create a new speaker
     * 
     * @param file the file to play
     * @param pos the 2D position of the speaker
     * @param rot the 2D rotation of the speaker
     */
    Speaker(const char* file, vec2 pos, float rot = 0);

    /**
     * @brief Create a new speaker
     * 
     * @param file the file to play
     * @param transform the 2D transformation of the spaker
     */
    Speaker(const char* file, Transform2D transform);

    /**
     * @brief Delete the speaker
     */
    ~Speaker();

    /**
     * @brief start playing the sound
     * 
     * @param force say if the playing should be forced by restarting the sound
     */
    void play(bool force = false);

    /**
     * @brief pause / unpause the sound
     * 
     * @param pause say if the sound should pause or unpause
     */
    void pause(bool pause);

    /**
     * @brief get if the sound is paused
     * 
     * @return true : the sound is pause | 
     * @return false : the sound is not paused
     */
    bool isPaused();

    /**
     * @brief toggle if the sound is paused
     */
    void togglePause();

    /**
     * @brief stop playing the sound
     */
    void stop();

    /**
     * @brief check if the sound is currently playing
     * 
     * @return true : the sound is currently playing | 
     * @return false : the sound is not playing
     */
    bool isPlaying();

    /**
     * @brief Set the gain / volume
     * 
     * @param gain the new gain value, must be greater than 0
     */
    void setGain(float gain);

    /**
     * @brief Get the gain / volume
     * 
     * @return float the current gain
     */
    float getGain();

    /**
     * @brief Set the pitch
     * 
     * @param pitch the new pitch, must be greater 0
     */
    void setPitch(float pitch);

    /**
     * @brief Get the pitch
     * 
     * @return float the current pitch
     */
    float getPitch();

    /**
     * @brief Set the position for the speaker
     * 
     * @param pos the new position for the speaker
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void setPos(vec3 pos, bool supressVel = false);

    /**
     * @brief Set the position for the speaker
     * 
     * @param x the position on the x axis for the speaker
     * @param y the position on the y axis for the speaker
     * @param z the position on the z axis for the speaker
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void setPos(float x, float y, float z, bool supressVel = false);

    /**
     * @brief Set the 2D position for the speaker
     * 
     * @param pos the new 2D position of the speaker
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void setPos(vec2 pos, bool supressVel = false);

    /**
     * @brief Set the 2D position for the speaker
     * 
     * @param x the position on the x axis for the speaker
     * @param y the position on the y axis for the speaker
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void setPos(float x, float y, bool supressVel = false);

    /**
     * @brief change the position of the speaker
     * 
     * @param deltaPos the difference in position
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void move(vec3 deltaPos, bool supressVel = false);

    /**
     * @brief change the position of the speaker
     * 
     * @param dx the difference in position of the speaker on the x axis
     * @param dy the difference in position of the speaker on the y axis
     * @param dz the difference in position of the speaker on the z axis
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void move(float dx, float dy, float dz, bool supressVel = false);

    /**
     * @brief change the position of the speaker in 2D space
     * 
     * @param deltaPos the difference in position of the speaker
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void move(vec2 deltaPos, bool supressVel = false);

    /**
     * @brief change the position of the speaker in 2D space
     * 
     * @param dx the difference in position of the speaker on the x axis
     * @param dy the difference in position of the speaker on the y axis
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void move(float dx, float dy, bool supressVel = false);

    /**
     * @brief Get the position of the speaker
     * 
     * @return vec3 the speakers position
     */
    vec3 getPos();

    /**
     * @brief Get the position of the speaker in 2D space
     * 
     * @return vec2 the position of the speaker in 2D space
     */
    vec2 getPos2D();

    /**
     * @brief Set the rotation for the speaker
     * 
     * @param rot the rotation of the speaker
     */
    void setRot(vec3 rot);

    /**
     * @brief Set the rotation for the speaker
     * 
     * @param rx the rotation around the x axis
     * @param ry the rotation around the y axis
     * @param rz the rotation around the z axis
     */
    void setRot(float rx, float ry, float rz);

    /**
     * @brief Set the 2D rotation of the object
     * 
     * @param rot the angle of rotation for the object
     */
    void setRot(float rot);

    /**
     * @brief rotate the speaker
     * 
     * @param deltaRot the difference in rotation
     */
    void rotate(vec3 deltaRot);

    /**
     * @brief rotate the speaker
     * 
     * @param drx the difference in rotation around the x axis
     * @param dry the difference in rotation around the y axis
     * @param drz the difference in rotation around the z axis
     */
    void rotate(float drx, float dry, float drz);

    /**
     * @brief rotate the speaker in 2D space
     * 
     * @param delta the difference in angles
     */
    void rotate(float delta);

    /**
     * @brief Get the rotation of the speaker
     * 
     * @return vec3 the speakers rotation
     */
    vec3 getRot();

    /**
     * @brief Get the 2D rotation of the speaker
     * 
     * @return float the 2D rotation of the speaker
     */
    float getRot2D();

    /**
     * @brief Set the transformation of the speaker
     * 
     * @param transform the new transform for the speaker (scale is ignored)
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void setTransform(Transform transform, bool supressVel = false);

    /**
     * @brief Set the 2D transformation of the speaker
     * 
     * @param transform the transformation of the object
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void setTransform(Transform2D transform, bool supressVel = false);

    /**
     * @brief change the transformation of the speaker
     * 
     * @param deltaTransform the difference in tranformations (scale is ignored)
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void changeTransform(Transform deltaTransform, bool supressVel = false);

    /**
     * @brief change the transformation of the speaker
     * 
     * @param deltaTrnasform the difference in tranformations (scale is ignored)
     * @param supressVel true : the velocity will be zeroed | false : the velocity will be updated according to the old position (default)
     */
    void changeTansform(Transform2D deltaTrnasform, bool supressVel = false);

    /**
     * @brief Get the transformation of the speaker (scale is always equal to 'vec3(1)')
     * 
     * @return Transform the transformation of the speaker
     */
    Transform getTransform();

    /**
     * @brief Get the 2D transformation of the speaker (scale is always equal to 'vec2(1)')
     * 
     * @return Transform2D the transformation of the speaker
     */
    Transform2D getTransform2D();

protected:
    /**
     * @brief the super constructor for the object
     * 
     * @param file the file to play
     * @param pos the position of the speaker
     * @param rot the rotation of the speaker
     */
    void super(const char* file, vec3 pos, vec3 rot);

private:
    /**
     * @brief store the position of the sound source in 3D space
     */
    vec3 pos = vec3(0);
    /**
     * @brief store the last position in 3D space
     */
    vec3 lastPos = vec3(0);
    /**
     * @brief store the speed and direction the speaker is moving
     */
    vec3 vel = vec3(0);
    /**
     * @brief store the rotation of the speaker
     */
    vec3 rot = vec3(0);
    /**
     * @brief store the direction vector
     */
    vec3 dir = vec3(0,0,-1);

    /**
     * @brief store the gain of the sound source
     */
    float gain = 1.f;
    /**
     * @brief store the pitch of the sound source
     */
    float pitch = 1.f;

    /**
     * @brief store the index of the speaker in OpenAL
     */
    unsigned int source = 0;
    /**
     * @brief store the buffer for the sound source
     */
    unsigned int buffer = 0;
};

#endif