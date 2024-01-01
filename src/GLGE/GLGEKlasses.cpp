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
//check if glew is allready included
#ifndef _GLGE_GLEW_
//say that glew is now included
#define _GLGE_GLEW_
//include glew
#include <GL/glew.h>
//close the if for glew
#endif

//include the class for defining things
#include "GLGEKlasses.hpp"

//include the GLGE dependencys
#include "glgeVars.hpp"
#include "glgeErrors.hpp"

//include freeglut
#include <GL/glew.h>

#include <iostream>

//include SDL2
#ifdef _WIN32
#include <SDL/SDL.h> /* Windows-specific SDL2 library */
#include <SDL/SDL_opengl.h>
#include <SDL2/SDL_misc.h>
#else
#include <SDL2/SDL.h> /* macOS- and GNU/Linux-specific */
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_misc.h>
#endif

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
void Keys::keyUpdate(int key, bool status)
{
    //update the key according to the key
    switch (key)
    {
        //check if the key is a
        case SDL_SCANCODE_A:
            //update a
            this->a = status;
            //exit
            break;
        //check if the key is b
        case SDL_SCANCODE_B:
            //update b
            this->b = status;
            //exit
            break;
        //check if the key is c
        case SDL_SCANCODE_C:
            //update c
            this->c = status;
            //exit
            break;
        //check if the key is d
        case SDL_SCANCODE_D:
            //update d
            this->d = status;
            //exit
            break;
        //check if the key is e
        case SDL_SCANCODE_E:
            //update e
            this->e = status;
            //exit
            break;
        //check if the key is f
        case SDL_SCANCODE_F:
            //update f
            this->f = status;
            //exit
            break;
        //check if the key is g
        case SDL_SCANCODE_G:
            //update g
            this->g = status;
            //exit
            break;
        //check if the key is h
        case SDL_SCANCODE_H:
            //update h
            this->h = status;
            //exit
            break;
        //check if the key is i
        case SDL_SCANCODE_I:
            //update i
            this->i = status;
            //exit
            break;
        //check if the key is j
        case SDL_SCANCODE_J:
            //update j
            this->j = status;
            //exit
            break;
        //check if the key is k
        case SDL_SCANCODE_K:
            //update k
            this->k = status;
            //exit
            break;
        //check if the key is l
        case SDL_SCANCODE_L:
            //update l
            this->l = status;
            //exit
            break;
        //check if the key is m
        case SDL_SCANCODE_M:
            //update m
            this->m = status;
            //exit
            break;
        //check if the key is n
        case SDL_SCANCODE_N:
            //update n
            this->n = status;
            //exit
            break;
        //check if the key is o
        case SDL_SCANCODE_O:
            //update o
            this->o = status;
            //exit
            break;
        //check if the key is p
        case SDL_SCANCODE_P:
            //update p
            this->p = status;
            //exit
            break;
        //check if the key is q
        case SDL_SCANCODE_Q:
            //update q
            this->q = status;
            //exit
            break;
        //check if the key is r
        case SDL_SCANCODE_R:
            //update r
            this->r = status;
            //exit
            break;
        //check if the key is s
        case SDL_SCANCODE_S:
            //update s
            this->s = status;
            //exit
            break;
        //check if the key is t
        case SDL_SCANCODE_T:
            //update t
            this->t = status;
            //exit
            break;
        //check if the key is u
        case SDL_SCANCODE_U:
            //update u
            this->u = status;
            //exit
            break;
        //check if the key is v
        case SDL_SCANCODE_V:
            //update v
            this->v = status;
            //exit
            break;
        //check if the key is w
        case SDL_SCANCODE_W:
            //update w
            this->w = status;
            //exit
            break;
        //check if the key is x
        case SDL_SCANCODE_X:
            //update x
            this->x = status;
            //exit
            break;
        //check if the key is y
        case SDL_SCANCODE_Y:
            //update y
            this->y = status;
            //exit
            break;
        //check if the key is z
        case SDL_SCANCODE_Z:
            //update z
            this->z = status;
            //exit
            break;
        //check if the key is space
        case SDL_SCANCODE_SPACE:
            //update space
            this->space = status;
            //exit
            break;
        //check if the key is f1
        case SDL_SCANCODE_F1:
            //update f1
            this->f1 = status;
            //exit
            break;
        //check if the key is f2
        case SDL_SCANCODE_F2:
            //update f2
            this->f2 = status;
            //exit
            break;
        //check if the key is f3
        case SDL_SCANCODE_F3:
            //update f3
            this->f3 = status;
            //exit
            break;
        //check if the key is f4
        case SDL_SCANCODE_F4:
            //update f4
            this->f4 = status;
            //exit
            break;
        //check if the key is f5
        case SDL_SCANCODE_F5:
            //update f5
            this->f5 = status;
            //exit
            break;
        //check if the key is f6
        case SDL_SCANCODE_F6:
            //update f6
            this->f6 = status;
            //exit
            break;
        //check if the key is f7
        case SDL_SCANCODE_F7:
            //update f7
            this->f7 = status;
            //exit
            break;
        //check if the key is f8
        case SDL_SCANCODE_F8:
            //update f8
            this->f8 = status;
            //exit
            break;
        //check if the key is f9
        case SDL_SCANCODE_F9:
            //update f9
            this->f9 = status;
            //exit
            break;
        //check if the key is f10
        case SDL_SCANCODE_F10:
            //update f10
            this->f10 = status;
            //exit
            break;
        //check if the key is f11
        case SDL_SCANCODE_F11:
            //update f11
            this->f11 = status;
            //exit
            break;
        //check if the key is f12
        case SDL_SCANCODE_F12:
            //update f12
            this->f12 = status;
            //exit
            break;
        //check if the key is r_shift
        case SDL_SCANCODE_RSHIFT:
            //update r_shift
            this->rightShift = status;
            //update the shift variable
            this->shift = (this->rightShift || this->leftShift);
            //exit
            break;
        //check if the key is l_shift
        case SDL_SCANCODE_LSHIFT:
            //update l_shift
            this->leftShift = status;
            //update the shift variable
            this->shift = (this->rightShift || this->leftShift);
            //exit
            break;
        //check if the right arrow key is pressed
        case SDL_SCANCODE_RIGHT:
            //update the right arrow
            this->arrowRight = status;
            //exit
            break;
        //check if the left arrow key is pressed
        case SDL_SCANCODE_LEFT:
            //update the left arrow
            this->arrowLeft = status;
            //exit
            break;
        //check if the up arrow key is pressed
        case SDL_SCANCODE_UP:
            //update the up arrow
            this->arrowUp = status;
            //exit
            break;
        //check if the down arrow key is pressed
        case SDL_SCANCODE_DOWN:
            //update the down arrow
            this->arrowDown = status;
            //exit
            break;
        //check if the right alt key is pressed
        case SDL_SCANCODE_RALT:
            //update the right arrow
            this->rightAlt = status;
            //update the alt variable
            this->alt = (this->rightAlt || this->leftAlt);
            //exit
            break;
        //check if the left alt key is pressed
        case SDL_SCANCODE_LALT:
            //update the left arrow
            this->leftAlt = status;
            //update the alt variable
            this->alt = (this->rightAlt || this->leftAlt);
            //exit
            break;
    }
}

