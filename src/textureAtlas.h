#include "GLGE/glgeErrors.hpp"
#include "GLGE/stb_image.hpp"
#include "GLGE/glgeFuncs.hpp"
#include <filesystem>
#include <iostream>
#include <vector>

class atlas {
    private:

        // The vector for holding all the image paths
        std::vector<const char*> paths;

    public:

        /**
         * @brief Default constructor
         * 
         */
        atlas();

        /**
         * @brief Adds any amount of texture paths to the path vector
         * 
         * @param path The paths to be added to the vetor
         */
        void add(const char* path);

        /**
         * @brief Builds the texture atlas
         * 
         */
        void build();

        // Developement function
        std::vector<const char*> dump();
};