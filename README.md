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
