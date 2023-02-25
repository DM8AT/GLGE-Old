#include "textureAtlas.h"

// Default constructor

atlas::atlas() {}

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
    std::cout << "test\n";
    
    for (long unsigned int i = 0; i < this->paths.size(); i++ ) {
        std::string buffer;
        readFile(this->paths[i], buffer);
        std::cout << buffer << "\n";
    }
}


std::vector<const char*> atlas::dump() {
    // return the paths vector
    return this->paths;
}