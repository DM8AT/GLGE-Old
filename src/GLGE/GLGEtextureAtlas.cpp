/**
 * @file textureAtlas.h
 * @author JuNi4
 * @brief Texture atlas for GLGE
 * @version 0.1
 * @date 2023-02-27
 * 
 * @copyright Copyright (c) JuNi4 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */
#include "GLGEtextureAtlas.h"
#include "fstream"
#include <iostream>

#include "GLGEIndependend/glgeErrors.hpp"
#include "GLGEIndependend/glgeImage.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// FIXME(JuNi): Fix atlas images corrupting at large sizes

/////////////////////////
// Default constructor //
/////////////////////////

Atlas::Atlas() {}

/////////////////////
// Private classes //
/////////////////////

void Atlas::_clear_image(unsigned char* img, vec3 info) {
    // go to all pixels
    for (int w = 0; w < info.x; w++) {
    for (int h = 0; h < info.y; h++) {
        // write 0,0,0,0 to image pixel
        this->_put_pixel(img, vec2(w,h), info, vec4(0,0,0,0));
    }
    }
}

void Atlas::_put_pixel(unsigned char* img, vec2 pos, vec3 info, vec4 color ) {
    // Info:
    // - width = info.x
    // - height = info.y
    // - channels = info.z
    // - r = color.x
    // - g = color.y
    // - b = color.z
    // - a = color.w

    // Goal:
    // set the pixel to a color

    // pixel posX = x * channels
    // pixel posY = y * width * channels

    unsigned long long posX = pos.x * info.z;
    unsigned long long posY = pos.y * info.x * info.z;

    // The X and Y pos combined in one int
    unsigned long long p = posX + posY;

    // create array for storing color values
    int arr[4] = {(int)color.x, (int)color.y, (int)color.z, (int)color.w};

    // write the RGBA values
    for (int i = 0; i < 4; i++ ) {
        img[p++] = arr[i];
    }
}

vec4 Atlas::_get_pixel(unsigned char* *img, vec2 pos, vec3 info ) {
    // Info:
    // - width = info.x
    // - height = info.y
    // - channels = info.z
    // - r = color.x
    // - g = color.y
    // - b = color.z
    // - a = color.w

    // Goal:
    // set the pixel to a color

    // pixel posX = x * channels
    // pixel posY = y * width * channels

    unsigned long long posX = pos.x * info.z;
    unsigned long long posY = pos.y * info.x * info.z;

    unsigned char* p = *img;

    // create array for storing color values
    int arr[4] = {0, 0, 0, 255};

    // go through all channels
    for (int i = 0; i < info.z; i++) {
        arr[i] = *(p + posX + posY + i );
    }

    return vec4(arr[0], arr[1], arr[2], arr[3]);
}

bool Atlas::_is_occupied(vec2 pos)
{
    // check whether or not a certain position on the texture atlas is occupied. This releys on the atlasData variable
    // check if atlas data is empty
    if ( this->atlasData.images.size() == 0 ) { return false; }

    // go through eachh image in atlasData
    for ( ulong i = 0; i < this->atlasData.images.size(); i++ )
    {
        // image x
        int iX = this->atlasData.images[i].pos.x;
        // image width
        int iW = iX + this->atlasData.images[i].size.y;
        // image y
        int iY = this->atlasData.images[i].pos.y;
        // image height
        int iH = iY + this->atlasData.images[i].size.y;

        // check if x is in range of x and w
        if ( iX < pos.x && pos.x < iW ) {
            return true;
        }
        // check if y is in range of y and h
        if ( iY < pos.y && pos.y < iH) {
            return true;
        }
    }

    return false;
}

