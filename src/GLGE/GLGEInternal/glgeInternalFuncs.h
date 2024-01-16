/**
 * @file glgeInternalFuncs.h
 * @author DM8AT
 * @brief define the graphics api independand internal functions for glge
 * @version 0.1
 * @date 2024-01-09
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license.
 */

#ifndef _GLGE_INTERNAL_FUNCS_H_
#define _GLGE_INTERNAL_FUNCS_H_

#include <string>

//include the CML functions
#include "../CML/CMLVec2.h"

//include the GLGE dependencys
#include "../GLGEInternal/glgeInternalFuncs.h"

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
 * @brief count the number of an character in an string
 * 
 * @param str the string with to count the amount of characters in
 * @param ch the symbol to count
 * @return int the amount of symbols in the string
 */
int count_char(std::string &str, char ch);

#endif