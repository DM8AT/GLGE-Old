/**
 * @file GLGEKlasses.cpp
 * @author DM8AT
 * @brief define the classes for GLGE.h, this will be included without any kind of core
 * @version 0.1
 * @date 2023-02-09
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

//include the class for defining things
#include "GLGEKlasses.hpp"

//include freeglut
#include <GL/freeglut.h>

#include <iostream>

/////////////
//KEY CLASS//
/////////////

Keys::Keys()
{
    //intialise the object
    //set all keys to false
    this->clear();
}

//update a key
void Keys::keyUpdate(unsigned char key, bool status)
{
    //check if a should be updated
    if (key == (unsigned char)'a' || key == (unsigned char)'A')
    {
        this->a = status;
    }
    //check if b should be updated
    else if (key == (unsigned char)'b' || key == (unsigned char)'B')
    {
        this->b = status;
    }
    //check if c should be updated
    else if (key == (unsigned char)'c' || key == (unsigned char)'C')
    {
        this->c = status;
    }
    //check if d should be updated
    else if (key == (unsigned char)'d' || key == (unsigned char)'D')
    {
        this->d = status;
    }
    //check if e should be updated
    else if (key == (unsigned char)'e' || key == (unsigned char)'E')
    {
        this->e = status;
    }
    //check if f should be updated
    else if (key == (unsigned char)'f' || key == (unsigned char)'F')
    {
        this->f = status;
    }
    //check if g should be updated
    else if (key == (unsigned char)'g' || key == (unsigned char)'G')
    {
        this->g = status;
    }
    //check if h should be updated
    else if (key == (unsigned char)'h' || key == (unsigned char)'H')
    {
        this->h = status;
    }
    //check if i should be updated
    else if (key == (unsigned char)'i' || key == (unsigned char)'I')
    {
        this->i = status;
    }
    //check if j should be updated
    else if (key == (unsigned char)'j' || key == (unsigned char)'J')
    {
        this->j = status;
    }
    //check if k should be updated
    else if (key == (unsigned char)'k' || key == (unsigned char)'K')
    {
        this->k = status;
    }
    //check if l should be updated
    else if (key == (unsigned char)'l' || key == (unsigned char)'L')
    {
        this->l = status;
    }
    //check if m should be updated
    else if (key == (unsigned char)'m' || key == (unsigned char)'M')
    {
        this->m = status;
    }
    //check if n should be updated
    else if (key == (unsigned char)'n' || key == (unsigned char)'N')
    {
        this->n = status;
    }
    //check if o should be updated
    else if (key == (unsigned char)'o' || key == (unsigned char)'O')
    {
        this->o = status;
    }
    //check if p should be updated
    else if (key == (unsigned char)'p' || key == (unsigned char)'P')
    {
        this->p = status;
    }
    //check if q should be updated
    else if (key == (unsigned char)'q' || key == (unsigned char)'Q')
    {
        this->q = status;
    }
    //check if r should be updated
    else if (key == (unsigned char)'r' || key == (unsigned char)'R')
    {
        this->r = status;
    }
    //check if s should be updated
    else if (key == (unsigned char)'s' || key == (unsigned char)'S')
    {
        this->s = status;
    }
    //check if t should be updated
    else if (key == (unsigned char)'t' || key == (unsigned char)'T')
    {
        this->t = status;
    }
    //check if u should be updated
    else if (key == (unsigned char)'u' || key == (unsigned char)'U')
    {
        this->u = status;
    }
    //check if v should be updated
    else if (key == (unsigned char)'v' || key == (unsigned char)'V')
    {
        this->v = status;
    }
    //check if w should be updated
    else if (key == (unsigned char)'w' || key == (unsigned char)'W')
    {
        this->w = status;
    }
    //check if x should be updated
    else if (key == (unsigned char)'x' || key == (unsigned char)'X')
    {
        this->x = status;
    }
    //check if y should be updated
    else if (key == (unsigned char)'y' || key == (unsigned char)'Y')
    {
        this->y = status;
    }
    //check if z should be updated
    else if (key == (unsigned char)'z' || key == (unsigned char)'Z')
    {
        this->z = status;
    }
    //check if space should be updated
    else if (key == (unsigned char)' ')
    {
        this->space = status;
    }
}

//update a special key
void Keys::keyUpdate(int key, bool status)
{
    //check for the function keys
    //check for F1
    if (key == GLUT_KEY_F1)
    {
        this->f1 = status;
    }
    //check for F2
    else if (key == GLUT_KEY_F2)
    {
        this->f2 = status;
    }
    //check for F3
    else if (key == GLUT_KEY_F3)
    {
        this->f3 = status;
    }
    //check for F4
    else if (key == GLUT_KEY_F4)
    {
        this->f4 = status;
    }
    //check for F5
    else if (key == GLUT_KEY_F5)
    {
        this->f5 = status;
    }
    //check for F6
    else if (key == GLUT_KEY_F6)
    {
        this->f6 = status;
    }
    //check for F7
    else if (key == GLUT_KEY_F7)
    {
        this->f7 = status;
    }
    //check for F8
    else if (key == GLUT_KEY_F8)
    {
        this->f8 = status;
    }
    //check for F9
    else if (key == GLUT_KEY_F9)
    {
        this->f9 = status;
    }
    //check for F10
    else if (key == GLUT_KEY_F10)
    {
        this->f10 = status;
    }
    //check for F11
    else if (key == GLUT_KEY_F11)
    {
        this->f11 = status;
    }
    //check for F12
    else if (key == GLUT_KEY_F12)
    {
        this->f12 = status;
    } 
    //check for shift keys
    else if (key == GLUT_KEY_SHIFT_L)
    {
        this->leftShift = status;
        //check if the other shift key is pressed
        if (!this->rightShift)
        {
            this->shift = status;
        }
    }
    else if (key == GLUT_KEY_SHIFT_R)
    {
        this->rightShift = status;
        //check if the other shift key is pressed
        if (!this->leftShift)
        {
            this->shift = status;
        }
    }
    //check for arrow keys
    else if (key == GLUT_KEY_RIGHT)
    {
        this->arrowRight = status;
    }
    else if (key == GLUT_KEY_LEFT)
    {
        this->arrowLeft = status;
    }
    else if (key == GLUT_KEY_UP)
    {
        this->arrowUp = status;
    }
    else if (key == GLUT_KEY_DOWN)
    {
        this->arrowDown =status;
    }
}

bool Keys::getKey(int key)
{
    //check for the function keys
    //check for F1
    if (key == GLUT_KEY_F1)
    {
        return this->f1;
    }
    //check for F2
    else if (key == GLUT_KEY_F2)
    {
        return this->f2;
    }
    //check for F3
    else if (key == GLUT_KEY_F3)
    {
        return this->f3;
    }
    //check for F4
    else if (key == GLUT_KEY_F4)
    {
        return this->f4;
    }
    //check for F5
    else if (key == GLUT_KEY_F5)
    {
        return this->f5;
    }
    //check for F6
    else if (key == GLUT_KEY_F6)
    {
        return this->f6;
    }
    //check for F7
    else if (key == GLUT_KEY_F7)
    {
        return this->f7;
    }
    //check for F8
    else if (key == GLUT_KEY_F8)
    {
        return this->f8;
    }
    //check for F9
    else if (key == GLUT_KEY_F9)
    {
        return this->f9;
    }
    //check for F10
    else if (key == GLUT_KEY_F10)
    {
        return this->f10;
    }
    //check for F11
    else if (key == GLUT_KEY_F11)
    {
        return this->f11;
    }
    //check for F12
    else if (key == GLUT_KEY_F12)
    {
        return this->f12;
    } 
    //check for shift keys
    else if (key == GLUT_KEY_SHIFT_L)
    {
        return this->leftShift;
    }
    else if (key == GLUT_KEY_SHIFT_R)
    {
        return this->rightShift;
    }
    //check for arrow keys
    else if (key == GLUT_KEY_RIGHT)
    {
        return this->arrowRight;
    }
    else if (key == GLUT_KEY_LEFT)
    {
        return this->arrowLeft;
    }
    else if (key == GLUT_KEY_UP)
    {
        return this->arrowUp;
    }
    else if (key == GLUT_KEY_DOWN)
    {
        return this->arrowDown;
    }

    //if the key is invalide, return false
    return false;
}

bool Keys::getKey(unsigned char key)
{
    //check if a should be updated
    if (key == (unsigned char)'a' || key == (unsigned char)'A')
    {
        return this->a;
    }
    //check if b should be updated
    else if (key == (unsigned char)'b' || key == (unsigned char)'B')
    {
        return this->b;
    }
    //check if c should be updated
    else if (key == (unsigned char)'c' || key == (unsigned char)'C')
    {
        return this->c;
    }
    //check if d should be updated
    else if (key == (unsigned char)'d' || key == (unsigned char)'D')
    {
        return this->d;
    }
    //check if e should be updated
    else if (key == (unsigned char)'e' || key == (unsigned char)'E')
    {
        return this->e;
    }
    //check if f should be updated
    else if (key == (unsigned char)'f' || key == (unsigned char)'F')
    {
        return this->f;
    }
    //check if g should be updated
    else if (key == (unsigned char)'g' || key == (unsigned char)'G')
    {
        return this->g;
    }
    //check if h should be updated
    else if (key == (unsigned char)'h' || key == (unsigned char)'H')
    {
        return this->h;
    }
    //check if i should be updated
    else if (key == (unsigned char)'i' || key == (unsigned char)'I')
    {
        return this->i;
    }
    //check if j should be updated
    else if (key == (unsigned char)'j' || key == (unsigned char)'J')
    {
        return this->j;
    }
    //check if k should be updated
    else if (key == (unsigned char)'k' || key == (unsigned char)'K')
    {
        return this->k;
    }
    //check if l should be updated
    else if (key == (unsigned char)'l' || key == (unsigned char)'L')
    {
        return this->l;
    }
    //check if m should be updated
    else if (key == (unsigned char)'m' || key == (unsigned char)'M')
    {
        return this->m;
    }
    //check if n should be updated
    else if (key == (unsigned char)'n' || key == (unsigned char)'N')
    {
        return this->n;
    }
    //check if o should be updated
    else if (key == (unsigned char)'o' || key == (unsigned char)'O')
    {
        return this->o;
    }
    //check if p should be updated
    else if (key == (unsigned char)'p' || key == (unsigned char)'P')
    {
        return this->p;
    }
    //check if q should be updated
    else if (key == (unsigned char)'q' || key == (unsigned char)'Q')
    {
        return this->q;
    }
    //check if r should be updated
    else if (key == (unsigned char)'r' || key == (unsigned char)'R')
    {
        return this->r;
    }
    //check if s should be updated
    else if (key == (unsigned char)'s' || key == (unsigned char)'S')
    {
        return this->s;
    }
    //check if t should be updated
    else if (key == (unsigned char)'t' || key == (unsigned char)'T')
    {
        return this->t;
    }
    //check if u should be updated
    else if (key == (unsigned char)'u' || key == (unsigned char)'U')
    {
        return this->u;
    }
    //check if v should be updated
    else if (key == (unsigned char)'v' || key == (unsigned char)'V')
    {
        return this->v;
    }
    //check if w should be updated
    else if (key == (unsigned char)'w' || key == (unsigned char)'W')
    {
        return this->w;
    }
    //check if x should be updated
    else if (key == (unsigned char)'x' || key == (unsigned char)'X')
    {
        return this->x;
    }
    //check if y should be updated
    else if (key == (unsigned char)'y' || key == (unsigned char)'Y')
    {
        return this->y;
    }
    //check if z should be updated
    else if (key == (unsigned char)'z' || key == (unsigned char)'Z')
    {
        return this->z;
    }
    //check if space should be updated
    else if (key == (unsigned char)' ')
    {
        return this->space;
    }

    //if the key is not valide, return false
    return false;
}

void Keys::clear()
{
    //set all keys to false
    this->a = false;
    this->b = false;
    this->c = false;
    this->d = false;
    this->e = false;
    this->f = false;
    this->g = false;
    this->h = false;
    this->i = false;
    this->j = false;
    this->k = false;
    this->l = false;
    this->m = false;
    this->n = false;
    this->o = false;
    this->p = false;
    this->q = false;
    this->r = false;
    this->s = false;
    this->t = false;
    this->u = false;
    this->v = false;
    this->w = false;
    this->x = false;
    this->y = false;
    this->z = false;

    //set all function keys to false
    this->f1 = false;
    this->f2 = false;
    this->f3 = false;
    this->f4 = false;
    this->f5 = false;
    this->f6 = false;
    this->f7 = false;
    this->f8 = false;
    this->f9 = false;
    this->f10 = false;
    this->f11 = false;
    this->f12 = false;

    //set the misc keys to false
    this->space = false;
    this->leftShift = false;
    this->rightShift = false;
    this->leftAlt = false;
    this->rightAlt = false;
    this->shift = false;
    this->alt = false;
    this->arrowRight = false;
    this->arrowLeft = false;
    this->arrowUp = false;
    this->arrowDown = false;
}

//MOUSE

Mouse::Mouse()
{
    //init the object
}

//update the mouse pressed buttons
void Mouse::update(int button, int state, int, int)
{
    //check if the inputed button is the left mouse button
    if (button == GLUT_LEFT_BUTTON)
    {
        //set the key to true if it is pressed
        if (state == GLUT_DOWN)
        {
            this->leftButton = true;
        }
        //set the key to false if it is not pressed
        else if (state == GLUT_UP)
        {
            this->leftButton = false;
        }
    }
    //check if the button is the right mouse button
    else if (button == GLUT_RIGHT_BUTTON)
    {
        //set the key to true if it is pressed
        if (state == GLUT_DOWN)
        {
            this->rightButton = true;
        }
        //set the key to false if it is not pressed
        else if (state == GLUT_UP)
        {
            this->rightButton = false;
        }
    }
    //check if the button is the middle mouse button
    else if (button == GLUT_MIDDLE_BUTTON)
    {
        //set the key to true if it is pressed
        if (state == GLUT_DOWN)
        {
            this->middleButton = true;
        }
        //set the key to false if it is not pressed
        else if (state == GLUT_UP)
        {
            this->middleButton = false;
        }
    }
    //neglate the button up
    else if (state == GLUT_UP)
    {
        return;
    }
    //if the button is the mouse scroll up, increase the scroll count
    else if (button == 3)
    {
        this->mouseWeel++;
    }
    //if the button is the mouse scroll down, decrease the scroll count
    else if (button == 4)
    {
        this->mouseWeel--;
    }
}

void Mouse::updatePos(int x, int y)
{
    //store the inputed position as the pixel positoin
    this->posPixel = vec2(x,y);
    //set the normalised position to the pixel position scaled by the inverse of the screen width & height
    this->pos = vec2((float)x / (float)glutGet(GLUT_WINDOW_WIDTH), (float )y / (float)glutGet(GLUT_WINDOW_HEIGHT));
}