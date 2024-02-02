/**
 * @file textureAtlas.h
 * @author JuNi4 (https://github.com/juni4)
 * @brief Texture atlas for GLGE
 * @version 0.1
 * @date 2023-02-27
 * 
 * @copyright Copyright (c) JuNi4 2023. This project is released under the MIT license. 
 * 
 */
#include "CML/CMLDVec2.h"
#include "CML/CMLDVec3.h"
#include "CML/CMLDVec4.h"
#include "CML/CMLVec2.h"
#include "CML/CMLVec3.h"
#include "CML/CMLVec4.h"
#include <filesystem>
#include <iostream>
#include <vector>
#include <math.h>

#include "GLGETextureAtlasFile.hpp"

// Include guard
#ifndef _ATLAS_H_
#define _ATLAS_H_

/**
 * @brief Texture Atlas Class for managing a texture atlas
 * 
 * <dl class="section warning"><dt>Warning</dt><dd><ul>
 * <li>The atlas will be reworked completely in the future.</li>
 * <li>The atlas currently has many bugs and may not work at all.</li>
 * </ul></dd></dl>
 * 
 */
class Atlas {
    private:

        /**
         * @brief A list of paths that will be included in the atlas
         * 
         */
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

        std::string _get_region_occupied(vec4 pos);

        /**
         * @brief Constructs an atlas for square images that are all the same size
         * 
         * @param size The size of the image (ex. 32x32 -> 32)
         * @param save_atlas Whether or not the atlas image and data file will be saved
         * 
         * @return The atlas image
         */
        unsigned char * _constructs_atlas_same(int size, bool save_atlas = false, bool verbous = true);

        /**
         * @brief Constructs an atlas for images that may be completely differently sized
         * 
         * @param save_atlas Whether or not the atlas image and data file will be saved
         * 
         * @return The atlas image
         */
        unsigned char * _constructs_atlas_tetris(bool save_atlas = false, bool verbous = true);

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

        /**
         * @brief The path to where the atlas should be saved, if requested
         * 
         */
        const char * path = "assets/atlas.png";

        /**
         * @brief The path for the missing texture
         * 
         * <dl class="section warning"><dt>Warning</dt><dd><ul>
         * <li>The missing texture is required at the moment but will most likley be hardcoded in future versions</li>
         * </ul>
         * </dd></dl>
         * 
         */
        const char * missing_path = "assets/missing_texture.png";

        /**
         * @brief The image that the atlas will be
         * 
         */
        unsigned char* iAtlas;

        /**
         * @brief Information about the atlas
         * 
         */
        GLGEAtlasFile atlasData;

        /**
         * @brief Default constructor
         * 
         */
        Atlas();

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
         * @param frame The frame of the animation from the texture. if the frame index is less than 0, it wont do anything
         * @param mode Whether or not the position will be a (0) GLGE texture position or a (1) pixel position
         * @return vec2 The position of the texture in the atlas
         */
        dvec2 getTexCoord(const char* texture, int corner, int frame = -1, int mode = 0);
    
        /**
         * @brief Loads an atlas from a file. The specified file must be a png and there must be a file with the same name but ending with .atlas
         *
         * @param path The path of the atlas png
         * @return bool Whether or not the atlas was loaded sucsessfully
         */
        bool loadAtlas(const char* path);

        /**
         * @brief Developement function for dumping stuff from the atlas
         * 
         * @return std::vector<const char*> IDK, been ages since i've made this
         */
        std::vector<const char*> dump();
};

#endif
