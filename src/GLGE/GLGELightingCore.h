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

//declare the base lighting shaders

//default Lighting, for not so strong devices
#define GLGE_LIGHT_SHADER_LOW std::string("#version 300 es\nprecision mediump float;out vec4 FragColor;in vec4 color;in vec2 texCoord;in vec3 normal;in vec3 currentPosition;uniform sampler2D Texture;uniform vec3 cameraPos;vec3 lightColor = vec3(1,1,1);vec3 lightPos = vec3(2,5,0);float ambient = 0.1;float specLight = 0.5;void main(){vec4 col = texture(Texture, texCoord);vec3 Normal = normalize(normal);vec3 lightDir = normalize(lightPos - currentPosition);vec3 viewDir = normalize(cameraPos - currentPosition);float diffuse = max(dot(normal, lightDir), 0.f);vec3 viewDirection = normalize(cameraPos - currentPosition);vec3 reflectionDirection = reflect(-lightDir, Normal);float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16.f);float specular = specAmount * specLight;FragColor = col * vec4(lightColor,1.f) * (diffuse + ambient + specular);FragColor.w = col.w;}")

class Light
{
public:
    Light();

    Light(vec3 pos, vec3 color = vec3(1,1,1));
};

#endif