# Makefile for GLGE
# 

# $*   Target file without file suffix
# $@   Target file with file suffix
# $<   File from file list that is currently worked on
# $!   All files from the file list
# $?   all files that not need to be recompiled

# STD Version specified
STD_VERS := c++17

# Compiler and compiler flags
CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=$(STD_VERS) -O3

# Specify some directories
# The directory for the executable
BIN		:= bin
# the direcotry for the obj files
OBJ_D	:= build
# the source code directory
SRC		:= src
# directory of GLGE
GLGE    := src/GLGE
# directory of GLGE/Independend
GLGE_IND:= $(GLGE)/GLGEIndependend
# directory of GLGE/GLGEOpenGL
GLGE_OGL:= $(GLGE)/GLGEOpenGL
# directory of GLGE/GLGESound
GLGE_SND:= $(GLGE)/GLGESound
# directory of CML
CML		:= $(GLGE)/CML

# the librarys needed to compile
LIBRARIES	:= -lGL -lGLEW -lSDL2main -lSDL2 -lSDL2_ttf -lopenal -lalut
# the name of the final executable
EXECUTABLE	:= main

# name list of all .o files for CML
CML_OBJ  = $(OBJ_D)/CMLDVec2.o $(OBJ_D)/CMLDVec3.o $(OBJ_D)/CMLDVec4.o $(OBJ_D)/CMLIVec2.o $(OBJ_D)/CMLIVec3.o $(OBJ_D)/CMLIVec4.o $(OBJ_D)/CMLMat2.o $(OBJ_D)/CMLMat3.o $(OBJ_D)/CMLMat4.o $(OBJ_D)/CMLQuaternion.o $(OBJ_D)/CMLVec2.o $(OBJ_D)/CMLVec3.o $(OBJ_D)/CMLVec4.o $(OBJ_D)/CMLVector.o

