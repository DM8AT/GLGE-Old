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
#include "../CML/CMLVec2.h"

/**
 * @brief store information about the pressed keys
 */
class Keys
{
public:
    //create a bool for all letters in the alphabet
    bool a=false,b=false,c=false,d=false,e=false,f=false,g=false,h=false,i=false,j=false,k=false,l=false,m=false,n=false,o=false,
         p=false,q=false,r=false,s=false,t=false,u=false,v=false,w=false,x=false,y=false,z=false;
    //create a bool for all special keys
    bool f1=false,f2=false,f3=false,f4=false,f5=false,f6=false,f7=false,f8=false,f9=false,f10=false,f11=false,f12=false, space=false, 
         leftShift=false, rightShift=false, leftAlt=false, rightAlt=false, shift=false, alt=false, arrowRight=false, arrowLeft=false, 
         arrowUp=false, arrowDown=false, 
         key1=false, key2=false, key3=false, key4=false, key5=false, key6=false, key7=false, key8=false, key9=false, key0=false, 
         backspace=false, enter=false, tab=false, capslockKey=false, dot=false, comma=false, dash=false, plus=false, hash=false;
        
    //store if capslock is active
    bool capslock = false;

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
    bool getKey(unsigned char key);
};

/**
 * @brief store information about the mouse in an organised way
 */
class Mouse
{
public:
    //store the position of the mouse in the window
    vec2 pos;
    //store the position of the mouse in the window in pixel space
    vec2 posPixel;
    //store the screen mouse position
    vec2 screenPos;
    //store the screen mouse position in pixels
    vec2 screenPosPixel;
    //store the status of the mouse buttons
    bool leftButton, rightButton, middleButton = false;
    //store the amoung of scrolles from the mouse wheel
    signed char mouseWheel;

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