bool Atlas::_is_region_occupied(vec4 pos)
{
    // check whether or not a certain position on the texture atlas is occupied. This releys on the atlasData variable
    // check if atlas data is empty
    if ( this->atlasData.images.size() == 0 ) { return false; }

    // go through eachh image in atlasData
    for ( ulong i = 0; i < this->atlasData.images.size(); i++ )
    {
        // image x
        int iX = this->atlasData.images[i].pos.x;
        // image width
        int iW = iX + this->atlasData.images[i].size.y;
        // image y
        int iY = this->atlasData.images[i].pos.y;
        // image height
        int iH = iY + this->atlasData.images[i].size.y;

        // temp var
        bool tmp = false;

        if (pos.z < iW - iX) {
            // check if x is in range of x and w
            if ( iX <= pos.x && pos.x < iW ) {
                tmp = true;
            }
            // check if x+width is in range of x and w
            if ( iX < pos.x+pos.z && pos.x+pos.z < iW ) {
                tmp = true;
            }
        } else {
            // check if x is in range of x and w
            if ( pos.x <= iX && iX < pos.x+pos.z ) {
                tmp = true;
            }
            // check if x+width is in range of x and w
            if ( pos.x < iW && iW < pos.x+pos.z ) {
                tmp = true;
            }
        }

        if (pos.w < iH - iY) {
            // check if y is in range of y and h
            if ( iY <= pos.x && pos.y < iH && tmp == true ) {
                return true;
            }
            // check if y+width is in range of x and w
            if ( iY < pos.y+pos.z && pos.y+pos.z < iH && tmp == true ) {
                return true;
            }
        } else {
            // check if y is in range of y and h
            if ( pos.y <= iY && iY < pos.y+pos.w && tmp == true ) {
                return true;
            }
            // check if y+width is in range of x and w
            if ( pos.y < iH && iH < pos.y+pos.w && tmp == true ) {
                return true;
            }
        }
    }

    return false;
}

std::string Atlas::_get_region_occupied(vec4 pos)
{
    // check whether or not a certain position on the texture atlas is occupied. This releys on the atlasData variable
    // check if atlas data is empty
    if ( this->atlasData.images.size() == 0 ) { return ""; }

    // go through eachh image in atlasData
    for ( ulong i = 0; i < this->atlasData.images.size(); i++ )
    {
        // image x
        int iX = this->atlasData.images[i].pos.x;
        // image width
        int iW = iX + this->atlasData.images[i].size.y;
        // image y
        int iY = this->atlasData.images[i].pos.y;
        // image height
        int iH = iY + this->atlasData.images[i].size.y;

        // temp var
        bool tmp = false;

        if (pos.z < iW - iX) {
            // check if x is in range of x and w
            if ( iX <= pos.x && pos.x < iW ) {
                tmp = true;
            }
            // check if x+width is in range of x and w
            if ( iX < pos.x+pos.z && pos.x+pos.z < iW ) {
                tmp = true;
            }
        } else {
            // check if x is in range of x and w
            if ( pos.x <= iX && iX < pos.x+pos.z ) {
                tmp = true;
            }
            // check if x+width is in range of x and w
            if ( pos.x < iW && iW < pos.x+pos.z ) {
                tmp = true;
            }
        }

        if (pos.w < iH - iY) {
            // check if y is in range of y and h
            if ( iY <= pos.x && pos.y < iH && tmp == true ) {
                return this->atlasData.images[i].name;
            }
            // check if y+width is in range of x and w
            if ( iY < pos.y+pos.z && pos.y+pos.z < iH && tmp == true ) {
                return this->atlasData.images[i].name;
            }
        } else {
            // check if y is in range of y and h
            if ( pos.y <= iY && iY < pos.y+pos.w && tmp == true ) {
                return this->atlasData.images[i].name;
            }
            // check if y+width is in range of x and w
            if ( pos.y < iH && iH < pos.y+pos.w && tmp == true ) {
                return this->atlasData.images[i].name;
            }
        }
    }

    return "";
}

