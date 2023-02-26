#include "textureAtlas.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

/////////////////////////
// Default constructor //
/////////////////////////

atlas::atlas() {}

/////////////////////
// Private classes //
/////////////////////

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

    // create array for storing color values
    int arr[4] = {(int)color.x, (int)color.y, (int)color.z, (int)color.w};

    *(img + posX + posY) = arr[0];
    *(img + posX + posY+1) = arr[1];
    *(img + posX + posY+2) = arr[2];
    *(img + posX + posY+3) = arr[3];

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

void atlas::_constructs_atlas_same(int size) {
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
        w = ((int) imgsroot + 1) * size;
        h = w;
    }

    std::cout << imgs << " " << w << "\n";

    // Allocate memory for the atlas
    size_t atlasSize = w * h * c;
 
    unsigned char* atlasImg = (unsigned char*) malloc(atlasSize);
    if(atlasImg == NULL) {
        printf(GLGE_ERROR_ALLOCATE_MEMORY, "texture atlas");
        exit(1);
    }

    // set all the pixels

    
    for (int i = 0; i < (int) this->paths.size(); i++ ) {
        // load image
        int w2, h2, c2;
        unsigned char *img = stbi_load(paths[i], &w2, &h2, &c2, 0);
        // if image was not opened, print error
        if(img == NULL) {
            stbi_image_free(img);
            printf(GLGE_ERROR_IMAGE_COULDNT_OPEN, paths[i]);
        }
        
        for (int h4 = 0; h4 < h2; h4++) {
        for (int w4 = 0; w4 < w2; w4++) {
            // get the colors from a pixel
            vec4 arr = this->_get_pixel(img, vec2(w4, h4), vec3(w2, h2, c2));
            // write them to the atlas
            //this->_put_pixel(atlasImg, vec2(w4,h4), vec3(w, h, c), arr);
            //stbiw__write_pixel(, );

            printf("R:%d G:%d B:%d A:%d\n",(int)arr.x, (int)arr.y, (int)arr.z, (int)arr.w);
        }
        }

        stbi_image_free(img);
    }

    // save atlas
    //stbi_write_png(this->path, w, h, 4, atlasImg, 100);
    //stbi_write_png("assets/atlas.png", w, h, c, atlasImg, 100);

    free(atlasImg);
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

void atlas::build() {

    // test if all images are same size and square
    int w = -1;
    bool square_same = true;

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

    if (square_same) { this->_constructs_atlas_same(w); }
}


std::vector<const char*> atlas::dump() {
    // return the paths vector
    return this->paths;
}