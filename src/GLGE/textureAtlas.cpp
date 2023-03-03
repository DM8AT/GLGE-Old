#include "textureAtlas.h"
#include "nlohmann/json.hpp"
#include "fstream"
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using json = nlohmann::json;

/////////////////////////
// Default constructor //
/////////////////////////

atlas::atlas() {}

/////////////////////
// Private classes //
/////////////////////

void atlas::_clear_image(unsigned char* img, vec3 info) {
    // go to all pixels
    for (int w = 0; w < info.x; w++) {
    for (int h = 0; h < info.y; h++) {
        // write 0,0,0,0 to image pixel
        this->_put_pixel(img, vec2(w,h), info, vec4(0,0,0,0));
    }
    }
}

void atlas::_put_pixel(unsigned char* img, vec2 pos, vec3 info, vec4 color ) {
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

    int posX = pos.x * info.z;
    int posY = pos.y * info.x * info.z;

    // The X and Y pos combined in one int
    int p = posX + posY;

    // create array for storing color values
    int arr[4] = {(int)color.x, (int)color.y, (int)color.z, (int)color.w};

    // write the RGBA values
    for (int i = 0; i < 4; i++ ) {
        img[p++] = arr[i];
    }
}

vec4 atlas::_get_pixel(unsigned char* img, vec2 pos, vec3 info ) {
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

    int posX = pos.x * info.z;
    int posY = pos.y * info.x * info.z;

    unsigned char *p = img;

    // create array for storing color values
    int arr[4] = {0, 0, 0, 255};

    // go through all channels
    for (int i = 0; i < info.z; i++) {
        arr[i] = *(p + posX + posY + i );
    }

    return vec4(arr[0], arr[1], arr[2], arr[3]);

}

