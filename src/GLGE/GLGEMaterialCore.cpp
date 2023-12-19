/**
 * @file GLGEMaterialCore.cpp
 * @author DM8AT
 * @brief in this file, all things from GLGEMaterialCore.h are defined. 
 * @version 0.1
 * @date 2023-02-23
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */
//check if glew is allready included
#ifndef _GLGE_GLEW_
//say that glew is now included
#define _GLGE_GLEW_
//include glew
#include <GL/glew.h>
//close the if for glew
#endif

//include GLGEMaterialCore
#include "GLGEMaterialCore.h"

//include the library core
#include "GLGE.h"

//include the shared variables
#include "glgeVars.hpp"

//includes from the default lib
#include <iostream>

Material::Material()
{
    //init the object
}

Material::Material(const char* image, const char* uniform, float roughness, float metalic)
{
    //store the inputed roughness
    this->roughness = roughness;
    //store the inputed metalic value
    this->metal = metalic;
    //bind the inputed texture
    this->setCustomTexture(uniform, image);
}

Material::Material(unsigned int texture, const char* uniform, float roughness, float metalic)
{
    //store the inputed roughness
    this->roughness = roughness;
    //store the inputed metalic value
    this->metal = metalic;
    //bind the inputed texture
    this->setCustomTexture(uniform, texture);
}

Material::Material(vec4 color, float roughness, float metalic)
{
    //store the inputed roughness
    this->roughness = roughness;
    //store the inputed metalic value
    this->metal = metalic;

    //store the inputed base color
    this->color = color;
}

void Material::setDefaultUnifromName(const char* newName, unsigned int type)
{
    //store the new name for the specefied unifrom
    switch (type)
    {
        //case if the new name is for a normal map
        case GLGE_NORMAL_MAP_ID:
            this->normalUniform = newName;
            break;

        //case if the new name is for a specular map
        case GLGE_SPECULAR_MAP_ID:
            this->specularUniform = newName;
            break;

        //case if the new name is for a height map
        case GLGE_HEIGHT_MAP_ID:
            this->heightUniform = newName;
            break;

        //case if the new name is for a color
        case GLGE_COLOR_ID:
            this->colorUniform = newName;
            break;
        
        //default case
        default:
            break;
    }
}

void Material::setNormalMap(const char* image, const char* uniformName)
{
    //add the normal map as an custom texture
    this->setCustomTexture(uniformName, image);
    //store the name of the uniform
    this->normalUniform = uniformName;
}

void Material::setRoughnessMap(const char* image, const char* uniformName)
{
    //add the normal map as an custom texture
    this->setCustomTexture(uniformName, image);
    //store the name of the uniform
    this->roughnessUniform = uniformName;
}

void Material::setHeightMap(const char* image, const char* uniformName)
{
    //add the normal map as an custom texture
    this->setCustomTexture(uniformName, image);
    //store the name of the uniform
    this->heightUniform = uniformName;
}

void Material::setMetalicMap(const char* image, const char* uniformName)
{
    //add the normal map as an custom texture
    this->setCustomTexture(uniformName, image);
    //store the name of the uniform
    this->metalicUniform = uniformName;

}

