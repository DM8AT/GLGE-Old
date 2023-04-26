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
- Not binding an camera before creating or updating 3D objects
- inputing wrong values to an Object constructor with pointer arrays

Often causes for GL_INVALIDE_OPERATION:
- Drawing an empty object

# Use GLGE
To use GLGE, you need the OpenGL developer library, GLEW, Freeglut, and all default C++ libraries. 

## Installation
To install, download the source code and move the "GLGE" folder to your preferred location,
updating the include path in the necessary files ("GLGE.h", "GLGE.cpp", "GLGE2DCore.h", "GLGE2DCore.cpp", "GLGE3DCore.h" and "GLGE3DCore.cpp"). 
Else, there would be compilation errors. 

In the make file, there is one path that need to be updated. It is called GLGE. It defines the path to the Core library. 

## Compiling
Compiling can be done with the Makefile provided, or with your preferred method. 
Make sure you include and compile with all the required librarys. 

# Upcoming
Upcoming features that may be added to GLGE in the future include:
- Loadable models from files
   - from .obj files (work in progress)
   - from .blend files
   - from other file formats
- A 3D and 2D sound function to the core
- 2D and 3D physics
- Procedrual materials
- Procedrual shaders
- A lighting core for 3D and 2D
   - preset of lighting shaders
   - preset for different light sources
   - different light source presets
      - directional light
      - point light
      - sun
      - light plates (some invisible shape that is emitting light)
- Post processing
   - preset for post processing core
   - preset for different effects (Bloom, distortion, etc.)
- Texture atleasing

## Working on
Currently working on improving the shader core to make handling shaders simpler and more functional.

# Changelogs
## Latest:
- added ability to load 3D .obj files into an mesh
## Other:
- added the abillity to create shaders from a kernel
- added things for post production
- added possibility for creating shaders from source code as std::strings
- deleted doxygen documentation, it was broken
- changed default shader to std::string instead of external file
- started logging the changelogs
