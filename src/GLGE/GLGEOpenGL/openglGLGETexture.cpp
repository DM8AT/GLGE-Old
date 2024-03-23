/**
 * @file openglGLGETexture.cpp
 * @author DM8AT
 * @brief 
 * @version 0.1
 * @date 2024-02-27
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */
//incldue the main header
#include "openglGLGETexture.hpp"
//include the image handler
#include "../GLGEInternal/glgeImage.h"
//include the variables
#include "openglGLGEVars.hpp"
//include defines
#include "../GLGEInternal/glgePrivDefines.hpp"
//default includes
#include <iostream>

Texture::Texture()
{ /* Default constructor */ }

Texture::Texture(const char* textureFile)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = glgeLoad(textureFile, &width, &height, &nrChannels);
    if(data)
    {
        if (nrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            //store the image number of chanels and encode type
            this->encodeType = GL_RGB;
            this->channels = GL_RGB;
        }
        else if (nrChannels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            //store the image number of chanels and encode type
            this->encodeType = GL_RGBA;
            this->channels = GL_RGBA;
        }
    }
    else
    {
        std::cout << "Failed to load texture file: " << textureFile << "\n";
    }

    //store the texture size
    this->size = ivec2(width, height);
    //clear memory and return the texture
    glgeImageFree(data);
    //store the texture
    this->texture = texture;
    //store the window
    this->windowID = glgeCurrentWindowIndex;
}

Texture::Texture(vec2 size, int encodeType, void* data)
{
    //cast to another constructor
    *this = Texture(size.x, size.y, encodeType, data);
}

Texture::Texture(unsigned int width, unsigned int height, int et, void* data, int dataType)
{
    //check if the type is valide
    if (!((dataType == GLGE_FLOAT) || (dataType == GLGE_UNSIGNED_INT) || (dataType == GLGE_UNSIGNED_BYTE)
          || (dataType == GLGE_INT) || (dataType == GLGE_BYTE)))
    {
        GLGE_THROW_ERROR("Data type " + std::to_string(dataType) + " is not a valdie type")
        //safety return
        return;
    }
    //store the inputed type
    this->type = dataType;
    //generate a texture to store the last tick image
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    //decode the type
    this->decode(et);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, this->encodeType, width, height, 0, this->channels, this->type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glgeInterpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //unbind this texture
    glBindTexture(GL_TEXTURE_2D, 0);
    //store the window
    this->windowID = glgeCurrentWindowIndex;
}

