/**
 * @file glgeAtlasErrors.hpp
 * @author JuNi4
 * @brief Tempoary file for atlas errors
 * @version 0.0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) JuNi4 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_ATLAS_ERRORS_H_
#define _GLGE_ATLAS_ERRORS_H_

///////////////////////
// Main Atlas Errors //
///////////////////////

//// Warnings ////

/**
 * this warning occures if a requested texture is not found in the texture atlas
 */
#define GLGE_WARNING_ATLAS_CHILD_TEXTURE_NOT_FOUND "[GLGE WARNING] the requested texture %s was not found in texture atlas.\n"

//// Critical Errors ////

/**
 * this message is printed if the textrue atlas does not have room for a texture. This hopefully does never happen.
 */
#define GLGE_ERROR_ATLAS_NOT_ENOUGH_IMAGE_SPACE "[GLGE ERROR] no space for image %s was found in texture atlas while creating.\n"

/**
 * this message is printed if the missing texture is not found.
 * HINT: Specify a missing texture by using 'myAtlas.missing_path = "path/to/your/texture";'
 */
#define GLGE_ERROR_ATLAS_NO_MISSING_TEXTURE "[GLGE ERROR] no missing texture specified or not found\n"

/**
 * this error is printed when a specific file is not found
 * 
 */
#define GLGE_ERROR_ATLAS_INVALID "[GLGE ERROR] specified file %s not found or not valid.\n"

///////////////////////
// Atlas File Errors //
///////////////////////

//// Errors ////

#define GLGE_ERROR_ATLAS_FILE_INVALID_MODE "[GLGE ERROR] The specified mode '%s' is invalid.\n"

#define GLGE_ERROR_FILE_COULDNT_OPEN "[GLGE ERROR] The specified file '%s' could not be opened.\n"

#define GLGE_ERROR_FILE_ALREADY_OPEN "[GLGE ERROR] The specified file '%s' is already open.\n"

#define GLGE_ERROR_WRONG_MODE_FOR_ACTION "[GLGE ERROR] Attempted an action that is not supperted with the current mode.\n"

#endif