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
//include the image wraper
#include "../GLGEIndependend/glgeImage.h"

/**
 * @brief define the value for auto-binding
 */
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

//Defines to say wich unit type to use when binding the texture

/**
 * @brief say that the texture should bind to a texture unit. Then it can be used in texture samplers to sample the texture. 
 */
#define GLGE_TEXTURE_BIND_TEXTURE_UNIT 0x00
/**
 * @brief say that the texture should bind to an image unit. Then it can be written to in a compute shader. 
 */
#define GLGE_TEXTURE_BIND_IMAGE_UNIT 0x01

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
    Texture(vec2 size, int encodeType, vec4* data);

    /**
     * @brief Construct a texture form a size
     * 
     * @param width the width of the texture
     * @param height the height of the texture
     * @param encodeType the way to encode the data
     * @param data the data to put in the texture
     */
    Texture(unsigned int width, unsigned int height, int encodeType, vec4* data = NULL);

    /**
     * @brief Construct a texture from an openGL texture
     * 
     * @param openGLTex the open gl texture
     * @param encodeType the way the texture is encoded
     */
    Texture(unsigned int openGLTex, unsigned int encodeType);

    /**
     * @brief Destroy the Texture
     */
    ~Texture();

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
     * @param port the port the texture should bind to. Default is 0. 
     * @param bindTo the sort of unit to use for binding. Default is GLGE_TEXTURE_BIND_TEXTURE_UNIT
     * @param force say if the binding call should always execute. !!Warning!!: This might result in OpenGL errors. 
     */
    void bind(int port = 0, int bindTo = GLGE_TEXTURE_BIND_TEXTURE_UNIT, bool force = false);

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

    /**
     * @brief get a pointer to the content of the texture. The length is texture width * texture height
     * @warning if the texture is resized, the pointer becomes invalide
     * @return vec4* a pointer to the texture
     */
    vec4* getTexture();

    /**
     * @brief calculate the index in the texture
     * 
     * @param pos the position of the pixel to acess
     * @return unsigned int the index in the texture array
     */
    unsigned int indexInTexture(ivec2 pos);
    /**
     * @brief calculate the index in the texture
     * 
     * @param pos the position of the pixel to acess
     * @return unsigned int the index in the texture array
     */
    unsigned int indexInTexture(vec2 pos);
    /**
     * @brief calculate the index in the texture
     * 
     * @param x the x position of the pixel in the texture
     * @param y the y position of the pixel in the texture
     * @return unsigned int the index in the texture array
     */
    unsigned int indexInTexture(int x, int y);

    /**
     * @brief read the texture from the GPU
     * @warning this reads back data from the GPU and may be slow. Use only when nesessary. 
     */
    void readbackTexture();

    /**
     * @brief uploades the texture data to the texture
     */
    void writeTexture();

    /**
     * @brief store the texture in a file
     * 
     * @param file the file to store the image in
     * @param format the format to store the image as (DEFAULT: GLGE_IMG_TYPE_ENDING)
     * @param readback say if the texture should be read back from the GPU before storing (DEFAULT: true)
     */
    void storeImage(const char* file, unsigned int format = GLGE_IMG_TYPE_ENDING, bool readback = true);

    /**
     * @brief Store the texture data for the object
     * 
     * @param data the new texture data IMPORTANT: size must match texture size
     */
    void setData(vec4* data);

    /**
     * @brief Get the texture handler
     * 
     * @return uint64_t the texture handler
     */
    uint64_t getHandler();

private:
    //store the texture data
    vec4* texData;
    //store the texture
    unsigned int texture = 0;
    //store the texture binding
    int binding = -1;
    //store the unit type
    int unit = 0;
    //store the texture size
    ivec2 size;
    //store the encode type
    int encodeType = 0;
    //store the chanel numbers
    int channels = 0;
    //store the owner window
    int windowID = -1;
    //store if this was bound using autobind
    bool autoBind = false;
    //store the bind intention
    int bindIntention = 0;
    //store the texture handler
    uint64_t handler = 0;

    /**
     * @brief decode a type specification
     * 
     * @param encodeType the type to decode
     */
    void decode(int encodeType);
};

#endif