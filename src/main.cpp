//test script to check if GLGE works, no real commenting of the script
#include "GLGE.h"
#include "GLGE3Dcore.h"
#include "GLGE2Dcore.h"

#include <iostream>
#include <cstdlib>

Object test;
Camera camera;

Object2D shape;

float camSpeed = 0.005;
float camRot = 0.0025;

void display()
{
    test.draw();
    //draw the object
    shape.draw();
}

void mainLoop()
{
    //update the projection matrix
    camera.recalculateProjection();
    camera.update();
    //update the object
    test.update();

    shape.rotate(2.5);
    shape.update();

    if (glgeGetKeys().w)
    {
        camera.move(0,0,camSpeed*glgeGetDeltaTime());
    }
    if (glgeGetKeys().s)
    {
        camera.move(0,0,-camSpeed*glgeGetDeltaTime());
    }
    if (glgeGetKeys().a)
    {
        camera.move(-camSpeed*glgeGetDeltaTime(),0,0);
    }
    if (glgeGetKeys().d)
    {
        camera.move(camSpeed*glgeGetDeltaTime(),0,0);
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

    if (glgeGetMouse().leftButton)
    {
        printf("MX: %f, MY: %f\n", glgeGetMouse().pos.x, glgeGetMouse().pos.y);
        printf("MPX: %f, MPY: %f\n", glgeGetMouse().posPixel.x, glgeGetMouse().posPixel.y);
    }
}

int main(int argc, char** argv)
{
    //init functions
    glgeInit(argc, argv);
    glgeCreateWindow("GLGE Test Script", 600,600);
    glgeInit3DCore();
    //setup
    glgeSetClearColor(.5,.5,.5);
    glgeBindDisplayFunc(display);
    glgeBindMainFunc(mainLoop);

    Vertex2D verts[] = {Vertex2D(-1,-1, 1,0),
                        Vertex2D( 1,-1, 1,1),
                        Vertex2D( 1, 1, 0,1),
                        Vertex2D(-1, 1, 0,0)};

    uint inds[] = {0,1,2,
                   0,2,3,
                   };


    verts[0].color = vec4(1,0,0,0.5);
    verts[2].color = vec4(0,0,1,0.5);
    shape = Object2D(verts, inds, sizeof(verts), sizeof(inds), Transform2D(-0.75,0.75, 45, vec2(0.25,0.25)));
    shape.setTexture("src/netherite_block.png");
    shape.setShader("src/GLGE/shaders/base2D");

    Vertex vertices[] = {
                         Vertex(-1,-1, 1, 0,0),
                         Vertex( 1,-1, 1, 1,0),
                         Vertex(-1, 1, 1, 0,1),
                         Vertex( 1, 1, 1, 1,1),

                         Vertex(-1,-1,-1, 0,0),
                         Vertex( 1,-1,-1, 1,0),
                         Vertex(-1, 1,-1, 0,1),
                         Vertex( 1, 1,-1, 1,1),
                        };

    uint indices[] = {//front and back
                      0,2,1,
                      1,2,3,
                      4,5,6,
                      5,7,6,
                      //top and bottom
                      0,1,4,
                      1,5,4,
                      2,6,3,
                      3,6,7,
                      //left and right
                      0,4,2,
                      2,4,6,
                      1,3,5,
                      3,7,5};

    test = Object(vertices, indices, sizeof(vertices), sizeof(indices));
    test.setShader("src/GLGE/shaders/base3D");
    test.setTexture(shape.getTexture());

    //camera setup
    camera = Camera(45.f, 0.1, 10.0, Transform(vec3(0,0,-5),vec3(0,0,0),1));
    camera.update();
    camera.recalculateProjection();
    glgeBindCamera(&camera);

    glgeRunMainLoop();
    return 0;
}