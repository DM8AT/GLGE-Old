/**
 * @file openglGLGEMaterialCore.h
 * @author DM8AT
 * @brief declare the material core of GLGE
 * @version 0.1
 * @date 2024-01-02
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_MATERIAL_CORE_OGL_H_
#define _GLGE_MATERIAL_CORE_OGL_H_

//include the GLGE core
#include "openglGLGETexture.hpp"

//include the needed math librarys
#include "../CML/CML.h"

//include the needed things from the default library
#include <vector>
#include <string>

/**
 * @brief a material, that can be applied to an object to change its look
 */
class Material
{
public:
    Material();

    Material(vec4 color, float roughness = 0.3, bool lit = true, float metalic = 0.0);

    Material(Texture* texture, float roughness = 0.3, bool lit = true, float metalic = 0.0);

    Material(const char* texture, float roughness = 0.3, bool lit = true, float metalic = 0.0);

    ~Material();

    void update();

    void bindToWindow(unsigned int windowId);

    void setColor(vec4 color);

    void setColor(float r, float g, float b, float a);

    vec4 getColor();

    void setRoughness(float roughness);

    float getRoughness();

    void setMetalic(float metalic);

    float getMetalic();

    void setLit(bool lit);

    bool isLit();

    void setAmbientTexture(Texture* texture);

    void setAmbientTexture(const char* texture);

    Texture* getAmbientTexture();

    void setNormalMap(Texture* normalMap);

    void setNormalMap(const char* texture);

    Texture* getNormalMap();

    void setRoughnessMap(Texture* roughnessMap);

    void setRoughnessMap(const char* texture);

    Texture* getRoughnessMap();

    void setMetalicMap(Texture* metalicMap);

    void setMetalicMap(const char* texture);

    Texture* getMetalicMap();

    /**
     * @brief makes this the currently active material
     */
    void apply();

    /**
     * @brief encode the object into some data
     * 
     * @param data the data to encode into
     */
    void encode(Data* data);
    /**
     * @brief decode the object from some data
     * 
     * @param data the data to decode from
     */
    void decode(Data data);

private:
    /**
     * @brief the material data that should be send to the GPU
     */
    struct MaterialData
    {
        //the material color
        float r = 0.8;
        float g = 0.8;
        float b = 0.8;
        float a = 1.0;
        //the material base roughness
        float roughness = 0.3;
        //the material base metalicnes
        float metalic;
        //say if the material is lit
        int lit = 1;
        //store if the ambient texture is active
        int ambientTexActive;
        //store if the normal map is active
        int normalMapActive;
        //store if the roughness map is active
        int roughnessMapActive;
        //store if the metalic map is active
        int metalicMapActive;
        //store the ambient texture
        uint64_t ambientTex;
        //store the normal map texture 
        uint64_t normalMapTex;
        //store the roughness map texture 
        uint64_t roughnessMap;
        //store the metalic map texture 
        uint64_t metalicMap;
    };

    //store a ambient map
    Texture* ambientMap = 0;
    //store a normal map
    Texture* normalMap = 0;
    //store a roughness map
    Texture* roughnessMap = 0;
    //store a metalic map
    Texture* metalicMap = 0;
    //store the window id
    int windowId = -1;
    //store the material data
    MaterialData matData = {};
    //store the ubo
    unsigned int ubo;
    //store if a update is qued
    bool quedUpdate = true;
};


#endif