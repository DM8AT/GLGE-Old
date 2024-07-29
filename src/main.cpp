/**
 * @file main.cpp
 * @author DM8AT
 * @brief This file contains an example of how a GLGE project could look. It demonstrates the core features and 
 * shows the basics on how to use GLGE. 
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

/**
 * @brief GLGEALL.h is a wraper file that includes all core featuers of GLGE. 
 */
#include "GLGE/GLGEALL.h"
#include "GLGE/GLGESound/glgeSoundCore.h"

/**
 * @brief this stores a pointer to the main application window
 */
Window* mainWin;
/**
 * @brief store the main camera
 */
Camera* cam;
/**
 * @brief store the planet in the center of the map
 */
Object* planet;
/**
 * @brief store the object that is the planet's core
 */
Object* core;
/**
 * @brief store the particle system for the astroid belt
 */
ParticleSystem* astroids;
/**
 * @brief store if the window is selected
 */
bool active = true;
/**
 * @brief store the listener wich can listen to sounds
 * @warning Cameras and Listeners are different, but in a sence the same. Trugh a camera you can see, 
 * but not hear. Trugh a listener you can hear, but you can't see. For the whole you need a listener and a camera. 
 */
Listener* listener;
/**
 * @brief a speaker is a sound source wich can be used in 2D and 3D enviroments. 
 */
Speaker* speaker;
/**
 * @brief store the speed the player is flying with
 */
float flightSpeed = 0.4;
/**
 * @brief store a 2D object to indicate the flight speed
 */
Object2D* speedIndicator;

///////////////////
// DATA FOR MAIN //
///////////////////

/**
 * @brief draw all objects for the main window
 */
void mainDraw()
{
    /**
     * @brief draw the speed indicator
     */
    speedIndicator->draw();
    /**
     * @brief draw the planet
     */
    planet->draw();
    /**
     * @brief draw the instaced astroid cubes
     */
    astroids->draw();
    /**
     * @brief draw the planet's core
     */
    core->draw();
}

/**
 * @brief update everything for the main window
 */
