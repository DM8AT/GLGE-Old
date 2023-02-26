/**
 * @file glgeFuncs.h
 * @author DM8AT
 * @brief this file stores all private functions used behind the sceens by OpenGL
 * @version 0.1
 * @date 2022-12-23
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

//check for double include
#ifndef _GLGE_FUNCS_H_
#define _GLGE_FUNCS_H_

//include needed defines
#include <string>

//include the CML functions
#include "CML/CMLVec2.h"

//include the glew library
#include <GL/glew.h>

/**
 * @brief Create a Window
 * 
 * behind the sceens function for GLGE
 * 
 * @param n the name of the window
 * @param s the size of the window
 * @param p the position of the window
 */
void createWindow(const char* n, vec2 s, vec2 p);

/**
 * @brief read a file
 * 
 * @param filename the name of the file
 * @param output a pointer to a string to write to
 * @return true the file could be read | 
 * @return false the file can't be found
 */
bool readFile(const char* filename, std::string& output);

/**
 * @brief convert an error code from OpenGL into an error string
 * 
 * @param error the error code
 * @return char* a string created from the error index
 */
char* getGLErrorString(unsigned int error);

/**
 * @brief Get a uniform variable in a shader
 * 
 * @param shaderProgram the program to get the variable from
 * @param name the name of the variable
 */
GLint getUniformVar(GLuint shaderProgram, const char* name);

/**
 * @brief add a new shader object
 * 
 * @param shaderProgram the program it should be added to
 * @param shadertext the text for the shader code
 * @param shaderType the type of the shader
 */
void addShader(GLuint shaderProgram, const char* shadertext, GLenum shaderType);

/**
 * @brief this function compiles the inputed two files into shaders
 * 
 * @param vertex the file for the vertex shader
 * @param fragment the file for the fragment shader
 */
GLuint compileShader(const char* vertex, const char* fragment);

/**
 * @brief this file loads the two inputed std::vectors of strings as shader
 * 
 * @param fileDataVertex the data of the vertex shader
 * @param fileDataFragment the data of the fragment shader
 * @return GLuint the compiled shader
 */
GLuint compileShader(std::string fileDataVertex, std::string fileDataFragment);

/**
 * @brief count the number of an character in an string
 * 
 * @param str the string with to count the amount of characters in
 * @param ch the symbol to count
 * @return int the amount of symbols in the string
 */
int count_char(std::string &str, char ch);

#endif