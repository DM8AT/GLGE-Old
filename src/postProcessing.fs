/**
 * @file postProcessing.fs
 * @author DM8AT
 * @brief This file contains an example post processing shader that just applys some simple tone mapping
 * 
 * @version 0.1
 * @date 2024-07-13
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 */

//every shader has to start with a version declaration. Post-Processing shaders should use 450 core if they are used with the 
//default post processing vertex shder, which is often the case. 
#version 450 core

//a variable can be inputed trough a uniform or directly be constant in the shader. If a variable isn't going to change, 
//decalring it constant in a shader is a lot better for the performance. 
/**
 * @brief this variable stores the gamma value to correct to
 */
const float gamma = 2.2;
/**
 * @brief this variable stores the exposure to adjust the image to
 */
const float exposure = 1.f;

//a post processing shader should sample the main image, wich is a default uniform for post processing shader. 
//if no sampler named 'glgeMainImage' is found, a low priority error is printed wich whon't crash the program. 
uniform sampler2D glgeMainImage;
//the output of a post processing shader is always into one color attachment, so color attachment changing is not
//required
out vec4 FragColor;

/**
 * @brief texCoords is a default variable passed from the vertex shader. It is the texture coordinate to sample a 
 * full-screen image and different for every vertex. 
 */
in vec2 texCoords;

/**
 * @brief this function is the start of the shader. In contrast to C++ it has no return value. 
 */
void main()
{
    //a value can be read from a sampler using the texture coordinate from the vertex shader and the texture directive
    vec3 hdrColor = texture(glgeMainImage, texCoords).rgb;

    //color mapping is than done like in this LearnOpenGL example:
    //https://learnopengl.com/Advanced-Lighting/HDR
    // exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

    //finaly the color is just writen to the output. 
    //during the post-processing pass the alpha value controlls the opacity of the output so 
    //multiple outputs can be mixed. 
    FragColor = vec4(mapped, 1.0);
}