bool atlas::_is_occupied(vec2 pos) {
    // check whether or not a certain position on the texture atlas is occupied. This releys on the atlasData variable
    // check if atlas data is empty
    if ( this->atlasData["images"].size() == 0 ) {
        return false;
    }

    // go through eachh image in atlasData
    for (nlohmann::json::iterator img = this->atlasData["images"].begin(); img != this->atlasData["images"].end(); img++) {
        // image x
        int iX = this->atlasData["images"][img.key()]["x"];
        // image width
        int iW = iX + (int) this->atlasData["images"][img.key()]["w"];
        // image y
        int iY = this->atlasData["images"][img.key()]["y"];
        // image height
        int iH = iY + (int) this->atlasData["images"][img.key()]["h"];

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

bool atlas::_is_region_occupied(vec4 pos) {
    // check whether or not a certain position on the texture atlas is occupied. This releys on the atlasData variable
    // check if atlas data is empty
    if ( this->atlasData["images"].size() == 0 ) {
        return false;
    }

    // go through eachh image in atlasData
    for (nlohmann::json::iterator img = this->atlasData["images"].begin(); img != this->atlasData["images"].end(); img++) {
        // image x
        int iX = this->atlasData["images"][img.key()]["x"];
        // image width
        int iW = iX + (int) this->atlasData["images"][img.key()]["w"];
        // image y
        int iY = this->atlasData["images"][img.key()]["y"];
        // image height
        int iH = iY + (int) this->atlasData["images"][img.key()]["h"];

        if (pos.z < iW - iX) {
            // check if x is in range of x and w
            if ( iX < pos.x && pos.x < iW ) {
                return true;
            }
            // check if x+width is in range of x and w
            if ( iX < pos.x+pos.z && pos.x+pos.z < iW ) {
                return true;
            }
        } else {
            // check if x is in range of x and w
            if ( pos.x < iX && iX < pos.x+pos.z ) {
                return true;
            }
            // check if x+width is in range of x and w
            if ( pos.x < iW && iW < pos.x+pos.z ) {
                return true;
            }
        }

        if (pos.w < iH - iY) {
            // check if y is in range of y and h
            if ( iY < pos.x && pos.y < iH) {
                return true;
            }
            // check if y+width is in range of x and w
            if ( iY < pos.y+pos.z && pos.y+pos.z < iH ) {
                return true;
            }
        } else {
            // check if y is in range of y and h
            if ( pos.y < iY && iY < pos.y+pos.w ) {
                return true;
            }
            // check if y+width is in range of x and w
            if ( pos.y < iH && iH < pos.y+pos.w ) {
                return true;
            }
        }
    }

    return false;
}

unsigned char * atlas::_constructs_atlas_same(int size, bool save_atlas) {
    // amount of images
    int imgs = this->paths.size();
    int w,h, c = 4;

    // get sqrt of imgs
    float imgsroot = sqrt(imgs);
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
    this->atlasData["w"] = w;
    this->atlasData["h"] = h;

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

    // counter for how many images have been processed already
    int imgCount = 0;
    // go through all images in the texture atlas
    for (int vert = 0; vert < imgsroot; vert++ ) {
        for (int hor = 0; hor < imgsroot; hor++ ) {
            // image values
            int width, height, channelCount;
            unsigned char *img = stbi_load(paths[imgCount], &width, &height, &channelCount, 0);

            // add image info to atlas data
            this->atlasData["images"][paths[imgCount]]["w"] = width;
            this->atlasData["images"][paths[imgCount]]["h"] = height;
            this->atlasData["images"][paths[imgCount]]["x"] = hor*size;
            this->atlasData["images"][paths[imgCount]]["y"] = vert*size;

            // go through all pixels of the image
            for (int hI = 0; hI < height; hI++) {
            for (int wI = 0; wI < width; wI++) {
                // calculate the current position in the image and atlas
                vec2 imgPos = vec2(wI, hI);
                vec2 atlasPos = vec2(imgPos.x+hor*size, imgPos.y+vert*size);
                // get the colors from a pixel
                vec4 arr = this->_get_pixel(img, imgPos, vec3(width, height, channelCount));
                // write them to the atlas
                this->_put_pixel(atlasImg, atlasPos, vec3(w, h, c), arr);
                
                // For debugging, print rgba values
                if (imgCount < 2 && 0) {
                    printf("R:%d G:%d B:%d A:%d; IX:%d IY:%d; AX:%d AY:%d\n",(int)arr.x, (int)arr.y, (int)arr.z, (int)arr.w, (int)imgPos.x, (int)imgPos.y, (int)atlasPos.x, (int)atlasPos.y);
                }
            }
            }

            // free image
            stbi_image_free(img);

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
        // save atlas to the specefied location
        stbi_write_png(this->path, w, h, c, atlasImg, w*c);
        // save the atlas info
        std::ofstream dataFile ((((std::string) this->path ) + ".json" ).c_str());
        dataFile << this->atlasData.dump(4).c_str();
        dataFile.close();
    }

    // return the atlas object
    return atlasImg;
}

unsigned char * atlas::_constructs_atlas_tetris(bool save_atlas) {
    // The slitly slower algorithm for placing images on a blank canvas, a bit like playing tetris

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
    // First pass through all images
    for ( long unsigned int i = 0; i < this->paths.size(); i++ ) {
        // image dimension vars
        int w, h, c;
        // load image info
        unsigned char* img = stbi_load(paths[i], &w, &h, &c, 0);
        // free image for no memoryleaks
        stbi_image_free(img);

        // Add w & h to totals
        if ( i < sqrt( this->paths.size() ) || w > totalWidth ) {
            totalWidth += w;
        }

        totalHeight += h;
    }

    // write template to atlas info data
    this->atlasData["w"] = totalWidth;
    this->atlasData["h"] = totalHeight;

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

    // Second pass, it's tetris time!
    for ( long unsigned int i = 0; i < this->paths.size(); i++ ) {
        // image dimension vars
        int w, h, c;
        // load image info
        unsigned char* img = stbi_load(paths[i], &w, &h, &c, 0);

        // break var
        bool done = false;

        // find a spot to put the image
        for (int y = 0; y < totalHeight-(h-1); y++) {
        for (int x = 0; x < totalWidth-(w-1); x++) {
            // check if spot is free
            if ( this->_is_region_occupied(vec4(x,y,w,h)) ) { 
                continue;
            }

            // add image info to atlas data
            this->atlasData["images"][paths[i]]["w"] = w;
            this->atlasData["images"][paths[i]]["h"] = h;
            this->atlasData["images"][paths[i]]["x"] = x;
            this->atlasData["images"][paths[i]]["y"] = y;

            // Write image to atlas
            // go through all pixels of the image
            for (int hI = 0; hI < h; hI++) {
            for (int wI = 0; wI < w; wI++) {
                // calculate the current position in the image and atlas
                vec2 imgPos = vec2(wI, hI);
                vec2 atlasPos = vec2(imgPos.x+x, imgPos.y+y);
                // get the colors from a pixel
                vec4 arr = this->_get_pixel(img, imgPos, vec3(w, h, c));
                // write them to the atlas
                this->_put_pixel(atlasImg, atlasPos, vec3(totalWidth, totalHeight, channelCount), arr);
            }
            }

            // set break var to true
            done = true;
            break;
        }
            // if spot has been found, break out of loop
            if (done) {break;}
        }
        // check if a position was not found
        if (!done) {
            printf("[GLGE ERROR RETURN] no space for image %s was found in texture atlas while creating.\n", this->paths[i]);
        }
    }

    // save image and data if needed;
    if (save_atlas) {
        // save atlas to the specefied location
        stbi_write_png(this->path, totalWidth, totalHeight, channelCount, atlasImg, totalWidth*channelCount);
        // save the atlas info
        std::ofstream dataFile ((((std::string) this->path ) + ".json" ).c_str());
        dataFile << this->atlasData.dump(4).c_str();
        dataFile.close();
    }

    // return atlas image
    return atlasImg;
}

int * atlas::_img_info(const char* path) {
    // load image
    int w, h, c; 
    unsigned char *img = stbi_load(path, &w, &h, &c, 0);
    // if image was not opened, print error
    if(img == NULL) {
        stbi_image_free(img);
        printf(GLGE_ERROR_IMAGE_COULDNT_OPEN, path);
        return new int[3] {-1, -1, -1};
    }
    stbi_image_free(img);
    // debug message
    //printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", w, h, c);

    return new int[3] {w, h, c};
}

////////////////////
// Public classes //
////////////////////

// add objects to the paths vector
void atlas::add(const char* path) {
    // check if path exists
    if ( ! std::filesystem::is_regular_file(path)) {
        // pring out error
        printf(GLGE_ERROR_FILE_NOT_FOUND, path);
        //return void
        return;
    }
    // add path to vector
    this->paths.push_back(path);
}

void atlas::build(bool save_atlas) {
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

    // create a placeholder variable to be set internally
    unsigned char* atlasImg;

    // Construct the atlas with the correct algorithm
    //  Square, all images are same size algorithm
    if (square_same) { atlasImg = this->_constructs_atlas_same(w, save_atlas); }
    //  "Tetris" Algorithm, more complex, potentianlly slower
    else { atlasImg = this->_constructs_atlas_tetris(save_atlas); }

    // set iAtlas to atlasImg
    this->iAtlas = atlasImg;
}


std::vector<const char*> atlas::dump() {
    // return the paths vector
    return this->paths;
}