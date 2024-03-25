/**
 * @file glge2DcoreDefClasses.cpp
 * @author DM8AT
 * @brief declare the graphics api independend functions for the 2D core
 * @version 0.1
 * @date 2024-01-23
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license.
 * 
 */

//include the base
#include "glge2DcoreDefClasses.h"

//include glge dependencys
#include "glgePrivDefines.hpp"
#include "glgeVars.hpp"

//the needed default C++ libs
#include <math.h>
#include <iostream>

///////////////////////
// PRIVATE FUNCTIONS //
///////////////////////

//create a polygon
inline void addCircle(std::vector<Vertex2D>* vertices, std::vector<unsigned int>* indices, unsigned int numbers, vec4 col)
{
    //store the amount of points
    unsigned int points = (numbers == 0) ? 32 : numbers;

    //calculate the angle
    double angle = (2.0*M_PI) / (double)points;

    //store the first index
    unsigned int first_ind = (unsigned int)vertices->size();

    //generate the vertices
    for (int i = 0; i < (int)points; i++)
    {
        //calculate the current angle
        double c_angle = angle * i;
        //store the calculated position
        vec2 p = vec2(std::sin(c_angle), std::cos(c_angle));
        //create the vertex
        Vertex2D vert = Vertex2D(p);
        //check if color should be added
        if (col.w != -1)
        {
            //add the color
            vert.color = col;
        }
        else
        {
            //clear the color
            vert.color = vec4(0);
            //check the texture mode
            //normal circle texturing
            vert.texCoord = vec2(p.x * 0.5 + 0.5, 1.f - (p.y * 0.5 + 0.5));            
        }
        //add the vertex
        vertices->push_back(vert);
    }

    //generate the indices
    for (int i = 0; i < (int)points - 2; i++)
    {
        //create a triangle
        indices->push_back(first_ind);
        indices->push_back(i + 2 + first_ind);
        indices->push_back(i + 1 + first_ind);
    }
}

///////////
//STRUCTS//
///////////

//TRANSFORM

//default constructor
Transform2D::Transform2D()
{
    //init the object
}

//constructor with vector
Transform2D::Transform2D(vec2 pos, float rot, vec2 scale)
{
    //save the inputed attributes
    this->pos = pos;
    this->rot = rot*GLGE_TO_RADIANS;
    this->size = scale;
}

//constructor with floats
Transform2D::Transform2D(float x, float y, float rot, vec2 scale)
{
    //save the inputed attributes and convert the floats to an vector
    this->pos = vec2(x,y);
    this->rot = rot*GLGE_TO_RADIANS;
    this->size = scale;
}

mat3 Transform2D::getMatrix()
{
    //move everything to the position
    mat3 moveMat(1,0,this->pos.x,
                 0,1,this->pos.y,
                 0,0,1);

    //rotate everything correctly
    mat3 rotaMat(std::cos(rot),-std::sin(rot),0,
                 std::sin(rot), std::cos(rot),0,
                 0,             0,            1);

    //scale everything correctly
    mat3 sizeMat(this->size.x, 0, 0,
                 0, this->size.y, 0,
                 0, 0,            1);

    //return the multiplied matrices
    return (moveMat * sizeMat) * rotaMat;
}

//VERTEX2D

//default constructor
Vertex2D::Vertex2D()
{
    //init the object
}

//constructor with vec
Vertex2D::Vertex2D(vec2 p, vec4 color)
{
    //set the position to the input
    this->pos = p;
    //store the inputed color
    this->color = color;
}

//constructor using floats
Vertex2D::Vertex2D(float x, float y, vec4 color)
{
    //set the position to the inputs
    this->pos = vec2(x,y);
    //store the inputed color
    this->color = color;
}

//vertex with a lot of arguments
Vertex2D::Vertex2D(float x, float y, float r, float g, float b, float a)
{
    //set the position to the inputs
    this->pos = vec2(x,y);
    //store the inputed color
    this->color = vec4(r,g,b,a);
}

//vertex with a lot of arguments and an position
Vertex2D::Vertex2D(vec2 pos, float r, float g, float b, float a)
{
    //set the position to the inputs
    this->pos = pos;
    //store the inputed color
    this->color = vec4(r,g,b,a);
}

//constructor with vec
Vertex2D::Vertex2D(vec2 p, vec2 texCoord)
{
    //set the position to the input
    this->pos = p;
    //store the inputed texture coordinate
    this->texCoord = texCoord;
}

//constructor using floats
Vertex2D::Vertex2D(float x, float y, vec2 texCoord)
{
    //set the position to the inputs
    this->pos = vec2(x,y);
    //store the inputed texture coordinate
    this->texCoord = texCoord;
}

//constructor with vector and floates
Vertex2D::Vertex2D(vec2 pos, float tX, float tY)
{
    //store the inputed position
    this->pos = pos;
    //store the inputed texture coordinate
    this->texCoord = vec2(tX,tY);
}

//constructor with a lot of floates
Vertex2D::Vertex2D(float x, float y, float tX, float tY)
{
    //store the inputed position
    this->pos = vec2(x,y);
    //store the inputed texture coordinate
    this->texCoord = vec2(tX,tY);
}

///////////
//CLASSES//
///////////

//MESH2D

//default constructor
Mesh2D::Mesh2D()
{
    //init the object
}