void mainTick()
{
    /**
     * @brief check if the window is focused
     */
    if (mainWin->isFocused())
    {
        /**
         * @brief Check if the window is currently active
         */
        if (active)
        {
            /**
             * @brief calculate the difference the mouse has moved from the window center
             */
            vec2 delta = (vec2(0.5) - glgeGetMouse().pos) * 0.5;
            /**
             * @brief rotate the camera by a fraction of that amount
             */
            cam->rotate(delta.y,delta.x,0);
            /**
             * @brief position the mouse in the center of the window
             */
            glgeWarpPointer(vec2(0));
            /**
             * @brief Hide the cursro
             */
            glgeSetCursor(GLGE_CURSOR_STYLE_NONE);

            /**
             * @brief calculate the amount to move this tick
             */
            float speed = flightSpeed * glgeGetDeltaTime();
            /**
             * @brief if the w key was pressed, move the camera forward (towards -z)
             */
            if (glgeGetKeys().w) {cam->move(-speed,0,0);}
            /**
             * @brief if the s key was pressed, move the camera backwards (towards +z)
             */
            if (glgeGetKeys().s) {cam->move(speed,0,0);}
            /**
             * @brief rotate the camera 90 degrees
             */
            cam->rotate(0,glgeToRadians(90),0);
            /**
             * @brief if it d key was pressed, move the camera right (towards +x)
             */
            if (glgeGetKeys().d) {cam->move(speed,0,0);}
            /**
             * @brief if the s key was pressed, move the camera left (towards -x)
             */
            if (glgeGetKeys().a) {cam->move(-speed,0,0);}
            /**
             * @brief rotate the camera back to normal orientation
             */
            cam->rotate(0,-glgeToRadians(90),0);
            /**
             * @brief if the space key was presed, move the camera up (towards +y)
             */
            if (glgeGetKeys().space) {cam->move(0,speed,0);}
            /**
             * @brief if the shift key was pressed, move the camera down (towards -y)
             */
            if (glgeGetKeys().leftShift) {cam->move(0,-speed,0);}
            /**
             * @brief if the F11 key is pressed
             */
            if (glgeGetKeys().f11)
            {
                /**
                 * @brief toggle the fullscreen mode
                 */
                glgeToggleFullscreen();
                /**
                * @brief position the mouse in the center of the window
                */
                glgeWarpPointer(vec2(0));
            }
            //check if the mouse wheel was scrolled
            if (glgeGetMouse().mouseWheel != 0)
            {
                //increment the flight speed by a portion of the scrolled wheel amount
                flightSpeed += glgeGetMouse().mouseWheel / 5.f;
                //clamp the new flight speed value
                flightSpeed = glgeClamp(flightSpeed,0.f,10.f);
                //update the speed
                speedIndicator->getShader()->setCustomFloat("speed", flightSpeed);
                //check if a sound is playing
                if (glgeGetCurrentSoundCount() == 0)
                {
                    //if not, start a sound with the pitch as the interpolated new speed
                    glgePlaySound("assets/speedup.wav", flightSpeed / 10.f);
                }
            }
        }
        else
        {
            /**
             * @brief set the cursor to the default
             */
            glgeSetCursor(GLGE_CURSOR_STYLE_DEFAULT);
        }
        /**
         * @brief check if the e key is pressed
         */
        if (glgeGetToggledKeys().e)
        {
            /**
             * @brief toggle the active
             */
            active = !active;
            /**
             * @brief position the mouse in the center of the window
             */
            glgeWarpPointer(vec2(0));
        }
    }

    /**
     * @brief repeatetly position the listener at the camera to keep both togeter
     */
    listener->setTransform(cam->getTransform());
    /**
     * @brief always play the sound so it is looping. 
     * If the sound should reset, the parameter 'force' in the play function can be set to true, wich 
     * will restart the sound. 
     */
    speaker->play();
    /**
     * @brief update the camera
     */
    cam->update();
    /**
     * @brief rotate the planet
     */
    planet->rotate(0,glgeToRadians(0.001) * glgeGetDeltaTime(),0);
    /**
     * @brief update the planet
     */
    planet->update();
    /**
     * @brief update the planet's core
     */
    core->update();
    /**
     * @brief rotate the astroid belt a bit
     */
    astroids->setRot(astroids->getRot() + vec3(0,glgeToRadians(0.0003) * glgeGetDeltaTime(),0));
    /**
     * @brief update the astroids (this will run the controll shader if one is bound)
     */
    astroids->update();
    /**
     * @brief update the speed indicator
     */
    speedIndicator->update();
    /**
     * @brief tick the sound core (do this once per frame)
     */
    glgeTickSounds();
}

