/**
 * @file textureAtlas.h
 * @author JuNi4
 * @brief 
 * @version 0.1
 * @date 2023-02-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "glgeErrors.hpp"
#include "stb_image.hpp"
#include "glgeFuncs.hpp"
#include "CML/CMLVec3.h"
#include "CML/CMLVec4.h"
#include <filesystem>
#include <iostream>
#include <vector>
#include <math.h>

// Errors

#define GLGE_ERROR_IMAGE_COULDNT_OPEN "[GLGE] could not open image %s\n"

#define GLGE_ERROR_ALLOCATE_MEMORY "[GLGE] could not allocate memory for %s"

#ifndef _ATLAS_H_
#define _ATLAS_H_

// texture atlas class
class atlas {
    private:

        // The vector for holding all the image paths
        std::vector<const char*> paths;

        /**
         * @brief Clears any image
         * 
         * @param img The stb image to be
         * @param info Information about the image - w,h,channels
         */
        void _clear_image(unsigned char* img, vec3 info);

        /**
         * @brief Puts a pixel on a stb image
         * 
         * @param img The stb image to be modified
         * @param pos The x and y of the target pixel
         * @param info Information about the image - w,h,channels
         * @param color The RGBA value to be put at the pixel
         */
        void _put_pixel(unsigned char* img, vec2 pos, vec3 info, vec4 color );

        /**
         * @brief Gets the pixel rgb values at a certain position
         * 
         * @param img The stb image object
         * @param pos The x and y of the target image
         * @param info Information about the image - w,h,channels
         * @return vec4 The RGBA color information from the target pixel
         */
        vec4 _get_pixel(unsigned char* img, vec2 pos, vec3 info );

        /**
         * @brief Constructs an atlas for square images that are all the same size
         * 
         * @param size The size of the image (ex. 32x32 -> 32)
         */
        unsigned char * _constructs_atlas_same(int size, bool save_atlas = false);

        /**
         * @brief Constructs an atlas for images that may be completely differently sized
         * 
         */
        unsigned char * _constructs_atlas_tetris(bool save_atlas = false);

        /**
         * @brief Gets the info like width and height of an image
         * 
         * @param path The path to the image
         * @return int* [3] width, height, channel cont
         */
        int * _img_info(const char* path);

    public:

        // The path of the atlas
        const char * path = "assets/atlas.png";

        /**
         * @brief Default constructor
         * 
         */
        atlas();

        /**
         * @brief Adds any amount of texture paths to the path vector
         * 
         * @param path The paths to be added to the vetor
         */
        void add(const char* path);

        /**
         * @brief Builds the texture atlas
         * 
         */
        unsigned char* build();

        // Developement function
        std::vector<const char*> dump();
};

#endif