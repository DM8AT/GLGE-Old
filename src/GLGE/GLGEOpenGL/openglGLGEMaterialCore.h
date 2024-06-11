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
    /**
     * @brief Default constructor
     */
    Material();

    /**
     * @brief Construct a new Material
     * 
     * @param color the color of the material
     * @param roughness define how much light scatters
     * @param lit say if the material is effected by lighting
     * @param metalic say how much the material is a metal
     */
    Material(vec4 color, float roughness = 0.3, bool lit = true, float metalic = 0.0);

    /**
     * @brief Construct a new Material
     * 
     * @param texture a pointer to an instance of the texture class to use as texture
     * @param roughness define how much light scatters
     * @param lit say if the material is effected by lighting
     * @param metalic say how much the material is a metal
     */
    Material(Texture* texture, float roughness = 0.3, bool lit = true, float metalic = 0.0);

    /**
     * @brief Construct a new Material
     * 
     * @param texture a texture file to load the texture from
     * @param roughness define how much light scatters
     * @param lit say if the material is effected by lighting
     * @param metalic say how much the material is a metal
     */
    Material(const char* texture, float roughness = 0.3, bool lit = true, float metalic = 0.0);

    /**
     * @brief Destroy the Material
     */
    ~Material();

    /**
     * @brief update the material data on the GPU
     */
    void update();

    /**
     * @brief bind the material data to a specific window's OpenGL context
     * 
     * @param windowId the window id to bind to
     */
    void bindToWindow(unsigned int windowId);

    /**
     * @brief Set the Color of the material
     * 
     * @param color the new color
     */
    void setColor(vec4 color);

    /**
     * @brief Set the color of the material
     * 
     * @param r the amount of red for the color
     * @param g the amount of green for the color
     * @param b the amount of blue for the color
     * @param a the alpha value for the color
     */
    void setColor(float r, float g, float b, float a);

    /**
     * @brief Get the color of the material
     * 
     * @return vec4 the color packed into a vec4 (r,g,b,a)
     */
    vec4 getColor();

    /**
     * @brief Set the Roughness for the material
     * 
     * @param roughness the new rougness
     */
    void setRoughness(float roughness);

    /**
     * @brief Get the Roughness from the material
     * 
     * @return float the roughness of the material. Dictates how much light scatters
     */
    float getRoughness();

    /**
     * @brief Set the Metalicness of the material
     * 
     * @param metalic the new metalicness value
     */
    void setMetalic(float metalic);

    /**
     * @brief Get the Metalicness value from the material
     * 
     * @return float the metalicness value
     */
    float getMetalic();

    /**
     * @brief Set if the material is effected by lighting
     * 
     * @param lit the new value for the effect of light on the material
     */
    void setLit(bool lit);

    /**
     * @brief get if the material is effected by lighting
     * 
     * @return true : the material is effected by lighting | 
     * @return false : the material is not effected by lighting
     */
    bool isLit();

    /**
     * @brief Set the Ambient Texture for the material
     * 
     * @param texture a pointer to an instance of the texture class with the ambient texture
     */
    void setAmbientTexture(Texture* texture);

    /**
     * @brief Set the Ambient Texture for the material
     * 
     * @param texture the path to a texture file with the ambient texture
     */
    void setAmbientTexture(const char* texture);

    /**
     * @brief Get the Ambient Texture from the material
     * 
     * @return Texture* a pointer to an instance of the texture class with the ambient texture
     */
    Texture* getAmbientTexture();

    /**
     * @brief Set the Normal Map for the material
     * 
     * @param normalMap a pointer to an instance of the texture class with the normal map
     */
    void setNormalMap(Texture* normalMap);

    /**
     * @brief Set the Normal Map for the material
     * 
     * @param texture the path to a texture file with the normal map
     */
    void setNormalMap(const char* texture);

    /**
     * @brief Get the Normal Map from the material
     * 
     * @return Texture* a pointer to an instance of the texture class with the normal map
     */
    Texture* getNormalMap();

    /**
     * @brief Set the Roughness Map for the material
     * 
     * @param roughnessMap a pointer to an instance of the texture class with the roughness map
     */
    void setRoughnessMap(Texture* roughnessMap);

    /**
     * @brief Set the Roughness Map for the material
     * 
     * @param texture the path to a texture file with teh roughness map
     */
    void setRoughnessMap(const char* texture);

    /**
     * @brief Get the Roughness Map from the material
     * 
     * @return Texture* a pointer to an instance of the texture class with the roughness map
     */
    Texture* getRoughnessMap();

    /**
     * @brief Set the Metalic Map for the material
     * 
     * @param metalicMap a pointer to an instance of the texture class with the metalic map
     */
    void setMetalicMap(Texture* metalicMap);

    /**
     * @brief Set the Metalic Map for the material
     * 
     * @param texture the path to the texture file with the metalic map
     */
    void setMetalicMap(const char* texture);

    /**
     * @brief Get the Metalic Map from the material
     * 
     * @return Texture* a pointer to an instance of the texture class with the metalic map
     */
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