void initMain()
{
    /**
     * @brief specify the function that should be used to update the main window
     */
    glgeBindMainFunc(mainTick);
    /**
     * @brief specify the function that should be used to draw the contents of the main window
     */
    glgeBindDisplayFunc(mainDraw);

    /**
     * @brief create a new camera
     */
    cam = new Camera(glgeToRadians(90), 0.1, 10000.f, Transform(vec3(1000), vec3(0,-glgeToRadians(90),0), vec3(0)));
    /**
     * @brief bind the camera as the active camera for GLGE
     */
    glgeBindCamera(cam);

    /**
     * @brief create a new listener with the same transformation as the camera
     */
    listener = new Listener(cam->getTransform());
    /**
     * @brief create a new sound source that plays an example sound
     */
    speaker = new Speaker("assets/firework.wav", vec3(0));
    /**
     * @brief increase the gain of the source so it is louder
     */
    speaker->setGain(100.f);

    /**
     * @brief create a new object with a new mesh using the base mesh of a UV sphere
     */
    planet = new Object(new Mesh(GLGE_PRESET_SPHERE, vec4(-1)), Transform(vec3(0,-1,0),vec3(0),vec3(1000)));
    /**
     * @brief set the material of the plane to a new material with a texture
     */
    planet->setMaterial(new Material("assets/grass.png", 0.6));

    /**
     * @brief create the core
     */
    core = new Object(new Mesh(GLGE_PRESET_SPHERE, vec4(-1)), Transform(vec3(0),vec3(0),vec3(10)));
    /**
     * @brief create a new material for the core
     */
    core->setMaterial(new Material(vec4(0,0,1,0.6), 0.2));
    /**
     * @brief say that the core is transparent
     */
    core->setTransparency(true);
    /**
     * @brief say that the core is fully transparent
     */
    core->setFullyTransparent(true);
    /**
     * @brief bind the default transparent shader to the object
     */
    core->setShader(mainWin->getDefault3DTransparentShader());

    /**
     * @brief set a skybox using 6 planes that form a cube
     */
    glgeSetSkybox("assets/Sky.png",
                  "assets/Sky.png",
                  "assets/Sky.png",
                  "assets/Sky.png",
                  "assets/Sky.png",
                  "assets/Sky.png");
    /**
     * @brief Create and add a new light source
     */
    glgeAddGlobalLighSource(new Light(Light(vec3(0), vec3(0,-1,0.5), GLGE_LIGHT_SOURCE_TYPE_DIRECTIONAL, vec3(0.98, 0.98, 0.6), 0, 3)));

    glgeSetPostProsessingShader("src/postProcessing.fs");

    /**
     * @brief create a new particle system
     */
    astroids = new  ParticleSystem(new Mesh(GLGE_PRESET_CUBE, vec4(-1), 0), 500000);
    /**
     * @brief set the controll shader for the particle system (this is not really required, else the particles will just do nothing)
     */
    astroids->setControllShader("src/Controll.cs");
    /**
     * @brief set the material for the particle system
     */
    astroids->setMaterial(new Material("assets/cubeTexture.png"));

    /**
     * @brief create the speed indicator object
     */
    speedIndicator = new Object2D(GLGE_PRESET_SQUARE, vec4(-1), 0, Transform2D(vec2(-0.65f,-0.9),0,vec2(0.3,0.05)), true);
    /**
     * @brief create a custom shader for the speed indicator
     */
    speedIndicator->setShader(new Shader(GLGE_DEFAULT_2D_VERTEX, "src/speedIndicator.fs"));
    /**
     * @brief add a custom float to the shader
     */
    speedIndicator->getShader()->setCustomFloat("speed", flightSpeed);
    /**
     * @brief recalculate the uniforms of the speed indicator shader
     */
    speedIndicator->getShader()->recalculateUniforms();
}

////////////////////
// DATA FOR DEBUG //
////////////////////

/**
 * @brief this stores a pointer to the window used to display the current FPS and some debug data
 */
Window* debugWin;
/**
 * @brief store a 2D text object used to displaying the current fps
 */
Text* d_fps;
/**
 * @brief store a 2D text object used to display the amount of draw calls
 */
Text* d_drawCalls;
/**
 * @brief store a 2D text object used to display the amount of triangels drawn
 */
Text* d_triCount;
/**
 * @brief store if the window was focused the last tick
 */
bool d_focusedLast = false;
/**
 * @brief store old mouse cursor position
 */
vec2 d_oldMousePos = vec2(0);
/**
 * @brief say how many samples to take to smooth out the FPS counter
 */
#define FPS_ACCUMULATION_COUNT 32
/**
 * @brief store the fps samples
 */
float d_fpsAccum = 0.f;
/**
 * @brief store the current sample index for smoothing the fps
 */
int d_fpsSample = 0;

/**
 * @brief this function is responsible for drawing all objects used in the deubg window
 */
void debugDisplay()
{
    /**
     * @brief draw the fps display
     */
    d_fps->draw();
    /**
     * @brief draw the draw call display
     */
    d_drawCalls->draw();
    /**
     * @brief draw the triangle count display
     */
    d_triCount->draw();
}

/**
 * @brief this function is responsible for updating all objects used in the debug window
 */