unsigned char * Atlas::_constructs_atlas_same(int size, bool save_atlas, bool verbous)
{
    // amount of images
    int imgs = this->paths.size();
    int w,h, c = 4;

    // get sqrt of imgs
    float imgsroot = sqrt(imgs);
    // verbal message
    if (verbous) {
        std::cout << "[GLGE] [Texture Atlas] Getting size of atlas...\r";
    }
    // if it is int and not float then the result times size is w & h
    if ( imgsroot == (int) imgsroot ) {
        // set width
        w = imgsroot * size;
        // hight is the same as width
        h = w;
    // else the w & h is the (result + 1) * size
    } else {
        // set width
        w = ( floor(imgsroot) + 1 ) * size;
        // hight is the same as width
        h = w;
        // set the imgsroot to what it should be
        imgsroot = w/size;
    }

    // write template to atlas info data
    this->atlasData.size.x = w;
    this->atlasData.size.y = h;

    // verbal message
    if (verbous) {
        std::cout << "[GLGE] [Texture Atlas] Getting size of atlas... Done!\n";
        std::cout << "[GLGE] [Texture Atlas] Creating image...\r";
    }

    // debug message
    //std::cout << imgs << " " << w << " " << size << "\n";

    // Calculate the size of the altas
    size_t atlasSize = w * h * c;
 
    // try to allocate the memory for the atlas
    unsigned char* atlasImg = (unsigned char*) malloc(atlasSize);
    // if allocating memory failed...
    if(atlasImg == NULL) {
        // ..print error message...
        printf(GLGE_ERROR_ALLOCATE_MEMORY, "texture atlas (same)");
        // ...and exit
        exit(1);
    }

    // clear atlas image. this gets rid of random pixel values that somehow happen
    this->_clear_image(atlasImg, vec3(w,h,c));

    // verbal message
    if (verbous) {
        std::cout << "[GLGE] [Texture Atlas] Creating image... Done!\n";
    }

    // counter for how many images have been processed already
    int imgCount = 0;
    // go through all images in the texture atlas
    for (int vert = 0; vert < imgsroot; vert++ ) {
        for (int hor = 0; hor < imgsroot; hor++ ) {
            // verbal message
            if (verbous) {
                int percent = 100./(float)this->paths.size() * (float)imgCount;
                std::cout << "[GLGE] [Texture Atlas] Placing images... " << percent << "%\r";
            }
            // image values
            int width, height, channelCount;
            unsigned char *img = glgeLoad(paths[imgCount], &width, &height, &channelCount);

            // add image info to atlas data
            GLGEAtlasImage i;
            i.name = this->paths[imgCount];
            i.size = vec2( width, height );
            i.pos = vec2( hor*size, vert*size );

            this->atlasData.addImage(i);

            // go through all pixels of the image
            for (int hI = 0; hI < height; hI++) {
            for (int wI = 0; wI < width; wI++) {
                // calculate the current position in the image and atlas
                vec2 imgPos = vec2(wI, hI);
                vec2 atlasPos = vec2(imgPos.x+hor*size, imgPos.y+vert*size);
                // get the colors from a pixel
                vec4 arr = this->_get_pixel(&img, imgPos, vec3(width, height, channelCount));
                // write them to the atlas
                this->_put_pixel(atlasImg, atlasPos, vec3(w, h, c), arr);
                
                // For debugging, print rgba values
                if (imgCount < 2 && 0) {
                    printf("R:%d G:%d B:%d A:%d; IX:%d IY:%d; AX:%d AY:%d\n",(int)arr.x, (int)arr.y, (int)arr.z, (int)arr.w, (int)imgPos.x, (int)imgPos.y, (int)atlasPos.x, (int)atlasPos.y);
                }
            }
            }

            // free image
            glgeImageFree(img);

            // Increment the image counter
            imgCount++;
            // check if all images have been written
            if (imgCount >= (int) this->paths.size()) { break; }
        }
        // check if all images have been written
        if (imgCount >= (int) this->paths.size()) { break; }
    }

    // save the atlas if requested
    if (save_atlas) {
        // verbal message
        if (verbous) {
            std::cout << "[GLGE] [Texture Atlas] Saving image...\r";
        }
        // save atlas to the specefied location
        stbi_write_png(this->path, w, h, c, atlasImg, w*c);
        // save the atlas info
        this->atlasData.open( (std::string)this->path+".atlas", 'w' );
        this->atlasData.write();
        this->atlasData.close();
        // verbal message
        if (verbous) {
            std::cout << "[GLGE] [Texture Atlas] Saving image... Done!\n";
        }
    }

    // return the atlas object
    return atlasImg;
}

