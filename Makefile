CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -O3

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib
GLGE    := src/GLGE

LIBRARIES	:= -lGL -lglut -lGLEW
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	./$(BIN)/$(EXECUTABLE)

c: clean all

$(BIN)/$(EXECUTABLE): $(GLGE)/CML/*.cpp $(GLGE)/*.cpp $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
