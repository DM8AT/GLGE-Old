# GLGE
GLGE is a C++ graphics library for creating 2D and 3D graphics. 

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

# Use GLGE
To use GLGE, you need the OpenGL developer library, GLEW, SDL2, and all default C++ libraries. 

## Installation
To install, download the source code and move the "GLGE" folder to your preferred location,
updating the include path in the necessary files ("GLGE.h", "GLGE.cpp", "GLGE2DCore.h", "GLGE2DCore.cpp", "GLGE3DCore.h" and "GLGE3DCore.cpp"). 
Else, there would be compilation errors. 

In the make file, there is one path that needs to be updated. It is called GLGE. It defines the path to the Core library. 

## Compiling
Compiling can be done with the Makefile provided, or with your preferred method. 
Make sure you include and compile with all the required librarys. 

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
- Texture atleasing

# Changelogs
## Update 0.5
- added the "Test" class to handle 2D text displays
   - inherates from the "Object2D" class
- added support for propper transparency for 2D objects
- added a new "Texture" class to bedder handle texture objects
   - simple creation from files using stb_image
   - create an empty texture with an specific size
   - create 
- added a new "ComputeShader" class. Compute shaders can now be used in GLGE
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