unsigned char * Atlas::_constructs_atlas_tetris(bool save_atlas, bool verbous)
{
    // The slightly slower algorithm for placing images on a blank canvas, a bit like playing tetris

    /*
        Go through all images in the first pass
            Add up the total width and height

        Create an Image with the befor calculated dimensions

        Second Pass
            Try to fit each image at (0,0) and then go through all spots up to the point where the image would go outside the image
            If a spot has been found, place the image and note down the postition and information of the image in atlasData
    */

    int channelCount = 4;

    // total w & h var
    int totalWidth = 0;
    int totalHeight = 0;
    // the minimum width to have in order for all textures to fit
    int minWidth = 0;
    int minHeight = 0;
    // verbal message
    if (verbous) {
        std::cout << "[GLGE] [Texture Atlas] Getting size of atlas...\r";
    }
    // First pass through all images
    for ( long unsigned int i = 0; i < this->paths.size(); i++ ) {
        // image dimension vars
        int w, h, c;
        // load image info
        unsigned char* img = glgeLoad(this->paths[i], &w, &h, &c);
        // check if image got loaded
        if (img == NULL) {
            // print Error
            printf(GLGE_ERROR_IMAGE_COULDNT_OPEN, this->paths[i]);
            // continue
            continue;
        }
        // free image for no memoryleaks
        glgeImageFree(img);

        // Add w & h to totals
        totalWidth += w;
        totalHeight += h;
        // update minimum width
        if ( w > minWidth ) { minWidth = w; }
        if ( h > minHeight ) { minHeight = h; }
    }
    // verbal message
    if (verbous) {
        std::cout << "[GLGE] [Texture Atlas] Getting size of atlas... Done!\n";
        std::cout << "[GLGE] [Texture Atlas] Creating image...\r";
    }

    // reduce the total width
    totalWidth /= 6;
    totalHeight /= 6;
    // make sure all images will fit
    if ( totalWidth < minWidth ) { totalWidth = minWidth; }
    if ( totalHeight < minHeight ) { totalHeight = minHeight; }

    // write template to atlas info data
    this->atlasData.size.x = totalWidth;
    this->atlasData.size.y = totalHeight;

    // create the image
    // Calculate the size of the altas
    size_t atlasSize = totalWidth * totalHeight * channelCount;
 
    // try to allocate the memory for the atlas
    unsigned char* atlasImg = (unsigned char*) malloc(atlasSize);
    // if allocating memory failed...
    if(atlasImg == NULL) {
        // ..print error message...
        printf(GLGE_ERROR_ALLOCATE_MEMORY, "texture atlas (tetris)");
        // ...and exit
        exit(1);
    }

    // clear atlas image. this gets rid of random pixel values that somehow happen
    this->_clear_image(atlasImg, vec3(totalWidth,totalHeight,channelCount));

    // position vars
    int x = 0, y = 0;
    // verbal message
    if (verbous) {
        std::cout << "[GLGE] [Texture Atlas] Creating image... Done!\n";
    }

    // TODO(JuNi): Make images not so spaced out
    // Second pass, it's tetris time!
    for ( long unsigned int i = 0; i < this->paths.size(); i++ ) {
        // verbal message
        if (verbous) {
            int percent = 100./(float)this->paths.size() * (float)i;
            std::cout << "[GLGE] [Texture Atlas] Placing images... " << percent << "%\r";
        }
        // image dimension vars
        int w, h, c;
        // load image info
        unsigned char* img = glgeLoad(paths[i], &w, &h, &c);

        // check if image got opened
        if (img == NULL) {
            // print Error
            printf(GLGE_ERROR_IMAGE_COULDNT_OPEN, this->paths[i]);
            // continue
            continue;
        }

        // find a spot to put the image //
        // if the image has reached the highest point
        bool reached_ceiling = false;
        // var to check if a spot was found
        bool done = false;

        while (true) {
            // get what is occupying the spot
            std::string occupend = this->_get_region_occupied(vec4( x, y, w, h ));
            // if region is occupied, move position
            if ( occupend != "" ) {
                // get the occupend data
                GLGEAtlasImage occupendData = this->atlasData.images[this->atlasData.find(occupend)];
                // get how much the x position has to change
                int change = (int)occupendData.size.x - (x - (int)occupendData.pos.x);
                // check if enough room is to the edge of the altas
                if ( (int)this->atlasData.size.x >= x + change + w ) {
                    // move x by change
                    x += change;
                    // continue
                    continue;
                }
                // reset x
                x = 0;
                // calculate vertical change
                change = (int)occupendData.size.y - (x - (int)occupendData.pos.y);
                // check if enough vertical room is left
                if ( (int)this->atlasData.size.y < y + h + change ) {
                    // if the ceiling was reached before
                    if (reached_ceiling) {
                        // break out of loop
                        break;
                    }
                    // otherwise set reached_ceiling to true
                    reached_ceiling = true;
                    // and reset x,y
                    x = 0, y = 0;
                }
                // otherwise, move y by height of the image
                y += change;
                continue;
            }
            // if occupend is nothing, place image //

            // add image info to atlas data
            GLGEAtlasImage ti;
            ti.name = paths[i];
            ti.size = vec2( w, h );
            ti.pos = vec2( x, y );

            this->atlasData.addImage(ti);

            //std::cout << i << " " << this->atlasData["images"][paths[i]] << "\n";

            // go through all pixels of the image
            for (int hI = 0; hI < h; hI++) {
            for (int wI = 0; wI < w; wI++) {
                // calculate the current position in the image and atlas
                vec2 imgPos = vec2(wI, hI);
                vec2 atlasPos = vec2(imgPos.x+x, imgPos.y+y);
                // get the colors from a pixel
                vec4 arr = this->_get_pixel(&img, imgPos, vec3(w, h, c));
                // write them to the atlas
                this->_put_pixel(atlasImg, atlasPos, vec3(totalWidth, totalHeight, channelCount), arr);
            }
            }
            // break out of the loop when done
            done = true;
            break;
        }

        // free image
        free(img);
        // check if a position was not found
        if (!done) {
            printf(GLGE_ERROR_ATLAS_NOT_ENOUGH_IMAGE_SPACE, this->paths[i]);
            exit(1);
        }
    }
    // verbal message
    if (verbous) {
        std::cout << "[GLGE] [Texture Atlas] Placing images... Done!\n";
    }

    // save image and data if needed;
    if (save_atlas) {
        // verbal message
        if (verbous) {
            std::cout << "[GLGE] [Texture Atlas] Saving image...\r";
        }
        // save atlas to the specefied location
        stbi_write_png(this->path, totalWidth, totalHeight, channelCount, atlasImg, totalWidth*channelCount);
        // save the atlas info
        this->atlasData.open( (std::string)this->path+".atlas", 'w' );
        this->atlasData.write();
        this->atlasData.close();
        // verbal message
        if (verbous) {
            std::cout << "[GLGE] [Texture Atlas] Saving image... Done!\n";
        }
    }

    // return atlas image
    return atlasImg;
}

