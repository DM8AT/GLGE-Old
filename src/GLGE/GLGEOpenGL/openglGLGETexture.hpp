/**
 * @file openglGLGETexture.hpp
 * @author DM8AT
 * @brief 
 * @version 0.1
 * @date 2024-02-27
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_TEXTURE_OGL_H_
#define _GLGE_TEXTURE_OGL_H_

//include the lib base
#include "openglGLGE.h"

//define the value for auto-binding
#define GLGE_TEXTURE_BINDING_AUTO -1

//defines to specify the types

/**
 * @brief a simple texture format with 3 chanels for red, green and blue | 
 * 
 * SIZE: dynamic |
 * CHANELS: 3 |
 * ENCODING: float
 */
#define GLGE_TEX_RGB 1
/**
 * @brief a simple texture format with 4 chanels for red, green, blue and a alpha chanel | 
 * 
 * SIZE: dynamic |
 * CHANELS: 4 |
 * ENCODING: float
 */
#define GLGE_TEX_RGBA 2
/**
 * @brief an RGB texture with 8 bits per chanel |
 * 
 * SIZE: CHANELS * 8 |
 * CHANELS: 3 |
 * ENCODING: float
 */
#define GLGE_TEX_RGB8 3
/**
 * @brief an RGBA texture with 8 bits per chanel |
 * 
 * SIZE: CHANELS * 8 |
 * CHANELS: 4 |
 * ENCODING: float
 */
#define GLGE_TEX_RGBA8 4
/**
 * @brief an RGB texture with 16 bits per chanel | 
 * 
 * SIZE: CHANELS * 16 |
 * CHANELS: 3 |
 * ENCODING: float
 */
#define GLGE_TEX_RGB16 5
/**
 * @brief an RGBA texture with 16 bits per chanel |
 * 
 * SIZE: CHANELS * 16 |
 * CHANELS: 4 |
 * ENCODING: float
 */
#define GLGE_TEX_RGBA16 6
/**
 * @brief an RGB texture with 32 bits per chanel | 
 * 
 * SIZE: CHANELS * 32 |
 * CHANELS: 3 |
 * ENCODING: float
 */
#define GLGE_TEX_RGB32 7
/**
 * @brief an RGBA texture with 32 bits per chanel |
 * 
 * SIZE: CHANELS * 32 |
 * CHANELS: 4 |
 * ENCODING: float
 */
#define GLGE_TEX_RGBA32 8
/**
 * @brief an RGB texture with 4 bits per chanel |
 * 
 * SIZE: CHANELS * 4 |
 * CHANELS: 3 |
 * ENCODING: float
 */
#define GLGE_TEX_RGB4 9
/**
 * @brief an RGBA texture with 4 bits per chanel | 
 * 
 * SIZE: CHANELS * 4 |
 * CHANELS: 4 |
 * ENCODING: float
 */
#define GLGE_TEX_RGBA4 10
/**
 * @brief an texture with only one chanel | 
 * 
 * SIZE: dynamic |
 * CHANELS: 1 |
 * ENCODING: float
 */
#define GLGE_TEX_R 11
/**
 * @brief an texture with one 8 bit chanel | 
 * 
 * SIZE: 8 bits | 
 * CHANELS: 1 |
 * ENCODING: float
 */
#define GLGE_TEX_R8 12
/**
 * @brief an texture with on 16 bit chanel | 
 * 
 * SIZE: 16 bits | 
 * CHANELS: 1 |
 * ENCODING: float
 */
#define GLGE_TEX_R16 13
/**
 * @brief an texture with on 32 bit chanel | 
 * 
 * SIZE: 32 bits | 
 * CHANELS: 1 |
 * ENCODING: float
 */
#define GLGE_TEX_R32 14
/**
 * @brief an texture with 2 color chanels: one for red and one for green | 
 * 
 * SIZE: dynamic | 
 * CHANELS: 2 | 
 * ENCODING: float
 */
