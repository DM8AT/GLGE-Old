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
Camera* camera;

//create an instance of the Object class to store an object named cube. Default constructor is used, setup is done later
Object* cube;
//crate an second instance of the Object class, also only default constructor
Object* grassFloor;
//create an instance of the Object class to store an object named thing. The default constructor is used. Named so, because it can be everything
//that can be loaded from an .obj file
Object* thing;
//this object is loaded from the file Enterpreis.obj. 
Object* enterprise;

Light* light;

Light* l2;

Light* spot;
Shader* invColPPS;

//store the first render target for the ping-pong blure
RenderTarget* pingPongBlure_first;
//store the second render target for the ping-pong blure
RenderTarget* pingPongBlure_second;
//store a shader for the final bloom calculation
Shader* bloomShader;
//store a shader for down sampeling
Shader* downSample;
//store a shader for up sampeling
Shader* upSample;

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
    //backface culling dosn't look good on transparent objects
    glgeDisableBackfaceCulling();
    //draw the Enterpreis
    enterprise->draw();
    //re-enable backface culling for the rest of the objects
    glgeEnableBackfaceCulling();

    //call the draw function from the grass floor object to draw it to the screen
    grassFloor->draw();
    //draw the cube to the screen
    cube->draw();

    //draw the thing to the screen
    thing->draw();

    //after this function is finished, the frame buffer gets fliped and everything is drawn to the screen
}

//the tick function is called every tick. It should contain thinks like updates, but no draw calls. 
void tick()
{
    //dont update this window if it is not focused
    if (!glgeGetWindowFocus()) { return; }
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
        camera->move(0,0,camTickSpeed);
    }
    //check if the s key is pressed
    if (glgeGetKeys().s)
    {
        //if the s key is pressed, then the camera should move backwards
        //it is basicaly the same as moving it forwards, the speed is just inverted
        camera->move(0,0,-camTickSpeed);
    }
    //store the camera angle
    vec3 rot = camera->getRotation();
    //temporeraly rotate the camera 90 degrees right
    camera->rotate(vec2(glgeToRadians(90),0));
    //check if the d key is pressed
    if (glgeGetKeys().d)
    {
        //if the d key is pressed, then the camera should strave right. It is like moving forward, but sine and cosine is inverted and the rotation is no longer inverted
        camera->move(camTickSpeed,0,0);
    }
    //check if the a key is pressed
    if (glgeGetKeys().a)
    {
        //if a is pressed, then the camera should strave right. It is like straving left, but the speed is inverted
        camera->move(-camTickSpeed,0,0);
    }
    //rotate the camera back
    camera->setRotation(rot.x, rot.y);
    //check if the space bar is pressed
    if (glgeGetKeys().space)
    {
        //if the space bar is pressed, the camera flies up with the normal movement speed
        camera->move(0,camTickSpeed,0);
    }
    //check if one of the two shift keys is pressed
    if (glgeGetKeys().shift)
    {
        //if one is pressed, then the camera should fly down. 
        camera->move(0,-camTickSpeed,0);
    }

    //here is the camera rotation
    //if the cursor is locked to the window
    if (isActive)
    {
        //rotate the camera by the mouse position
        camera->rotate((-(glgeGetMouse().pos.x-0.5f)/glgeGetDeltaTime())*mouseSensetivity, (-((glgeGetMouse().pos.y-0.5f))/glgeGetDeltaTime())*mouseSensetivity, 0);
    }
    //clamp the camera rotation on the y axis
    camera->setRotation(camera->getRotation().x, glgeClamp(camera->getRotation().y, glgeToRadians(-90), glgeToRadians(90)));
    
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

    //then, update the camera to make all transform changes work correctly
    camera->update();

    //recalculate the height for the cube obj
    //cube->setPos(0, 1.5 + std::sin(glgeToDegrees(glgeGetCurrentElapsedTime() * 0.00001)) / 2.f, 0);

    //secondly, update the objects, so that 

    //rotate the spot light a bit
    spot->setDir(glgeRotateVector(glgeToRadians(1),vec3(0,1,0),spot->getDir()));

    //update the grass floor, to make shure the transform and projection is correct. 
    grassFloor->update();
    //update the cube
    cube->update();
    //update the thing
    thing->update();
    //update the enterprise
    enterprise->update();

    //set the position of the light source to be exactly at the player
    //light.setPos(camera->getPos());

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
    Mesh* m = new Mesh(vertices, indices, sizeof(vertices), sizeof(indices));
    //the normal vectors of the mesh are recalculated, so it can be lit. 
    m->recalculateNormals();
    //the grass floor is constructed from the previously created mesh by overwriting the data previously stored in it with the new mesh
    grassFloor = new Object(m);

    //an shader can be asigned to the object, but it is no longer necesery. 
    //then create a material for the grass floor
    Material* grassFloorMat = new Material("assets/grass.png", 0.2);
    //apply the material to the grass floor
    grassFloor->setMaterial(grassFloorMat);
}

