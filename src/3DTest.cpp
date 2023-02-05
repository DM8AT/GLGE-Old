#include "3DTest.hpp"

#include "GLGE.h"
#include "GLGE3Dcore.h"

#include <math.h>

Camera camera;

Object cube;
Object grassFloor;

float camSpeed = 0.005;
float camRot = 0.0025;

void display()
{
    grassFloor.draw();
    cube.draw();
}

void tick()
{
    grassFloor.update();
    cube.update();
    camera.recalculateProjection();
    camera.update();

    float camTickSpeed = camSpeed * glgeGetDeltaTime();

    if (glgeGetKeys().w)
    {
        camera.move(camTickSpeed*std::sin(-camera.getRotation().x),0,camTickSpeed*std::cos(-camera.getRotation().x));
    }
    if (glgeGetKeys().s)
    {
        camera.move(-camTickSpeed*std::sin(-camera.getRotation().x),0,-camTickSpeed*std::cos(-camera.getRotation().x));
    }
    if (glgeGetKeys().d)
    {
        camera.move(camTickSpeed*std::cos(camera.getRotation().x),0,camTickSpeed*std::sin(camera.getRotation().x));
    }
    if (glgeGetKeys().a)
    {
        camera.move(-camTickSpeed*std::cos(camera.getRotation().x),0,-camTickSpeed*std::sin(camera.getRotation().x));
    }
    if (glgeGetKeys().space)
    {
        camera.move(0,camSpeed*glgeGetDeltaTime(),0);
    }
    if (glgeGetKeys().shift)
    {
        camera.move(0,-camSpeed*glgeGetDeltaTime(),0);
    }

    if (glgeGetKeys().arrowUp)
    {
        camera.rotate(0,camRot*glgeGetDeltaTime());
    }
    if (glgeGetKeys().arrowDown)
    {
        camera.rotate(0,-camRot*glgeGetDeltaTime());
    }
    if (glgeGetKeys().arrowRight)
    {
        camera.rotate(-camRot*glgeGetDeltaTime(),0);
    }
    if (glgeGetKeys().arrowLeft)
    {
        camera.rotate(camRot*glgeGetDeltaTime(),0);
    }

    camSpeed += (float)glgeGetMouse().mouseWeel / (float)1000;

    if (camSpeed < 0.001)
    {
        camSpeed = 0.001;
    }

    if (camSpeed > 0.5)
    {
        camSpeed = 0.5;
    }
}

void floorSetup()
{
    float floorSize = 200;
    Vertex vertices[] = {Vertex( floorSize, 0, floorSize, vec4(0,1,0,1)),
                         Vertex(-floorSize, 0, floorSize, vec4(0,1,0,1)),
                         Vertex(-floorSize, 0,-floorSize, vec4(0,1,0,1)),
                         Vertex( floorSize, 0,-floorSize, vec4(0,1,0,1))};

    uint indices[] = {0,1,2,
                      0,2,3};

    grassFloor = Object(vertices, indices, sizeof(vertices), sizeof(indices));
    grassFloor.setShader(GLGE_DEFAULT_3D_SHADER);
}

void cubeSetup()
{
    Vertex vertices[] = {
                         //front and back
                         Vertex(-1,-1, 1, 0,0),
                         Vertex( 1,-1, 1, 1,0),
                         Vertex(-1, 1, 1, 0,1),
                         Vertex( 1, 1, 1, 1,1),

                         Vertex(-1,-1,-1, 1,0),
                         Vertex( 1,-1,-1, 0,0),
                         Vertex(-1, 1,-1, 1,1),
                         Vertex( 1, 1,-1, 0,1),

                         //top and bottom
                         Vertex(-1,-1, 1, 0,1),
                         Vertex( 1,-1, 1, 1,1),
                         Vertex(-1, 1, 1, 0,0),
                         Vertex( 1, 1, 1, 1,0),

                         Vertex(-1,-1,-1, 0,0),
                         Vertex( 1,-1,-1, 1,0),
                         Vertex(-1, 1,-1, 0,1),
                         Vertex( 1, 1,-1, 1,1),

                         //left and right
                         Vertex(-1,-1, 1, 1,0),
                         Vertex( 1,-1, 1, 0,0),
                         Vertex(-1, 1, 1, 1,1),
                         Vertex( 1, 1, 1, 0,1),

                         Vertex(-1,-1,-1, 0,0),
                         Vertex( 1,-1,-1, 1,0),
                         Vertex(-1, 1,-1, 0,1),
                         Vertex( 1, 1,-1, 1,1),
                        };

    uint indices[] = {
                      //front and back
                      0,2,1,
                      1,2,3,
                      4,5,6,
                      5,7,6,
                      //top and bottom
                      0+8,1+8,4+8,
                      1+8,5+8,4+8,
                      2+8,6+8,3+8,
                      3+8,6+8,7+8,
                      //left and right
                      0+16,4+16,2+16,
                      2+16,4+16,6+16,
                      1+16,3+16,5+16,
                      3+16,7+16,5+16};

    cube = Object(vertices, indices, sizeof(vertices), sizeof(indices), Transform(vec3(0,1,0),vec3(0,0,0),1));
    cube.setShader(grassFloor.getShader());
    cube.setTexture("assets/cubeTexture.png");
}

void run3Dexample(int argc, char** argv)
{
    glgeInit(argc, argv);

    glgeCreateWindow("3D example script for GLGE", 1000, 1000);

    glgeInit3DCore();

    glgeSetClearColor(0.5,0.5,0.5);

    glgeBindDisplayFunc(display);
    glgeBindMainFunc(tick);

    camera = Camera(90, 0.1,1000, Transform(vec3(0,0,-5),vec3(0,0,0),1));
    camera.move(1,1,1);
    glgeBindCamera(&camera);

    floorSetup();
    cubeSetup();

    glgeRunMainLoop();
}