# file list of all .o files for GLGE
GLGE_OBJ = $(OBJ_D)/glge2DcoreDefClasses.o $(OBJ_D)/glge3DcoreDefClasses.o $(OBJ_D)/GLGEData.o $(OBJ_D)/glgeImage.o $(OBJ_D)/glgeInternalFuncs.o $(OBJ_D)/GLGEKlasses.o $(OBJ_D)/GLGEScene.o $(OBJ_D)/glgeVars.o $(OBJ_D)/openglGLGE.o $(OBJ_D)/openglGLGE2Dcore.o $(OBJ_D)/openglGLGE3Dcore.o $(OBJ_D)/openglGLGEComputeShader.o $(OBJ_D)/openglGLGEDefaultFuncs.o $(OBJ_D)/openglGLGEFuncs.o $(OBJ_D)/openglGLGELightingCore.o $(OBJ_D)/openglGLGEMaterialCore.o $(OBJ_D)/openglGLGERenderTarget.o $(OBJ_D)/openglGLGEShaderCore.o $(OBJ_D)/openglGLGETexture.o $(OBJ_D)/openglGLGEVars.o $(OBJ_D)/openglGLGEWindow.o $(OBJ_D)/GLGEMath.o $(OBJ_D)/glgeAtlasFile.o $(OBJ_D)/GLGEtextureAtlas.o $(OBJ_D)/openglGLGERenderPipeline.o $(OBJ_D)/openglGLGEParticles.o $(OBJ_D)/openglGLGEMetaObject.o $(OBJ_D)/glgeSoundCore.o $(OBJ_D)/glgeSoundVars.o $(OBJ_D)/glgeSoundListener.o $(OBJ_D)/glgeSoundSpeaker.o
# file list of all file of CML
CML_ALL_FILES = $(CML)/CMLDVec2.cpp $(CML)/CMLDVec2.h $(CML)/CMLDVec3.cpp $(CML)/CMLDVec3.h $(CML)/CMLDVec4.cpp $(CML)/CMLDVec4.h $(CML)/CMLIVec2.cpp $(CML)/CMLIVec2.h $(CML)/CMLIVec3.cpp $(CML)/CMLIVec3.h $(CML)/CMLIVec4.cpp $(CML)/CMLIVec4.h $(CML)/CMLMat2.cpp $(CML)/CMLMat2.h $(CML)/CMLMat3.cpp $(CML)/CMLMat3.h $(CML)/CMLMat4.cpp $(CML)/CMLMat4.h $(CML)/CMLQuaternion.cpp $(CML)/CMLQuaternion.h $(CML)/CMLVec2.cpp $(CML)/CMLVec2.h $(CML)/CMLVec3.cpp $(CML)/CMLVec3.h $(CML)/CMLVec4.cpp $(CML)/CMLVec4.h $(CML)/CMLVector.cpp $(CML)/CMLVector.h
# file list of all graphic lib independend files from GLGE
GLGE_ALL_IND = $(GLGE_IND)/glge2DcoreDefClasses.cpp $(GLGE_IND)/glge2DcoreDefClasses.h $(GLGE_IND)/glge3DcoreDefClasses.cpp $(GLGE_IND)/glge3DcoreDefClasses.h $(GLGE_IND)/GLGEData.cpp $(GLGE_IND)/GLGEData.h $(GLGE_IND)/glgeErrors.hpp $(GLGE_IND)/glgeImage.cpp $(GLGE_IND)/glgeImage.h $(GLGE_IND)/glgeInternalFuncs.cpp $(GLGE_IND)/glgeInternalFuncs.h $(GLGE_IND)/GLGEKlasses.cpp $(GLGE_IND)/GLGEKlasses.hpp $(GLGE_IND)/glgePrivDefines.hpp $(GLGE_IND)/GLGEScene.cpp $(GLGE_IND)/GLGEScene.hpp $(GLGE_IND)/glgeVars.cpp $(GLGE_IND)/glgeVars.hpp
# file list of all OpenGL dependend files from GLGE
GLGE_ALL_OGL = $(GLGE_OGL)/openglGLGE.cpp $(GLGE_OGL)/openglGLGE.h $(GLGE_OGL)/openglGLGE2Dcore.cpp $(GLGE_OGL)/openglGLGE2Dcore.h $(GLGE_OGL)/openglGLGE3Dcore.cpp $(GLGE_OGL)/openglGLGE3Dcore.h $(GLGE_OGL)/openglGLGEComputeShader.cpp $(GLGE_OGL)/openglGLGEComputeShader.hpp $(GLGE_OGL)/openglGLGEDefaultFuncs.cpp $(GLGE_OGL)/openglGLGEDefaultFuncs.hpp $(GLGE_OGL)/openglGLGEDefines.hpp $(GLGE_OGL)/openglGLGEFuncs.cpp $(GLGE_OGL)/openglGLGEFuncs.hpp $(GLGE_OGL)/openglGLGELightingCore.cpp $(GLGE_OGL)/openglGLGELightingCore.h $(GLGE_OGL)/openglGLGEMaterialCore.cpp $(GLGE_OGL)/openglGLGEMaterialCore.h $(GLGE_OGL)/openglGLGERenderTarget.cpp $(GLGE_OGL)/openglGLGERenderTarget.h $(GLGE_OGL)/openglGLGEShaderCore.cpp $(GLGE_OGL)/openglGLGEShaderCore.h $(GLGE_OGL)/openglGLGETexture.cpp $(GLGE_OGL)/openglGLGETexture.hpp $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp $(GLGE_OGL)/openglGLGEWindow.cpp $(GLGE_OGL)/openglGLGEWindow.h $(GLGE_OGL)/openglGLGERenderPipeline.hpp $(GLGE_OGL)/openglGLGERenderPipeline.cpp $(GLGE_OGL)/openglGLGEParticles.cpp $(GLGE_OGL)/openglGLGEParticles.h $(GLGE_OGL)/openglGLGEMetaObject.cpp $(GLGE_OGL)/openglGLGEMetaObject.hpp 
# file list of all remaining GLGE files
GLGE_ALL_REM = $(GLGE)/glgeAtlasFile.cpp $(GLGE)/glgeAtlasFile.hpp $(GLGE)/GLGEMath.cpp $(GLGE)/GLGEMath.h $(GLGE)/GLGEtextureAtlas.cpp $(GLGE)/GLGEtextureAtlas.h
# file list of all GLGE sound files
GLGE_ALL_SND = $(GLGE_SND)/glgeSoundCore.cpp $(GLGE_SND)/glgeSoundCore.h $(GLGE_SND)/glgeSoundVars.cpp $(GLGE_SND)/glgeSoundVars.hpp $(GLGE_SND)/glgeSoundListener.cpp $(GLGE_SND)/glgeSoundListener.h $(GLGE_SND)/glgeSoundSpeaker.cpp $(GLGE_SND)/glgeSoundSpeaker.h