//in this function the cube is set up
void cubeSetup()
{
    Mesh sphereMesh = Mesh(GLGE_PRESET_SPHERE, vec4(-1));
    sphereMesh.applyTransform(Transform(vec3(0,2,0), vec3(0), vec3(0.5)));
    Mesh* cubeMesh = new Mesh(GLGE_PRESET_CUBE, vec4(-1));
    //join the two meshes together
    cubeMesh->joinThis(sphereMesh);
    //the cube object is constructed using a cube preset using an alpha of -1 for the color to use textures
    cube = new Object(cubeMesh, Transform(vec3(0,1,2),vec3(0,0,0), 1));
    //create a material for the cube
    Material* cubeMaterial = new Material("assets/GLGEImage.png", 0);
    //set it to unlit
    cubeMaterial->setLit(false);

    //apply the material to the cube
    cube->setMaterial(cubeMaterial);
}

//setup the thing
void thingSetup()
{
    //load a mesh from an file        specify the file format to be .obj
    Mesh* m = new Mesh("assets/Vertices.obj", GLGE_OBJ);

    //load the mesh to an object and change the position of it
    thing = new Object(m, Transform(vec3(0,0,-5), vec3(0,0,0), 1));

    //create a material for the thing
    Material* thingMat = new Material("assets/cubeTexture.png", 0.2);
    //apply the material to the thing
    thing->setMaterial(thingMat);
}

void enterpriseSetup()
{
    //load the Enterprise object from an file, the file format is .obj
    enterprise = new Object("assets/Enterpreis.obj", GLGE_OBJ, Transform(vec3(0,10,0), vec3(5,10,6), 1), true);

    //create a material for the object
    Material* mat = new Material(vec4(vec3(0.25), 0.5f), 0.25, 1.0);

    //bind the material to the enterprise
    enterprise->setMaterial(mat);
    //say that the enterpreise is fully transparent
    enterprise->setFullyTransparent(true);
}

void windowResized(int width, int height)
{
    //check if the width is less than 50
    if (width < 50)
    {
        //resize the window to an width of 50
        glgeResizeWindow(50, height);
        //stop this script
        return;
    }
    //check if the height is less than 50
    if (height < 50)
    {
        //resize the window to an height of 50
        glgeResizeWindow(width, 50);
        //stop this script
        return;
    }
    //print debug information
    printf("Window resized to: %d, %d\n", width,height);
}

