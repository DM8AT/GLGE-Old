/**
 * @file GLGEImage.cpp
 * @author JuNi4 (https://github.com/juni4)
 * @brief Image manager for stb_image
 * @version 1.0
 * @date 2024-02-02
 * 
 * @copyright Copyright (c) JuNi4 2024. This project is released under the MIT license. 
 * 
 */

#include "GLGEImage.hpp"
#include "GLGEInternal/glgeImage.h"

#include "GLGEInternal/glgeErrors.hpp"

#include "GLGEMath.h"

// std_image_write setup, i think it needs to be replaced be replaced with something from glge
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"

// Constructors

Image::Image() {}

/**
 * @brief Construct a new Image object with an empty image from the given parameters
 * 
 * @param size The size of the image
 * @param channels The amount of channels for the image
 */
Image::Image( ivec2 size, int channels )
{
    // call the create function
    this->create( size, channels );
}

/**
 * @brief Construct a new Image object with an image being loaded form a fileapth
 * 
 * @param path The path to the image
 */
Image::Image( const char* path )
{
    // call the open function
    this->open( path );
}

// functions //

/**
 * @brief Creates a new image
 * 
 * @param size The width and height of the image
 * @param channels The amount of channels for the image. 1 - 4
 */
void Image::create( ivec2 size, int channels )
{
    // calculate the size of the image
    size_t imageSize = size.x * size.y * channels;
 
    // try to allocate the memory for the atlas
    this->image = (unsigned char*) malloc(imageSize);

    // check if memory was successfully allocated
    if(this->image == NULL) {
        // ..print error message...
        printf(GLGE_ERROR_ALLOCATE_MEMORY, "image creation");
        // ...and exit (In the future, this needs to be in an if)
        exit(1);
    }

    // clear the image once, because the image can have random colors after creation
    this->clear(); 
}

/**
 * @brief Open an image
 * 
 * @param path The path to the image
 */
void Image::open( const char* path )
{
    // open the image
    this->image = glgeLoadImage( path, &this->size.x, &this->size.y, &this->channelCount );
}

/**
 * @brief Save the image
 * 
 * @param path The path to the image
 */
void Image::save( const char* path )
{
    // save the image   | Size      | Channels          | Image data   //| Size of one row
    glgeSaveImage( path, this->size, this->channelCount, this->image); //this->size.x*this->channelCount);
}

/**
 * @brief Close the image
 * 
 */
void Image::close()
{
    // free the image ( is linked to free )
    glgeImageFree(this->image);
}

/**
 * @brief Put a pixel at the specified position
 * 
 * @param pos The target position
 * @param col The new color
 */
void Image::put_pixel( ivec2 pos, ivec4 col ) 
{
    // calculate the position in the image
    unsigned long long posX = pos.x * this->channelCount;
    unsigned long long posY = pos.y * this->size.x * this->channelCount;

    // The X and Y pos combined in one int
    unsigned long long p = posX + posY;
    
    // create array for storing color values
    int arr[4] = {col.x, col.y, col.z, col.w};

    // write the RGBA values
    for (int i = 0; i < this->channelCount; i++ ) {
        this->image[p++] = arr[i];
    }
}

/**
 * @brief Get a pixel at the specified position
 * 
 * @param pos The target position
 * @return ivec4 The color at the pixel
 */
ivec4 Image::get_pixel( ivec2 pos )
{

    // calculate the position in the image
    unsigned long long posX = pos.x * this->channelCount;
    unsigned long long posY = pos.y * this->size.x * this->channelCount;

    // create array for storing color values
    int arr[4] = {0, 0, 0, 0};

    // go through all channels
    for (int i = 0; i < this->channelCount; i++) {
        arr[i] = this->image[ posX+posY+i ];
    }

    return ivec4(arr[0], arr[1], arr[2], arr[3]);
}

/**
 * @brief Fills the entire image with the specified color
 * 
 * @param col The color to fill the image with
 */
void Image::fill( ivec4 col )
{
    // go through all pixels
    for ( int i = 0; i < this->size.x * this->size.y; i++ ) {
        this->put_pixel( ivec2(i,0) /* Cheat by having x & y in one */, col );
    }
}

/**
 * @brief Clears the entire image
 * 
 */
void Image::clear()
{
    // go through all pixels
    for ( int i = 0; i < this->size.x * this->size.y * this->channelCount; i++ ) {
        this->image[i] = 0;
    }
}

/**
 * @brief Puts the image onto another image at the specified position
 * 
 * @param pos Target position for the image
 * @param imgb The target image, to put the image onto
 */
void Image::blit( ivec2 pos, Image *imgb )
{
    // blit one image onto another
    for ( int x = 0; x < glgeMax(this->size.x, imgb->size.x-pos.x); x++ ) {
    for ( int y = 0; y < glgeMax(this->size.y, imgb->size.y-pos.y); y++ ) {
        // set pixel
        imgb->put_pixel( pos + ivec2(x,y), this->get_pixel(ivec2(x,y)) );
    }
    }
}