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
#include "GLGETextureAtlas.hpp"
#include "fstream"
#include <iostream>

#include "GLGEInternal/glgeErrors.hpp"
#include "GLGETextureAtlasErrors.hpp"
#include "GLGEInternal/glgeImage.h"

// FIX(JuNi): Fix atlas images corrupting at large sizes

/////////////////
// constructor //
/////////////////

Atlas::Atlas() {}

/////////////////////
// Private classes //
/////////////////////