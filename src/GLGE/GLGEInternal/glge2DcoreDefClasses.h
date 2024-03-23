/**
 * @file glge2DcoreDefClasses.h
 * @author DM8AT
 * @brief define the default classes for the 2D core
 * @version 0.1
 * @date 2024-01-23
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license.
 * 
 */

#ifndef _GLGE_INTERNAL_2D_CORE_DEF_CLASSES_H_
#define _GLGE_INTERNAL_2D_CORE_DEF_CLASSES_H_

/////////////
// defines //
/////////////

//check if the input for texture coordinates was allready defined
#ifndef GLGE_PRESET_USE_TEXTURE_COORDINATES
//define the input for texture coordinates
#define GLGE_PRESET_USE_TEXTURE_COORDINATES vec4(-1)
#endif //GLGE_PRESET_USE_TEXTURE_COORDINATES generation
//check if the input for a special function was allready defined
#ifndef GLGE_PRESET_USE_SPECIAL
//define the input for presets used for special things (like color gradients)
#define GLGE_PRESET_USE_SPECIAL vec4(-2)
#endif //GLGE_PRESET_USE_SPECIAL define

//check if a define for an empty object allreday exists
#ifndef GLGE_PRESET_EMPTY
//generate an empty object without any mesh
#define GLGE_PRESET_EMPTY 0
#endif //GLGE_PRESET_EMPTY creation
//generate a square
#define GLGE_PRESET_SQUARE 1
//generate a triangle in the unit square
#define GLGE_PRESET_TRIANGLE 2
//check if the circle allready exists
#ifndef GLGE_PRESET_CIRCLE
//genearte an object with an circle as the mesh
#define GLGE_PRESET_CIRCLE 3
#endif // GLGE_PRESET_CIRCLE generation
//generate any filled uniform polygon
#define GLGE_PRESET_POLYGON 4
//generate an arrow
#define GLGE_PRESET_ARROW 5

//needed librarys

//CML
#include "../CML/CML.h"

//Basic C++
#include <vector>
#include <string>

///////////
//STRUCTS//
///////////

/**
 * @brief a transform in 2D space
 */
struct Transform2D
{
    //an 2D position argument
    vec2 pos = vec2(0);
    //the rotation of the object
    float rot = 0;
    //the scaling of the object
    vec2 size = vec2(1,1);

    /**
     * @brief Construct a new Transform 2D
     * 
     * default constructor
     */
    Transform2D();

    /**
     * @brief Construct a new Transform 2D
     * 
     * @param pos the posi
     * @param rot 
     * @param scale 
     */
    Transform2D(vec2 pos, float rot = 0, vec2 scale = vec2(1,1));

    /**
     * @brief Construct a new Transform 2D
     * 
     * @param x the x position of the object
     * @param y the y position of the object
     * @param rot the rotation of the object
     * @param scale the scale  of the object
     */
    Transform2D(float x, float y, float rot = 0, vec2 scale = vec2(1,1));

    /**
     * @brief Get the Matrix for transforming an object to this scale, rot and pos
     * 
     * @return mat3 the transformation matrix
     */
    mat3 getMatrix();
};

/**
 * @brief a vertex in 2D space
 */
struct Vertex2D
{
    //the position of the vertex
    vec2 pos = vec2(0);
    //store a texture coordinate
    vec2 texCoord = vec2(0,0);
    //store a color
    vec4 color = vec4(0,0,0,1);

    /**
     * @brief Construct a new Vertex 2D
     * 
     * basic constructor
     */
    Vertex2D();

    /**
     * @brief Construct a new Vertex 2D
     * 
     * @param pos the position of the vertex
     * @param color the vertex color
     */
    Vertex2D(vec2 pos, vec4 color = vec4(1,1,1,1));

    /**
     * @brief Construct a new Vertex 2D
     * 
     * @param x the positoin on the x axis
     * @param y the positoin on the y axis
     * @param color the vertex color
     */
    Vertex2D(float x, float y, vec4 color = vec4(1,1,1,1));

    /**
     * @brief Construct a new Vertex 2D
     * 
     * @param x the position on the x axis
     * @param y the position on the y axis
     * @param r the amount of red in the color
     * @param g the amount of green in the color
     * @param b the amount of blue in the color
     * @param a the transperency of the color
     */
    Vertex2D(float x, float y, float r, float g, float b, float a);

    /**
     * @brief Construct a new Vertex 2D
     * 
     * @param pos the position of the vertex
     * @param r the amount of red in the color
     * @param g the amount of green in the color
     * @param b the amount of blue in the color
     * @param a the transperency of the color
     */
    Vertex2D(vec2 pos, float r, float g, float b, float a);

    /**
     * @brief Construct a new Vertex 2D
     * 
     * @param pos the position of the vertex
     * @param texCoord the texture coordinates
     */
    Vertex2D(vec2 pos, vec2 texCoord);

    /**
     * @brief Construct a new Vertex 2D
     * 
     * @param x the positoin on the x axis
     * @param y the positoin on the y axis
     * @param texCoord the texture coordinate
     */
    Vertex2D(float x, float y, vec2 texCoord);

    /**
     * @brief Construct a new Vertex 2D
     * 
     * @param pos the position for the vertex
     * @param tX the x part for the texture sampling
     * @param tY the y part for the texture sampling
     */
    Vertex2D(vec2 pos, float tX, float tY);

    /**
     * @brief Construct a new Vertex 2D
     * 
     * @param x the x position of the Vertex
     * @param y the y position of the Vertex
     * @param tX the x part for the texture sampling
     * @param tY the y part for the texture sampling
     */
    Vertex2D(float x, float y, float tX, float tY);
};

///////////
//CLASSES//
///////////

/**
 * @brief store a 2D mesh
 */
class Mesh2D
{
public:
    /**
     * @brief Construct a new Mesh 2D
     * 
     * default constructor
     */
    Mesh2D();

    /**
     * @brief Construct a new Mesh 2D
     * 
     * @param vertices the vertices as an pointer array
     * @param indices the indices as an pointer array
     * @param sizeOfVertices the size of the vertex pointer array
     * @param sizeOfIndices the size of the indices pointer array
     */
    Mesh2D(Vertex2D* vertices, unsigned int* indices, unsigned int sizeOfVertices, unsigned int sizeOfIndices);

    /**
     * @brief Construct a new Mesh 2D
     * 
     * @param vertices the vertices in an std vector
     * @param indices the indices in an std vector
     */
    Mesh2D(std::vector<Vertex2D> vertices, std::vector<unsigned int> indices);

    /**
     * @brief Construct a new Mesh 2D from an preset
     * 
     * @param preset the preset to choose (Starting with GLGE_PRESET_)
     * @param resolution the resolution or scale for the preset
     * @param color the color for the vertices (w = -1 for texture coordinates)
     */
    Mesh2D(unsigned int preset, unsigned int resolution, vec4 color = vec4(1));

    //store the indices of the mesh
    std::vector<unsigned int> indices;
    //store the vertices of the mehs
    std::vector<Vertex2D> vertices; 
};

#endif