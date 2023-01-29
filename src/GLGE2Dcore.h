/**
 * @file GLGE2Dcore.h
 * @author DM8AT
 * @brief the 2D core for GLGE, everything containing 2D games and GUI
 * @version 0.1
 * @date 2023-01-29
 * 
 * @copyright Copyright (c) 2023
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

private:
    std::vector<Triangle> faces;
    std::vector<Vertex2D> vertices;
};

#endif