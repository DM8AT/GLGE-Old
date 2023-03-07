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
#include "nlohmann/json.hpp"
#include "glgeErrors.hpp"
#include "stb_image.hpp"
#include "glgeFuncs.hpp"
#include "CML/CMLVec3.h"
#include "CML/CMLVec4.h"
#include <filesystem>
#include <iostream>
#include <vector>
#include <math.h>

// Include guard
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
        vec4 _get_pixel(unsigned char* *img, vec2 pos, vec3 info);

        /**
         * @brief Checks whether or not the specified position is already occupied by an image
         * 
         * @param pos The target position to be checked
         * @return true The position already is occupied
         * @return false The position is free
         */
        bool _is_occupied(vec2 pos);

        /**
         * @brief Checks whether or not the specified region is already occupied by an image
         * 
         * @param pos The target region (x,y,w,h) to be checked
         * @return true The region already is occupied
         * @return false The region is free
         */
        bool _is_region_occupied(vec4 pos);

        /**
         * @brief Constructs an atlas for square images that are all the same size
         * 
         * @param size The size of the image (ex. 32x32 -> 32)
         * @param save_atlas Whether or not the atlas image and data file will be saved
         * 
         * @return The atlas image
         */
        unsigned char * _constructs_atlas_same(int size, bool save_atlas = false);

        /**
         * @brief Constructs an atlas for images that may be completely differently sized
         * 
         * @param save_atlas Whether or not the atlas image and data file will be saved
         * 
         * @return The atlas image
         */
        unsigned char * _constructs_atlas_tetris(bool save_atlas = false);

        /**
         * @brief Optimizes the atlas by shrinking it to only the required size.
         * 
         */
        void _optimize_atlas(bool save_atlas = true);

        /**
         * @brief Gets the info like width and height of an image
         * 
         * @param path The path to the image
         * @return int* [3] width, height, channel cont
         */
        int * _img_info(const char* path);

    public:

        // The path of the atlas if it should be saved
        const char * path = "assets/atlas.png";

        // The path to the missing texture, this is required
        const char * missing_path = "assets/missing_texture.png";

        // The atlas image
        unsigned char* iAtlas;

        // The information on where each texture on the atlas is and how wide and tall it is
        nlohmann::json atlasData;

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
         * @param save_atlas Whether or not the atlas image and data file will be saved
         */
        void build(bool save_atlas = false);

        /**
         * @brief Get the Tex Coord object
         * 
         * @param texture The texture name (including path), this will be the same as the texture string from the add function, for example 'assets/cubeTexture.png'
         * @param corner Which corner position to return, 0 top left, 1 top right, 2 bottom left, 3 bottom right
         * @param mode Whether or not the position will be a (0) GLGE texture position or a (1) pixel position
         * @return vec2 The position of the texture in the atlas
         */
        vec2 getTexCoord(const char* texture, int corner, int mode = 0);

        // Developement / Debug function, returns the paths vector
        std::vector<const char*> dump();
};

#endif
