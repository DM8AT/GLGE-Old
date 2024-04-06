/**
 * @file WidgetTest.cpp
 * @author DM8AT
 * @brief an example widget that displays an digital analog clock
 * @version 0.1
 * @date 2023-12-16
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

#include "WidgetTest.hpp"
#include "GLGE/GLGEALL.h"
#include <ctime>
#include <math.h>

//store the pointer for seconds
Object2D* clockPointerSec;
//store the pointer for minutes
Object2D* clockPointerMin;
//store the pointer for hours
Object2D* clockPointerHour;

//store the default triangle
Object2D* tri;
//store a spinning cube
Object* spinCube;
//store an close button
Button* button;
//store the Camera
Camera camSecWin;
//store a second window
Window win;
//store a window for a compute shader example
Window compute;
//store a texture for the compute shader
Texture* compOutTexture;
//store the compute shader
ComputeShader compShader;

void widgetDraw()
{
    //draw the seconds pointer
    clockPointerSec->draw();
    //draw the clock pointer
    clockPointerMin->draw();
    //draw the hour pointer
    clockPointerHour->draw();
}

void widgetTick()
{
    //check if the F4 key is pressed
    if (glgeGetKeys().f4)
    {
        //if it is, close the window
        glgeCloseWindow();
    }

    //calculate the current second
    float sec = floor(((float)(time(0)%60)))/60.f;
    //calculate the current minute
    float min = floor(((float)(time(0)%3600))/60.f)/60.f;
    //calculate the current hour
    float hour = floor(((float)(time(0)%(3600*12)))/3600.f)/12.f + 1.f/12.f;
    //rotate the pointer to the current second
    clockPointerSec->setRotation(sec * -360.f);
    //rotate the pointer to the current minute
    clockPointerMin->setRotation(min * -360.f);
    //rotate the pointer to the current hour
    clockPointerHour->setRotation(hour * -360.f);

    //update the clock pointers
    clockPointerSec->update();
    clockPointerMin->update();
    clockPointerHour->update();
}

/**
 * @brief Create a clock pointer
 * 
 * @param w the width of the pointer
 * @param h the height of the pointer
 * @return Mesh2D the finished pointer mesh
 */
Mesh2D createPointerMesh(float w, float h, vec3 col)
{
    //create an rectangle that covers the whole screen
    std::vector<Vertex2D> pointerVerts = 
    {
        //Coords
        Vertex2D(w/2.f,  h    , vec4(col, 1)),
        Vertex2D(w/2.f,  0.f  , vec4(col, 1)),
        Vertex2D(-w/2.f, h    , vec4(col, 1)),
        Vertex2D(-w/2.f, 0.f  , vec4(col, 1)),
    };

    //create the indices for the rectangle
    std::vector<unsigned int> pointerInds = {
        0,2,1,
        1,2,3
    };

    return Mesh2D(pointerVerts, pointerInds);
}

void secondDraw()
{
    //draw the triangle to the other window
    tri->draw();
    //draw the cube
    spinCube->draw();
    //draw the button
    button->draw();
}

void secondTick()
{
    //update the camera
    camSecWin.update();
    //recalculate the projection matrix
    camSecWin.recalculateProjection();
    //update the triangle
    tri->update();
    //rotate the cube a bit
    spinCube->rotate(vec3(0.002, 0.001, 0.0005));
    //update the cube
    spinCube->update();
    //update the button
    button->update();

    //check if the button was pressed this tick
    if (button->clickThisTick())
    {
        //print a chat message
        printf("[GLGE INFO] You clicked the button\n");
    }
}

void secWindowInit()
{
    //create the camera
    camSecWin = Camera(90);
    //bind the camera
    win.setCamera(&camSecWin);

    //set the triangle to an 2D object created out of the vertices scaled down by 0.5
    tri = new Object2D(GLGE_PRESET_TRIANGLE, 0, GLGE_PRESET_USE_SPECIAL, Transform2D(0.75,0.75,0,vec2(0.25,0.25)));

    //load the Cube object
    spinCube = new Object(GLGE_PRESET_CUBE, vec4(-1), 0, Transform(vec3(0,0,1), vec3(0,0,0), vec3(0.25)));
    //set the material for the cube
    Material mat = Material("assets/cubeTexture.png", GLGE_TEXTURE, 0);
    //make the cube unlit
    mat.setLit(false);
    //bind the material to the cube
    spinCube->setMaterial(mat);

    //load the close button
    button = new Button("assets/GLGEImage.png", Transform2D(-0.75,-0.75,0,vec2(0.25,0.25)));

    //bind a draw function to the window
    win.setDrawFunc(secondDraw);
    //bind a tick function to the window
    win.setTickFunc(secondTick);
    //set the window icon
    win.setWindowIcon("assets/GLGEImage.png");
}

