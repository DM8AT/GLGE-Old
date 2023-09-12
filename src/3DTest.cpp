/**
 * @file 3DTest.cpp
 * @author DM8AT
 * 
 * @brief show how the GLGE library works for 3D
 * 
 * @version 0.1
 * @date 2023-02-05
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 */

//include the header file, so it can be run from the main file
#include "3DTest.hpp"

//say that the SDL specific functions should be included as well
#define GLGE_USE_SDL
//include the library core, needed for 3D and 2D
//used for first initalisation, keyboard functions, mouse functions and simple display things like creating and updating a windows
#include "GLGE/GLGE.h"
//include the 3D core, needed for:
//  - creating 3D things
//  - creating a 3D camera
//  - creating a 3D world
#include "GLGE/GLGE3Dcore.h"
//include the shader core of the library
//it is not nececeraly needed to handle shaders, but it is recomended. 
#include "GLGE/GLGEShaderCore.h"

//include the light core for good lighting
#include "GLGE/GLGELightingCore.h"

//include math, used to make the camera movement relative to the player
#include <math.h>
#include <iostream>
#include <time.h>

//create the main camera, an instance of the Camera class. Default constructor is used in the moment, setup is later
Camera camera;

//create an instance of the Object class to store an object named cube. Default constructor is used, setup is done later
Object cube;
//crate an second instance of the Object class, also only default constructor
Object grassFloor;
//create an instance of the Object class to store an object named thing. The default constructor is used. Named so, because it can be everything
//that can be loaded from an .obj file
Object thing;
//this object is loaded from the file Enterpreis.obj. 
Object enterprise;
//it is a brick wall
Object wall;

Light light;

Light l2;

Shader* invColPPS;

RenderTarget renderTarget;

//set the speed for the camera, so it is constant everywhere
float camSpeed = 0.005;
//set the mouse sensetivity
float mouseSensetivity = 20.f;

//say if the mouse is currently locked to the screen
bool isActive = true;

//create an display function, it is needed to display things on the monitor (name is not important)
void display()
{
    //before this function is called, the complete screen gets cleared. 

    //draw the Enterpreis
    enterprise.draw();

    //call the draw function from the grass floor object to draw it to the screen
    grassFloor.draw();
    //draw the cube to the screen
    cube.draw();

    //draw the thing to the screen
    thing.draw();

    //draw the wall
    wall.draw();

    //after this function is finished, the frame buffer gets fliped and everything is drawn to the screen
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
    //if the cursor is locked to the window
    if (isActive)
    {
        //rotate the camera by the mouse position
        camera.rotate((-(glgeGetMouse().pos.x-0.5f)/glgeGetDeltaTime())*mouseSensetivity, (-((glgeGetMouse().pos.y-0.5f))/glgeGetDeltaTime())*mouseSensetivity, 0);
    }
    //clamp the camera rotation on the y axis
    camera.setRotation(camera.getRotation().x, glgeClamp(camera.getRotation().y, glgeToRadians(-90), glgeToRadians(90)));
    
    //if the cursor is clicked and not locked to the window, lock it to the window
    if (glgeGetMouse().leftButton && !isActive)
    {
        glgeSetCursor(GLGE_CURSOR_STYLE_NONE);
        isActive = true;
    }

    //if the e key is pressed while the mouse is locked, unlock it
    if (glgeGetToggledKeys().e && isActive)
    {
        glgeSetCursor(GLGE_CURSOR_STYLE_DEFAULT);
        isActive = false;
    }

    //if the mouse is locked, move it to 0,0
    if (isActive)
    {
        //check first if the pointer is not allready at 0,0
        if ((glgeGetMouse().posPixel.x != (glgeGetWindowSize().x/2)) || (glgeGetMouse().posPixel.y != (glgeGetWindowSize().y/2)))
        {
            glgeWarpPointer(0,0);
        }
    }

    //check if the key y is pressed
    if (glgeGetKeys().y)
    {
        //check if the mouse was scrolled
        if (glgeGetMouse().mouseWheel != 0)
        {
            //check the direction of the mouse wheel
            if (glgeGetMouse().mouseWheel > 0)
            {
                //change the amount of color inversion by a bit
                invColPPS->setCustomFloat("strength", 0.01, GLGE_MODE_ADD);
            }
            else
            {
                //change the amount of color inversion by a bit
                invColPPS->setCustomFloat("strength", 0.01, GLGE_MODE_SUBTRACT);
            }
        }
    }
    else
    {
        //to make the speed changeable in runtime, add the mouse wheel status divided by 1000 to it, to make the scrolling more sensitive
        camSpeed += (float)glgeGetMouse().mouseWheel / (float)1000;
    }


    //clamp the camera speed in an range betwean 0.001 and 0.5
    camSpeed = glgeClamp(camSpeed, 0.001, 0.5);

    //then, update everything on screen

    //check if the F11 key was pressed
    if (glgeGetToggledKeys().f11)
    {
        //then toggle the fullscreen mode
        glgeToggleFullscreen();
    }

    //first, update the camera

    //to update the camera, recalculate the projection, this is optional, if your window will not change size
    camera.recalculateProjection();
    //then, update the camera to make all transform changes work correctly
    camera.update();

    //recalculate the height for the cube obj
    cube.setPos(0, 1.5 + std::sin(glgeToDegrees(glgeGetCurrentElapsedTime() * 0.00001)) / 2.f, 0);

    //secondly, update the objects, so that 

    //update the grass floor, to make shure the transform and projection is correct. 
    grassFloor.update();
    //update the cube
    cube.update();
    //update the thing
    thing.update();
    //update the enterprise
    enterprise.update();
    //update the wall
    wall.update();

    //set the position of the light source to be exactly at the player
    //light.setPos(camera.getPos());

    //write the current FPS as the window title
    glgeSetWindowTitle(std::string("3D example script for GLGE : " + std::to_string(glgeGetCurrentFPS())).c_str());
}