void Atlas::_optimize_atlas(bool save_atlas) {
    // image vars
    int width = 0, height = 0, channelCount = 4;
    
    // calculate the size
    // go through all images and see wich one is the widest and tallest and such
    for ( ulong i = 0; i < this->atlasData.images.size(); i++ )
    {
        // check if x+w is bigger than width
        if ( (int) this->atlasData.images[i].pos.x + (int) this->atlasData.images[i].size.x > width ) { width = (int) this->atlasData.images[i].pos.x + (int) this->atlasData.images[i].size.x; }
        // check if y+h is bigger than height
        if ( (int) this->atlasData.images[i].pos.y + (int) this->atlasData.images[i].size.y > height ) { height = (int) this->atlasData.images[i].pos.y + (int) this->atlasData.images[i].size.y; }
    }

    // create the image

    // Calculate the size of the altas
    size_t atlasSize = width * height * channelCount;
 
    // try to allocate the memory for the atlas
    unsigned char* atlasImg = (unsigned char*) malloc(atlasSize);
    // if allocating memory failed...
    if(atlasImg == NULL) {
        // ..print error message...
        printf(GLGE_ERROR_ALLOCATE_MEMORY, "texture atlas (optimize)");
        // ...and exit
        exit(1);
    }

    // clear atlas image. this gets rid of random pixel values that somehow happen
    this->_clear_image(atlasImg, vec3(width,height,channelCount));

    // TODO(JuNi): Make '_optimize_atlas()' faster

    // transplant old atlas to new
    // go through all pixels of the old atlas
    for (int hI = 0; hI < height; hI++) {
    for (int wI = 0; wI < width; wI++) {
        // calculate the current position in the image and atlas
        vec2 imgPos = vec2(wI, hI);
        // get the colors from a pixel
        vec4 arr = this->_get_pixel(&this->iAtlas, imgPos, vec3((int)this->atlasData.size.x, (int)this->atlasData.size.y, 4));
        // write them to the atlas
        this->_put_pixel(atlasImg, imgPos, vec3(width, height, channelCount), arr);
    }
    }

    // update width and height of atlas image data
    this->atlasData.size.x = width;
    this->atlasData.size.y = height;

    // overwrite atlas image
    this->iAtlas = atlasImg;

    // save image if requested
    if (save_atlas)  {
        // save atlas to the specefied location
        stbi_write_png(this->path, width, height, channelCount, atlasImg, width*channelCount);
        // save the atlas info
        this->atlasData.open( (std::string)this->path+".atlas", 'w' );
        this->atlasData.write();
        this->atlasData.close();
    }
}

