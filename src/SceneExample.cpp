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

//store all the scene objects
std::vector<Object*> sObjects;
//store a simple camera
Camera* sCam;

//draw the entire scene
void sceneDraw()
{
    //go over all the scene objects
    for (Object* obj : sObjects)
    {
        //draw the object one by one
        obj->draw();
    }
}

//update the objects and camera
void sceneTick()
{
    //set the rotation speed for the camera
    float rotSpeed = 0.05;
    //move the camera to the rotaion center
    sCam->set(0,0,5);
    //rotate the camera by a small amount
    sCam->rotate(vec2((glgeToRadians(0.5) * glgeGetDeltaTime()) * rotSpeed,0));
    //move the camera back by 5 units
    sCam->move(0,0,-5);

    //update the camera
    sCam->update();
    //recalculate the procection matrix
    sCam->recalculateProjection();

    //loop over all the objects
    for (Object* obj : sObjects)
    {
        //update the objects
        obj->update();
    }
}

//get all the light sources and objecs from a scene
void decodeScene(Scene* scene)
{
    //get all named objects
    std::vector<NamedObject*> objs = scene->getAllObjects();

    //loop over all objects
    for (NamedObject* obj : objs)
    {
        //get the type of the object
        std::string type = obj->getTypeName();

        //check if the type is a 3D object
        if (type == typeid(Object).name())
        {
            //if it is, add it to all the 3D objects
            sObjects.push_back(obj->getObject<Object>());
        }
        //check if the type is a light source
        else if (type == typeid(Light).name())
        {
            //if it is, add it as an global light souce
            glgeAddGlobalLighSource(obj->getObject<Light>());
        }
    }
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

    //bind the draw function
    glgeBindDisplayFunc(sceneDraw);
    //bind the tick function
    glgeBindMainFunc(sceneTick);

    // create post processing shader
    glgeSetPostProsessingShader("src/Shaders/testPostProcessingShader.fs");
    //bind another post processing shader
    Shader* invColPPS = glgeSetPostProsessingShader("src/Shaders/invertColors.fs");
    //set a uniform float in the shader to controll the strength for the invertion
    invColPPS->setCustomFloat("strength", 1);
    //recalculate the positions of the uniforms
    invColPPS->recalculateUniforms();

    //set the icon of the window
    glgeSetWindowIcon("assets/GLGEImage.png");

    //create a new scene
    Scene* scene = new Scene("TestScene");
    //load the scene from an file
    scene->load();
    //decode the scene into its objects
    decodeScene(scene);

    //set a skybox, this reverses the use of a clear color, but I wanted to show how it works
    glgeSetSkybox("assets/skybox/top.jpg",
                  "assets/skybox/bottom.jpg",
                  "assets/skybox/left.jpg",
                  "assets/skybox/right.jpg",
                  "assets/skybox/front.jpg",
                  "assets/skybox/back.jpg");

    //run the main loop
    glgeRunMainLoop();
}