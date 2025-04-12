# IMPORTANT NOTE
This is a old version of GLGE. For the newset version, please see [here](https://github.com/DM8AT/GLGE). 

# GLGE
GLGE is a C++ graphics library for creating 2D and 3D graphics. 

## Information about the current developement
I am currently working on a major re-write of the backend and for this purpos I am developing a new [Class-Wraper around OpenGL](https://github.com/DM8AT/ObjectGL). It may take some time before the update is ready, so please stand patient. The developement is not discontinued. 

## Examples
Example code is provided in the files 2DTest.hpp, 2DTest.cpp, 3DTest.hpp, and 3DTest.cpp. 
The code is commented, with the 3D code outlined to highlight the differences from the 2D code.

## Documentation
--Under Construction--

Documentation is provided via doxygen, with the documentation files found in the "documentation" folder.
All files in the folder are created by doxygen automatically, I haven't changed anything in there. 

## Debugging
If you encounter errors, check the error outputs defined in "GLGE/glgeErrors.hpp". 

Often causes for memory access errors:
- Not binding a camera before creating or updating 3D objects
- inputing wrong values to an Object constructor with pointer arrays

Often causes for GL_INVALIDE_OPERATION:
- Drawing an empty object

## Installation
**Run setup.bash for a simple setup, if you just want to start developing with the default settings**
To install GLGE, download the source code or clone the project. Make sure to install following librarys:
- OpenGL
   - Linux: `sudo apt install libgl-dev`
- GLEW
   - Linux: `sudo apt install libglew-dev`
- SDL2
   - Linux: `sudo apt install libsdl2-dev`
- SDL2_TTF
   - Linux: `sudo apt install libsdl2-ttf-dev`
- OpenAL
   - Linux: `sudo apt install libopenal-dev`
- ALUT
   - Linux: `sudo apt install libalut-dev`
To install all librarys on linux, just run `make install` in the project directory or manualy run `sudo apt install libgl-dev libglew-dev libsdl2-dev libsdl2-ttf-dev libopenal-dev libalut-dev` in a terminal. 

**Superuser access is requred to install the librarys, but not to compile and run any GLGE applications**
   
also make sure that you have C++ 17 or newer installed and that you have a C++ compiler of your choise (the makefile is configured to work with gcc)

## Compiling
In the moment there is only a makefile set up for the GNU C/C++ Compiler. 
### Using the makefile
The makefile in the root directory of this project is set up for the GNU C/C++ under Linux. You can try compiling with the same compiler under another operating system, but I'm not sure if it will work. It defenetly dosn't work with another compiler. You must create **two new directorys** when using the makefile. The first is called **"bin"**. There, the output files like static librarys and executables will be stored. The other is **"build"**. There, all the intermediate object files will be stored. 
#### Compiling the library
To compile GLGE to an static library using the makefile, start a terminal and navigate to the folder GLGE was installed into. Then, run the command `make -j GLGE` to compile the library to an static compiled library. This will create two files called "libGLGE.a" and "libCML.a" in the folder called "bin". A system for shared objects dosn't currently exist. 
#### Using the library
Now, to compile your own project that uses GLGE, you need to add the files "libGLGE.a" and "libCML.a", that where created by the compiler in [the previouse section](https://github.com/DM8AT/GLGE/tree/dev?tab=readme-ov-file#compiling-the-library), to the .o files in the linker. An example could be:
`g++ main.cpp libGLGE.a libCML.a -o main.exe`
#### Compiling the examples
To compile the example scripts included in this repository, run `make -j`. This will create all the static librarys like shown in [this section](https://github.com/DM8AT/GLGE/tree/dev?tab=readme-ov-file#compiling-the-library), so they can be used afterwards. It will also create a file called "main" in the "bin" directory. This is the final executable. To try the examples, run the executable using `./bin/main` from the root directory of the project. **Do not change into the "bin" directory to run the project.**
### Seting up an own compiler
To use your own compiler, compile all cpp files in the directory "src/GLGE/CML" into .o files. Then, link those .o files into a static library. Also, compile all files in "src/GLGE", "src/GLGE/GLGEIndependend" and "src/GLGE/GLGEOpenGL" into .o files and link them to a static library. When compiling your own files, make sure to link the two created static librarys to your project. 

# Upcoming
Upcoming features that may be added to GLGE in the future include:
- Loadable models from files
   - from .blend files
   - from other file formats
- A 3D and 2D sound function to the core
- 2D and 3D physics
- A lighting core for 2D
   - preset for different light sources
   - different light source presets
      - directional light
      - sun
      - light plates (some invisible shape that is emitting light)
- Texture atlassing

# Changelogs
## Update 0.5
- added the "Text" class to handle 2D text displays
   - inherates from the "Object2D" class
- added the "TextInput" class to handle simple text inputs
   - inherates from the "Text" class, wich inherates from the "Object2D" class
- added support for propper transparency for 2D objects
- added a new 'Texture' class to better handle texture objects
   - simple creation from files using stb_image
   - create an empty texture with an specific size
      - data can be passed to the texture as an array to initalise it
- textures can be bound to be accessed in shaders
   - binding to texture units allows acces to the texture using texture samplers
      - use 'GLGE_TEXTURE_BIND_TEXTURE_UNIT' as second argument in the binding function to bind to an texture sampler
   - binding to an image unit allows read and write access in compute shaders through an image2D
      - use 'GLGE_TEXTURE_BIND_IMAGE_UNIT' as second argument in the binding function to bind to an image unit
- made glgeImage usable in the user's scope (no sensetive functions, so it is ok)
   - users can load images using glgeLoadImage
   - users can store images using glgeStoreImage
      - suppoerted formats: PNG, JPG / JPEG, BMP (Windows Bitmap), PPM
   - added a function to convert texture data (format is one vec4 per pixel) to image data (4 uint8_t per pixel)
- added a function to store textures
   - suppoerted formats: PNG, JPG / JPEG, BMP (Windows Bitmap), PPM
- added a new "ComputeShader" class. Compute shaders can now be used in GLGE
- transforms can be applied to an mesh with the new function 'applyTransform'
- meshes can be joind together
   - use '+' or 'join' to get a new joined mesh from two meshes
   - use '+=' or 'joinThis' to join the mesh on the right side to the mesh on the left side
- remade the makefile
   - compiling can now be threaded. Use `-j` when compiling to enable threading. 
   - compiling CML and GLGE to static libraris is now easy. To compile both to static librarys, run `make -j GLGE`. 
- rewrote a part of the README.md file
- transparent objects now correctly opperate with the light shader
- added destructors for 3D objects, 2D objects and Buttons
   - destructors clean up the object
- renamed "GLGEWindow" to "Window", so it better fits the naming scheme of GLGE
- updated the way the shaders access uniforms, now using uniform buffers instead of passing everything all the time, hoping this improves performance
- added simple shadow mapping for spotlights
- updated the way scenes safe, it should be faster now. WARNING: old scenes won't work with the new version
- added custom render pipelines
   - class "RenderPipeline" is used to wrap the order of execution for each element in the render pipeline
      - there can be less or more than one elements of each type
   - class "PostProcessingStack" is used to wrap the information on wich shaders or functions to use for each post processing pass in a render stage
- custom render pipelines can be bound to a window
- each window has its own default render pipeline with the same stages and order as before
- fixed some wierd behavieor of normals when an object was rotated
- added built in support for parallax occlusion mapping to materials
   - depth peeling
   - binary refinement
- added a compute shader based particle system
   - using instancing to be able to draw a lot of objects at a real-time performance
   - particles can be fully lit objects
   - particles have the same transparency behaviour like normal objects
   - create a compute shader to controll the behaviour of every single particle
- changed the behaviour of meshes, meshes are now pointer-based components of objects
   - meshes now store they're own vertex and index buffers
   - when a mesh is created and given to a object, the object now has the ownership of the mesh exept something else is specifyed
- converted all examples into one, see main.cpp
- GLGE now only uses the radians system, use "glgeToRadians" if you wish to use degrees
- all rotate and set rotation functions now use the modulo opperator to enable infinite rotation
- corrected the rotation for the camera, x and y axis where swapped and the camera now uses vec3 instead of vec2 for rotation
- updated the 2D core to the new code structure (now using the reference dependend structure with destructors)
- added a sound core using OpenAL and ALUT
   - the sound core is initalised by opening a device conection
   - added a simple function to play a sound without a position
   - added the 'Listener' class to define the position of a sound listener
      - multiple listeners can exists, but only one can be bound at any time. The bound listener is the currently opperating one and will be updated every time the sound core ist ticked
      - a listener can be positiond in 3D and 2D space to support audio for both spaces
         - using 2D will simply ignore positon along the y-axis and rotation around the x and z axis
   - added the 'Speaker' class to act as a positional audio source
      - a speaker can be positiond in 3D and 2D space like a listener
      - sounds can be played, paused and stopped
   - added functions to handle the functionality of the doppler effect
## Update 0.5-1
- added 2 new light source types:
   - spot light: a light with a direction and angle component
   - directional light: like a sun, direction, but no position
- added a C-Stype #include feature to GLSL shaders
- added default includes for GLSL shaders
## Update 0.5-pre 3
- added presets for 2D and 3D objects
   - now creation of simple geometry is really simple
   - prefix for presets is GLGE_PRESET_
- fixed wierd GLGE warning ("[GLGE WARNING] tried to update an allready an object in a different window" is now "[GLGE WARNING] tried to update an object in a different window")
- added button class for simple buttons
- documentated a lot of old functions
   - CML is now compleatly documentated
- added a new integer type : ivec2, ivec3 and ivec4. Like dvec or vec but with integers not with floats or doubles
- fixed bug where SDL window index wouldn't start at 1, now window offset to SDL window indices is dynamic
- disabled backface culling for enterprise in 3D example
- added infrastructure for different light source types
## Update 0.5-pre 2
- made new folder structure a little bit bedder
- split up a few files into multiple (so if Vulkan gets added in the future, less code needs to be re-written)
## Update 0.5-pre
- compleately restructured the internal source code of GLGE
- new structure (root: GLGE/):
   - CML
      - the math library used by GLGE
   - glgeDefaultShaders
      - uncompiled source code of the GLGE default shaders in human redable form, only for complex shaders
   - GLGEInternal
      - all internal files used by GLGE that are graphics API unspecific
   - GLGEOpenGL
      - all files for GLGE that are dependand on the graphics API
   - files ment to be included by the user
- use "GLGE_USE_VULKAN" to switch between OpenGL and Vulkan implementation
   - currently throws an error because the Vulkan implementation is jet to come
- functions to get if GLGE uses OpenGL or Vulkan
## Update 0.4
- added files:
   - GLGEWindow.h   : declares the window class, private variables of the window class and functions of the window class
   - GLGEWindow.cpp : defines the functions of the window class declared in file "GLGEWindow.h"
- GLGE now runs on an "main window", all window dependend functions are now passed to that window when called
   - the creation of an main window is not needed, nor so handeling it. But to start the main loop, at least one window will need to be created and started. starting a window will mark it redy for executing functions when the main loop is started. Starting windows during the main loop should work too, but I didn't test it. 
- added the window class to manage multiple windows at once in the main loop
- GLGE will now chatch SDL2 errors
- custom windows can use init functions to clearly declare Objects for the window
- objects can only render in the window in whos context they where created

## Update 0.3-1
- an alpha of 1.f will now be renderd solid on transparent objects
- transparent objects will render in the opaque and transparent pass by default now to render solid and transparent parts
- a object can be set to be "fullyTransparent", then it will have the old behaviour. This drasticaly improves performance for only transparent objects. 
## Update 0.3
- added a lot of new window related functions
   - show/hide the window border
   - force the window to staty open even if closing is issued using window close actions | default window closing actions are:
      - the 'x' in the top left of the window border
      - 'alt + F4' on Windows and Linux
   - glge main window can now be closed by code
      - if this closing was wrongly initiated, it can be stopped during the tick it was initiated
   - the glge main window can be always renderd on top of other windows
   - added a function to change the window brightness
      - WARNING: on some operating systems, this effects the compleate screen brightness, if the window is the currently focused one
- glgeGetScreenSize now returns the actual screen size in pixels, not the window size
- added ability to add a few window flags
   - window can be hidden from task bar
   - window can be treated as a popup menu
   - window can be treated as a tooltip
   - window can be treated as an utility window
   - the window should be created in high-DPI mode if supported
      - if it is not supported, no warning nor error is shown
- added a function to get if the window is focused
- added a function to get if the mouse is touching the window
- added a function to capture the mouse inside of the window
- updated install.py to now install into /usr/local/include and /usr/local/lib
## Update 0.2-1
- switched to SDL_SetWindowResizable to allow window resizing
- removed now redundend code for fixing window size
## Update 0.2
- removed misteriouse 1 form 2DTest.cpp
- changed std::endl with \n
- argc and argv are no longer needed to initalise GLGE
- seperated draws to transparent and opaque
- added alpha clipping with an value of 0.5 for opaque objects
- added a fast way to render transparent objects (using Bavoil and Myers method from 2008 that improves uppon Meshkins OIT opperator)
   PRO: 
   - it is really fast
   - allowing deffered rendering for solid geometry
   - only one combine pass for solid and opaque geometry
   CONS:
   - it is not really exact and solid geometry renderd as transparent looks wierd
      - these consequenses are acceptable for the huge performance boost compared to sortet transparency
## Update 0.15-2
- importing a vector of a size > 2 will now include all lower size vectors by default
- added more constructors to vec2, vec3, vec4, dvec2, dvec3, dvec4
   - supporting construction from an vec3 from one vec2 and a float/double
   - supporting construction from an vec4 from one vec3 and a float/double
   - supporting construction from an vec4 from two vec2s
   - supporting construction from an vec2/vec3/vec4 from one float/double
- most changes can be reverted by adding CML_NO_VEC_CASTS before importing the modules
   - constructor for one float will always be created
## Update 0.15-1
- removed OpenGL access from main script
   - OpenGL can be included as an additional library, but it is not needed. The option is still available
- added bloom effect as example in 3DTest.cpp
   - bloom and ping-pong blure will have integrated functions in the future, it is just a showcase
## Update 0.15
- added custom render targets
   - ability to render more complex post processing effects
- added comments for all classes
- removed a few left-overs from the change to SDL2 (in: GLGEKlasses.cpp)
- now performing all framebuffer status checks
- changed from RGBA32F_EXB framebuffers to RGB16F (there should be no visible difference, it is just a bit faster)
- updated upcoming list
## Update 0.14
- switched post processing shaders to use the class "Shader" instead of unsigned int
- support for more than one post-processing shader
- added the ability, to use functions as post-processing shaders
   - return type of function must be an instance of the GLGE "Shader" class
   - input must be one unsigned int to input the main image
- added ability to acess the main image passes
   - albedo buffer
   - normal buffer
   - position buffer
   - roughness buffer
      - red chanel: roughness as float in range 0-1
      - green chanel: metallic as float in range 0-1
      - blue chanel: lit as bool
   - lighning buffer
- added new frame buffer object for the post processing pass
## Update 0.13
- parse custom variables to an shader using a material
   - Supported types:
      - float
      - integer
      - boolean
      - vec2
      - vec3
      - vec4
      - mat2
      - mat3
      - mat4
      - textures
   - execute operations while parsing the uniform, suppoerted opperations:
      - set
      - add
      - subtract
      - multiply
      - divide
      - logical and
      - logical or
      - logical not
      - logical nand
      - logical nor
      - logical xor
      - cross product
- implemented the same functionality for uniforms for materials
- changed internal structure from class "Object" to use the class "Shader" for shader management
- get a pointer to the material used by an 3D object: changed function from returning a material to a material*
- get a pointer to the shader used by an 3D object
### Update 0.12-2
- disabled VSync, custom FPS work again
- added option for unlit materials as a material property
- added cubemaps for skyboxes
- removed mouse lag from demo
### Update 0.12-1
- reversed update log order
- Switched from FreeGLUT to SDL2 as background lib
- added abillity to set the window icon
## Update 0.12
- added a function that will be called if the window is resized
- added a parameter to controll if GLGE will close if an error occures
- added funktions for handeling fullscreen mode
- added access to keys that where toggled (bools only true if the key is pressed this tick)
- added a function to resize the window trough code, if parameter force is true, it can be used if the window size is locked
- the window size and position can now be locked, disabeling all user window size and position controll
- the window can be positiond trough code after it was initalised
### Update 0.11-6
- changed HDR from lighting pass to post-processing pass
### Update 0.11-5
- fixed single-color material
- optimised lighting pass (no longer using a pass if no light source is active)
### Update 0.11-4
- added inverse square law to default lighting pass
- fixed some floats in 2D pps shader
### Update 0.11-3
- fixed tonemapping, now uses correct HDR
### Update 0.11-2
- made default lighting better (Specular lighting)
### Update 0.11-1
- fixed 2D example
- re-added post processing pass
## Update 0.11
- removed old lighting code from shaders
- added Lighting pass
- removed post processing pass (temporearly, now used as lighting pass)
- added access to things like the last frame
- shader no longer nessesary for lighting (Updated default 3D shader)