# all files in glge
GLGE_ALL_FILES = $(GLGE_ALL_IND) $(GLGE_ALL_OGL) $(GLGE_ALL_REM) $(GLGE_ALL_SND)

all: $(BIN)/$(EXECUTABLE)

run: clean all
	./$(BIN)/$(EXECUTABLE)

c: clean all

#$(BIN)/$(EXECUTABLE): $(GLGE)//CML/*.cpp $(GLGE)//*.cpp $(SRC)/*.cpp $(GLGE)//GLGEOpenGL/* $(GLGE)//GLGEIndependend/*.cpp
#	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

##################################### APP BEGIN

$(BIN)/$(EXECUTABLE): $(OBJ_D)/main.o $(BIN)/libGLGE.a $(BIN)/libCML.a 
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $(BIN)/$(EXECUTABLE) $(LIBRARIES)

$(OBJ_D)/main.o: $(SRC)/main.cpp $(GLGE_ALL_FILES) $(CML_ALL_FILES)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

##################################### APP END

##################################### GLGE BEGIN

# compile the whole GLGE library
GLGE: $(BIN)/libGLGE.a

# whole GLGE library compiler
$(BIN)/libGLGE.a: $(GLGE_OBJ) $(BIN)/libCML.a
	-ar rcs $(BIN)/libGLGE.a $(GLGE_OBJ) $(BIN)/libCML.a

# Dep. on CML_ALL glgeSoundVars glgePrivDefines glgeSoundListener glgeSoundSpeaker
$(OBJ_D)/glgeSoundCore.o: $(GLGE_SND)/glgeSoundCore.cpp $(GLGE_SND)/glgeSoundCore.h $(GLGE_SND)/glgeSoundVars.cpp $(GLGE_SND)/glgeSoundVars.hpp $(CML_ALL) $(GLGE_IND)/glgePrivDefines.hpp $(GLGE_SND)/glgeSoundListener.cpp $(GLGE_SND)/glgeSoundListener.h $(GLGE_SND)/glgeSoundSpeaker.cpp $(GLGE_SND)/glgeSoundSpeaker.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on CML_ALL glgeSoundListener
$(OBJ_D)/glgeSoundVars.o: $(GLGE_SND)/glgeSoundVars.cpp $(GLGE_SND)/glgeSoundVars.hpp $(CML_ALL) $(GLGE_SND)/glgeSoundListener.cpp $(GLGE_SND)/glgeSoundListener.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on CML_ALL glgeSoundVars glge3DcoreDefClasses glge2DcoreDefClasses
$(OBJ_D)/glgeSoundListener.o: $(GLGE_SND)/glgeSoundListener.cpp $(GLGE_SND)/glgeSoundListener.h $(GLGE_SND)/glgeSoundVars.cpp $(GLGE_SND)/glgeSoundVars.hpp $(CML_ALL) $(GLGE_IND)/glge3DcoreDefClasses.cpp $(GLGE_IND)/glge3DcoreDefClasses.h $(GLGE_IND)/glge2DcoreDefClasses.cpp $(GLGE_IND)/glge2DcoreDefClasses.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on CML_ALL glgeSoundVars glge3DcoreDefClasses glgePrivDefines glge2DcoreDefClasses
$(OBJ_D)/glgeSoundSpeaker.o: $(GLGE_SND)/glgeSoundSpeaker.cpp $(GLGE_SND)/glgeSoundSpeaker.h $(GLGE_SND)/glgeSoundVars.cpp $(GLGE_SND)/glgeSoundVars.hpp $(CML_ALL) $(GLGE_IND)/glge3DcoreDefClasses.cpp $(GLGE_IND)/glge3DcoreDefClasses.h $(GLGE_IND)/glgePrivDefines.hpp $(GLGE_IND)/glge2DcoreDefClasses.cpp $(GLGE_IND)/glge2DcoreDefClasses.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

