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

#ifndef GLGE_STB_IMAGE
#define GLGE_STB_IMAGE

/**
 * @brief an call of stb_image function "stbi_load"
 * 
 * @param filename a path to the file
 * @param x the width of the image
 * @param y the height of the image
 * @param comp the bit size per chanel
 * @param req_comp i dont know, look at the stbi image docomentation
 * @return unsigned char* a data pointer to an image
 */
unsigned char* glgeLoad(const char* filename, int *x, int *y, int *comp, int req_comp = 0);

/**
 * @brief free the data allocated to an image
 * 
 * @param data the allocated data
 */
void glgeImageFree(unsigned char* data);

#endif