void Material::applyShader(unsigned int shader)
{
    //activate the inputed shader
    glUseProgram(shader);
    //store the state of the GLGE error output
    bool err = glgeErrorOutput;
    //disable the GLGE error output
    glgeErrorOutput = false;

    //get the location of the signal to store if the normal map is used
    this->normIsActivLoc = glgeGetUniformVar(shader, (std::string(this->normalUniform)+std::string(GLGE_SUFFIX_IS_ACTIVE_TEXTURE)).c_str());

    //get the location of the signal to store if the roughness map is used
    this->roughIsActivLoc = glgeGetUniformVar(shader, (std::string(this->roughnessUniform)+std::string(GLGE_SUFFIX_IS_ACTIVE_TEXTURE)).c_str());

    //get the location of the signal to store if the height map is used
    this->highIsActiveLoc = glgeGetUniformVar(shader, (std::string(this->heightUniform)+std::string(GLGE_SUFFIX_IS_ACTIVE_TEXTURE)).c_str());

    //get the location of the roughness variable
    this->roughnessLoc = glgeGetUniformVar(shader, "glgeRoughness");

    //get the location of the metalic variable
    this->metalicLoc = glgeGetUniformVar(shader, "glgeMetalic");

    //get the location of the lit parameter
    this->litLoc = glgeGetUniformVar(shader, "glgeLit");

    //get the location to say if a texture is bound
    this->hasTextureLoc = glgeGetUniformVar(shader, "glgeHasTexture");

    //get the location of the color
    this->colorLoc = glgeGetUniformVar(shader, this->colorUniform);

    //get the location of the var to say how many pictures are used
    this->usedLoc = glgeGetUniformVar(shader, GLGE_USED_TEXTURES);

    //reset the error output
    glgeErrorOutput = err;

    //load all the custom variables

    //load the floats
    //define a variable for the current element
    std::map<std::string, float>::iterator currF;
    //loop over the elements
    for (currF = this->floats.begin(); currF != this->floats.end(); currF++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->floatLocs[currF->first] = glgeGetUniformVar(shader, currF->first.c_str());
    }
    //load the integers
    //define a variable for the current element
    std::map<std::string, int>::iterator currI;
    //loop over the elements
    for (currI = this->integers.begin(); currI != this->integers.end(); currI++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->intLocs[currI->first] = glgeGetUniformVar(shader, currI->first.c_str());
    }
    //load the booleans
    //define a variable for the current element
    std::map<std::string, bool>::iterator currB;
    //loop over the elements
    for (currB = this->booleans.begin(); currB != this->booleans.end(); currB++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->boolLocs[currB->first] = glgeGetUniformVar(shader, currB->first.c_str());
    }
    //load the vec2s
    //define a variable for the current element
    std::map<std::string, vec2>::iterator currV2;
    //loop over the elements
    for (currV2 = this->vec2s.begin(); currV2 != this->vec2s.end(); currV2++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->vec2Locs[currV2->first] = glgeGetUniformVar(shader, currV2->first.c_str());
    }
    //load the vec3s
    //define a variable for the current element
    std::map<std::string, vec3>::iterator currV3;
    //loop over the elements
    for (currV3 = this->vec3s.begin(); currV3 != this->vec3s.end(); currV3++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->vec3Locs[currV3->first] = glgeGetUniformVar(shader, currV3->first.c_str());
    }
    //load the vec4s
    //define a variable for the current element
    std::map<std::string, vec4>::iterator currV4;
    //loop over the elements
    for (currV4 = this->vec4s.begin(); currV4 != this->vec4s.end(); currV4++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->vec4Locs[currV4->first] = glgeGetUniformVar(shader, currV4->first.c_str());
    }
    //load the mat2s
    //define a variable for the current element
    std::map<std::string, mat2>::iterator currM2;
    //loop over the elements
    for (currM2 = this->mat2s.begin(); currM2 != this->mat2s.end(); currM2++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->mat2Locs[currM2->first] = glgeGetUniformVar(shader, currM2->first.c_str());
    }
    //load the mat3s
    //define a variable for the current element
    std::map<std::string, mat3>::iterator currM3;
    //loop over the elements
    for (currM3 = this->mat3s.begin(); currM3 != this->mat3s.end(); currM3++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->mat3Locs[currM3->first] = glgeGetUniformVar(shader, currM3->first.c_str());
    }
    //load the mat4s
    //define a variable for the current element
    std::map<std::string, mat4>::iterator currM4;
    //loop over the elements
    for (currM4 = this->mat4s.begin(); currM4 != this->mat4s.end(); currM4++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->mat4Locs[currM4->first] = glgeGetUniformVar(shader, currM4->first.c_str());
    }
    //load the textures
    //define a variable for the current element
    std::map<std::string, unsigned int>::iterator currTex;
    //loop over the elements
    for (currTex = this->customTextures.begin(); currTex != this->customTextures.end(); currTex++)
    {
        //store the position of the uniform in the corresponding element in the position map
        this->customTextureLocs[currTex->first] = glgeGetUniformVar(shader, currTex->first.c_str());
    }

    //unbind the shader
    glUseProgram(0);
}