bool Keys::getKey(int key)
{
    //check for the function keys
    //check for F1
    if (key == SDL_SCANCODE_F1)
    {
        return this->f1;
    }
    //check for F2
    else if (key == SDL_SCANCODE_F2)
    {
        return this->f2;
    }
    //check for F3
    else if (key == SDL_SCANCODE_F3)
    {
        return this->f3;
    }
    //check for F4
    else if (key == SDL_SCANCODE_F4)
    {
        return this->f4;
    }
    //check for F5
    else if (key == SDL_SCANCODE_F5)
    {
        return this->f5;
    }
    //check for F6
    else if (key == SDL_SCANCODE_F6)
    {
        return this->f6;
    }
    //check for F7
    else if (key == SDL_SCANCODE_F7)
    {
        return this->f7;
    }
    //check for F8
    else if (key == SDL_SCANCODE_F8)
    {
        return this->f8;
    }
    //check for F9
    else if (key == SDL_SCANCODE_F9)
    {
        return this->f9;
    }
    //check for F10
    else if (key == SDL_SCANCODE_F10)
    {
        return this->f10;
    }
    //check for F11
    else if (key == SDL_SCANCODE_F11)
    {
        return this->f11;
    }
    //check for F12
    else if (key == SDL_SCANCODE_F12)
    {
        return this->f12;
    } 
    //check for shift keys
    else if (key == SDL_SCANCODE_LSHIFT)
    {
        return this->leftShift;
    }
    else if (key == SDL_SCANCODE_RSHIFT)
    {
        return this->rightShift;
    }
    //check for arrow keys
    else if (key == SDL_SCANCODE_RIGHT)
    {
        return this->arrowRight;
    }
    else if (key == SDL_SCANCODE_LEFT)
    {
        return this->arrowLeft;
    }
    else if (key == SDL_SCANCODE_UP)
    {
        return this->arrowUp;
    }
    else if (key == SDL_SCANCODE_DOWN)
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
void Mouse::update(int button, int state)
{
    //check if the inputed button is the left mouse button
    if (button == SDL_BUTTON_LEFT)
    {
        //store the state
        this->leftButton = (bool)state;
    }
    //check if the button is the right mouse button
    else if (button == SDL_BUTTON_RIGHT)
    {
        //store the state
        this->rightButton = (bool)state;
    }
    //check if the button is the middle mouse button
    else if (button == SDL_BUTTON_MIDDLE)
    {
        //store the state
        this->middleButton = (bool)state;
    }
    //if the button is the mouse scroll, scroll by the input
    else if (button == GLGE_MOUSE_SCROLL)
    {
        this->mouseWheel += state;
    }
}

/////////////////
//RENDER TARGET//
/////////////////

//PUBLIC

RenderTarget::RenderTarget()
{
    //initalise everyting
}

RenderTarget::RenderTarget(vec2 size)
{
    //generate the texture and the render target
    this->generateTexture(size.x, size.y, true);
    //store the current window index as the own window index
    this->windowIndex = glgeCurrentWindowIndex;
}

RenderTarget::RenderTarget(int width, int height)
{
    //generate the texture and the render target
    this->generateTexture(width, height, true);
    //store the current window index as the own window index
    this->windowIndex = glgeCurrentWindowIndex;
}

RenderTarget::RenderTarget(unsigned int glTexture)
{
    //store the inputed texture
    this->texture = glTexture;
    //two ints to store the width and height
    int w,h = 0;
    //activate texture 0
    glActiveTexture(GL_TEXTURE0);
    //bind the texture
    glBindTexture(GL_TEXTURE_2D, this->texture);
    //get the width from the texture
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    //get the height from the texture
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
    //generate the render target
    this->generateTexture(w,h, false);
    //store the current window index as the own window index
    this->windowIndex = glgeCurrentWindowIndex;
}

void RenderTarget::draw()
{
    //check if the object belongs to the window
    if (glgeCurrentWindowIndex != this->windowIndex)
    {
        //check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] Can't draw an render target in an window it was not created in.\n";
        }
        //check if the program should close on an error
        if (glgeExitOnError)
        {
            //close with an error
            exit(1);
        }
        //stop the function
        return;
    }
    //check if the shader is a nullpointer
    if (this->shader == NULL)
    {
        //print an warning
        if (glgeWarningOutput)
        {
            //print the warning
            std::cerr << "[GLGE WARNING] shader for a custom render target is a nullpointer, but it\'s draw methode was called" << "\n";
        }
        //stop the script
        return;
    }

    //check if a callback function is bound
    if (this->callback != NULL)
    {
        //call the callback function, taking this as an input
        this->callback(this);
    }

    //activate the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

    //activate the shader
    this->shader->applyShader();

    //bind the screen rect
    glgeWindows[this->windowIndex]->bindScreenRect();

    //draw the screen
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //unbind the screen rect
    glgeWindows[this->windowIndex]->unbindScreenRect();

    //unbind the shader
    this->shader->removeShader();
    //make sure to enable texture unit 0
    glActiveTexture(GL_TEXTURE0);

    //unbind the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::changeSize(vec2 s)
{
    //update the inernal size
    this->updateSize(s);
}

void RenderTarget::setCustomCallbackFunction(void(*func)(RenderTarget*))
{
    //store the inputed function (NULL is catched later)
    this->callback = func;
}

void RenderTarget::changeSize(int newWidth, int newHeight)
{
    //update the inernal size
    this->updateSize(vec2(newWidth, newHeight));
}

void RenderTarget::setShader(Shader* shader, bool ownShader)
{
    //check if an own shader is currently used
    if (ownShader)
    {
        //if it is, delete the shader
        delete this->shader;
    }
    //store the inputed shader
    this->shader = shader;
    //say that the shader isn't its own
    this->ownShader = ownShader;
}

void RenderTarget::setShader(unsigned int shader)
{
    //check if an own shader is currently used
    if (ownShader)
    {
        //if it is, delete the shader
        delete this->shader;
    }
    //generate the shader
    this->shader = new Shader(shader);
    //say that the sader is its own
    this->ownShader = true;
}

Shader* RenderTarget::getShader()
{
    //return the shader
    return this->shader;
}

unsigned int RenderTarget::getTexture()
{
    //return the texture
    return this->texture;
}

vec2 RenderTarget::getSize()
{
    //return the stored size
    return this->size;
}

//PRIVATE

void RenderTarget::generateTexture(int w, int h, bool genTexture)
{
    //check if the size is to small
    if ((w < 1) || (h < 1))
    {
        //check if an error should be outputed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] an size of " << w << ", " << h << "\n";
        }
        //check if the program should close on an error
        if (glgeExitOnError)
        {
            //close with error code 1
            exit(1);
        }
    }

    //create and bind the custom frame buffer
    glGenFramebuffers(1, &this->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

    if (genTexture)
    
    {
        //generate a texture to store the render target texture
        glGenTextures(1, &this->texture);
        glBindTexture(GL_TEXTURE_2D, this->texture);
        //set the texture parameters so it dosn't loop around the screen
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //bind the texture to the frame buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);
        //unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        //bind the texture
        glBindTexture(GL_TEXTURE_2D, this->texture);
        //bind the texture to the frame buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);
        //unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    //check if the framebuffer compiled correctly
    unsigned int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //if the frame buffer compiled not correctly
    if ( fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        //print an error
        std::cerr << GLGE_FATAL_ERROR_FRAMEBUFFER_NOT_COMPILED << fboStatus << "\n";
        //stop the program
        exit(1);
    }

    //store the width and height
    this->size = vec2(w,h);
    //update the parameter if the texture is owned
    this->ownTex = genTexture;
    //unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //unbind the render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderTarget::updateSize(vec2 s)
{
    //check if the size is to small
    if ((s.x < 1.f) || (s.y < 1.f))
    {
        //check if an error should be outputed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] an size of " << s.x << ", " << s.y << " is not possible for a render target" << "\n";
        }
        //check if the program should close on an error
        if (glgeExitOnError)
        {
            //close with error code 1
            exit(1);
        }
    }
    //check if the texture exists
    if (this->texture != 0)
    {
        //if it dose, resize it

        //bind texture unit 0
        glActiveTexture(GL_TEXTURE0);
        //unbind any 3D textures
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        //update the texture
        glBindTexture(GL_TEXTURE_2D, this->texture);
        //set the texture parameters so it dosn't loop around the screen
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, s.x, s.y, 0, GL_RGBA, GL_FLOAT, NULL);
        //unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    //change the internal size
    this->size = s;
}