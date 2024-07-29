/**
 * @file glgeInternalFuncs.cpp
 * @author DM8AT
 * @brief define the graphics api independand internal functions for glge
 * @version 0.1
 * @date 2024-01-09
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license.
 */
#include "glgeInternalFuncs.h"

//include the GLGE dependencys
#include "glgeErrors.hpp"
#include "glgeVars.hpp"

//include the default librarys
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

//read a file
bool readFile(const char* filename, std::string& output)
{
    //create a new ifstream object containing the file
    std::fstream f(filename);

    //check if the file can be read
    bool ret = false;

    //check if the file is opend
    if(f.good())
    {
        //create a variable to store a single line
        std::string line;
        //loop over all lines in the file and store them
        while(getline(f, line))
        {
            //add the lines to the output
            output.append(line);
            //add a new line character to the output
            output.append("\n");
        }
        //close the file
        f.close();

        //say that the file could be opend
        ret = true;
    }
    else
    {
        //else print an error message
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_FILE_NOT_FOUND, filename);
        }
        //check if the program should close on an error
        if (glgeExitOnError)
        {
            exit(1);
        }
    }

    //return if the file could be read
    return ret;
}

int count_char(std::string &str, char ch)
{
    //begin the counting at 0
    int c = 0;
    //get the length of the str
    int length = str.length();
    //loop over every item in the string
    for (int i = 0; i < length; i++)
    {
        //if the item of the string is equal to the specified letter, increase c by one
        if (str[i] == ch)
            c++;
    }
    //return c, that contains the amount of ch in str
    return c;
}

std::string precompileShaderSource(std::string source, std::vector<std::string> files)
{
    //store the new source code
    std::string src = source;
    //loop over the file
    for (int i = 0; i < (int)src.length(); i++)
    {
        //check if a # was found
        if (src[i] == '#')
        {
            //store the preprocessing command string
            std::string commandType = std::string("");
            //loop until the end of the file or a new line is found
            for (int j = i; (j < (int)src.length()) && (src[j] != ' ') && (src[j] != '\n'); j++)
            {
                //add the current symbol to the string
                commandType += src[j];
            }

            //store the line end
            int lineEnd = i;
            //store the precompile precessing argument
            std::string commandArgs = std::string("");
            //loop until the end of the file or a new line is found
            for (int j = i+(int)commandType.size(); (j < (int)src.length()) && (src[j] != '\n'); j++)
            {
                //add the current symbol to the string
                commandArgs += src[j];
                //store the line end
                lineEnd = j;
            }
            //delete the first element
            commandArgs.erase(0,1);

            if (commandType == "#include")
            {
                //delete the command
                src.erase(i, (lineEnd-i)+1);

                //check if a file should be included
                if ((commandArgs[0] == '\"') && (commandArgs[(int)commandArgs.length() - 1] == '\"'))
                {
                    //delete the first and last symbol
                    commandArgs.erase(0,1);
                    commandArgs.erase((int)commandArgs.length() - 1, 1);

                    //check if the file is allready included
                    if (std::count(files.begin(), files.end(), commandArgs))
                    {
                        //check if an error should be printed
                        if (glgeErrorOutput)
                        {
                            //print an error
                            std::cerr << "[GLGE ERROR] Recursive include: file " << commandArgs.c_str() << " is allready included in GLSL shader\n";
                        }
                        //check if the program should stop
                        if (glgeExitOnError)
                        {
                            //close the program
                            exit(1);
                        }
                    }
                    else
                    {
                        //store the requested file
                        std::string file = "";
                        //read the requested file
                        if (!readFile(commandArgs.c_str(), file))
                        {
                            //check if an error should be printed
                            if (glgeErrorOutput)
                            {
                                //print an error
                                std::cerr << "[GLGE ERROR] Could not include file " << commandArgs.c_str() << " into GLSL shader\n";
                            }
                            //check if the program should be closed
                            if (glgeExitOnError)
                            {
                                //close the program
                                exit(1);
                            }
                        }
                        else
                        {
                            //add the file to the included files
                            files.push_back(commandArgs);
                            //do the precompile on the new file
                            std::string subsrc = precompileShaderSource(file, files);
                            //insert the subsourcefile
                            src.insert((size_t)i, subsrc);
                        }
                    }
                }
                //check for GLGE default includes
                else if ((commandArgs[0] == '<') && (commandArgs[(int)commandArgs.length() - 1] == '>'))
                {
                    //check if the map contains the thing to include
                    if (glgeIncludeDefaults.find(commandArgs) == glgeIncludeDefaults.end())
                    {
                        //check if an error should be printed
                        if (glgeErrorOutput)
                        {
                            //print an error
                            std::cerr << "[GLGE ERROR] " << commandArgs.c_str() << " dosn't name an include default\n";
                        }
                        //check if the program should be closed
                        if (glgeExitOnError)
                        {
                            //close the program
                            exit(1);
                        }
                    }
                    else
                    {
                        //include the other needed things
                        std::string newSRC = precompileShaderSource(glgeIncludeDefaults[commandArgs], files);
                        //insert the string corresponding to the input
                        src.insert((size_t)i, newSRC);
                    }
                }
                else
                {
                    //check if an error should be printed
                    if (glgeErrorOutput)
                    {
                        //print an error
                        std::cerr << "[GLGE ERROR] invalide argument for precompiler instruction #include\n";
                    }
                    //check if the program should be closed
                    if (glgeExitOnError)
                    {
                        //close the program
                        exit(1);
                    }
                }
            }
        }
    }

    return src;
}

