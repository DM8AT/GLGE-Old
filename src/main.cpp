/**
 * @file main.cpp
 * @author DM8AT
 * @brief 
 * @version 0.1
 * @date 2023-02-05
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

//include the examples
#include "3DTest.hpp"
#include "2DTest.hpp"
#include "WidgetTest.hpp"

//for input and printing, iostream is included
#include <iostream>

// Atlas
#include "GLGE/GLGEtextureAtlas.h"

//the main function, code execution will start here. The input arguments are needed to initalise the library
int main()
{
    #if 0
    // texture atlas demo code

    // create new atlas object. This is for creating and using a texture atlas
    atlas x;

    // add files that will make up the texture atlas
    x.add("assets/cubeTexture.png");
    x.add("assets/cubeTexture.png");
    x.add("assets/Crosshair.png");
    x.add("assets/grass.png");

    // get the list of texture paths
    std::vector<const char*> tmp = x.dump();
    // print the size of the texture path vector
    std::cout << "Size of vector: " << tmp.size() << "\n";

    // print the contents of the path vector
    for (long unsigned int i = 0; i < tmp.size(); i++) {
        std::cout << tmp[i] << "\n";
    }

    // Then, build the atlas
    x.build(true);

    // get atlas image
    unsigned char* iAtlas = x.iAtlas;
    // get the information about the atlas
    nlohmann::json data = x.atlasData;
    // print atlas info
    std::cout << "W: " << data["w"] << "\n";
    std::cout << "H: " << data["h"] << "\n";

    // print the texture coor for cube texture, the top left corner
    std::cout << "C: " << x.getTexCoord("assets/cubeTexture.png", 0, 1).x << " " << x.getTexCoord("assets/cubeTexture.png", 0, 1).y << "\n";

    #endif
    #if 1
    //first, write information on what to input to start the examples
    printf("Write 0 to run the 3D example\n");
    printf("Write 1 to run the 2D example\n");
    printf("Write 2 to run the widget example\n");

    //then, get the user input
    int inp;
    std::cin >> inp;

    //start the example corresponding to the input
    if (inp == 0)
    {
        //launch the 3D core
        printf("\nLaunching 3D example\n");
        run3Dexample();
    }
    else if (inp == 1)
    {
        //launch the 2D core
        printf("\nLaunching 2D example\n");
        run2Dexample();
    }
    else if (inp == 2)
    {
        //launch the widget example
        printf("\nLaunching widget example\nTo exit press F4\n");
        runWidgetExample();
    }
    //osterei
    else if (inp == 42)
    {
        printf("6*8 = %d\n", inp);
    }
    //oseteri
    else if (inp == 2147483647)
    {
        printf("Stack Overflow: %d is out of bounds\n", inp);
    }
    //if something else was inputed, print that the value is invalide
    else
    {
        printf("The value %d dose not name an example\n", inp);
    }
    #endif
    return 0;
}