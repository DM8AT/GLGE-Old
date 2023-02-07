/**
 * @file 3DTest.cpp
 * @author DM8AT
 * 
 * @brief show how the GLGE library works for 3D
 * 
 * @version 0.1
 * @date 2023-02-05
 * 
 * @copyright CC BY (see  https://creativecommons.org/licenses/by/4.0/)
 */

//include the header file, so it can be run from the main file
#include "3DTest.hpp"

//include the library core, needed for 3D and 2D
//used for first initalisation, keyboard functions, mouse functions and simple display things like creating and updating a windows
#include "GLGE.h"
//include the 3D core, needed for:
//  - creating 3D things
//  - creating a 3D camera
//  - creating a 3D world
#include "GLGE3Dcore.h"

//include math, used to make the camera movement relative to the player
#include <math.h>

//create the main camera, an instance of the Camera class. Default constructor is used in the moment, setup is later
Camera camera;

//create an instance of the Object class to store an object named cube. Default constructor is used, setup is done later
Object cube;
//crate an second instance of the Object class, also only default constructor
Object grassFloor;

//set the speed for the camera, so it is constant everywhere
float camSpeed = 0.005;
//set the rotation speed of the camera
float camRot = 0.0025;

//create an display function, it is needed to display things on the monitor (name is not important)
void display()
{
    //before this function is called, the complete screen gets cleared. 

    //call the draw function from the grass floor object to draw it to the screen
    grassFloor.draw();
    //draw the cube to the screen
    cube.draw();

    //after this function is finished, the buffer gets fliped and everything is drawn to the screen
}

//the tick function is called every tick. It should contain thinks like updates, but no draw calls. 
void tick()
{
    //make changes to the camera by changing its position or rotating it

    //calculate the cam speed for this frame, to make the movement speed framerate independend. To do that, multiply the base speed by the delta time
    float camTickSpeed = camSpeed * glgeGetDeltaTime();

    //here is the movement code for the camera

    //check for keybord inputs to move the camera like in first person cameras. 
    //check if the key is pressed. first, get all momentanly pressed keys stored in the library. then check if w is pressed. 
    if (glgeGetKeys().w)
    {
        //if the w key is pressed, the camera should move forwards
        //
        //move the camera in the direction it is looking. the camera direction is handeld a little bit different than the normal coordinate system. 
        //Key coordinate system:
        //left, right (yaw) = x
        //Up, down (pitch) = y
        //the rotation around z is currently not supported
        //
        //      x
        //      | z
        //      |/
        //      x----y
        //
        //          multiply the camera speed with some sine and cosine of the inverted camera rotation on the x axis
        camera.move(camTickSpeed*std::sin(-camera.getRotation().x),0,camTickSpeed*std::cos(-camera.getRotation().x));
    }
    //check if the s key is pressed
    if (glgeGetKeys().s)
    {
        //if the s key is pressed, then the camera should move backwards
        //it is basicaly the same as moving it forwards, the speed is just inverted
        camera.move(-camTickSpeed*std::sin(-camera.getRotation().x),0,-camTickSpeed*std::cos(-camera.getRotation().x));
    }
    //check if the d key is pressed
    if (glgeGetKeys().d)
    {
        //if the d key is pressed, then the camera should strave right. It is like moving forward, but sine and cosine is inverted and the rotation is no longer inverted
        camera.move(camTickSpeed*std::cos(camera.getRotation().x),0,camTickSpeed*std::sin(camera.getRotation().x));
    }
    //check if the a key is pressed
    if (glgeGetKeys().a)
    {
        //if a is pressed, then the camera should strave right. It is like straving left, but the speed is inverted
        camera.move(-camTickSpeed*std::cos(camera.getRotation().x),0,-camTickSpeed*std::sin(camera.getRotation().x));
    }
    //check if the space bar is pressed
    if (glgeGetKeys().space)
    {
        //if the space bar is pressed, the camera flies up with the normal movement speed
        camera.move(0,camTickSpeed,0);
    }
    //check if one of the two shift keys is pressed
    if (glgeGetKeys().shift)
    {
        //if one is pressed, then the camera should fly down. 
        camera.move(0,-camTickSpeed,0);
    }

    //here is the camera rotation

    //check if the arrow pointing up is pressed
    if (glgeGetKeys().arrowUp)
    {
        //if it is pressed, rotate the camera up by the rotation speed multiplied with delta time, to make it framerate independend. 
        camera.rotate(0,camRot*glgeGetDeltaTime());
    }
    //check if the arrow down is pressed
    if (glgeGetKeys().arrowDown)
    {
        //rotate the camera down by the framerate independend rotation speed
        camera.rotate(0,-camRot*glgeGetDeltaTime());
    }
    //check if the arrow right is pressed
    if (glgeGetKeys().arrowRight)
    {
        //if it is pressed, rotate the camera right by the framerate independend rotation speed
        camera.rotate(-camRot*glgeGetDeltaTime(),0);
    }
    //check if the arrow left is pressed
    if (glgeGetKeys().arrowLeft)
    {
        //rotate the camera left by the framerate independend rotation speed
        camera.rotate(camRot*glgeGetDeltaTime(),0);
    }

    //to make the speed changeable in runtime, add the mouse wheel status divided by 1000 to it, to make the scrolling more sensitive
    camSpeed += (float)glgeGetMouse().mouseWeel / (float)1000;

    //clamp the camera speed in an range betwean 0.001 and 0.5
    if (camSpeed < 0.001)
    {
        camSpeed = 0.001;
    }

    if (camSpeed > 0.5)
    {
        camSpeed = 0.5;
    }

    //then, update everything on screen

    //first, update the camera

    //to update the camera, recalculate the projection, this is optional, if your window will not change size
    camera.recalculateProjection();
    //then, update the camera to make all transform changes work correctly
    camera.update();

    //secondly, update the objects, so that 

    //update the grass floor, to make shure the transform and projection is correct. 
    grassFloor.update();
    //update the 
    cube.update();
}

