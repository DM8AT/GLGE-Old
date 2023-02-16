/**
 * @file 2DTest.cpp
 * @author DM7AT
 * @brief show how the GLGE library works for 2D
 * @version 0.1
 * @date 2023-02-06
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

//include the header to make the script accesable form the main file 
#include "2DTest.hpp"

//include the core to the library
#include "GLGE.h"
//include the 2D core to have acces to all of the 2D functions

#include "GLGE2Dcore.h"

//create the camera to move around in the 2D world 
Camera2D cam;
//create an 2D object for an triangle
Object2D triangle;
//create an 2D object for an square
Object2D square;
//create an 2D object for an crosshair
Object2D crosshair;

//set the camera speed to an variable
float cameraSpeed = 0.05;

//set the world scale to an variable so it can be changed later
float worldScale = 1.f;

//this function is called after the screen is cleared and before the buffer is swaped
void dispFunc()
{
    //in this case, the first drawn object is the lowest on the draw stack and coverd by the other objects.
    //the object on top should be drawn first

    //draw the triangle
    triangle.draw();
    //draw the square
    square.draw();
    //draw the crosshair
    crosshair.draw();
}

//this function is called once a tick
void tickFunc()
{
    //the speed of the camera is framerate dependend

    //check if the w key is pressed
    if (glgeGetKeys().w)
    {
        //change the position of the camera up by the camera speed
        cam.move(0,-cameraSpeed);
    }
    //check if the s key is pressed
    if (glgeGetKeys().s)
    {
        //change the position of the camera down by the camera speed
        cam.move(0,cameraSpeed);
    }
    //check if the a key is pressed
    if (glgeGetKeys().a)
    {
        //change the position of the camera left by the camera speed
        cam.move(cameraSpeed,0);
    }
    //check if the d key is pressed
    if (glgeGetKeys().d)
    {
        //change the position of the camera right by the camera speed
        cam.move(-cameraSpeed,0);
    }

    //change the world speed by the mouse wheel delta scroll divided by 10
    worldScale += glgeGetMouse().mouseWeel/10.f;

    //clamp the world scale beteween 0.1 and 3
    if (worldScale < 0.1)
    {
        worldScale = 0.1;
    }
    else if (worldScale > 5)
    {
        worldScale = 5;
    }

    //set the camera scale to the world scale on both axis
    cam.setScale(vec2(worldScale,worldScale));

    //update the camera
    cam.update();

    //rotate the triangle by 2.5 degrees
    triangle.rotate(2.5);

    //update the triangle
    triangle.update();
    //update the square
    square.update();
    //update the crosshair
    crosshair.update();
}

//setup the triangle
void triangleSetup()
{
    //set the vertices for the triangle
    Vertex2D vertices[] = {Vertex2D(0,1, 1,0,0,1),
                           Vertex2D(-1,-1, 0,1,0,1),
                           Vertex2D( 1,-1, 0,0,1,1)};

    //bind the vertices to an triangle1
    uint indices[] = {0,1,2};

    //set the triangle to an 2D object created out of the vertices scaled down by 0.5
    triangle = Object2D(vertices, indices, sizeof(vertices), sizeof(indices), Transform2D(0,0,0,vec2(0.5,0.5)));
}

//setup the square
void squareSetup()
{
    //set the vertices for the square
    Vertex2D vertices[] = {Vertex2D(1,1, 0.99,0.99),
                           Vertex2D(0,1, 0.01,0.99),
                           Vertex2D(0,0, 0.01,0.01),
                           Vertex2D(1,0, 0.99,0.01)};

    //conect the vertices to two triangles
    uint indices[] = {0,1,2,
                      0,2,3};
    
    //set the square to an 2D object moved 1 right and 0.75 up and scaled down by 0.75
    square = Object2D(vertices, indices, sizeof(vertices), sizeof(indices), Transform2D(1,0.75, 0, vec2(0.75,0.75)));
    //set the shader to the shader from the triangle
    square.setShader(triangle.getShader());
    //load the texture for the square
    square.setTexture("assets/cubeTexture.png");
}

//setup the crosshair
void setupCrosshair()
{
    //set the vertices for the crosshair like an square
    Vertex2D vertices[] = {Vertex2D(1,1, 0.99,0.99),
                           Vertex2D(0,1, 0.01,0.99),
                           Vertex2D(0,0, 0.01,0.01),
                           Vertex2D(1,0, 0.99,0.01)};

    //conect them to an square
    uint indices[] = {0,1,2,
                      0,2,3};

    //set the crosshair to an object made of the vertices and indices scaled down by 0.125 and that dose not move with the camera
    crosshair = Object2D(vertices, indices, sizeof(vertices), sizeof(indices), Transform2D(0,0, 0, vec2(0.125,0.125)), true);
    //set the shader to the shader from the triangle
    crosshair.setShader(triangle.getShader());
    //load the texture for the crosshair
    crosshair.setTexture("assets/Crosshair.png");
}

//this is the main function
void run2Dexample(int argc, char** argv)
{
    //initalise the library
    glgeInit(argc, argv);

    //create an window named "2D GLGE Example" and with the size of 1000px*1000px
    glgeCreateWindow("2D GLGE Example", 1000, 1000);

    //initalise the 2D core
    glgeInit2DCore();

    //disable the backface culling, it is stupid for 2D
    glgeDisableBackfaceCulling();

    //set the background color
    glgeSetClearColor(0.25,0.25,0.5);

    //bind the function to display thinks to the screen
    glgeBindDisplayFunc(dispFunc);
    //bind the function that should be called every tick
    glgeBindMainFunc(tickFunc);

    //bind the camera
    glgeBindMain2DCamera(&cam);

    //setup the triangle
    triangleSetup();
    //setup the square
    squareSetup();
    //setup the crosshair
    setupCrosshair();

    //execute the script
    glgeRunMainLoop();
}