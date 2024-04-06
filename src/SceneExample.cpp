/**
 * @file SceneExample.cpp
 * @author DM8AT
 * @brief an example for using scene files
 * @version 0.1
 * @date 2024-03-28
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 */

//include the main header
#include "SceneExample.hpp"
//includes from the default library
#include <iostream>

//include GLGE
#include "GLGE/GLGEALL.h"

Object* sCube;
Camera* sCam;

void sceneDraw()
{
    sCube->draw();
}

void sceneTick()
{
    sCam->update();
    sCam->recalculateProjection();

    sCube->rotate(vec3(0.1,0.2,0.3) * M_PI/180);
    sCube->update();
}

void runSceneExample()
{
    //initalise GLGE
    glgeInit();
    //create a window
    glgeCreateWindow("Scene Example", 1000,1000);
    //init the 3D core
    glgeInit3DCore();

    //create the camera
    sCam = new Camera(90, 0.1, 100.0);
    //bind the camera
    glgeBindCamera(sCam);

    //create the cube
    sCube = new Object(GLGE_PRESET_CUBE, vec4(0,0,0,1), 0, Transform(vec3(0,0,5), vec3(0), vec3(1)));
    //create a new material
    Material mat = Material(vec4(1,0,0,1), 0.3);
    //add the material
    sCube->setMaterial(mat);

    //bind the draw function
    glgeBindDisplayFunc(sceneDraw);
    //bind the tick function
    glgeBindMainFunc(sceneTick);

    //run the main loop
    glgeRunMainLoop();

    //clean up
    delete sCube;
}