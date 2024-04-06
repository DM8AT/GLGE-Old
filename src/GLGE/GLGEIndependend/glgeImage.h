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

//define the values to use for string different file formats

/**
 * @brief say that the file ending should be used to determine the format
 */
#define GLGE_IMG_TYPE_ENDING 0
/**
 * @brief say that the input image should be stored as PNG
 */
#define GLGE_IMG_TYPE_PNG 1
/**
 * @brief say that the input image should be stored as JPG
 */
#define GLGE_IMG_TYPE_JPG 2
/**
 * @brief JPEG is the same as JPG
 */
#define GLGE_IMG_TYPE_JPEG GLGE_IMG_TYPE_JPG
/**
 * @brief say that the image should be stored as bitmap
 */
#define GLGE_IMG_TYPE_BMP 3
/**
 * @brief say that the image should be stored as ppm file
 */
#define GLGE_IMG_TYPE_PPM 4

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
uint8_t* glgeLoadImage(const char* filename, int *x, int *y, int *comp, int req_comp = 0);

/**
 * @brief free the data allocated to an image
 * 
 * @param data the allocated data
 */
void glgeImageFree(uint8_t* data);

/**
 * @brief convert the texture data stored in an texture object into image data that can be stored as an image
 * 
 * @param texSize the size of the texture to store
 * @param texData the texture data as Vec4 (1 vec4 per pixe, x = r, y = g, b = b, w = a)
 * @return uint8_t* a pointer to an array of RGBA image datas, 1 byte per chanel (4 byte per pixel)
 */
uint8_t* glgeTextureDataToImageData(ivec2 texSize, vec4* texData);

/**
 * @brief store a file using my own implementation or stb_image
 * 
 * @param file the file to store the texture in
 * @param texSize the size of the texture to store
 * @param imgData the data of the image to store
 * @param fileFormat the file format to use for texture storage (DEFAULT: GLGE_IMG_TYPE_ENDING)
 */
void glgeStoreImage(const char* file, ivec2 texSize, uint8_t* imgData, unsigned int fileFormat = GLGE_IMG_TYPE_ENDING);

#endif