//this function is used to setup the grassFloor instance of the Object class
void floorSetup()
{
    //the floor size is used to set the half width of the floor
    float floorSize = 1000;
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

    //then, the arrays are inputed to the Mesh constructor to create an new mesh, that can be used to create an Object. 
    Mesh m = Mesh(vertices, indices, sizeof(vertices), sizeof(indices));
    //the normal vectors of the mesh are recalculated, so it can be lit. 
    m.recalculateNormals();
    //the grass floor is constructed from the previously created mesh by overwriting the data previously stored in it with the new mesh
    grassFloor = Object(m);

    //an shader can be asigned to the object, but it is no longer necesery. 
        //then, an shader is asigned to the grass floor. A shader is nececery, because else the objects could not be 3D. 
        //grassFloor.setShader(GLGE_DEFAULT_3D_SHADER);
    //then create a material for the grass floor
    Material grassFloorMat = Material("assets/grass.png", GLGE_TEXTURE, 0.2);
    //apply the material to the grass floor
    grassFloor.setMaterial(grassFloorMat);
}

//in this function the cube is set up
void cubeSetup()
{
    //first, the vertices for the cube need to be created. An untextured cube would only need 8 vertices, for each point, but because
    //the cube should be textured, each point needs to be made up of 3 vertices to handle the texture data correctly. 
    //I know that this is not elegant, but it only needs 24 vertices for a cube, without indices a cube would need 36 vertices. 
    Vertex vertices[] = {
                                //pos   //tex   //normal
                         //front and back
                         Vertex(-1,-1, 1, 0,0,  0, 0, 1),
                         Vertex( 1,-1, 1, 1,0,  0, 0, 1),
                         Vertex(-1, 1, 1, 0,1,  0, 0, 1),
                         Vertex( 1, 1, 1, 1,1,  0, 0, 1),

                         Vertex(-1,-1,-1, 1,0,  0, 0,-1),
                         Vertex( 1,-1,-1, 0,0,  0, 0,-1),
                         Vertex(-1, 1,-1, 1,1,  0, 0,-1),
                         Vertex( 1, 1,-1, 0,1,  0, 0,-1),

                         //top and bottom
                         Vertex(-1,-1, 1, 0,1,  0,-1, 0),
                         Vertex( 1,-1, 1, 1,1,  0,-1, 0),
                         Vertex(-1, 1, 1, 0,0,  0, 1, 0),
                         Vertex( 1, 1, 1, 1,0,  0, 1, 0),

                         Vertex(-1,-1,-1, 0,0,  0,-1, 0),
                         Vertex( 1,-1,-1, 1,0,  0,-1, 0),
                         Vertex(-1, 1,-1, 0,1,  0, 1, 0),
                         Vertex( 1, 1,-1, 1,1,  0, 1, 0),

                         //left and right
                         Vertex(-1,-1, 1, 1,0, -1, 0, 0),
                         Vertex( 1,-1, 1, 0,0,  1, 0, 0),
                         Vertex(-1, 1, 1, 1,1, -1, 0, 0),
                         Vertex( 1, 1, 1, 0,1,  1, 0, 0),

                         Vertex(-1,-1,-1, 0,0, -1, 0, 0),
                         Vertex( 1,-1,-1, 1,0,  1, 0, 0),
                         Vertex(-1, 1,-1, 0,1, -1, 0, 0),
                         Vertex( 1, 1,-1, 1,1,  1, 0, 0),
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
    cube = Object(vertices, indices, sizeof(vertices), sizeof(indices), Transform(vec3(0,1.01,2),vec3(0,0,0),1));

    //create a material for the cube
    Material cubeMaterial = Material(glgeGetLastFrame(), GLGE_TEXTURE, 0);
    //set it to unlit
    cubeMaterial.setLit(false);

    //apply the material to the cube
    cube.setMaterial(cubeMaterial);
}

//setup the thing
void thingSetup()
{
    //load a mesh from an file        specify the file format to be .obj
    Mesh m = Mesh("assets/Vertices.obj", GLGE_OBJ);

    //load the mesh to an object and change the position of it
    thing = Object(m, Transform(vec3(0,0,-5), vec3(0,0,0), 1));

    //create a material for the thing
    Material thingMat = Material("assets/cubeTexture.png", GLGE_TEXTURE, 0.2);

    //apply the material to the thing
    thing.setMaterial(thingMat);
}

void enterpriseSetup()
{
    //load the Enterprise object from an file, the file format is .obj
    enterprise = Object("assets/Enterpreis.obj", GLGE_OBJ, Transform(vec3(0,10,0), vec3(5,10,6), 1));

    //create a material for the object
    Material mat = Material(vec4(0.25,0.25,0.25, 1.f), 0.2, 1.0);

    //bind the material to the enterprise
    enterprise.setMaterial(mat);
}

void wallSetup()
{
    //create the material for the wall
    Material mat;
    //load the albedo to the material
    mat = Material("assets/WallTexture/harshbricks-albedo.png", GLGE_TEXTURE, 0.2);
    //set the Interpolation mode to linear, so the normal get linearly interpolated. It looks better
    glgeSetInterpolationMode(GLGE_LINEAR);

    //load the normal map to the material
    mat.setNormalMap("assets/WallTexture/harshbricks-normal.png", GLGE_NORMAL_MAP);

    //load the roughness map
    mat.setRoughnessMap("assets/WallTexture/harshbricks-roughness.png", GLGE_ROUGHNESS_MAP);

    //load the height map
    mat.setHeightMap("assets/WallTexture/harshbricks-height5-16.png");

    //reset the Interpolation mode
    glgeSetInterpolationMode(GLGE_NEAREST);

    //create a shader for the wall
    Shader shader = Shader("src/vertexShader.vs", "src/fragmentShader330.fs");
    //set a geometry shader for the wall
    shader.addGeometryShader("src/geometryShader.gs");

    //create the Wall from an file
    wall = Object("assets/Wall.obj", GLGE_OBJ, Transform(vec3(5,0.1,2), vec3(0,155,0), 1.f));

    //bind the shader of the object
    wall.setShader(shader.getShader());

    //apply the material to the wall
    wall.setMaterial(mat);
}

void windowResized(int width, int height)
{
    //update the size of the render target
    renderTarget.changeSize(width, height);
    //print debug information
    printf("Window resized to: %d, %d\n", width,height);
}

//a function to set up an shader
Shader testCustomPPSFunc(unsigned int curr)
{
    //create a custom shader object
    Shader s("src/testShader.fs", GLGE_FRAGMENT_SHADER);
    //set the input for the main image (can be named anything)
    s.setCustomTexture("glgeMainImage", curr);
    //set the input for the window size (can be named anything)
    s.setCustomVec2("glgeWindowSize", glgeGetWindowSize());
    //give the shader to the program
    return s;
}

//this function is like the main function in an normal scripted, but it is called form an other file, so it is named differently. 
//The inputs are declared in the 3DTest.hpp file to make the function acessable from the main.cpp file. 
void run3Dexample(int argc, char** argv)
{
    //first, the library is initalised. if the library is used, this should alway be done first. 
    glgeInit(argc, argv);

    //as second step, a window is created. This should be done second, because it also initalises the glew library
    glgeCreateWindow("3D example script for GLGE : init...", 1000, 1000);

    //after creating the window, the 3D core is initalised. This sets all thinks like depth buffer correctly
    glgeInit3DCore();

    //set the icon of the window
    glgeSetWindowIcon("assets/GLGEImage.png");

    //Normaly, backface culling is enabled. But it can be disabled using the following function:
    //glgeDisableBackfaceCulling();

    //a lighting shader is bound by default, so binding one is not needed (source: GLGE/glgeDefaultLightingShaderSource.fs)
    glgeSetLightingShader("src/GLGE/glgeDefaultLightingShaderSource.fs");

    //bind a post processing shader
    glgeSetPostProsessingShader("src/testPostProcessingShader.fs");
    //bind another post processing shader
    invColPPS = glgeSetPostProsessingShader("src/invertColors.fs");
    //set a uniform float in the shader to controll the strength for the invertion
    invColPPS->setCustomFloat("strength", 1);
    //recalculate the positions of the uniforms
    invColPPS->recalculateUniforms();
    //add a function that will be executed as a post-processing shader during the post-processing passes
    glgeAddCustomPostProcessingFunc(testCustomPPSFunc);

    //generate a render target using an shader
    renderTarget = RenderTarget(glgeGetScreenSize());
    //set the shader for the render target
    renderTarget.setShader(new Shader(GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER, "src/invertColors.fs"), true);

    //the clear color is set here. The default clear color is the default clear color used in OpenGL. 
    glgeSetClearColor(0.5,0.5,0.5);
    
    //set a skybox, this reverses the use of a clear color, but I wanted to show how it works
    glgeSetSkybox("assets/skybox/top.jpg",
                  "assets/skybox/bottom.jpg",
                  "assets/skybox/left.jpg",
                  "assets/skybox/right.jpg",
                  "assets/skybox/front.jpg",
                  "assets/skybox/back.jpg");

    //set the FPS limit (Base Limit = 60)
    glgeSetMaxFPS(70);

    //the binding of an display function is here optional, because an default function is allready created behind the scens. But it is recomended if something
    //like drawing an object should be used
    glgeBindDisplayFunc(display);
    //then, the tick function is bound. It is like the main loop of the program and called once every tick. Before it is called, all values behind the
    //scenes are updated. 
    glgeBindMainFunc(tick);
    //bind a function that will be called if the window is resized
    glgeBindOnWindowResizeFunc(windowResized);

    //a window is resizable by default, so no function is needed to say that it can be resized (but you can say that it can't be resized)

    //a window can be moved by default, so no function is needed to say that it can be moved (but you can say that it can't be moved)

    //the camera is bound before the objects are set up, because else there would be an memory access error
    camera = Camera(90, 0.1,1000, Transform(vec3(0,0,-3),vec3(0,0,0),1));
    //then, the camera is moved one unit up
    camera.move(0,1,0);
    //after the camera is set up, the camera is bound to the library using an pointer. So, the camera can be changed and updated
    //without needing to rebind it. 
    glgeBindCamera(&camera);

    //then, the floor is set up. See above for greater detail
    floorSetup();
    //the cube is set up, like the floor is set up
    cubeSetup();
    //setup the thing by loading it form an .obj file
    thingSetup();
    //setup the enterprise object
    enterpriseSetup();
    //setup the wall
    wallSetup();

    l2 = Light(2,5,0, 0.35,0.125,0.5, 50);
    glgeAddGlobalLighSource(&l2);

    light = Light(10,5,0, 1,1,1, 250);
    glgeAddGlobalLighSource(&light);

    //disable the mouse pointer
    glgeSetCursor(GLGE_CURSOR_STYLE_NONE);
    //position the pointer in the middle of the window
    glgeWarpPointer(0,0);

    //at the end of the function, the main loop is run. This is the point where the program will start. No code behind this line will be run. 
    glgeRunMainLoop();

    //the program will continue running after closing, until the main file finishes
    printf("\n");
}