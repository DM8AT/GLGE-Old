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

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_image_write.h"
#include "../CML/CML.h"
#include <fstream>
#include "glgePrivDefines.hpp"
#include "glgeImage.h"

uint8_t* glgeLoadImage(const char* filename, int *x, int *y, int *comp, int req_comp)
{
    //just call STB_Image function to load the image
    return stbi_load(filename, x, y, comp, req_comp); 
}

void glgeImageFree(uint8_t* data)
{
    //call the stb_image function to free the data
    stbi_image_free(data);
}

uint8_t* glgeTextureDataToImageData(ivec2 texSize, vec4* texData)
{
    //make enough space for the texture
    uint8_t* data = new uint8_t[texSize.x*texSize.y*4];
    //loop over the texture
    for (int x = 0; x < texSize.x; x++)
    {
        for (int y = 0; y < texSize.y; y++)
        {
            //get the current pixel
            vec4 c = texData[(int)((texSize.x - x)*texSize.y + y)];
            //convert to an ivec4
            ivec4 col = ivec4(c.x*255, c.y*255, c.z*255, c.w*255);
            //store the pixel
            //store the r component
            data[(x*texSize.x + y)*4 + 0] = col.x;
            //store the g component
            data[(x*texSize.x + y)*4 + 1] = col.y;
            //store the b component
            data[(x*texSize.x + y)*4 + 2] = col.z;
            //store the a component
            data[(x*texSize.x + y)*4 + 3] = col.w;
        }
    }
    //return the pointer
    return data;
}

void storePPM(const char* file, ivec2 imgSize, uint8_t* imgData)
{
    //create the file
    std::fstream f;
    //open the file
    f.open(file, std::fstream::out);
    //check if the file was opend
    if (!f.is_open())
    {
        //throw an error
        GLGE_THROW_ERROR("Failed to open file " + std::string(file))
        return;
    }
    //clear the file
    f.clear();

    //create the header
    std::string head = "P3 ";
    //add the width to the header
    head += std::to_string(imgSize.x) + " ";
    //add the height to the header
    head += std::to_string(imgSize.y) + " 255\n";
    //add a comment
    head += "# This file was generated with GLGE\n";
    //write the header to the file
    f << head;

    //loop over the texture
    for (int x = 0; x < imgSize.x; x++)
    {
        for (int y = 0; y < imgSize.y; y++)
        {
            //get the current pixel
            ivec4 col;
            //store the r component
            col.x = imgData[(x*imgSize.x + y)*4 + 0];
            //store the g component
            col.y = imgData[(x*imgSize.x + y)*4 + 1];
            //store the b component
            col.z = imgData[(x*imgSize.x + y)*4 + 2];
            //store the a component
            col.w = imgData[(x*imgSize.x + y)*4 + 3];
            //convert the color to an string
            std::string str = std::to_string(col.x) + " " + std::to_string(col.y) + " " + std::to_string(col.z) + "\n";
            //store the string
            f << str;
        }
    }

    //store the file
    f.close();
}

unsigned int getAutoFormat(const char* file)
{
    //creata a string from the file name
    std::string f = std::string(file);
    //store the file ending
    std::string ending = "";
    //store the current character
    char curr = 0;
    //store the current position
    int i = f.size();
    //repeat untill a point is hit
    while (curr != '.')
    {
        //check if the index is less than 0
        if (i < 0)
        {
            //throw an error
            GLGE_THROW_ERROR("Inputed filename '" + f + "' has no file ending, but constructing a type from the ending was requested")
            //safy error return
            return -1;
        }
        //get the current character
        curr = f[i];
        //store the character
        ending += curr;
        //move i lower
        --i;
    }

    //convert the ending to the actual ending
    //store the actual ending
    std::string end = "";
    //make enough size for the data
    end.resize(ending.size()-1);
    //store the position in the flipped ending
    int p = 0;
    //loop over the ending
    for (int i = ending.size()-1; i > 0; --i)
    {
        end[p++] += ending[i];
    }

    //if-elseif snake to get type, switches don't work for strings
    //check if the ending is png
    if (end == ".png")
    {
        //return the identifyer for a png
        return GLGE_IMG_TYPE_PNG;
    }
    //check if the ending is jpeg alias jpg
    else if ((end == ".jpg") || (ending == ".jpeg"))
    {
        //return the identifyer for a jpg
        return GLGE_IMG_TYPE_JPG;
    }
    //check if the ending is for a bitmap
    else if (end == ".bmp")
    {
        //return the identifyer for a bitmap
        return GLGE_IMG_TYPE_BMP;
    }
    //check if the ending is for ppm
    else if (end == ".ppm")
    {
        //return the identifyer for a ppm
        return GLGE_IMG_TYPE_PPM;
    }

    //at this point, the ending is undefined
    //so, throw an error
    GLGE_THROW_ERROR(std::string("The file ending '") + end + std::string("' dose not name a valid image file format supported by GLGE."))
    //safty error return
    return -1;
}

void glgeStoreImage(const char* file, ivec2 imgSize, uint8_t* imgData, unsigned int fileFormat)
{
    //store the wanted file format
    int type = fileFormat;
    //check if the file type should depend on the file ending
    if (type == GLGE_IMG_TYPE_ENDING)
    {
        //if it is, read the file ending as type
        type = getAutoFormat(file);
        //check if an error occured
        if (type == -1)
        {
            //throw an error
            GLGE_THROW_ERROR("An error occured while getting the file type for storing the image. Please look above for more information.")
            //stop the function
            return;
        }
    }

    //switch to store as wich type (swtiches are faster)
    switch (type)
        {
        //store as a png
        case GLGE_IMG_TYPE_PNG:
        {
            //call to STB image
            stbi_write_png(file, imgSize.x, imgSize.y, 4, (void*)imgData, 0);
            break;
        }
        //store a jpg image
        case GLGE_IMG_TYPE_JPG:
        {
            //call to STB image
            stbi_write_jpg(file, imgSize.x, imgSize.y, 4, (void*)imgData, 90);
            break;
        }
        //store a bitmap image
        case GLGE_IMG_TYPE_BMP:
        {
            //call to STB image
            stbi_write_bmp(file, imgSize.x, imgSize.y, 4, (void*)imgData);
            break;
        }
        //store as ppm file
        case GLGE_IMG_TYPE_PPM:
        {
            //call a custom function
            storePPM(file, imgSize, imgData);
            break;
        }
        
        default:
            //throw an error
            GLGE_THROW_ERROR("The image type requested to store is undefined")
            break;
    }
}