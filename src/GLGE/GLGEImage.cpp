/**
 * @file GLGEImage.cpp
 * @author JuNi4 (https://github.com/juni4)
 * @brief Image manager for stb_image
 * @version 0.1
 * @date 2024-02-02
 * 
 * @copyright Copyright (c) JuNi4 2024. This project is released under the MIT license. 
 * 
 */

#include "GLGEImage.hpp"

// std_image_write setup
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Constructors

Image::Image() {}

// functions //

/***/
void create() {}

/**
 * @brief Open an image
 * 
 * @param path The path to the image
 */
void Image::open( const char* path ) {}

/**
 * @brief Save the image
 * 
 * @param path The path to the image
 */
void Image::save( const char* path )
{
    // save the image    | Width       | Height      | Channels          | Image data | Size of one row
    stbi_write_png( path, this->size.x, this->size.x, this->channelCount, this->image, this->size.x*this->channelCount);
}

/**
 * @brief Close the image
 * 
 */
void Image::close() {}

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
    for (int i = 0; i < 4; i++ ) {
        this->image[p++] = arr[i];
    }
}

/**
 * @brief Get a pixel at the specified position
 * 
 * @param pos The target position
 * @return ivec4 The color at the pixel
 */
ivec4 Image::get_pixel( ivec2 pos ) {

    // calculate the position in the image
    unsigned long long posX = pos.x * this->channelCount;
    unsigned long long posY = pos.y * this->size.x * this->channelCount;

    // create array for storing color values
    int arr[4] = {0, 0, 0, 255};

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
    for ( ulong i = 0; i < this->size.x * this->size.y; i++ ) {
        this->put_pixel( ivec2(i,0) /* Cheat by having x & y in one */, col );
    }
}

/**
 * @brief Clears the entire image
 * 
 */
void Image::clear(){
    // go through all pixels
    for ( ulong i = 0; i < this->size.x * this->size.y * this->channelCount; i++ ) {
        this->image[i] = 0;
    }
}