//this function is used to setup the grassFloor instance of the Object class
void floorSetup()
{
    //the floor size is used to set the half width of the floor
    float floorSize = 200;
    //the texture size is used to make the texture smaler or bigger on the floor
    float texturesize = 2;
    //here, an pointer array of vertices is created to hold the vertex data for the object
    //
    //Vertices:
    //
    //      *       *
    //
    //
    //
    //      *       *
    //
    Vertex vertices[] = {Vertex( floorSize, 0, floorSize, floorSize*texturesize, floorSize*texturesize),
                         Vertex(-floorSize, 0, floorSize,-floorSize*texturesize, floorSize*texturesize),
                         Vertex(-floorSize, 0,-floorSize,-floorSize*texturesize,-floorSize*texturesize),
                         Vertex( floorSize, 0,-floorSize, floorSize*texturesize,-floorSize*texturesize)};

    //then, an second vertex array is created to store how to conect the vertices to indices
    //
    //Vertices conected using indices
    //
    //      *-------*
    //      |      /|
    //      |   /   |
    //      |/      |
    //      *-------*
    //
    uint indices[] = {0,1,2,
                      0,2,3};

    //then, the arrays are inputed to the Object constructor to create an new object, overwriting the data temporeraly assigned to the grass floor in the
    //default constructor. to use pointer arrays, it is also important to input the size of them, like shown below
    grassFloor = Object(vertices, indices, sizeof(vertices), sizeof(indices));
    //then, an shader is asigned to the grass floor. A shader is nececery, because else the objects could not be 3D. 
    grassFloor.setShader(GLGE_DEFAULT_3D_SHADER);
    //then the texture for the grass floor is loaded from an png file. 
    grassFloor.setTexture("assets/grass.png");
}

//in this function the cube is set up
void cubeSetup()
{
    //first, the vertices for the cube need to be created. An untextured cube would only need 8 vertices, for each point, but because
    //the cube should be textured, each point needs to be made up of 3 vertices to handle the texture data correctly. 
    //I know that this is not elegant, but it only needs 24 vertices for a cube, without indices a cube would need 36 vertices. 
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

    //then, the indices for the cube are created. I used values in the range of 0 to 7 to say the vertex ID and then added 8 or 16, if the triangle should
    //show the top or bottom(8) or the left or right(16)
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

    //the cube mesh is asigned to it like the grass floor mesh, but an optional transform is inputed to lift the cube out of the floor. else, it would be
    //stuck in there and that would not look good. The cube is moved up such an strange amount to prevent something called Z-Fighting
    cube = Object(vertices, indices, sizeof(vertices), sizeof(indices), Transform(vec3(0,1.001,0),vec3(0,0,0),1));
    //then, the same shader as used for the grass floor (Basic 3D shader) is asigned. It is inputed as shown to avoid dupication on the graphics card. 
    //this methode can create problems, if objects are created dynamicaly, because if the shader is deleted in one object, It is deleted for all objects. 
    //because of this issue, only the default constructor is existing for all classes contained in the library, so the buffers, shaders and textures are
    //left behind once it is deleted. 
    cube.setShader(grassFloor.getShader());
    //set the texture to the cube texture, loaded from an png file
    cube.setTexture("assets/cubeTexture.png");
}

//this function is like the main function in an normal scripted, but it is called form an other file, so it is named differently. 
//The inputs are declared in the 3DTest.hpp file to make the function acessable from the main.cpp file. 
void run3Dexample(int argc, char** argv)
{
    //first, the library is initalised. if the library is used, this should alway be done first. 
    glgeInit(argc, argv);

    //as second step, a window is created. This should be done second, because it also initalises the glew library
    glgeCreateWindow("3D example script for GLGE", 1000, 1000);

    //after creating the window, the 3D core is initalised. This sets all thinks like depth buffer correctly
    glgeInit3DCore();

    //Normaly, backface culling is enabled. But because my demo project is not that big, I decided to deactivate it
    glgeDisableBackfaceCulling();

    //the clear color is set here. The default clear color is the default clear color used in OpenGL. 
    glgeSetClearColor(0.5,0.5,0.5);

    //the binding of an display function is here optional, because an default function is allready created behind the scens. But it is recomended if something
    //like drawing an object should be used
    glgeBindDisplayFunc(display);
    //then, the tick function is bound. It is like the main loop of the program and called once every tick. Before it is called, all values behind the
    //scenes are updated. 
    glgeBindMainFunc(tick);

    //the camera is bound before the objects are set up, because else there would be an memory access error
    camera = Camera(90, 0.1,1000, Transform(vec3(0,0,-5),vec3(0,0,0),1));
    //then, the camera is moved one unit up
    camera.move(0,1,0);
    //after the camera is set up, the camera is bound to the library using an pointer. So, the camera can be changed and updated
    //without needing to rebind it. 
    glgeBindCamera(&camera);

    //then, the floor is set up. See above for greater detail
    floorSetup();
    //the cube is set up, like the floor is set up
    cubeSetup();

    //at the end of the function, the main loop is run. This is the point where the program will start. No code behind this line will be run. 
    glgeRunMainLoop();

    //this will not show up after closing the window
    printf("This will not be printed\n");
}