Shader calculateBloom(unsigned int img)
{
    //specefy the amount of downsamples
    unsigned int samples = 1;
    //make the first setup for the blure using the brightness map
    //set the size to half the size of the brightness map
    pingPongBlure_first->changeSize(glgeGetWindowSize()/vec2(2,2));
    //load the texture from the brightness map to the shader
    downSample->setCustomTexture("image", glgeGetLighningBuffer());
    //pass the size of the brightness map to the shader
    downSample->setCustomVec2("screenSize", glgeGetWindowSize());
    //pass a multiplyer for the texture coordinates to the shader
    downSample->setCustomInt("sampleMult", 2);
    //bind the down sampleing shader to the render target
    pingPongBlure_first->setShader(downSample);
    
    //down sample
    for (unsigned int i = 0; i < samples; i++)
    {
        //draw the first ping-pong blure
        pingPongBlure_first->draw();
        //setup the second ping-pong blure
        //change the size of the render target by dividing the size by 2
        pingPongBlure_second->changeSize(pingPongBlure_first->getSize()/vec2(2,2));
        //pass the image of the other blure pass to the shader
        downSample->setCustomTexture("image", pingPongBlure_first->getTexture());
        //pass the size of the other render target to the shader
        downSample->setCustomVec2("screenSize", pingPongBlure_first->getSize());
        //multiply the texture coordinate multiplyer by 2
        downSample->setCustomInt("sampleMult", 2, GLGE_MODE_MULTIPLY);
        //bind the shader (just to be sure it is correct)
        pingPongBlure_second->setShader(downSample);
        //draw the blure pass
        pingPongBlure_second->draw();

        //setup the blure pass for the first render target
        //set the size to half the size of the other render target
        pingPongBlure_first->changeSize(pingPongBlure_second->getSize()/vec2(2,2));
        //bind the output texture of the other render target
        downSample->setCustomTexture("image", pingPongBlure_second->getTexture());
        //pass the size of the other render target
        downSample->setCustomVec2("screenSize", pingPongBlure_second->getSize());
        //multiply the multiplyer for the texture coordinates by 2
        downSample->setCustomInt("sampleMult", 2, GLGE_MODE_MULTIPLY);
    }
    //reset the value for the sample multiplyer, make sure to divide instantly by 2 because it was multiplied one more time than drawn
    upSample->setCustomInt("sampleMult", downSample->getIntByName("sampleMult")/2);
    //up sampling
    for (unsigned int i = 0; i < samples; i++)
    {
        //set the size for the first render target to two times the size of the second render target
        pingPongBlure_first->changeSize(pingPongBlure_second->getSize().scale(vec2(2,2)));
        //pass the output image of the second render target to the shader
        upSample->setCustomTexture("image", pingPongBlure_second->getTexture());
        //pass the size of the second render target to the shader
        upSample->setCustomVec2("screenSize", pingPongBlure_second->getSize());
        //divide the multiplyer for the texture coordinates by 2
        upSample->setCustomInt("sampleMult", 2, GLGE_MODE_DIVIDE);
        //bind the up sampeling shader to the first render target
        pingPongBlure_first->setShader(upSample);
        //draw the first render target
        pingPongBlure_first->draw();

        //set the size of the second render target to half the size of the first render target
        pingPongBlure_second->changeSize(pingPongBlure_first->getSize().scale(vec2(2,2)));
        //pass the output image of the first render target to the shader
        upSample->setCustomTexture("image", pingPongBlure_first->getTexture());
        //pass the size of the first render target to the shader
        upSample->setCustomVec2("screenSize", pingPongBlure_first->getSize());
        //divide the multiplyer for the texture coordinates by 2
        upSample->setCustomInt("sampleMult", 2, GLGE_MODE_DIVIDE);
        //bind the up sampeling shader to the second render target
        pingPongBlure_second->setShader(upSample);
        //draw the second render target
        pingPongBlure_second->draw();
    }

    //pass the current texture to the bloom shader
    bloomShader->setCustomTexture("currImage", img);
    //recalgulate the uniforms for the bloom shader
    bloomShader->recalculateUniforms();
    //output the bloom shader
    return *bloomShader;
}