//constructor with pointer array
Mesh2D::Mesh2D(Vertex2D* vertices, unsigned int* indices, unsigned int sizeOfVertices, unsigned int sizeOfIndices)
{
    //convert the pointer arrays to vectors and save them
    this->vertices = std::vector<Vertex2D>(vertices, vertices + (sizeOfVertices/sizeof(vertices[0])));
    this->indices = std::vector<unsigned int>(indices, indices + (sizeOfIndices/sizeof(indices[0])));
}

//constructor with vectors
Mesh2D::Mesh2D(std::vector<Vertex2D> vertices, std::vector<unsigned int> indices)
{
    //save the inputed vectors
    this->vertices = vertices;
    this->indices = indices;
}

//constructor with presets
Mesh2D::Mesh2D(unsigned int preset, unsigned int resolution, vec4 color)
{
    switch (preset)
    {
    case GLGE_PRESET_EMPTY:
        {
            //clear the indices
            this->indices.clear();
            //clear the vertices
            this->vertices.clear();
        }
        break;
    
    case GLGE_PRESET_SQUARE:
        {
            //load a preset of a square
            //store the vertices
            std::vector<Vertex2D> vertices;
            //case for texture coordinates
            if (color.w == -1)
            {
                //create the triangle
                //set the vertices for the triangle
                vertices = {Vertex2D( 1, 1, 1,  0),
                            Vertex2D( 1,-1, 1,  1),
                            Vertex2D(-1, 1, 0,  0),
                            Vertex2D(-1,-1, 0,  1)};
            }
            //specal case
            else if (color.w == -2)
            {
                //create the triangle
                //set the vertices for the triangle
                vertices = {Vertex2D( 1, 1, 1,0,0,1),
                            Vertex2D( 1,-1, 0,1,0,1),
                            Vertex2D(-1, 1, 0,1,0,1),
                            Vertex2D(-1,-1, 0,0,1,1)};
            }
            else
            {
                //create the triangle
                //set the vertices for the triangle
                vertices = {Vertex2D( 1, 1, color),
                            Vertex2D( 1,-1, color),
                            Vertex2D(-1, 1, color),
                            Vertex2D(-1,-1, color)};
            }
            //bind the vertices to an triangle1
            std::vector<unsigned int> indices = {1,0,2, 1,2,3};

            //store the indices
            this->vertices = vertices;
            this->indices = indices;
        }
        break;

    case GLGE_PRESET_TRIANGLE:
        {
            //store the vertices
            std::vector<Vertex2D> vertices;
            //case for texture coordinates
            if (color.w == -1)
            {
                //create the triangle
                //set the vertices for the triangle
                vertices = {Vertex2D(0,1,   0.5,0),
                            Vertex2D(-1,-1, 0,  1),
                            Vertex2D( 1,-1, 1,  1)};
            }
            //specal case
            else if (color.w == -2)
            {
                //create the triangle
                //set the vertices for the triangle
                vertices = {Vertex2D(0,1,   1,0,0,1),
                            Vertex2D(-1,-1, 0,1,0,1),
                            Vertex2D( 1,-1, 0,0,1,1)};
            }
            else
            {
                //create the triangle
                //set the vertices for the triangle
                vertices = {Vertex2D(0,1,   color),
                            Vertex2D(-1,-1, color),
                            Vertex2D( 1,-1, color)};
            }
            //bind the vertices to an triangle1
            std::vector<unsigned int> indices = {0,1,2};

            //store the indices
            this->vertices = vertices;
            this->indices = indices;
        }
        break;

    case GLGE_PRESET_POLYGON:
        {
            //default the resolution to 5
            if (resolution == 0)
            {
                resolution = 5;
            }
            //generate a polygon
            addCircle(&this->vertices, &this->indices, resolution, color);
        }
        break;

    case GLGE_PRESET_CIRCLE:
        {
            //generate a circle
            addCircle(&this->vertices, &this->indices, resolution, color);
        }
        break;

    case GLGE_PRESET_ARROW:
        {
            //create the vertices
            std::vector<Vertex2D> verts = {Vertex2D(-0.17, -1), Vertex2D(0.17, -1 ), Vertex2D(-0.17,0.5),
                                           Vertex2D( 0.17,0.5), Vertex2D(-0.48,0.5), Vertex2D( 0.48,0.5),
                                           Vertex2D(0,1)};


            //loop over all vertices
            for (int i = 0; i < (int)verts.size(); i++)
            {
                //check if color should be added
                if (color.w == -1)
                {
                    //if not, add texcoords
                    verts[i].texCoord = vec2(verts[i].pos.x * 0.5 + 0.5, 1.f - (verts[i].pos.y * 0.5 + 0.5));
                    //remove the color
                    verts[i].color = vec4(0);
                }
                else
                {
                    //add color
                    verts[i].color = color;
                }

                //exclude the first two vertices
                if (i < 2)
                { continue; }

                //move the position by the resolution
                verts[i].pos += vec2(0, resolution);
            }

            //create the indices
            std::vector<unsigned int> inds = {0,1,2, 2,1,3, 4,5,6};

            //load the mesh
            this->vertices = verts;
            this->indices = inds;
        }
        break;

    default:
        {
            //check if an error should print
            if (glgeErrorOutput)
            {
                //print an error message
                std::cerr << "[GLGE ERROR] Invalid preset used to construct a 2D object\n";
            }
            //check if error should stop the program
            if (glgeExitOnError)
            {
                //close the program with an error
                exit(1);
            }
            //stop the function
            return;
        }
        break;
    }
}