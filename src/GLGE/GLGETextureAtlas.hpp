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
#include "GLGEImage.hpp"

// Include guard
#ifndef _GLGE_ATLAS_H_
#define _GLGE_ATLAS_H_

/**
 * @brief Texture Atlas Class for managing a texture atlas
 * 
 */
class Atlas {

    //private:
    // for debug purposes
    public:
    //////////////////////
    // Private Elements //
    //////////////////////

    /**
     * @brief The image the atlas is going to be stored in
     * 
     */
    Image atlasImage;

    

    public:
    /////////////////
    // Constructor //
    /////////////////

    Atlas();

    /////////////////////
    // Public Elements //
    /////////////////////

    // loading & saving //

    /**
     * @brief Opens an atlas from a file
     * 
     * @param path The path to the atlas image (must be png/jpg)
     * @param atlasDataPath (Optional) The path to the .atlas file containing the atlas data
     */
    void open( const char* path, const char* atlasDataPath = "" );

    /**
     * @brief Saves the atlas to the specefied file
     * 
     * @param path The path for the atlas image (must be png/jpg)
     * @param atlasDataPath (Optional) The path for the .atlas file containing the atlas data
     */
    void save( const char* path, const char* atlasDataPath = "" );

    /**
     * @brief Close the atlas object
     * 
     */
    void close();

    // Atlas Creation //

    void addImage();
    void removeImageByIndex();
    void removeImageByName();

    void create();

    // Atlas Using //

    std::vector<vec2> getTexture();
    

};

#endif