void debugMain()
{
    /**
     * @brief increase the accumulation buffer by the current fps
     */
    d_fpsAccum += glgeGetCurrentFPS();
    /**
     * @brief increase the amount of taken fps samples
     */
    d_fpsSample++;
    /**
     * @brief check if the samples have reached the limit
     */
    if (d_fpsSample == FPS_ACCUMULATION_COUNT)
    {
        /**
         * @brief update the text with the average of all the samples
         */
        d_fps->setText(std::string("FPS : ") + std::to_string((int)(d_fpsAccum / (float)FPS_ACCUMULATION_COUNT)));
        /**
         * @brief reset the sample count and accumulation buffer
         */
        d_fpsSample = 0; d_fpsAccum = 0;

        //Only update the rest if the FPS are updated, writing a lot of textures to the GPU every frame isn't good

        /**
         * @brief update the text to display the amount of draw calls
         */
        d_drawCalls->setText(std::string("Draw Calls : " + std::to_string(glgeDebugGetDrawCallCount())));
        /**
         * @brief update the text to display the amount of triangles drawn
         */
        d_triCount->setText(std::string("Tries drawn : " + std::to_string(glgeDebugGetDrawnTriangleCount())));
    }
    /**
     * @brief update the fps display
     */
    d_fps->update();
    /**
     * @brief update the text
     */
    d_drawCalls->update();
    /**
     * @brief update the text
     */
    d_triCount->update();
    /**
     * @brief check if the window is focused
     */
    if (debugWin->isFocused())
    {
        /**
         * @brief check if the left mouse button is pressed
         */
        if (glgeGetMouse().leftButton)
        {
            /**
             * @brief set the cursor style to 4 arrows
             */
            glgeSetCursor(GLGE_CURSOR_STYLE_FOUR_ARROWS);
            /**
             * @brief change the window position by the difference of the mouse position to the last tick
             */
            debugWin->setPos(debugWin->getPos() + (glgeGetMouse().screenPosPixel - d_oldMousePos), true);
        }
        else
        {
            /**
             * @brief set the cursor style to the default
             */
            glgeSetCursor(GLGE_CURSOR_STYLE_DEFAULT);
        }
    }
    /**
     * @brief record the mouse position
     */
    d_oldMousePos = glgeGetMouse().screenPosPixel;
}

/**
 * @brief initalise the secondary window used to display debug data
 */
void initDebugWindow()
{
    /**
     * @brief Enable the gathering of debug information
     */
    glgeSetDebugGathering(true);
    //////////////////
    // WINDOW SETUP //
    //////////////////
    /**
     * @brief store the size of the debug window
     */
    vec2 debugWindowSize = vec2(150,75);
    /**
     * @brief create a new window for the debug window and position it on the top right.
     */
    debugWin = new Window("Loading...",debugWindowSize, glgeGetScreenSize().x - debugWindowSize.x, 0);
    /**
     * @brief start the window. This will tell GLGE to start applying the main and display functions of this window during
     * updating and drawing. 
     * @warning a window can't be started twice, so make sure this only applies once!
     * @warning make this before anly graphic api dependend calls are made for this window, else there will be an error
     */
    debugWin->start();
    /**
     * @brief hide the window border
     */
    debugWin->setBorderVisible(false);
    /**
     * @brief forbind the window to be moved
     */
    debugWin->setMovable(false);
    /**
     * @brief forbind the resizing of the winow
     */
    debugWin->setResizable(false);
    /**
     * @brief say that the window should always be displayed on top
     */
    debugWin->setWindowAlwaysOnTop(true);
    /**
     * @brief create a new render pipeline to optimise the rendering of the debug window. 
     * This step is not strictly necessery but makes things a tiny bit faster. 
     * A render pipeline consists of stages, wich each have a name, the first parameter, and a pass, 
     * the second parameter. The name isn't used by GLGE but can be used to identify the current stage. 
     * The pass is what defines what should be done durring the stage. Every stage can have a function 
     * that executes before or after the pass is executed. These functions are passed with function pointers. 
     * Leave them as NULL for no execution. 
     */
    RenderPipeline* debugPipeline = new RenderPipeline({
        /**
         * @brief this pass is used to dictate when it is time to clear the buffer used for lighing. 
         * @warning in a lot of cases having a clear-pass is wanted, but it is not strictly nececerry!
         */
        Stage{"Debug window : clear", GLGE_PASS_CLEAR_G_BUFFER},
        /**
         * @brief this is the pass that draws all geometry identifyed as solid
         */
        Stage{"Debug window : draw solid", GLGE_PASS_DRAW_SOLID},
        /**
         * @brief this pass copys the data from the buffer used for lighting into the post processing buffer, which is then 
         * shown on the screen. To select the color attachment, add the specific color attachment define to the 
         * pass define. 
         */
        Stage{"Debug window : copy to screen", GLGE_PASS_COPY_G_TO_PP + GLGE_G_BUFF_COLOR_ATTACHMENT_ALBEDO}
    });
    /**
     * @brief bind the custom render pipeline object to the window. This passes the ownership of the render pipeline
     * to the window
     */
    debugWin->setRenderPipeline(debugPipeline);
    /**
     * @brief this changes the color used for clearing the screen
     * this can be used to change the background color. The format
     * of the inputed color can be change. By default it is a float 
     * in range from 0 to 1
     */
    debugWin->setClearColor(0.75,0.75,0.75);
    /**
     * @brief the tick / main function is a function that is used while updating the 
     * window. It is called once per tick. 
     */
    debugWin->setTickFunc(debugMain);
    /**
     * @brief the draw / display function is responcible for drawing all the objects. 
     * It can be called multiple times per tick. 
     */
    debugWin->setDrawFunc(debugDisplay);

    /////////////////////
    // OBJECT CREATION //
    /////////////////////

    /**
     * @brief store the path to the used font
     */
    const char* fontPath = "assets/FreeSerif.ttf";

    /**
     * @brief create the text to display the current fps
     */
    d_fps = new Text(" ", fontPath);
    /**
     * @brief set the position for the text
     */
    d_fps->setPos(-1.9,1);
    /**
     * @brief change the scaling of the text
     */
    d_fps->setScale(vec2(0.3));

    /**
     * @brief create the 2D text for displaying the amount of draw calls
     */
    d_drawCalls = new Text(" ", fontPath, vec4(0,0,0,1),30,Transform2D(vec2(-1.9,0.7),0,vec2(0.3)));
    /**
     * @brief create the 2D text for displaying the amount of triangles drawn
     */
    d_triCount = new Text(" ", fontPath, vec4(0,0,0,1),30,Transform2D(vec2(-1.9,0.4),0,vec2(0.3)));
}

