/**
 * @file GLGELightingCore.h
 * @author DM8AT
 * @brief the lighting functionality for GLGE
 * @version 0.1
 * @date 2023-02-26
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_LIGHTING_CORE_H_
#define _GLGE_LIGHTING_CORE_H_

//include the needed math librarys
#include "CML/CMLVec3.h"
//include GLEW
#include <GL/glew.h>

//declare the base lighting shaders

//default Lighting, for not so strong devicesred
#define GLGE_LIGHT_SHADER_LOW std::string("#version 300 es\nprecision mediump float;out vec4 FragColor;in vec4 color;in vec2 texCoord;in vec3 normal;in vec3 currentPosition;uniform sampler2D Texture;uniform vec3 cameraPos;vec3 lightColor = vec3(1,1,1);vec3 lightPos = vec3(2,5,0);float ambient = 0.1;float specLight = 0.5;void main(){vec4 col = texture(Texture, texCoord);vec3 Normal = normalize(normal);vec3 lightDir = normalize(lightPos - currentPosition);vec3 viewDir = normalize(cameraPos - currentPosition);float diffuse = max(dot(normal, lightDir), 0.f);vec3 viewDirection = normalize(cameraPos - currentPosition);vec3 reflectionDirection = reflect(-lightDir, Normal);float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16.f);float specular = specAmount * specLight;FragColor = col * vec4(lightColor,1.f) * (diffuse + ambient + specular);FragColor.w = col.w;}")

//define the limit an light sources for the defalut shader
#define GLGE_LIGHT_SOURCE_MAX 255

class shadowCubeMapFBO
{
public:
    shadowCubeMapFBO();

    ~shadowCubeMapFBO();

    bool init(unsigned int size = 1000);

    void bindForWriting(GLenum cubeFace);

    void bindForReading(GLenum textureUnit);

private:

    unsigned int size = 0;
    GLuint fbo = 0;
    GLuint rbo = 0;
    GLuint shadowCubeMap  =0;
    GLuint depth = 0;
};

class Light
{
public:
    /**
     * @brief Construct a new Light source
     * 
     * default constructor
     */
    Light();

    /**
     * @brief Construct a new Light source
     * 
     * @param pos the position of the light source
     * @param color the color of the light source
     * @param intensity the strength of the light source
     */
    Light(vec3 pos, vec3 color = vec3(1,1,1), float intensity = 1.f);

    /**
     * @brief Construct a new Light source
     * 
     * @param x the x position of the light source
     * @param y the y position of the light source
     * @param z the z position of the light source
     * @param r the color on the red chanel
     * @param g the color on the green chanel
     * @param b the color on the blue chanel
     * @param intensity the strength of the light source
     */
    Light(float x, float y, float z, float r = 1, float g = 1, float b = 1, float intensity = 1.f);

    /**
     * @brief position the light new
     * 
     * @param position the new position for the light
     */
    void setPos(vec3 position);

    /**
     * @brief position the light new
     * 
     * @param x the new x position for the light
     * @param y the new y position for the light
     * @param z the new z position for the light
     */
    void setPos(float x, float y, float z);

    /**
     * @brief move the light
     * 
     * @param deltaPosition the difference from the old to the new position
     */
    void move(vec3 deltaPosition);

    /**
     * @brief move the light
     * 
     * @param dX the difference on the x axis
     * @param dY the difference on the y axis
     * @param dZ the difference on the z axis
     */
    void move(float dX, float dY, float dZ);

    /**
     * @brief Get the position from the light
     * 
     * @return vec3 the position of the light
     */
    vec3 getPos();

    /**
     * @brief Set the Color for the light
     * 
     * @param color the new color for the light (x = r, y = g, z = b)
     */
    void setColor(vec3 color);

    /**
     * @brief Set the Color for the light
     * 
     * @param r the red chanel of the new color
     * @param g the green chanel of the new color
     * @param b the blue chanel of the new color
     */
    void setColor(float r, float g, float b);

    /**
     * @brief multiply the old color with the inputed color
     * 
     * @param color the scales for the old color
     */
    void pitchColor(vec3 color);

    /**
     * @brief multiply the old color with the inputed color
     * 
     * @param sR the scale for the old color on the red chanel
     * @param sG the scale for the old color on the green chanel
     * @param sB the scale for the old color on the blue chanel
     */
    void pitchColor(float sR, float sG, float sB);

    /**
     * @brief add the inputed color to the old color
     * 
     * @param color the difference in color
     */
    void addColor(vec3 color);

    /**
     * @brief add the inputed color to the old color
     * 
     * @param dR the difference in color on the red chanel
     * @param dG the difference in color on the green chanel
     * @param dB the difference in color on the blue chanel
     */
    void addColor(float dR, float dG, float dB);

    /**
     * @brief Get the Color from the light
     * 
     * @return vec3 the color of the light (x = r, y = g, z = b)
     */
    vec3 getColor();

    /**
     * @brief Set the Intensity of the light source
     * 
     * @param intensity the new intensity of the light source
     */
    void setIntensity(float intensity);

    /**
     * @brief scale the intensity of the light source
     * 
     * @param s the scale for the intensity
     */
    void scaleIntensity(float s);

    /**
     * @brief add something to the intensity of the light source
     * 
     * @param dIntensity the difference in intensity
     */
    void addIntensity(float dIntensity);

    /**
     * @brief Get the Insensity from the light source
     * 
     * @return float the light intensity
     */
    float getInsensity();

    //store the shadow map of the light source
    shadowCubeMapFBO shadowMap = shadowCubeMapFBO();

private:
    //store the light position
    vec3 pos;
    //store the light color
    vec3 color;
    //store the light intensity
    float lightIntensity;
};

/**
 * @brief store a new global light source in the global light source vector
 * 
 * @param light a pointer to the light to store
 */
void glgeAddGlobalLighSource(Light* light);

#endif