void Material::applyMaterial()
{
    //store how many materials are bound
    this->boundTextures = 0;
    //pass the color of the material
    if (this->colorLoc != -1)
    {
        glUniform4f(this->colorLoc, this->color.x, this->color.y, this->color.z, this->color.w);
    }

    //pass the roughness to the material
    if (this->roughnessLoc != -1)
    {
        glUniform1f(this->roughnessLoc, roughness);
    }

    //pass the lit parameter to the shader
    if (this->litLoc != -1)
    {
        glUniform1i(this->litLoc, (int)lit);
    }

    //pass the metalic value to the material
    if (this->metalicLoc != -1)
    {
        glUniform1f(this->metalicLoc, metal);
    }

    //pass if the normal map is bound
    if (this->normIsActivLoc != -1)
    {
        //pass if the texture vector contains the normal map
        glUniform1i(this->normIsActivLoc, (int)(this->customTextures[this->normalUniform] != 0));
    }
    //pass if the roughness map is bound
    if (this->roughIsActivLoc != -1)
    {
        //pass if the texture vector contains the normal map
        glUniform1i(this->roughIsActivLoc, (int)(this->customTextures[this->roughnessUniform] != 0));
    }
    //pass hasTexture if it is bound
    if (this->hasTextureLoc != -1)
    {
        //pass if there is a texture
        glUniform1i(this->hasTextureLoc, (int)this->customTextures.count(GLGE_TEXTURE));
    }

    //pass the costom floats
    //store the current float
    std::map<std::string, unsigned int>::iterator curr;
    //loop over the floats
    for (curr = this->floatLocs.begin(); curr != this->floatLocs.end(); curr++)
    {
        //pass the float
        glUniform1f(curr->second, this->floats[curr->first]);
    }
    //pass the costom ints
    //loop over the bools
    for (curr = this->intLocs.begin(); curr != this->intLocs.end(); curr++)
    {
        //pass the ints
        glUniform1i(curr->second, this->integers[curr->first]);
    }
    //pass the costom bools
    //loop over the bools
    for (curr = this->boolLocs.begin(); curr != this->boolLocs.end(); curr++)
    {
        //pass the bools
        glUniform1i(curr->second, this->booleans[curr->first]);
    }
    //pass the costom vec2s
    //loop over the vec2s
    for (curr = this->vec2Locs.begin(); curr != this->vec2Locs.end(); curr++)
    {
        //pass the vec2s
        glUniform2f(curr->second, this->vec2s[curr->first].x, this->vec2s[curr->first].y);
    }
    //pass the costom vec3s
    //loop over the vec3s
    for (curr = this->vec3Locs.begin(); curr != this->vec3Locs.end(); curr++)
    {
        //pass the vec3s
        glUniform3f(curr->second, this->vec3s[curr->first].x, this->vec3s[curr->first].y, this->vec3s[curr->first].z);
    }
    //pass the costom vec4s
    //loop over the vec4s
    for (curr = this->vec4Locs.begin(); curr != this->vec4Locs.end(); curr++)
    {
        //pass the vec4s
        glUniform4f(curr->second, this->vec4s[curr->first].x, this->vec4s[curr->first].y, this->vec4s[curr->first].z, this->vec4s[curr->first].w);
    }
    //pass the costom mat2s
    //loop over the mat2s
    for (curr = this->mat2Locs.begin(); curr != this->mat2Locs.end(); curr++)
    {
        //pass the mat2s
        glUniformMatrix2fv(curr->second, 1, GL_FALSE, this->mat2s[curr->first].m[0]);
    }
    //pass the costom mat3s
    //loop over the mat3s
    for (curr = this->mat3Locs.begin(); curr != this->mat3Locs.end(); curr++)
    {
        //pass the mat3s
        glUniformMatrix3fv(curr->second, 1, GL_FALSE, this->mat3s[curr->first].m[0]);
    }
    //pass the costom mat4s
    //loop over the mat4s
    for (curr = this->mat4Locs.begin(); curr != this->mat4Locs.end(); curr++)
    {
        //pass the mat4s
        glUniformMatrix4fv(curr->second, 1, GL_FALSE, this->mat4s[curr->first].m[0]);
    }
    //pass the costom textures
    //loop over the textures
    for (curr = this->customTextureLocs.begin(); curr != this->customTextureLocs.end(); curr++)
    {
        //increment the amount of bound textures
        this->boundTextures++;
        //activate the current texture unit
        glActiveTexture(GL_TEXTURE0 + this->boundTextures);
        //bind the texture
        glBindTexture(GL_TEXTURE_2D, this->customTextures[curr->first]);
        //pass the textures
        glUniform1i(curr->second, this->boundTextures);
    }
}