# Dep. on CML_ALL, glgeVars, glgePrivDefines
$(OBJ_D)/glge2DcoreDefClasses.o: $(GLGE_IND)/glge2DcoreDefClasses.cpp $(GLGE_IND)/glge2DcoreDefClasses.h $(GLGE_IND)/glgeVars.cpp $(GLGE_IND)/glgeVars.hpp $(CML_ALL_FILES)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on CMLVec3, CMLVec4, CMLMat4, glgeErrors, glgeVars, glgePrivDefines, glgeInternalFuncs, CMLQuaternion
$(OBJ_D)/glge3DcoreDefClasses.o: $(GLGE_IND)/glge3DcoreDefClasses.cpp $(GLGE_IND)/glge3DcoreDefClasses.h $(CML)/CMLVec3.cpp $(CML)/CMLVec3.h $(CML)/CMLVec4.cpp $(CML)/CMLVec4.h $(CML)/CMLMat4.cpp $(CML)/CMLMat4.h $(GLGE_IND)/glgeVars.cpp $(GLGE_IND)/glgeVars.hpp $(GLGE_IND)/glgeInternalFuncs.cpp $(GLGE_IND)/glgeInternalFuncs.h $(CML)/CMLQuaternion.cpp $(CML)/CMLQuaternion.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on CML_ALL
$(OBJ_D)/GLGEData.o: $(GLGE_IND)/GLGEData.cpp $(GLGE_IND)/GLGEData.h $(CML_ALL)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on --
$(OBJ_D)/glgeImage.o: $(GLGE_IND)/glgeImage.cpp $(GLGE_IND)/glgeImage.h $(CML_ALL)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on CMLVec2, glgeVars
$(OBJ_D)/glgeInternalFuncs.o: $(GLGE_IND)/glgeInternalFuncs.cpp $(GLGE_IND)/glgeInternalFuncs.h $(GLGE_IND)/glgeVars.cpp $(GLGE_IND)/glgeVars.hpp $(CML)/CMLVec2.cpp $(CML)/CMLVec2.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on CMLVec2, glgeVars
$(OBJ_D)/GLGEKlasses.o: $(GLGE_IND)/GLGEKlasses.cpp $(GLGE_IND)/GLGEKlasses.hpp $(GLGE_IND)/glgeVars.cpp $(GLGE_IND)/glgeVars.hpp $(CML)/CMLVec2.cpp $(CML)/CMLVec2.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on GLGEData
$(OBJ_D)/GLGEScene.o: $(GLGE_IND)/GLGEScene.cpp $(GLGE_IND)/GLGEScene.hpp $(GLGE_IND)/GLGEData.cpp $(GLGE_IND)/GLGEData.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on GLGEKlasses, CMLVec4
$(OBJ_D)/glgeVars.o: $(GLGE_IND)/glgeVars.cpp $(GLGE_IND)/glgeVars.hpp $(GLGE_IND)/GLGEKlasses.cpp $(GLGE_IND)/GLGEKlasses.hpp $(CML)/CMLVec4.cpp $(CML)/CMLVec4.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

