CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib
GLGE    := src/GLGE

LIBRARIES	:= -lGL -lGLEW -lSDL2main -lSDL2 -lSDL2_ttf
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	./$(BIN)/$(EXECUTABLE)

c: clean all

$(BIN)/$(EXECUTABLE): $(GLGE)/CML/*.cpp $(GLGE)/*.cpp $(SRC)/*.cpp $(GLGE)/GLGEOpenGL/* $(GLGE)/GLGEIndependend/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/$(EXECUTABLE)