void computeDraw()
{
    //stop if this is the transparent pass
    if (glgeGetTransparencyPass()) { return; }
    //bind the texture
    compOutTexture->bind(0, GLGE_TEXTURE_BIND_IMAGE_UNIT);
    //update the time, divide it by 1000 so it dosn't go too fast
    compShader.setCustomFloat("t", glgeGetDeltaTime() / 1000.f, GLGE_MODE_ADD);
    //run the compute shader
    compShader.dispatch(vec3(compute.getSize().x, compute.getSize().y, 1));
    //draw the texture
    compOutTexture->draw();
}

void computeClose()
{
    //store a screenshot of the compute shader generated image
    compOutTexture->storeImage("test.png");
    //delete the texture
    delete compOutTexture;
}

void computeSetup()
{
    //create a new window for the compute shader example
    compute = Window("Compute Example", 512,512, glgeGetScreenSize()-vec2(512));
    //start the window so the window will be able to be drawn
    compute.start();
    //disable resizing so the amount of pixels needed to compute dosn't change
    compute.setResizable(false);
    //create a new texture to render into
    compOutTexture = new Texture(compute.getSize(), GLGE_TEX_RGBA32);
    //create the compute shader from an source file
    compShader = ComputeShader("src/Shaders/computeExample.comp");
    //add a variable for the time
    compShader.setCustomFloat("t", 0);
    //update the uniform bindings
    compShader.recalculateUniforms();
    //bind a draw function for the window
    compute.setDrawFunc(computeDraw);
    //bind an exit function
    compute.setExitFunc(computeClose);
    //a tick function is not needed for the window, so don't bind one
}

void closeWin()
{
    //delete the 3D object
    delete spinCube;
}

//start of the program
void runWidgetExample()
{
    //initalise GLGE
    glgeInit();
    //say that the window shouldn't be added to the task bar
    glgeAddWindowFlag(GLGE_WINDOW_FLAG_SKIP_TASKBAR);
    //create the glge window
    glgeCreateWindow("Widget", 150,150);
    //initalise the 2D core
    glgeInit2DCore();
    //set the bg color for the window
    glgeSetClearColor(vec4(vec3(0.25),1));
    //say that the window always renders on top
    glgeSetWindowAlwaysOnTop(true);
    //hide the window border
    glgeSetWindowBorderVisible(false);
    //say that default window close events get ignored
    glgeSetWindowForceOpen(true);
    //diable window resizing
    glgeSetWindowResizable(false);

    //create the pointer mesh for seconds
    Mesh2D m = createPointerMesh(0.05, 1, vec3(1,0,0));
    //setup the pointer for seconds
    clockPointerSec = new Object2D(m.vertices, m.indices);
    //create the pointer mesh for minutes
    m = createPointerMesh(0.1, 1, vec3(1));
    //setup the pointer for minutes
    clockPointerMin = new Object2D(m.vertices, m.indices);
    //create the pointer mesh for hours
    m = createPointerMesh(0.2, 0.8, vec3(0));
    //setup the pointer for hours
    clockPointerHour = new Object2D(m.vertices, m.indices);

    //move the window to the top right of the screen
    glgeSetWindowPosition(vec2(glgeGetScreenSize().x-glgeGetWindowSize().x + 5, glgeGetWindowPosition().y + 29.f));
    //bind the main tick function
    glgeBindMainFunc(widgetTick);
    //bind the drawing function
    glgeBindDisplayFunc(widgetDraw);

    //create the second window
    win = Window("Hello World!", 250,250);
    //set the clear color of the window to white
    win.setClearColor(1,1,1);
    //bind an initalisation function
    win.setInitFunc(secWindowInit);
    //bind an exit function
    win.setExitFunc(closeWin);
    //mark the window as ready for updates
    win.start();
    
    //generate a window for the compute shader example
    computeSetup();

    //say that GLGE should exit once the main window closes
    glgeSetExitOnMainWindowClose(true);
    //switch the focus to the main window
    glgeFocusWindow();

    //run the program
    glgeRunMainLoop();
    //close the custom window
    win.close();
}