/**
 * @file main.cpp
 * @author DM8AT
 * @brief 
 * @version 0.1
 * @date 2023-02-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "GLGE.h"

#include "3DTest.hpp"

#include <iostream>

int main(int argc, char** argv)
{
    printf("Write 0 to run the 3D example\n");
    printf("Write 1 to run the 2D example\n");

    int inp;
    std::cin >> inp;

    if (inp == 0)
    {
        //launch the 3D core
        printf("\nLaunching 3D example\n");
        run3Dexample(argc, argv);
    }
    else if (inp == 1)
    {
        //launch the 2D core
        printf("\nCurrently, there is no 2D example\n");
    }
    else
    {
        printf("The value %d dose not name an example\n", inp);
    }

    return 0;
}