int * Atlas::_img_info(const char* path) {
    // load image
    int w, h, c; 
    unsigned char *img = glgeLoad(path, &w, &h, &c);
    // if image was not opened, print error
    if(img == NULL) {
        glgeImageFree(img);
        printf(GLGE_ERROR_IMAGE_COULDNT_OPEN, path);
        return new int[3] {-1, -1, -1};
    }
    glgeImageFree(img);
    // debug message
    //printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", w, h, c);

    return new int[3] {w, h, c};
}

////////////////////
// Public classes //
////////////////////

bool endsWith(std::string string, std::string suffix)
{
    //check if the suffix is bigger than the string
    if (string.length() < suffix.length())
    {
        //the string can't have the suffix
        return false;
    }

    //loop over all symbols in the suffix
    for (int i = (int)suffix.length(); i > 0; i--)
    {
        //check if the symbol of the suffix is correct
        if (string[(int)string.length() - i] != suffix[(int)suffix.length()-i])
        {
            //return that they are not the same, if they are not the same
            return false;
        }
    }
    //if the loop is passed, they are the same
    return true;
}

// add objects to the paths vector
void Atlas::add(const char* path) {
    // check if path exists
    if ( ! std::filesystem::is_regular_file(path)) {
        // pring out error
        printf(GLGE_ERROR_FILE_NOT_FOUND, path);
        //return void
        return;
    }
    // check if file is png or jpg
    std::string sPath = path;
    if ( ! endsWith(sPath, ".png") && ! endsWith(sPath, ".jpg")) {
        // print warning
        printf(GLGE_WARNING_INCORRECT_FILE_TYPE, path, "png or jpg");
        return;
    }


    // add path to vector
    this->paths.push_back(path);
}

