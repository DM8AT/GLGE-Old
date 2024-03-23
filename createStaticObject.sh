#!/bin/bash
#
#  
mkdir bin/dump
cd bin/dump
g++ -c ../../src/GLGE/*.cpp ../../src/GLGE/CML/*.cpp ../../src/GLGE/GLGEInternal/*.cpp ../../src/GLGE/GLGEOpenGL/*.cpp -lGL -lGLEW -lSDL2main -lSDL2
ar crf ../libGLGE.a *.o