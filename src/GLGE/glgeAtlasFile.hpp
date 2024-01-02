/**
 * @file glgeAtlasFile.hpp
 * @author JuNi4
 * @brief A file format for storing texture atlas data for the glge game engine
 * @version 1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) JuNi4 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

// Includes for std, files stuff
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

#include "glgeAtlasErrors.hpp"

#include "CML/CML.h"

#ifndef _GLGE_ATLAS_FILE_H_
#define _GLGE_ATLAS_FILE_H_

/**
 * @brief Header for a glge atlas file
 * 
 * Blocks: Marker (this variable), version (this var), Texture count, width, height
 * 
 * Texture format: name length, name, x, y, w, h
 * 
 */
#define GLGE_ATLAS_FILE_HEADER "GLGE_AtlasFile_1"

/**
 * @brief A struct for keeping the data of the images
 * 
 */
struct GLGEAtlasImage {

    std::string name;

    vec2 pos;

    vec2 size;

};

/**
 * @brief Class for housing all of the atlas data
 * 
 */
class GLGEAtlasFile {

    private:

    /**
     * @brief The operation mode of the file (read, write, ...)
     * 
     */
    char mode;

    /**
     * @brief filepath
     * 
     */
    std::string fp;

    // Var for reading from a file
    std::ifstream infile;
    // Var for writing to a file
    std::ofstream outfile;

    /**
     * @brief Variable for keeping track if the file is open
     * 
     */
    bool opened = false;

    // helper functions //

    /**
     * @brief converts an int to a vector with uint8_t (basicly splits a number into singular bytes)
     * 
     * @param input the number to be converted
     * @param numBytes how many bytes should be processed
     * @return std::vector<uint8_t> the resulting bytes
     */
    std::vector<uint8_t> packInt( int input, int numBytes = 4 );

    int unpackInt( std::string inp );

    /**
     * @brief Writes bytes to a file
     * 
     * @param num the number to be written
     * @param numBytes size of number in bytes
     */
    void writeInt( int num, int numBytes = 4 );

    public:

    // General information //

    /**
     * @brief List of stored images
     * 
     */
    std::vector<GLGEAtlasImage> images;

    /**
     * @brief the size of the atlas image
     * 
     */
    vec2 size;

    // File Functions //

    /**
     * @brief Construct a new GLGEAtlasFile object from
     * 
     */
    GLGEAtlasFile();

    /**
     * @brief Open a glge atlas file from the specified path
     * 
     * @param fp Path to file
     * @param mode Sets in which mode the file will operate ('r','w')
     * @return true The operation was sucsessfull
     * @return false The operation failed
     */
    bool open( std::string fp, const char mode = 'r' );

    /**
     * @brief Closes the file
     * 
     * @return true The operation was sucsessfull
     * @return false The operation failed
     */
    bool close();

    /**
     * @brief Reads the contents from the file
     * 
     * @return true The operation was sucsessfull
     * @return false The operation failed
     */
    bool read();

    /**
     * @brief Writes the current state of the class to the file
     * 
     * @return true The operation was sucsessfull
     * @return false The operation failed
     */
    bool write();

    // Image functions //

    /**
     * @brief Adds an image to the atlas file
     * 
     * @param name Name of the image / id
     * @param pos position of the image
     * @param size size of the image
     */
    void addImage( std::string name, vec2 pos, vec2 size );

    /**
     * @brief Adds an image to the atlas
     * 
     * @param image Image to be added
     */
    void addImage( GLGEAtlasImage image );

    /**
     * @brief Finds the index of 
     * 
     * @param name The name of the image
     * @return ulong the index
     */
    ulong find( std::string name );

};

#endif