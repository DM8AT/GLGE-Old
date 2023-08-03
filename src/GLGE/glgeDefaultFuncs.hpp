/**
 * @file glgeDefaultFuncs.h
 * @author DM8AT
 * @brief this is a file designed to be used only internaly by the GLGE library
 * 
 * in this file, all the functions needed for behind the sceens operations of GLGE are stored
 * 
 * @date 2022-12-23
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */
#ifndef _GLGE_DEFAULT_FUNCS_H_
#define _GLGE_DEFAULT_FUNCS_H_

/**
 * @brief this is the default display function for GLGE
 * update the screen
 */
void glgeDefaultDisplay();

/**
 * @brief this is the default timer function for GLGE
 * limit the timer to the set maximal frames per second
 */
void glgeDefaultTimer();

/**
 * @brief the default keyboard function for key presses
 * store the pressed keys
 * @param key the pressed key
 */
void glgeDefaultKeyFunc(int key);

/**
 * @brief the default keyboard function for lifting a key
 * store wich keys aren't pressed
 * @param key the key that is no longer pressed
 */
void glgeDefaultKeyUpFunc(int key);

/**
 * @brief this function is called when a mouse button updates
 * store the mouse buttons
 * @param button the updated mouse button
 * @param state the state for the mouse button
 * @param x the mouse x position
 * @param y the mouse y position
 */
void glgeDefaultMouseFunc(int button, int state);

/**
 * @brief this function is called if the window is resize
 * 
 * @param width the new width of the window
 * @param height the new height of the window
 */
void glgeDefaultResizeFunc(int width, int height);

/**
 * @brief a function that is called if the window is moved
 * 
 * @param x the x position of the window
 * @param y the y position of the window
 */
void glgeDefaultMoveFunc(int x, int y);

#endif