/**
 * @brief flatten an std::vector down to a string
 * 
 * @param strings the strings in an std::vector
 * @return std::string the final string without newlines
 */
std::string flattenString(std::vector<std::string> strings)
{
    //store the final string
    std::string out = "";
    //loop over all elements in the vector
    for (int i = 0; i <  (int)strings.size(); i++)
    {
        //add the string to the output
        out += strings[i];
    }
    //return the combined string
    return out;
}

void loadIncludeDefaults()
{
    //the default for a light structure
    glgeIncludeDefaults["<glgeLightStructure>"] = flattenString({
"\n#ifndef glgeLight\n"
"struct glgeLight",
"{",
"vec4 color;",
"vec3 dir;",
"float intAngle;",
"vec3 pos;",
"float angle;",
"vec4 lightSpaceMatR0;",
"vec4 lightSpaceMatR1;",
"vec4 lightSpaceMatR2;",
"vec4 lightSpaceMatR3;",
"int type;",
"};"
"\n#endif\n"
    });
    //the default structure for a pixel
    glgeIncludeDefaults["<glgePixelStructure>"] = flattenString({
"struct glgePixel",
"{",
"    vec3 color;",
"    vec3 pos;",
"    vec3 normal;",
"    float roughness;",
"    float metallic;",
"};"
    });
    //the light evaluation function default
    glgeIncludeDefaults["<glgeLightEvaluationFunction>"] = flattenString({
"#include <glgeLightStructure>\n",
"#include <glgePixelStructure>\n",
"/*GLGE LIGHT EVALUATION FUNCTION START*/",
"/*attenuation values for the light sources*/",
"/*constant attenuation, isn't influenced by distance*/",
"const float constantAttenuation = 1.f;",
"/*linear attenuation, increases linearly as the distance increases*/",
"const float linearAttenuation = 0.f;",
"/*quadratic attenuation, increses quadraticaly as the distnace increses*/",
"const float quadraticAttenuation = 1.f;",
"float ambient = 0.1;",
"float biasAngle = 0.0005;",
"/* for more information on pbr-lighting see : https://learnopengl.com/PBR/Lighting*/",
"float DistributionGGX(vec3 N, vec3 H, float roughness)",
"{",
"    float a      = roughness*roughness;",
"    float a2     = a*a;",
"    float NdotH  = max(dot(N, H), 0.0);",
"    float NdotH2 = NdotH*NdotH;",
"	",
"    float num   = a2;",
"    float denom = (NdotH2 * (a2 - 1.0) + 1.0);",
"    denom = PI * denom * denom;",
"	",
"    return num / denom;",
"}",
"",
"float GeometrySchlickGGX(float NdotV, float roughness)",
"{",
"    float r = (roughness + 1.0);",
"    float k = (r*r) / 8.0;",
"",
"    float num   = NdotV;",
"    float denom = NdotV * (1.0 - k) + k;",
"	",
"    return num / denom;",
"}",
"float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)",
"{",
"    float NdotV = max(dot(N, V), 0.0);",
"    float NdotL = max(dot(N, L), 0.0);",
"    float ggx2  = GeometrySchlickGGX(NdotV, roughness);",
"    float ggx1  = GeometrySchlickGGX(NdotL, roughness);",
"	",
"    return ggx1 * ggx2;",
"}",
"vec3 fresnelSchlick(float cosTheta, vec3 F0)",
"{",
"    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);",
"}",
"",
"/*Calculate the shadow factor for this pixel*/",
"float glgeCalcShadowFac(glgePixel p, glgeLight l)",
"{",
"    if (l.type != 1) { return 1.f; }",
"    return 1.f;",
"}",
"",
"vec3 glgeEvaluateLight(glgeLight l, glgePixel p)",
"{",
"    vec3 N = normalize(p.normal);",
"    vec3 V = normalize(glgeCameraPos - p.pos);",
"",
"    vec3 F0 = vec3(0.04); ",
"    F0 = mix(F0, p.color, p.metallic);",
"	           ",
"    /* reflectance equation*/",
"    vec3 Lo = vec3(0.0);",
"    /* calculate per-light radiance*/",
"    float dist = 0.f;",
"    vec3 L = vec3(0);",
"    float scale = 1.f;",
"    if (l.type == 0) {",
"        L = normalize(l.pos - p.pos);",
"        dist = length(l.pos - p.pos);",
"    }",
"    else if (l.type == 1) {",
"        float theta = dot(normalize(p.pos - l.pos), normalize(-l.dir));",
"        float epsilon = l.angle - l.intAngle;",
"        scale = clamp((theta - l.angle) / epsilon, 0.f, 1.f);",
"        L = normalize(l.pos - p.pos);",
"        dist = length(l.pos - p.pos);",
"    }",
"    else if (l.type == 2) {",
"        L = normalize(-l.dir);",
"        dist = 1.f;",
"    }",
"    vec3 H = normalize(V + L);",
"    float attenuation = 1.f/(constantAttenuation + linearAttenuation*dist + quadraticAttenuation*(dist * dist));",
"    vec3 radiance     = l.color.rgb * attenuation;        ",
"    ",
"    /* cook-torrance brdf*/",
"    float NDF = DistributionGGX(N, H, p.roughness);        ",
"    float G   = GeometrySmith(N, V, L, p.roughness);      ",
"    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       ",
"    ",
"    vec3 kS = F;",
"    vec3 kD = vec3(1.0) - kS;",
"    kD *= 1.0 - p.metallic;	  ",
"    ",
"    vec3 numerator    = NDF * G * F;",
"    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;",
"    vec3 specular     = numerator / denominator;  ",
"        ",
"    /* add to outgoing radiance Lo*/",
"    float NdotL = max(dot(N, L), 0.0);",
"    Lo += (kD * p.color / PI + specular) * radiance * NdotL * (l.color.w) * scale;",
"    vec3 ambient = vec3(0.03) * p.color * ambient;",
"    vec3 col = ambient + (max(Lo, vec3(0)) * glgeCalcShadowFac(p,l));",
"    return col;",
"}",
"/*GLGE LIGHT EVALUATION FUNCTION END*/"
    });
    //the way to include the camera data
    glgeIncludeDefaults["<glgeCamera>"] = flattenString({
"layout (std140, binding = 1) uniform glgeCameraData",
"{",
"    mat4 glgeCamMat;",
"    mat4 glgeProjMat;",
"    mat4 glgeCamTransfMat;",
"    mat4 glgeCamRotMat;",
"    vec4 glgeCameraPosAndFov;",
"    vec4 glgeCameraRotAndFar;",
"    float glgeNear;",
"};\n",
"float glgeFar = glgeCameraRotAndFar.w;\n",
"vec3 glgeCameraRot = glgeCameraRotAndFar.xyz;\n",
"float glgeFov = glgeCameraPosAndFov.w;\n",
"vec3 glgeCameraPos = glgeCameraPosAndFov.xyz;\n",
    });
    //the way to get the current object data
    glgeIncludeDefaults["<glgeObject>"] = flattenString({
"layout (std140, binding = 0) uniform glgeObjectData",
"{",
"    mat4 glgeModelMat;",
"    mat4 glgeRotMat;",
"    int glgeObjectUUID;",
"};",
    });
    //the way to get the current material data
    glgeIncludeDefaults["<glgeMaterial>"] = flattenString({
"layout (std140, binding = 2) uniform glgeMaterialData",
"{",
"    vec4 glgeColor;",
"    float glgeRoughness;",
"    float glgeMetalic;",
"    float dispStrength;",
"    int minLayers;",
"    int maxLayers;",
"    int binarySteps;",
"    int glgeLit;",
"    int glgeAmbientMapActive;",
"    int glgeNormalMapActive;",
"    int glgeRoughnessMapActive;",
"    int glgeMetalicMapActive;",
"    int glgeDisplacementMapActive;",
"};",
"uniform sampler2D glgeAmbientMap;",
"uniform sampler2D glgeNormalMap;",
"uniform sampler2D glgeRoughnessMap;",
"uniform sampler2D glgeMetalicMap;",
"uniform sampler2D glgeDisplacementMap;",
    });
    //the way to get the light data
    glgeIncludeDefaults["<glgeLights>"] = flattenString({
"#include <glgeLightStructure>\n"
"layout (std140, binding=3) uniform glgeLightData",
"{",
"    int glgeActiveLights;",
"    layout(offset = 16) glgeLight glgeLights[128];",
"};",
    });
    //the structure of a single particle
    glgeIncludeDefaults["<glgeParticle>"] = flattenString({
"struct Particle",
"{",
"    vec3 pos;",
"    vec3 rot;",
"    vec3 scale;",
"    vec3 velocity;",
"    float lifetime;",
"    mat4 modelMat;",
"    mat4 rotMat;"
"};"
    });
    //the way to get the particle data
    glgeIncludeDefaults["<glgeParticles>"] = flattenString({
"struct BuffParticle",
"{",
"    mat4 modelMat;",
"    mat4 rotMat;",
"    float px; float py; float pz;",
"    float rx; float ry; float rz;",
"    float sx; float sy; float sz;",
"    float vx; float vy; float vz;",
"    float lifetime;",
"};",
"layout (std140, binding=4) buffer glgeParticleBuff",
"{",
"    BuffParticle glgeParticles[];",
"};",
    });
    //simple utility functions for particles
    glgeIncludeDefaults["<glgeParticlesUtil>"] = flattenString({
"\n#include <glgeParticles>\n",
"#include <glgeParticle>\n",
"void glgeComputeParticleMatrices(inout BuffParticle part)",
"{",
"    mat4 sMat = mat4(",
"        part.sx, 0,0,0,",
"        0,part.sy, 0,0,",
"        0,0,part.sz, 0,",
"        0,0,0,1",
"    );",
"",
"    mat4 rotXMat = mat4(1, 0,0,0,",
"                        0, cos(part.rx),-sin(part.rx), 0,",
"                        0, sin(part.rx), cos(part.rx), 0,",
"                        0, 0,0,1);",
"",
"    mat4 rotYMat = mat4(cos(part.ry), 0, sin(part.ry), 0,",
"                        0,1,0, 0,",
"                       -sin(part.ry), 0, cos(part.ry), 0,",
"                        0,0,0, 1);",
"",
"    mat4 rotZMat = mat4(cos(part.rz),-sin(part.rz), 0, 0,",
"                        sin(part.rz), cos(part.rz), 0, 0,",
"                        0,0,1, 0,",
"                        0,0,0, 1);",
"",
"    part.rotMat = rotXMat * rotYMat * rotZMat;",
"",
"    mat4 transMat = mat4(",
"        1,0,0,part.px,",
"        0,1,0,part.py,",
"        0,0,1,part.pz,",
"        0,0,0,1",
"    );",
"",
"    part.modelMat = sMat * part.rotMat * transMat;",
"}",
"",
"Particle glgeUnpackParticle(BuffParticle buff)",
"{",
"    return Particle(",
"        vec3(buff.px,buff.py,buff.pz),",
"        vec3(buff.rx,buff.ry,buff.rz),",
"        vec3(buff.sx,buff.sy,buff.sz),",
"        vec3(buff.vx,buff.vy,buff.vz),",
"        buff.lifetime,",
"        buff.modelMat,",
"        buff.rotMat",
"    );",
"}",
"",
"BuffParticle glgePackParticle(Particle part)",
"{",
"    return BuffParticle(",
"        part.modelMat,",
"        part.rotMat,",
"        part.pos.x,part.pos.y,part.pos.z,",
"        part.rot.x,part.rot.y,part.rot.z,",
"        part.scale.x,part.scale.y,part.scale.z,",
"        part.velocity.x,part.velocity.y,part.velocity.z,",
"        part.lifetime",
"    );",
"}"
    });
}