/**
 * @file main.cpp
 * @author DM8AT
 * @brief 
 * @version 0.1
 * @date 2023-02-05
 * 
 * @copyright CC BY (see  https://creativecommons.org/licenses/by/4.0/)
 * 
 */

//include the examples
#include "3DTest.hpp"
#include "2DTest.hpp"

//for input and printing, iostream is included
#include <iostream>

//the main function, code execution will start here. The input arguments are needed to initalise the library
int main(int argc, char** argv)
{
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

    return 0;
}