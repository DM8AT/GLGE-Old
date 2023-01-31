/**
 * @file GLGE2Dcore.h
 * @author DM8AT
 * @brief the 2D core for GLGE, everything containing 2D games and GUI
 * @version 0.1
 * @date 2023-01-29
 * 
 * @copyright CC BY SA (see  https://creativecommons.org/licenses/by-sa/4.0/)
 */

#ifndef _GLGE2DCORE_H_
#define _GLGE2DCORE_H_

//needed librarys
//GLGE
//GLGE main core
#include "GLGE.h"

//CML
//a 2D vector
#include "GLGE/CML/CMLVec2.h"
//a 3x3 matrix
#include "GLGE/CML/CMLMat3.h"

//Basic C++
#include <vector>

///////////
//STRUCTS//
///////////

//an 2D transform
struct Transform2D
{
    //an 2D position argument
    vec2 pos;
    //the rotation of the object
    float rot;
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

//a 2D transform
struct Vertex2D
{
    //the position of the vertex
    vec2 pos;
    //store a texture coordinate
    vec2 texCoord = vec2(0,0);
    //store a color
    vec4 color = vec4(0,0,0,0);

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
};

///////////
//CLASSES//
///////////

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

    std::vector<unsigned int> indices;
    std::vector<Vertex2D> vertices; 
};

class Object2D
{
public:
    /**
     * @brief Construct a new Object 2D
     * 
     * default constructor
     */
    Object2D();

    /**
     * @brief Construct a new 2D Object
     * 
     * @param vertices the vertices as an pointer array
     * @param indices the indices as an pointer array
     * @param sizeOfVertices the size of the vertex pointer array
     * @param sizeOfIndices the size of the indices pointer array
     * @param transform the transform of the object
     * @param isStatic say if the object should stay on the same screen position relative to the world
     */
    Object2D(Vertex2D* vertices, unsigned int* indices, unsigned int sizeOfVertices, unsigned int sizeOfIndices, Transform2D transform = Transform2D(), bool isStatic = false);

    /**
     * @brief Construct a new 2D Object
     * 
     * @param vertices the vertices in an std vector
     * @param indices the indices in an std vector
     * @param transform the transform of the object
     * @param isStatic say if the object should stay on the same screen position relative to the world
     */
    Object2D(std::vector<Vertex2D> vertices, std::vector<unsigned int> indices, Transform2D transform = Transform2D(), bool isStatic = false);

    /**
     * @brief draw the object to the screen
     */
    void draw();

private:
    //store the mesh for the object
    Mesh2D mesh;
    //store the transform of the object
    Transform2D transf;
    //store the vertex and index buffer
    GLuint VBO, IBO;
    //save the shader
    GLint shader;
    //store the move matrix location
    GLuint moveMatLoc;
    //the local matrix to make the object correct
    mat3 moveMat = mat3(1,0,0,
                        0,1,0,
                        0,0,1);
    //store a texture
    GLuint texture;
    //say if the object is static
    bool isStatic;

    void createBuffers();
};

#endif