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
    vec2 size;

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
     */
    Vertex2D(vec2 pos);

    /**
     * @brief Construct a new Vertex 2D
     * 
     * @param x the positoin on the x axis
     * @param y the positoin on the y axis
     */
    Vertex2D(float x, float y);
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
     * @param faces the faces as an pointer array
     * @param sizeOfVertices the size of the vertex pointer array
     * @param sizeOfFaces the size of the faces pointer array
     */
    Mesh2D(Vertex2D* vertices, Triangle* faces, unsigned int sizeOfVertices, unsigned int sizeOfFaces);

    /**
     * @brief Construct a new Mesh 2D
     * 
     * @param vertices the vertices in an std vector
     * @param faces the faces in an std vector
     */
    Mesh2D(std::vector<Vertex2D> vertices, std::vector<Triangle> faces);

private:
    std::vector<Triangle> faces;
    std::vector<Vertex2D> vertices;

    
};

#endif