int main()
{
    /**
     * @brief initalise GLGE
     * @warning always run this at the start of your project
     */
    glgeInit();
    /**
     * @brief create a new window
     * In the project structure i'm using it is stored in a pointer, but that isn't strictly necessary. 
     * Just make sure that it dosn't run out of scope, else it will close. 
     */
    mainWin = new Window("Loading...", 1000,1000);

    /**
     * @brief Bind a main window to GLGE
     * This is the window any window-dependend GLGE comand will refer to and the window that dictates the program finish
     * if not declared otherwise
     * @warning this will start the window. Make sure that it is not started first!
     */
    glgeBindMainWindow(mainWin);
    /**
     * @brief Open a conection to the default audio device. This is like initalising the sound core. 
     * It is required before constructing any sound-dependend structured, else there will be an error. 
     */
    glgeOpenDevice();
    /**
     * @brief This function initalises the 2D-functionality of GLGE
     * @warning This function will only work after a main window is bound
     */
    glgeInit2DCore();
    /**
     * @brief This function initalises the 3D-functionality of GLGE
     * @warning This function will only work after a main window is bound
     */
    glgeInit3DCore();
    /**
     * @brief initalise the main window
     */
    initMain();
    /**
     * @brief initalise the debug window. THIS IS NOT A GLGE FUNCTION!
     */
    initDebugWindow();

    /**
     * @brief change the title of the main window
     */
    glgeSetWindowTitle("GLGE example application");
    /**
     * @brief change the name of the debug window
     */
    debugWin->rename("GLGE debug data");
    /**
     * @brief explicitly focus the main window
     */
    glgeFocusWindow(true);
    /**
     * @brief position the mouse pointer in the middle of the main window
     */
    glgeWarpPointer(vec2(0));

    /**
     * @brief Run the GLGE main loop. This dictates the finishing of the initalisation and will start to update and draw all
     * started windows. If no main function and display function was bound to the window, custom code execution will be 
     * skipped, but the program will still work. 
     */
    glgeRunMainLoop();

    return 0;
}