# Dep. on CML_ALL, GLGEKlasses, GLGEMath, openglGLGEShaderCore, openglGLGERenderTarget, GLGEScene, GLGEData, openglGLGEVars, openglGLGEFuncs, openglGLGEDefaultFuncs, glgeImage
$(OBJ_D)/openglGLGE.o: $(GLGE_OGL)/openglGLGE.cpp $(GLGE_OGL)/openglGLGE.h $(CML_ALL_FILES) $(GLGE_IND)/GLGEKlasses.cpp $(GLGE_IND)/GLGEKlasses.hpp $(GLGE)/GLGEMath.cpp $(GLGE)/GLGEMath.h $(GLGE_OGL)/openglGLGEShaderCore.cpp $(GLGE_OGL)/openglGLGEShaderCore.h $(GLGE_OGL)/openglGLGERenderTarget.cpp $(GLGE_OGL)/openglGLGERenderTarget.h $(GLGE_IND)/GLGEScene.cpp $(GLGE_IND)/GLGEScene.hpp $(GLGE_IND)/GLGEData.cpp $(GLGE_IND)/GLGEData.h $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp $(GLGE_OGL)/openglGLGEFuncs.cpp $(GLGE_OGL)/openglGLGEFuncs.hpp $(GLGE_OGL)/openglGLGEDefaultFuncs.cpp $(GLGE_OGL)/openglGLGEDefaultFuncs.hpp $(GLGE_IND)/glgeImage.cpp $(GLGE_IND)/glgeImage.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on glge2DcoreDefClasses openglGLGE openglGLGEDefines glgePrivDefines openglGLGEFuncs openglGLGEVars GLGEData
$(OBJ_D)/openglGLGE2Dcore.o: $(GLGE_OGL)/openglGLGE2Dcore.cpp $(GLGE_OGL)/openglGLGE2Dcore.h $(GLGE_IND)/glge2DcoreDefClasses.cpp $(GLGE_IND)/glge2DcoreDefClasses.h $(GLGE_OGL)/openglGLGE.cpp $(GLGE_OGL)/openglGLGE.h $(GLGE_OGL)/openglGLGEDefines.hpp $(GLGE_IND)/glgePrivDefines.hpp $(GLGE_OGL)/openglGLGEFuncs.cpp $(GLGE_OGL)/openglGLGEFuncs.hpp $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp $(GLGE_IND)/GLGEData.cpp $(GLGE_IND)/GLGEData.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on glgeVars openglGLGEFuncs openglGLGEVars CMLQuaternion openglGLGEMaterialCore openglGLGEShaderCore openglGLGE glge3DcoreDefClasses GLGEData
$(OBJ_D)/openglGLGE3Dcore.o: $(GLGE_OGL)/openglGLGE3Dcore.cpp $(GLGE_OGL)/openglGLGE3Dcore.h $(GLGE_IND)/glgeVars.cpp $(GLGE_IND)/glgeVars.hpp $(GLGE_OGL)/openglGLGEFuncs.cpp $(GLGE_OGL)/openglGLGEFuncs.hpp $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp $(GLGE_OGL)/openglGLGEDefines.hpp $(CML)/CMLQuaternion.cpp $(CML)/CMLQuaternion.h $(GLGE_OGL)/openglGLGEMaterialCore.cpp $(GLGE_OGL)/openglGLGEMaterialCore.h $(GLGE_OGL)/openglGLGEShaderCore.cpp $(GLGE_OGL)/openglGLGEShaderCore.h $(GLGE_OGL)/openglGLGE.cpp $(GLGE_OGL)/openglGLGE.h $(GLGE_IND)/glge3DcoreDefClasses.cpp $(GLGE_IND)/glge3DcoreDefClasses.h $(GLGE_IND)/GLGEData.cpp $(GLGE_IND)/GLGEData.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on glgeInternalFuncs openglGLGEVars openglGLGE openglGLGEShaderCore
$(OBJ_D)/openglGLGEComputeShader.o: $(GLGE_OGL)/openglGLGEComputeShader.cpp $(GLGE_OGL)/openglGLGEComputeShader.hpp $(GLGE_IND)/glgeInternalFuncs.cpp $(GLGE_IND)/glgeInternalFuncs.h $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp $(GLGE_OGL)/openglGLGE.cpp $(GLGE_OGL)/openglGLGE.h $(GLGE_OGL)/openglGLGEShaderCore.cpp $(GLGE_OGL)/openglGLGEShaderCore.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on GLGEKlasses openglGLGEVars openglGLGEFuncs
$(OBJ_D)/openglGLGEDefaultFuncs.o: $(GLGE_OGL)/openglGLGEDefaultFuncs.cpp $(GLGE_OGL)/openglGLGEDefaultFuncs.hpp $(GLGE_IND)/GLGEKlasses.cpp $(GLGE_IND)/GLGEKlasses.hpp $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp $(GLGE_OGL)/openglGLGEFuncs.cpp $(GLGE_OGL)/openglGLGEFuncs.hpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on openglGLGEVars openglGLGEDefaultFuncs openglGLGE openglGLGEShaderCore openglGLGEWindow glgeInternalFuncs
$(OBJ_D)/openglGLGEFuncs.o: $(GLGE_OGL)/openglGLGEFuncs.cpp $(GLGE_OGL)/openglGLGEFuncs.hpp $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp $(GLGE_OGL)/openglGLGEDefaultFuncs.cpp $(GLGE_OGL)/openglGLGEDefaultFuncs.hpp $(GLGE_OGL)/openglGLGE.cpp $(GLGE_OGL)/openglGLGE.h $(GLGE_OGL)/openglGLGEShaderCore.cpp $(GLGE_OGL)/openglGLGEShaderCore.h $(GLGE_OGL)/openglGLGEWindow.cpp $(GLGE_OGL)/openglGLGEWindow.h $(GLGE_IND)/glgeInternalFuncs.cpp $(GLGE_IND)/glgeInternalFuncs.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on openglGLGEVars openglGLGEFuncs CMLMat4
$(OBJ_D)/openglGLGELightingCore.o: $(GLGE_OGL)/openglGLGELightingCore.cpp $(GLGE_OGL)/openglGLGELightingCore.h $(GLGE_OGL)/openglGLGE.cpp $(GLGE_OGL)/openglGLGE.h $(GLGE_IND)/GLGEData.cpp $(GLGE_IND)/GLGEData.h $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp $(GLGE_OGL)/openglGLGEFuncs.cpp $(GLGE_OGL)/openglGLGEFuncs.hpp $(CML)/CMLMat4.cpp $(CML)/CMLMat4.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on CML_ALL openglGLGE openglGLGEVars
$(OBJ_D)/openglGLGEMaterialCore.o: $(GLGE_OGL)/openglGLGEMaterialCore.cpp $(GLGE_OGL)/openglGLGEMaterialCore.h $(GLGE_OGL)/openglGLGE.cpp $(GLGE_OGL)/openglGLGE.h $(GLGE_IND)/GLGEData.cpp $(GLGE_IND)/GLGEData.h $(CML_ALL_FILES) $(GLGE_OGL)/openglGLGE.cpp $(GLGE_OGL)/openglGLGE.h $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on openglGLGEShaderCore openglGLGEVars
$(OBJ_D)/openglGLGERenderTarget.o: $(GLGE_OGL)/openglGLGERenderTarget.cpp $(GLGE_OGL)/openglGLGERenderTarget.h $(GLGE_OGL)/openglGLGEShaderCore.cpp $(GLGE_OGL)/openglGLGEShaderCore.h $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on CML_ALL openglGLGEVars openglGLGEFuncs GLGEMath
$(OBJ_D)/openglGLGEShaderCore.o: $(GLGE_OGL)/openglGLGEShaderCore.cpp $(GLGE_OGL)/openglGLGEShaderCore.h $(GLGE_OGL)/openglGLGE.cpp $(GLGE_OGL)/openglGLGE.h $(GLGE_IND)/GLGEData.cpp $(GLGE_IND)/GLGEData.h $(CML_ALL_FILES) $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp $(GLGE_OGL)/openglGLGEFuncs.cpp $(GLGE_OGL)/openglGLGEFuncs.hpp $(GLGE)/GLGEMath.cpp $(GLGE)/GLGEMath.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on openglGLGE glgeImage openglGLGEVars
$(OBJ_D)/openglGLGETexture.o: $(GLGE_OGL)/openglGLGETexture.cpp $(GLGE_OGL)/openglGLGETexture.hpp $(GLGE_OGL)/openglGLGE.cpp $(GLGE_OGL)/openglGLGE.h $(GLGE_IND)/glgeImage.cpp $(GLGE_IND)/glgeImage.h $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp $(GLGE_IND)/glgeImage.cpp $(GLGE_IND)/glgeImage.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on glgeVars openglGLGEWindow openglGLGE2Dcore openglGLGE3Dcore
$(OBJ_D)/openglGLGEVars.o: $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp $(GLGE_IND)/glgeVars.cpp $(GLGE_IND)/glgeVars.hpp $(GLGE_OGL)/openglGLGEWindow.cpp $(GLGE_OGL)/openglGLGEWindow.h $(GLGE_OGL)/openglGLGE2Dcore.cpp $(GLGE_OGL)/openglGLGE2Dcore.h $(GLGE_OGL)/openglGLGE3Dcore.cpp $(GLGE_OGL)/openglGLGE3Dcore.h $(GLGE_OGL)/openglGLGELightingCore.cpp $(GLGE_OGL)/openglGLGELightingCore.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on openglGLGELightingCore openglGLGE3Dcore openglGLGE2Dcore CMLVec2 CMLVec3 openglGLGEVars openglGLGEFuncs glgeImage
$(OBJ_D)/openglGLGEWindow.o: $(GLGE_OGL)/openglGLGEWindow.cpp $(GLGE_OGL)/openglGLGEWindow.h $(GLGE_OGL)/openglGLGELightingCore.cpp $(GLGE_OGL)/openglGLGELightingCore.h $(GLGE_OGL)/openglGLGE3Dcore.cpp $(GLGE_OGL)/openglGLGE3Dcore.h $(GLGE_OGL)/openglGLGE2Dcore.cpp $(GLGE_OGL)/openglGLGE2Dcore.h $(CML)/CMLVec2.cpp $(CML)/CMLVec2.h $(CML)/CMLVec3.cpp $(CML)/CMLVec3.h $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp $(GLGE_OGL)/openglGLGEFuncs.cpp $(GLGE_OGL)/openglGLGEFuncs.hpp $(GLGE_IND)/glgeImage.cpp $(GLGE_IND)/glgeImage.h $(GLGE_OGL)/openglGLGERenderPipeline.hpp $(GLGE_OGL)/openglGLGERenderPipeline.cpp 
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on openglGLGE
$(OBJ_D)/openglGLGERenderPipeline.o: $(GLGE_OGL)/openglGLGERenderPipeline.cpp $(GLGE_OGL)/openglGLGERenderPipeline.hpp $(GLGE_OGL)/openglGLGE.h $(GLGE_OGL)/openglGLGE.cpp $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp $(GLGE_IND)/glgeVars.cpp $(GLGE_IND)/glgeVars.hpp $(GLGE_OGL)/openglGLGEWindow.cpp $(GLGE_OGL)/openglGLGEWindow.h $(GLGE_OGL)/openglGLGE2Dcore.cpp $(GLGE_OGL)/openglGLGE2Dcore.h $(GLGE_OGL)/openglGLGE3Dcore.cpp $(GLGE_OGL)/openglGLGE3Dcore.h $(GLGE_OGL)/openglGLGELightingCore.cpp $(GLGE_OGL)/openglGLGELightingCore.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on openglGLGE3DCore, openglGLGEVars
$(OBJ_D)/openglGLGEParticles.o: $(GLGE_OGL)/openglGLGEParticles.cpp $(GLGE_OGL)/openglGLGEParticles.h $(GLGE_OGL)/openglGLGE3Dcore.cpp $(GLGE_OGL)/openglGLGE3Dcore.h $(GLGE_IND)/glgeVars.cpp $(GLGE_IND)/glgeVars.hpp $(GLGE_OGL)/openglGLGEFuncs.cpp $(GLGE_OGL)/openglGLGEFuncs.hpp $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp $(GLGE_OGL)/openglGLGEDefines.hpp $(CML)/CMLQuaternion.cpp $(CML)/CMLQuaternion.h $(GLGE_OGL)/openglGLGEMaterialCore.cpp $(GLGE_OGL)/openglGLGEMaterialCore.h $(GLGE_OGL)/openglGLGEShaderCore.cpp $(GLGE_OGL)/openglGLGEShaderCore.h $(GLGE_OGL)/openglGLGE.cpp $(GLGE_OGL)/openglGLGE.h $(GLGE_IND)/glge3DcoreDefClasses.cpp $(GLGE_IND)/glge3DcoreDefClasses.h $(GLGE_IND)/GLGEData.cpp $(GLGE_IND)/GLGEData.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on openglGLGEWindow
$(OBJ_D)/openglGLGEMetaObject.o: $(GLGE_OGL)/openglGLGEMetaObject.cpp $(GLGE_OGL)/openglGLGEMetaObject.hpp $(GLGE_OGL)/openglGLGEWindow.cpp $(GLGE_OGL)/openglGLGEWindow.h $(GLGE_OGL)/openglGLGELightingCore.cpp $(GLGE_OGL)/openglGLGELightingCore.h $(GLGE_OGL)/openglGLGE3Dcore.cpp $(GLGE_OGL)/openglGLGE3Dcore.h $(GLGE_OGL)/openglGLGE2Dcore.cpp $(GLGE_OGL)/openglGLGE2Dcore.h $(CML)/CMLVec2.cpp $(CML)/CMLVec2.h $(CML)/CMLVec3.cpp $(CML)/CMLVec3.h $(GLGE_OGL)/openglGLGEVars.cpp $(GLGE_OGL)/openglGLGEVars.hpp $(GLGE_OGL)/openglGLGEFuncs.cpp $(GLGE_OGL)/openglGLGEFuncs.hpp $(GLGE_IND)/glgeImage.cpp $(GLGE_IND)/glgeImage.h $(GLGE_OGL)/openglGLGERenderPipeline.hpp $(GLGE_OGL)/openglGLGERenderPipeline.cpp 
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

