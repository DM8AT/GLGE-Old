# GLGE
A simple graphics library for 3D and 2D in C++ 

## Examples
example code is in the files 2DTest.hpp, 2DTest.cpp as well as 3DTest.hpp and 3DTest.cpp. 
The test code is commented, especaly the 3D code, because it is very similar to the
2D code you need to write to create, draw and setup an Object. The differences are
outlined in the comments. 

## Documentation
the code is documented using doxygen, in the future an documentation should be in
the files. But the documentation can allready be created like normal using doxygen. 

## Debugging
If you get an error, reading it can help you find the cause of it. The error outputs
are defined in "GLGE/glgeErrors.hpp". If wanted, the errors can be checkted there, 
often there is a bit more information. 
Often causes for memory acces errors:
- Not binding an camera before creating or updating 3D objects
- inputing wrong values to an Object constructor with pointer arrays
Often causes for GL_INVALIDE_OPERATION:
- Drawing an empty object

# Use GLGE
Below is writen how to run GLGE on Linux. 
Technicaly, the library will run everywhere where the following librarys work:
- OpenGL developer library
- GLEW
- Freeglut
- all default C++ librarys

## Installation
To install the lybrary, you need to download the source code. Then you can move the
folder called "GLGE" wherever you want, just make shure to change the include path in
"GLGE.h", "GLGE.cpp", "GLGE2DCore.h", "GLGE2DCore.cpp", "GLGE3DCore.h" and "GLGE3DCore.cpp". 
Else, there would be compilation errors. 
The files "GLGE.h", "GLGE.cpp", "GLGE2DCore.h", "GLGE2DCore.cpp", "GLGE3DCore.h" and
"GLGE3DCore.cpp" should be in the same directory as your main file, else there will be errors
if you use the the Makefile included in the main folder. 

## Compiling
To compile a program that uses GLGE, the Makefile in the main folder can be used. This can
output a few errors, and compiling it differently also works. Just make shure that the you
include and compile with the needed librarys. 
