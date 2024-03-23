#!/bin/bash
#
#  
g++ -shared -o bin/libGLGE.so -fPIC src/GLGE/*.cpp src/GLGE/CML/*.cpp src/GLGE/GLGEInternal/*.cpp src/GLGE/GLGEOpenGL/*.cpp