#define GLGE_TEX_RG 15
/**
 * @brief an RG texture with 8 bits per chanel | 
 * 
 * SIZE: CHANELS * 8 | 
 * CHANELS: 2 | 
 * ENCODING: float
 */
#define GLGE_TEX_RG8 16
/**
 * @brief an RG texture with 16 bits per chanel | 
 * 
 * SIZE: CHANELS * 16 | 
 * CHANELS: 2 | 
 * ENCODING: float
 */
#define GLGE_TEX_RG16 17
/**
 * @brief an RG texture with 32 bits per chanel | 
 * 
 * SIZE: CHANELS * 32 | 
 * CHANELS: 2 | 
 * ENCODING: float
 */
#define GLGE_TEX_RG32 18
/**
 * @brief an texture with different chanel sizes
 * 
 * SIZE: R: 3 bit | G: 3 bit | B: 2 bit || 
 * CHANELS: 3 | 
 * ENCODING: float
 */
#define GLGE_TEX_R3_G3_B2 19

/**
 * @brief a 4byte float
 */
#define GLGE_FLOAT 0x1406
/**
 * @brief a 4 byte unsigned int
 */
#define GLGE_UNSIGNED_INT 0x1405
/**
 * @brief a 1 byte unsigned integer
 */
#define GLGE_UNSIGNED_BYTE 0x1401
/**
 * @brief a 4 byte signed integer
 */
#define GLGE_INT 0x1404
/**
 * @brief a 1 byte signed integer
 */
#define GLGE_BYTE 0x1400

/**
 * @brief a simple texture
 */
class Texture
{
public:
    /**
     * @brief Default constructor
     */
    Texture();

    /**
     * @brief create a texture from an file
     * 
     * @param textureFile the texture file to read from
     */
    Texture(const char* textureFile);

    /**
     * @brief Construct a texture from a size
     * 
     * @param size the size for the texture
     * @param encodeType the way to encode the data
     * @param data the data to put in the texture
     */
    Texture(vec2 size, int encodeType, void* data = NULL);

    /**
     * @brief Construct a texture form a size
     * 
     * @param width the width of the texture
     * @param height the height of the texture
     * @param encodeType the way to encode the data
     * @param data the data to put in the texture
     * @param dataType the type of the data to encode, float by default
     */
    Texture(unsigned int width, unsigned int height, int encodeType, void* data = NULL, int dataType = GLGE_FLOAT);

    /**
     * @brief change the size of the texture
     * 
     * @param size the new size for the texture
     */
    void resize(vec2 size);

    /**
     * @brief change the size of the texture
     * 
     * @param width the new width of the texture
     * @param height the new height of the texture
     */
    void resize(unsigned int width, unsigned int height);

    /**
     * @brief bind the texture
     * 
     * @param port the port the texture should bind to. Use GLGE_TEXTURE_BINDING_AUTO for auto-select of the current max texture unit
     */
    void bind(int port = GLGE_TEXTURE_BINDING_AUTO);

    /**
     * @brief unbind the texture
     */
    void unbind();

    /**
     * @brief draw the texture over the whole screen
     */
    void draw();

    /**
     * @brief Get the size of the texture
     * 
     * @return ivec2 the size of the texture
     */
    ivec2 getSize();

    /**
     * @brief Get the OpenGL texture
     * 
     * @return unsigned int the OpenGL texture
     */
    unsigned int getOpenGLTexture();

private:
    //store the texture
    unsigned int texture = 0;
    //store the texture binding
    int binding = -1;
    //store the texture size
    ivec2 size;
    //store the encode type
    int encodeType = 0;
    //store the chanel numbers
    int channels = 0;
    //store the type
    int type = 0;
    //store the owner window
    int windowID = -1;
    //store if this was bound using autobind
    bool autoBind = false;

    void decode(int encodeType);
};

#endif