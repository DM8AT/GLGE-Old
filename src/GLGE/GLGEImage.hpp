/**
 * @file GLGEImage.hpp
 * @author JuNi4 (https://github.com/juni4)
 * @brief Image manager for stb_image
 * @version 1.0
 * @date 2024-02-02
 * 
 * @copyright Copyright (c) JuNi4 2024. This project is released under the MIT license. 
 * 
 */

// include guard
#ifndef _GLGE_IMAGE_
#define _GLGE_IMAGE_
#endif

#include <iostream>
#include "CML/CML.h"

/**
 * @brief Class for managing an image file
 * 
 * Don't forget to close the image!
 * 
 */
class Image {

    //////////////////////
    // Private Elements //
    //////////////////////

    private:

    /**
     * @brief The number of channels
     * 
     */
    int channelCount;

    /**
     * @brief The size of the image
     * 
     */
    ivec2 size;

    /**
     * @brief The var where the image is stored
     * 
     */
    unsigned char* image;

    public:

    /////////////////
    // Constructor //
    /////////////////

    /**
     * @brief Construct a new Image object
     * 
     */
    Image();

    /**
     * @brief Construct a new Image object with an empty image from the given parameters
     * 
     * @param size The size of the image
     * @param channels The amount of channels for the image
     */
    Image( ivec2 size, int channels );

    /**
     * @brief Construct a new Image object with an image being loaded form a fileapth
     * 
     * @param path The path to the image
     */
    Image( const char* path );

    /////////////////////
    // Public Elements //
    /////////////////////

    /**
     * @brief Creates a new image
     * 
     * @param size The width and height of the image
     * @param channels The amount of channels for the image. 1 - 4
     */
    void create( ivec2 size, int channels );

    /**
     * @brief Open an image
     * 
     * @param path The path to the image
     */
    void open( const char* path );

    /**
     * @brief Save the image
     * 
     * @param path The path to the image
     */
    void save( const char* path );

    /**
     * @brief Close the image
     * 
     */
    void close();

    /**
     * @brief Put a pixel at the specified position
     * 
     * @param pos The target position
     * @param col The new color
     */
    void put_pixel( ivec2 pos, ivec4 col );

    /**
     * @brief Get a pixel at the specified position
     * 
     * @param pos The target position
     * @return ivec4 The color at the pixel
     */
    ivec4 get_pixel( ivec2 pos );

    /**
     * @brief Fills the entire image with the specified color
     * 
     * @param col The color to fill the image with
     */
    void fill( ivec4 col );

    /**
     * @brief Clears the entire image
     * 
     */
    void clear();

    /**
     * @brief Puts the image onto another image at the specified position
     * 
     * @param pos Target position for the image
     * @param imgb The target image, to put the image onto
     */
    void blit( ivec2 pos, Image *imgb );

};