# Dep. on --
$(OBJ_D)/GLGEMath.o: $(GLGE)/GLGEMath.cpp $(GLGE)/GLGEMath.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on CML_ALL
$(OBJ_D)/glgeAtlasFile.o: $(CML_ALL_FILES)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
# Dep. on glgeImage CML_ALL
$(OBJ_D)/GLGEtextureAtlas.o: $(CML_ALL_FILES) $(GLGE_IND)/glgeImage.cpp $(GLGE_IND)/glgeImage.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

##################################### GLGE END

##################################### CML BEGIN

# compile the whole CML library
CML: $(BIN)/libCML.a

$(BIN)/libCML.a: $(CML_OBJ)
	-ar rcs $(BIN)/libCML.a $(CML_OBJ)

# CML N-Dimensional double vector
$(OBJ_D)/CMLDVec2.o: $(CML)/CMLDVec2.cpp $(CML)/CMLDVec2.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
$(OBJ_D)/CMLDVec3.o: $(CML)/CMLDVec3.cpp $(CML)/CMLDVec3.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
$(OBJ_D)/CMLDVec4.o: $(CML)/CMLDVec4.cpp $(CML)/CMLDVec4.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

# CML N-Dimensional integer vector
$(OBJ_D)/CMLIVec2.o: $(CML)/CMLIVec2.cpp $(CML)/CMLIVec2.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
$(OBJ_D)/CMLIVec3.o: $(CML)/CMLIVec3.cpp $(CML)/CMLIVec3.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
$(OBJ_D)/CMLIVec4.o: $(CML)/CMLIVec4.cpp $(CML)/CMLIVec4.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

