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
#include "GLGE/GLGEWindow.h"
#include <ctime>
#include <math.h>

#include "GLGE/glgeVars.hpp"

//store the pointer for seconds
Object2D clockPointerSec;
//store the pointer for minutes
Object2D clockPointerMin;
//store the pointer for hours
Object2D clockPointerHour;

GLGEWindow win;

void widgetDraw()
{
    //draw the seconds pointer
    clockPointerSec.draw();
    //draw the clock pointer
    clockPointerMin.draw();
    //draw the hour pointer
    clockPointerHour.draw();
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
    clockPointerSec.setRotation(sec * -360.f);
    //rotate the pointer to the current minute
    clockPointerMin.setRotation(min * -360.f);
    //rotate the pointer to the current hour
    clockPointerHour.setRotation(hour * -360.f);

    //update the clock pointers
    clockPointerSec.update();
    clockPointerMin.update();
    clockPointerHour.update();
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

    glgeWindows[glgeMainWindowIndex]->makeCurrent();

    //create the pointer mesh for seconds
    Mesh2D m = createPointerMesh(0.05, 1, vec3(1,0,0));
    //setup the pointer for seconds
    clockPointerSec = Object2D(m.vertices, m.indices);
    //create the pointer mesh for minutes
    m = createPointerMesh(0.1, 1, vec3(1));
    //setup the pointer for minutes
    clockPointerMin = Object2D(m.vertices, m.indices);
    //create the pointer mesh for hours
    m = createPointerMesh(0.2, 0.8, vec3(0));
    //setup the pointer for hours
    clockPointerHour = Object2D(m.vertices, m.indices);

    win = GLGEWindow("Hallo Welt!", 250,250);
    win.setClearColor(1,1,1);
    win.start();

    //move the window to the top right of the screen
    glgeSetWindowPosition(vec2(glgeGetScreenSize().x-glgeGetWindowSize().x + 5, glgeGetWindowPosition().y + 29.f));
    //bind the main tick function
    glgeBindMainFunc(widgetTick);
    //bind the drawing function
    glgeBindDisplayFunc(widgetDraw);
    //run the program
    glgeRunMainLoop();
    //close the custom window
    win.close();
}