void Material::removeMaterial()
{
    //loop over all bound texture units and bind 0
    for (int i = this->boundTextures; i > 0; i--)
    {
        //bind the current texture unit
        glActiveTexture(GL_TEXTURE0 + i);
        //bind the texture to 0
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    //set the texture unit to 0
    glActiveTexture(GL_TEXTURE0);
}

void Material::setLit(bool l)
{
    //store the lit parameters
    this->lit = l;
}

void Material::setCustomFloat(std::string name, float value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->floats[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the floats
        this->floats[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the floats
        this->floats[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the floats
        this->floats[name] *= value;
        //stop this script
        return;
    }
    //divide the value
    if (mode == GLGE_MODE_DIVIDE)
    {
        //divide the floats
        this->floats[name] /= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"float\"" << "\n";
    }
}

void Material::setCustomInt(std::string name, int value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->integers[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the ints
        this->integers[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the ints
        this->integers[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the ints
        this->integers[name] *= value;
        //stop this script
        return;
    }
    //divide the value
    if (mode == GLGE_MODE_DIVIDE)
    {
        //divide the ints
        this->integers[name] /= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"integer\"" << "\n";
    }
}

void Material::setCustomBool(std::string name, bool value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->booleans[name] = value;
        //stop this script
        return;
    }
    //and the value
    if (mode == GLGE_MODE_AND)
    {
        //and the bools
        this->booleans[name] = this->booleans[name] && value;
        //stop this script
        return;
    }
    //or the value
    if (mode == GLGE_MODE_OR)
    {
        //or the bools
        this->booleans[name] = this->booleans[name] || value;
        //stop this script
        return;
    }
    //not the value
    if (mode == GLGE_MODE_NOT)
    {
        //not the bool
        this->booleans[name] = !value;
        //stop this script
        return;
    }
    //nand the value
    if (mode == GLGE_MODE_NAND)
    {
        //nand the bools
        this->booleans[name] = !(value && this->booleans[name]);
        //stop this script
        return;
    }
    //nor the value
    if (mode == GLGE_MODE_NOR)
    {
        //nor the bools
        this->booleans[name] = !(value || this->booleans[name]);
        //stop this script
        return;
    }
    //xor the value
    if (mode == GLGE_MODE_XOR)
    {
        //xor the bools
        this->booleans[name] = (value ? !this->booleans[name] : this->booleans[name]);
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"boolean\"" << "\n";
    }
}

void Material::setCustomVec2(std::string name, vec2 value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->vec2s[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the vec2s
        this->vec2s[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the vec2s
        this->vec2s[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the vec2s
        this->vec2s[name] = this->vec2s[name].scale(value);
        //stop this script
        return;
    }
    //divide the value
    if (mode == GLGE_MODE_DIVIDE)
    {
        //divide the vec2s
        this->vec2s[name] /= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"vec2\"" << "\n";
    }
}

void Material::setCustomVec3(std::string name, vec3 value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->vec3s[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the vec3s
        this->vec3s[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the vec3s
        this->vec3s[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the vec3s
        this->vec3s[name] = this->vec3s[name].scale(value);
        //stop this script
        return;
    }
    //divide the value
    if (mode == GLGE_MODE_DIVIDE)
    {
        //divide the vec3s
        this->vec3s[name] /= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"vec3\"" << "\n";
    }
}

void Material::setCustomVec4(std::string name, vec4 value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->vec4s[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the vec4s
        this->vec4s[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the vec4s
        this->vec4s[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the vec4s
        this->vec4s[name] = this->vec4s[name].scale(value);
        //stop this script
        return;
    }
    //divide the value
    if (mode == GLGE_MODE_DIVIDE)
    {
        //divide the vec4s
        this->vec4s[name] /= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"vec4\"" << "\n";
    }
}

void Material::setCustomMat2(std::string name, mat2 value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->mat2s[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the mat2s
        this->mat2s[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the mat2s
        this->mat2s[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the mat2s
        this->mat2s[name] *= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"mat2\"" << "\n";
    }
}

void Material::setCustomMat3(std::string name, mat3 value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->mat3s[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the mat3s
        this->mat3s[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the mat3s
        this->mat3s[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the mat3s
        this->mat3s[name] *= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"mat3\"" << "\n";
    }
}

void Material::setCustomMat4(std::string name, mat4 value, unsigned int mode)
{
    //set the value
    if (mode == GLGE_MODE_SET)
    {
        //set the value
        this->mat4s[name] = value;
        //stop this script
        return;
    }
    //add the value
    if (mode == GLGE_MODE_ADD)
    {
        //add the mat4s
        this->mat4s[name] += value;
        //stop this script
        return;
    }
    //subtract the value
    if (mode == GLGE_MODE_SUBTRACT)
    {
        //subtract the mat4s
        this->mat4s[name] -= value;
        //stop this script
        return;
    }
    //multiply the value
    if (mode == GLGE_MODE_MULTIPLY)
    {
        //multiply the mat4s
        this->mat4s[name] *= value;
        //stop this script
        return;
    }
    //if the script reaches this point, the inputed mode is not supported : print a warning

    //check if the warning output is active
    if (glgeWarningOutput)
    {
        //print a warning
        std::cerr << "[GLGE WARNING] Mode \"" << this->getModeString(mode).c_str() << "\" is not supported for a variable of type \"mat4\"" << "\n";
    }
}

void Material::setCustomTexture(std::string name, char* file)
{
    //load the file into the map
    this->customTextures[name] = glgeTextureFromFile(file);
}

void Material::setCustomTexture(std::string name, std::string file)
{
    //load the file into the map
    this->customTextures[name] = glgeTextureFromFile(file.c_str());
}

void Material::setCustomTexture(std::string name, unsigned int texture)
{
    //store the inputed texture pointer
    this->customTextures[name] = texture;
}

std::string Material::getModeString(unsigned int mode)
{
    //check if the mode is set
    if (mode == GLGE_MODE_SET)
    {
        //return the mode string
        return std::string("set");
    }
    //check if the mode is add
    else if (mode == GLGE_MODE_ADD)
    {
        //return the mode string
        return std::string("add");
    }
    //check if the mode is subtract
    else if (mode == GLGE_MODE_SUBTRACT)
    {
        //return the mode string
        return std::string("subtract");
    }
    //check if the mode is multiply
    else if (mode == GLGE_MODE_MULTIPLY)
    {
        //return the mode string
        return std::string("multiply");
    }
    //check if the mode is divide
    else if (mode == GLGE_MODE_DIVIDE)
    {
        //return the mode string
        return std::string("divide");
    }
    //check if the mode is cross
    else if (mode == GLGE_MODE_CROSS)
    {
        //return the mode string
        return std::string("cross");
    }
    //check if the mode is and
    else if (mode == GLGE_MODE_AND)
    {
        //return the mode string
        return std::string("and");
    }
    //check if the mode is or
    else if (mode == GLGE_MODE_OR)
    {
        //return the mode string
        return std::string("or");
    }
    //check if the mode is not
    else if (mode == GLGE_MODE_NOT)
    {
        //return the mode string
        return std::string("not");
    }
    //check if the mode is nand
    else if (mode == GLGE_MODE_NAND)
    {
        //return the mode string
        return std::string("nand");
    }
    //check if the mode is nor
    else if (mode == GLGE_MODE_NOR)
    {
        //return the mode string
        return std::string("nor");
    }
    //check if the mode is xor
    else if (mode == GLGE_MODE_XOR)
    {
        //return the mode string
        return std::string("cor");
    }
    else
    {
        //if the mode can't be found, return an error
        return (std::string("mode ") + std::to_string(mode) + std::string(" dose not exist"));
    }
}

float Material::getFloatByName(std::string name)
{
    //return the wanted element from the map
    return this->floats[name];
}

int Material::getIntByName(std::string name)
{
    //return the wanted element from the map
    return this->integers[name];
}

bool Material::getBoolByName(std::string name)
{
    //return the wanted element from the map
    return this->booleans[name];
}

vec2 Material::getVec2ByName(std::string name)
{
    //return the wanted element from the map
    return this->vec2s[name];
}

vec3 Material::getVec3ByName(std::string name)
{
    //return the wanted element from the map
    return this->vec3s[name];
}

vec4 Material::getVec4ByName(std::string name)
{
    //return the wanted element from the map
    return this->vec4s[name];
}

mat2 Material::getMat2ByName(std::string name)
{
    //return the wanted element from the map
    return this->mat2s[name];
}

mat3 Material::getMat3ByName(std::string name)
{
    //return the wanted element from the map
    return this->mat3s[name];
}

mat4 Material::getMat4ByName(std::string name)
{
    //return the wanted element from the map
    return this->mat4s[name];
}

unsigned int Material::getTextureByName(std::string name)
{
    //return the wanted element from the map
    return this->customTextures[name];
}