# CML NxN-Dimensional matrix
$(OBJ_D)/CMLMat2.o: $(CML)/CMLMat2.cpp $(CML)/CMLMat2.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
$(OBJ_D)/CMLMat3.o: $(CML)/CMLMat3.cpp $(CML)/CMLMat3.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
$(OBJ_D)/CMLMat4.o: $(CML)/CMLMat4.cpp $(CML)/CMLMat4.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

# CML Quaternion
$(OBJ_D)/CMLQuaternion.o: $(CML)/CMLQuaternion.cpp $(CML)/CMLQuaternion.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

# CML N-Dimensional float vector
$(OBJ_D)/CMLVec2.o: $(CML)/CMLVec2.cpp $(CML)/CMLVec2.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
$(OBJ_D)/CMLVec3.o: $(CML)/CMLVec3.cpp $(CML)/CMLVec3.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
$(OBJ_D)/CMLVec4.o: $(CML)/CMLVec4.cpp $(CML)/CMLVec4.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

# CML N-Dimensional calculatable float vector
$(OBJ_D)/CMLVector.o: $(CML)/CMLVector.cpp $(CML)/CMLVector.h
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

################################################ CML END

clean:
	-rm $(BIN)/$(EXECUTABLE)
	-rm $(BIN)/libCML.a
	-rm $(BIN)/libGLGE.a
	-rm $(OBJ_D)/*.o

install:
	-sudo apt install libgl-dev libglew-dev libsdl2-dev libsdl2-ttf-dev libopenal-dev libalut-dev