//this function is like the main function in an normal scripted, but it is called form an other file, so it is named differently. 
//The inputs are declared in the 3DTest.hpp file to make the function acessable from the main.cpp file. 
void run3Dexample()
{
    //first, the library is initalised. if the library is used, this should alway be done first. 
    glgeInit();

    //as second step, a window is created. This should be done second, because it also initalises the glew library
    glgeCreateWindow("3D example script for GLGE : init...", 1000, 1000);
    //set the interpolation mode to linear
    glgeSetInterpolationMode(GLGE_LINEAR);

    //after creating the window, the 3D core is initalised. This sets all thinks like depth buffer correctly
    glgeInit3DCore();

    //set the icon of the window
    glgeSetWindowIcon("assets/GLGEImage.png");

    //Normaly, backface culling is enabled. But it can be disabled using the following function:
    //glgeDisableBackfaceCulling();

    //a lighting shader is bound by default, so binding one is not needed (source: GLGE/glgeDefaultLightingShaderSource.fs)
    glgeSetLightingShader("src/GLGE/glgeDefaultShaders/glgeDefaultLightingShaderSource.fs");

    /////////
    //BLOOM//
    /////////
    //bind a post processing funciton for bloom calculation
    glgeAddCustomPostProcessingFunc(calculateBloom);
    //setup the first render target for the blure
    pingPongBlure_first = new RenderTarget(glgeGetWindowSize());
    //setup the second render target for the blure
    pingPongBlure_second = new RenderTarget(glgeGetWindowSize());
    //setup the bloom shader
    bloomShader = new Shader(GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER, "src/Shaders/finalBloomShader.fs");
    //pass the lit image to the bloom shader
    bloomShader->setCustomTexture("bloomMap", pingPongBlure_first->getTexture());
    //compile the shader for down sampeling
    downSample = new Shader(GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER, "src/Shaders/downSampleShader.fs");
    //set down the image for the down sampeling shader
    downSample->setCustomTexture("image", pingPongBlure_first->getTexture());
    //set down the image size for the down sampeling shader
    downSample->setCustomVec2("screenSize", pingPongBlure_first->getSize());
    //set the uniform for the texture map multiplyer
    downSample->setCustomInt("sampleMult", 1);
    //recalculate the uniform variables
    downSample->recalculateUniforms();
    //compile the shader for up sampeling
    upSample = new Shader(GLGE_DEFAULT_POST_PROCESSING_VERTEX_SHADER, "src/Shaders/upSampleShader.fs");
    //set up the image for the down sampeling shader
    upSample->setCustomTexture("image", pingPongBlure_first->getTexture());
    //set up the image size for the down sampeling shader
    upSample->setCustomVec2("screenSize", pingPongBlure_first->getSize());
    //set the uniform for the texture map multiplyer
    upSample->setCustomInt("sampleMult", 1);
    //recalculate the uniform variables
    upSample->recalculateUniforms();

    //bind another post processing shader
    invColPPS = glgeSetPostProsessingShader("src/Shaders/invertColors.fs");
    //set a uniform float in the shader to controll the strength for the invertion
    invColPPS->setCustomFloat("strength", 1);
    //recalculate the positions of the uniforms
    invColPPS->recalculateUniforms();
    //create post processing shader
    glgeSetPostProsessingShader("src/Shaders/glgeFinalise.fs");
    //add the anit-alisaing shader
    glgeSetPostProsessingShader("src/Shaders/FxAA.fs");

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
    glgeSetMaxFPS(100);

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
    camera = new Camera(90, 0.1,1000, Transform(vec3(0,2,-1),vec3(0,0,0),1));
    //after the camera is set up, the camera is bound to the library using an pointer. So, the camera can be changed and updated
    //without needing to rebind it. 
    glgeBindCamera(camera);

    //then, the floor is set up. See above for greater detail
    floorSetup();
    //the cube is set up, like the floor is set up
    cubeSetup();
    //set the interpolation mode for textures to nearest
    glgeSetInterpolationMode(GLGE_NEAREST);
    //setup the thing by loading it form an .obj file
    thingSetup();
    //set the interpolation mode for textures to linear
    glgeSetInterpolationMode(GLGE_LINEAR);
    //setup the enterprise object
    enterpriseSetup();

    //add and store two point lights
    l2 = new Light(2,5,0, 0.35,0.125,0.5, 100);
    glgeAddGlobalLighSource(l2);
    light = new Light(10,5,0, 1,1,1, 50);
    glgeAddGlobalLighSource(light);

    //add a sun light
    glgeAddGlobalLighSource(new Light(vec3(0), vec3(0,-1,0.5), GLGE_LIGHT_SOURCE_TYPE_DIRECTIONAL, vec3(0.98, 0.98, 0.6), 0, 1));
    //create a spot light (the blue spinny light on the floor)
    spot = new Light(vec3(0,0.1,0), glgeAngleToDir(0,0,0), GLGE_LIGHT_SOURCE_TYPE_SPOT, vec3(0.1,0.1,1), glgeToRadians(45.f), 10000);
    //add the spot light to the light sources
    glgeAddGlobalLighSource(spot);

    //disable the mouse pointer
    glgeSetCursor(GLGE_CURSOR_STYLE_NONE);
    //position the pointer in the middle of the window
    glgeWarpPointer(0,0);

    //at the end of the function, the main loop is run. This is the point where the program will start. No code behind this line will be run. 
    glgeRunMainLoop();
}