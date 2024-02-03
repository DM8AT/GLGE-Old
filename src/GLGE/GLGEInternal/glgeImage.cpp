/**
 * @file glgeImage.cpp
 * @author DM8AT
 * @brief an interface to interact with STB_IMAGE and only compile it once
 * @version 0.1
 * @date 2023-08-02
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

#include "glgeImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_image_write.h"

unsigned char* glgeLoadImage(const char* filename, int *x, int *y, int *comp, int req_comp)
{
    //just call STB_Image function to load the image
    return stbi_load(filename, x, y, comp, req_comp); 
}

void glgeSaveImage(const char* filename, ivec2 size, int channelCount, unsigned char* image)
{
    // save the image
    stbi_write_png( filename, size.x, size.x, channelCount, image, size.x*channelCount);
}

void glgeImageFree(unsigned char* data)
{
    //call the stb_image function to free the data
    stbi_image_free(data);
}