void Texture::resize(vec2 size)
{
    //check if the object belongs to the window
    if (glgeCurrentWindowIndex != this->windowID)
    {
        //check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] Can't resize an image in an window it was not created in.\n";
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

    //bind the texture
    glBindTexture(GL_TEXTURE_2D, this->texture);
    //set the texture parameters so it dosn't loop around the screen
    glTexImage2D(GL_TEXTURE_2D, 0, this->encodeType, size.x, size.y, 0, this->channels, this->type, NULL);
    //unbind this texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::resize(unsigned int width, unsigned int height)
{
    //cast to another function
    this->resize(vec2(width, height));
}

void Texture::bind(int port)
{
    //if this texture is bound, stop the function
    if (this->binding != -1)
    { return; }
    //check if texture unit is set to auto
    if (port == GLGE_TEXTURE_BINDING_AUTO)
    {
        //actiave the specific texture unit
        glActiveTexture(GL_TEXTURE0 + glgeTextureUnit);
        //store the texture unit this texture is bound to
        this->binding = glgeTextureUnit;
        //increase the texture unit by 1
        glgeTextureUnit++;
        //say that auto was not used
        this->autoBind = false;
    }
    else
    {
        //activate the selected texture unit
        glActiveTexture(GL_TEXTURE0 + port);
        //store the binding
        this->binding = port;
        //cleare the texture unit
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        //say that auto was used
        this->autoBind = true;
    }
    //bind the texture
    glBindTexture(GL_TEXTURE_2D, this->texture);
}

void Texture::unbind()
{
    //if this texture is not bound, stop the function
    if (this->binding == -1)
    {
        return;
    }
    //actiave the specific texture unit
    glActiveTexture(GL_TEXTURE0 + this->binding);
    //reset the texture binding
    this->binding = -1;
    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    //check if autobind was not used
    if (!this->autoBind)
    {
        //decrease the main texture unit by one
        glgeTextureUnit--;
    }
}

void Texture::draw()
{
    //check if the object belongs to the window
    if (glgeCurrentWindowIndex != this->windowID)
    {
        //check if an error should be printed
        if (glgeErrorOutput)
        {
            //print an error
            std::cerr << "[GLGE ERROR] Can't draw a texture in an window it was not created in.\n";
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
    //disable this texture
    this->unbind();

    //disable depth testing
    glDisable(GL_DEPTH_TEST);
    //enable color blending
    glEnable(GL_BLEND);
    //setup the blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //get the image shader
    Shader* imgShader = glgeWindows[this->windowID]->getDefaultImageShader();
    //set the image
    imgShader->setCustomTexture("image", this->texture);
    //bind the shader
    imgShader->applyShader();
    //bind this texture
    this->bind();

    //bind the screen rect
    glgeWindows[this->windowID]->bindScreenRect();

    //draw the screen
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //unbind the screen rect
    glgeWindows[this->windowID]->unbindScreenRect();
    //unbind the shader
    imgShader->removeShader();
    //unbind this texture
    this->unbind();
}

ivec2 Texture::getSize()
{
    //return the size
    return this->size;
}

unsigned int Texture::getOpenGLTexture()
{
    //return the texture
    return this->texture;
}

///////////////////////
// PRIVATE FUNCTIONS //
///////////////////////

void Texture::decode(int encodeType)
{
    switch (encodeType)
    {
    //STRUCTURE: 
    // the type to check for
    //  the way to encode the chanels
    //  the chanels
    //  the type as wich the chanels should be encoded
    case GLGE_TEX_RGB:
        this->encodeType = GL_RGB;
        this->channels = GL_RGB;
        break;

    case GLGE_TEX_RGBA:
        this->encodeType = GL_RGBA;
        this->channels = GL_RGBA;
        break;

    case GLGE_TEX_RGB8:
        this->encodeType = GL_RGB8;
        this->channels = GL_RGB;
        break;

    case GLGE_TEX_RGBA8:
        this->encodeType = GL_RGBA8;
        this->channels = GL_RGBA;
        break;

    case GLGE_TEX_RGB16:
        this->encodeType = GL_RGB16F;
        this->channels = GL_RGB;
        break;

    case GLGE_TEX_RGBA16:
        this->encodeType = GL_RGBA16F;
        this->channels = GL_RGBA;
        break;

    case GLGE_TEX_RGB32:
        this->encodeType = GL_RGB32F;
        this->channels = GL_RGB;
        break;

    case GLGE_TEX_RGBA32:
        this->encodeType = GL_RGBA32F;
        this->channels = GL_RGBA;
        break;

    case GLGE_TEX_RGB4:
        this->encodeType = GL_RGB4;
        this->channels = GL_RGB;
        break;

    case GLGE_TEX_RGBA4:
        this->encodeType = GL_RGBA4;
        this->channels = GL_RGBA;
        break;

    case GLGE_TEX_R:
        this->encodeType = GL_RED;
        this->channels = GL_RED;
        break;

    case GLGE_TEX_R8:
        this->encodeType = GL_R8;
        this->channels = GL_RED;

        break;

    case GLGE_TEX_R16:
        this->encodeType = GL_R16F;
        this->channels = GL_RED;

        break;

    case GLGE_TEX_R32:
        this->encodeType = GL_R32F;
        this->channels = GL_RED;

        break;

    case GLGE_TEX_RG:
        this->encodeType = GL_RG;
        this->channels = GL_RG;
        break;

    case GLGE_TEX_RG8:
        this->encodeType = GL_RG8;
        this->channels = GL_RG;
        break;

    case GLGE_TEX_RG16:
        this->encodeType = GL_RG16F;
        this->channels = GL_RG;
        break;

    case GLGE_TEX_RG32:
        this->encodeType = GL_RG32F;
        this->channels = GL_RG;
        break;

    case GLGE_TEX_R3_G3_B2:
        this->encodeType = GL_R3_G3_B2;
        this->channels = GL_RGB;
        break;
    
    //only called on an invalide type
    default:
        //check if a warning should be outputed
        if (glgeWarningOutput)
        {
            //print an warning
            printf("[GLGE WARNING] Unknown texture encoding type %d\n", encodeType);
        }
        //use RGB-Float as default
        this->encodeType = GL_RGB;
        this->channels = GL_RGB;

        return;
    }
}