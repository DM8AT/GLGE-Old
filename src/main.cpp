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

//for input and printing, iostream is included
#include <iostream>

// Atlas
#include "textureAtlas.h"

//the main function, code execution will start here. The input arguments are needed to initalise the library
int main(int argc, char** argv)
{
    #if 0
    //first, write information on what to input to start the examples
    printf("Write 0 to run the 3D example\n");
    printf("Write 1 to run the 2D example\n");

    //then, get the user input
    int inp;
    std::cin >> inp;

    //start the example corresponding to the input
    if (inp == 0)
    {
        //launch the 3D core
        printf("\nLaunching 3D example\n");
        run3Dexample(argc, argv);
    }
    else if (inp == 1)
    {
        //launch the 2D core
        printf("\nLaunching 2D example\n");
        run2Dexample(argc, argv);
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
    #if 1
    // texture atlas demo code

    // create new atlas object. This is for creating and using a texture atlas
    atlas x;

    // add files
    x.add("assets/grass.png");
    x.add("assets/grass.png");
    x.add("assets/grass.png");
    x.add("assets/grass.png");

    // dump contents of path vector and print it
    std::vector<const char*> tmp = x.dump();
    std::cout << "Size of vector: " << tmp.size() << "\n";

    for (long unsigned int i = 0; i < tmp.size(); i++) {
        std::cout << tmp[i] << "\n";
    }

    x.build();
    #endif
    return 0;
}