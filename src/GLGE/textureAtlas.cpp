#include "textureAtlas.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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

unsigned char * atlas::_constructs_atlas_same(int size, bool save_atlas) {
    // amount of images
    int imgs = this->paths.size();
    int w,h, c = 4;

    // get sqrt of imgs
    // if it is int and not float then the result times size is w & h
    // else the w & h is the (result + 1) * size
    float imgsroot = sqrt(imgs);
    if ( imgsroot == (int) imgsroot ) {
        w = imgsroot * size;
        h = w;
    } else {
        w = ( floor(imgsroot) + 1 ) * size;
        h = w;
        imgsroot = w/size;
    }

    std::cout << imgs << " " << w << " " << size << "\n";

    // Allocate memory for the atlas
    size_t atlasSize = w * h * c;
 
    unsigned char* atlasImg = (unsigned char*) malloc(atlasSize);
    if(atlasImg == NULL) {
        printf(GLGE_ERROR_ALLOCATE_MEMORY, "texture atlas");
        exit(1);
    }

    // clear atlas image. this gets rid of random pixel values
    this->_clear_image(atlasImg, vec3(w,h,c));

    // go through all images in the texture atlas
    int imgCount = 0;
    for (int vert = 0; vert < imgsroot; vert++ ) {
        for (int hor = 0; hor < imgsroot; hor++ ) {
            // image values
            int width, height, channelCount;
            unsigned char *img = stbi_load(paths[imgCount], &width, &height, &channelCount, 0);

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

            // Increment the image counter
            imgCount++;
            // check if all images have been written
            if (imgCount >= (int) this->paths.size()) { break; }
        }
        if (imgCount >= (int) this->paths.size()) { break; }
    }

    if (save_atlas) {
        // save atlas to the specefied location
        stbi_write_png(this->path, w, h, c, atlasImg, w*c);
    }

    return atlasImg;
}

unsigned char * atlas::_constructs_atlas_tetris(bool save_atlas) {

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

unsigned char* atlas::build() {
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

    // create a placeholder variable to be returned
    unsigned char* atlasImg;

    // Construct the atlas with the correct algorithm
    //  Square, all images are same size algorithm
    if (square_same) { atlasImg = this->_constructs_atlas_same(w); }
    //  "Tetris" Algorithm, more complex, potentianlly slower
    else { atlasImg = this->_constructs_atlas_tetris(); }

    // Return the atlas
    return atlasImg;
}


std::vector<const char*> atlas::dump() {
    // return the paths vector
    return this->paths;
}