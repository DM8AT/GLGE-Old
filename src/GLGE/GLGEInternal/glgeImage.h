/**
 * @file glgeImage.h
 * @author DM8AT
 * @brief an interface to interact with STB_IMAGE for GLGE, multiple includes of STB_IMAGE result in errors
 * @version 0.1
 * @date 2023-08-02
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

#include "../CML/CML.h"

#ifndef GLGE_STB_IMAGE
#define GLGE_STB_IMAGE

/**
 * @brief an call of stb_image function "stbi_load"
 * 
 * @param filename a path to the file
 * @param x the width of the image
 * @param y the height of the image
 * @param comp The number of channels
 * @param req_comp The desired number of channels, usefull for only loading rgb when the image is rgba. 0 means that all channels are loaded
 * @return unsigned char* a data pointer to an image or NULL incase of an error
 */
unsigned char* glgeLoadImage(const char* filename, int *x, int *y, int *comp, int req_comp = 0);

/**
 * @brief Save an image
 * 
 * @param filename The name of the image
 * @param size The width and height of the image
 * @param channelCount Number of channels in the image
 * @param image The image data
 */
void glgeSaveImage(const char* filename, ivec2 size, int channelCount, unsigned char* image);

/**
 * @brief free the data allocated to an image
 * 
 * @param data the allocated data
 */
void glgeImageFree(unsigned char* data);

#endif