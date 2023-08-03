/**
 * @file GLGEKlasses.h
 * @author DM8AT
 * @brief create all needed classes for default GLGE
 * @version 0.1
 * @date 2022-12-26
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_KLASSES_H_
#define _GLGE_KLASSES_H_

//define the mouse scroll type
#define GLGE_MOUSE_SCROLL 5

//define a button press
#define GLGE_MOUSE_BUTTON_PRESS 1
//define a button release
#define GLGE_MOUSE_BUTTON_RELEASE 0


//include the needed var types
#include "CML/CMLVec2.h"

/**
 * @brief a Keys object stores all pressed keys
 */
class Keys
{
public:
    //create a bool for all letters in the alphabet
    bool a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z;
    //create a bool for all special keys
    bool f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12, space, leftShift, rightShift, leftAlt, rightAlt, shift, alt, arrowRight, arrowLeft, arrowUp, arrowDown;

    /**
     * @brief Construct a new Keys 
     * default constructor
     */
    Keys();

    /**
     * @brief update a special key
     * 
     * @param key the key to update
     * @param status the new status for the key
     */
    void keyUpdate(int key, bool status);

    /**
     * @brief set all keys to false
     */
    void clear();

    /**
     * @brief Get the status of one key
     * 
     * @param key the key to check
     * @return true : the key is pressed | 
     * @return false : the key is not pressed
     */
    bool getKey(int key);

    /**
     * @brief Get the status of one key
     * 
     * @param key the key to check
     * @return true : the key is pressed | 
     * @return false : the key is not pressed
     */
    bool getKey(unsigned char key);
};

class Mouse
{
public:
    //store the position of the mouse on the screen
    vec2 pos;
    //store the position of the mouse on the screen in pixel space
    vec2 posPixel;
    //store the status of the mouse buttons
    bool leftButton, rightButton, middleButton = false;
    //store the amoung of scrolles from the mouse wheel
    signed char mouseWeel;

    //default constructor
    Mouse();

    /**
     * @brief update the mouse
     * 
     * @param button the botton to handle
     * @param state the button status
     */
    void update(int button, int state);
};

#endif