void Atlas::build(bool save_atlas) {
    // add missing texture and make sure it exists
    if (std::filesystem::is_regular_file(this->missing_path)) { this->add(this->missing_path); }
    // if it does not exist, return error
    else {
        printf(GLGE_ERROR_ATLAS_NO_MISSING_TEXTURE);
        exit(1);
    }

    // the width that every image must be to pass the test
    int w = -1;
    // the result of the test
    bool square_same = true;

    // test if all images are same size and square
    for (long unsigned int i = 0; i < this->paths.size(); i++ ) {
        // get image info
        int * arr = this->_img_info(this->paths[i]);

        // check if image is square
        if (arr[0] != arr[1]) {
            // images are not square so break and set square_same to false
            square_same = false;
            break;
        }

        // if w = NULL, set w to first img w
        if (w == -1) { w = arr[0]; }
        else if (w != arr[0] ) {
            // if image is not same size as all the others, break
            square_same = false;
            break;
        }
    }

    // Construct the atlas with the correct algorithm
    //  Square, all images are same size algorithm
    if (square_same) { this->iAtlas = this->_constructs_atlas_same(w, save_atlas); }
    //  "Tetris" Algorithm, more complex, potentianlly slower
    else { this->iAtlas = this->_constructs_atlas_tetris(save_atlas); }

    // optimize atlas image to shrink it to the required size
    this->_optimize_atlas(save_atlas);
}

dvec2 Atlas::getTexCoord(const char* texture, int corner, int, int mode)
{
    // get texture atlas info
    float aWidth = this->atlasData.size.x;
    float aHeight = this->atlasData.size.y;
    ulong ti = this->atlasData.find(texture);
    // check if atlas has the texture
    if ( ti == (ulong)-1 ) {
        // Print warning message
        printf(GLGE_WARNING_ATLAS_CHILD_TEXTURE_NOT_FOUND, texture);
        // set texture to missing texture
        texture = this->missing_path;
    }
    // get texture info
    
    float iWidth = this->atlasData.images[ti].size.x;
    float iHeight = this->atlasData.images[ti].size.y;
    float iX = this->atlasData.images[ti].pos.x;
    float iY = this->atlasData.images[ti].pos.y;

    //  GLGE Texture Coords
    // Top left corner
    if ( corner == 0 && mode == 0 ) {
        // x/aWidth, y/aHeight
        return dvec2(iX/aWidth, iY/aHeight);
    }
    // Top right corner
    if ( corner == 1 && mode == 0 ) {
        // (x+w)/aWidth, y/aHeight
        return dvec2((iX+iWidth)/aWidth, iY/aHeight);
    }
    // bottom left corner
    if ( corner == 2 && mode == 0 ) {
        // x/aWidth, (y+h)/aHeight
        return dvec2(iX/aWidth, (iY+iHeight)/aHeight);
    }
    // Top left corner
    if ( corner == 3 && mode == 0 ) {
        // (x+w)/aWidth, (y+h)/aHeight
        return dvec2((iX+iWidth)/aWidth, (iY+iHeight)/aHeight);
    }
    //  Pixel Coords
    // Top left corner
    if ( corner == 0 && mode == 1 ) {
        // x, y
        return dvec2(iX, iY);
    }
    // Top right corner
    if ( corner == 1 && mode == 1 ) {
        // x+w, y
        return dvec2(iX+iWidth, iY);
    }
    // bottom left corner
    if ( corner == 2 && mode == 1 ) {
        // x, y+h
        return dvec2(iX, iY+iHeight);
    }
    // Top left corner
    if ( corner == 3 && mode == 1 ) {
        // x+w, y+h
        return dvec2(iX+iWidth, iY+iHeight);
    }
    return dvec2(-1,-1);
}

std::vector<const char*> Atlas::dump()
{
    // return the paths vector
    return this->paths;
}

bool Atlas::loadAtlas(const char* path)
{
    // check if specified path is valid atlas
    if (!std::filesystem::is_regular_file(path)) {
        printf(GLGE_ERROR_ATLAS_INVALID, path);
        return(false);
    }
    if (!std::filesystem::is_regular_file((std::string)path+".atlas")) {
        printf(GLGE_ERROR_ATLAS_INVALID, path);
        return(false);
    }

    // load atlas image
    int w, h, c;
    unsigned char* iAtlas = glgeLoad(path, &w, &h, &c);
    this->iAtlas = iAtlas;
    // load atlas data
    this->atlasData.open((std::string)path+".atlas", 'r');
    this->atlasData.read();
    this->atlasData.close();
    // return true for sucsessfull atlas loading
    return(true);
}