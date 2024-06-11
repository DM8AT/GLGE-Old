/**
 * @file glge3DcoreDefClasses.cpp
 * @author DM8AT
 * @brief define the graphics api independand classes for the 3D Core
 * @version 0.1
 * @date 2024-01-09
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license.
 */
#include "glge3DcoreDefClasses.h"

//include the GLGE dependencys
#include "glgeErrors.hpp"
#include "glgeVars.hpp"
#include "glgePrivDefines.hpp"
#include "glgeInternalFuncs.h"

//include needed CML librarys
#include "../CML/CMLQuaternion.h"

//include the default librarys
#include <iostream>
#include <cstring>
#include <math.h>
#include <sstream>
#include <map>
#include <algorithm>

/////////////////////
//PRIVATE FUNCTIONS//
/////////////////////

/**
 * @brief check if a string contains a double shalsh. This function is used when creating an object from an .obj file
 * 
 * @param str the string to check
 * @return true : the string contains a double slash somewhere | 
 * @return false : the sting dosn't contain a double slash
 */
bool has_double_slash(std::string &str)
{
    //get the length of the stringt and decreese it by 2
    int length = str.length() - 2;
    //loop over every element in the string, but not the last one, since the length is decreesed by two instead of one
    for (int i = 0; i < length; i++)
    {
        //if the item i and the next item are boath a slash, return true
        if (str[i] == '/' && str[i + 1] == '/')
            return true;
    }
    //at this point, the loop hasn't return true, so return false
    return false;
}

/**
 * @brief add a vertex that lies on the unit sphere
 * 
 * @param p the un-normalised vertex position
 * @param color the color of the vertex
 * @param vertices the allready existing vertices to add to
 * @return unsigned int the index if the vertex allready exists
 */
unsigned int addVertexOnUintSphere(Vertex p, vec4 color, std::vector<Vertex>* vertices)
{
    //calculate the distance from the middle of the sphere
    float length = std::sqrt(p.pos.x * p.pos.x + p.pos.y * p.pos.y + p.pos.z * p.pos.z);
    //store the position of the vertex
    vec3 pos = vec3(p.pos.x/length, p.pos.y/length, p.pos.z/length);
    //check if the vertex exists
    for (int i = 0; i < (int)vertices->size(); i++)
    {
        //if it exists, return the index of the vertex
        if ((*vertices)[i].pos == pos)
        {
            return i;
        }
    }

    //use the normalised position to generate the vertex
    Vertex vert = Vertex(pos);

    //check if the color should be stored
    if (color.w == -1)
    {
        //calculate the uv coordinates
        float u = std::atan2(pos.x, pos.z) / (2.f*M_PI) + 0.5;
        float v = pos.y * 0.5 + 0.5;
        //store the uv coordinates
        vert.texCoord = vec2(u,v);
        //set the color to black
        vert.color = vec4(0,0,0,1);
    }
    else
    {
        //store the color
        vert.color = color;
    }
    //the normal is just the position of the vertex
    vert.normal = vert.pos;
    //store the vertex
    vertices->push_back(vert);
    //return the index of the vertex
    return (unsigned int) vertices->size()-1;
}

/**
 * @brief add a polygon / circle to an allready existing mesh
 * 
 * @param vertices the vertices of the allready existing mesh
 * @param indices the indices of the allready existing mesh
 * @param numbers the number of points for the polygon / the resolution of the circle
 * @param col the color for the polygon / circle. Use vec4(-1) for texture coordinates
 * @param pos a position offset for all vertices
 * @param flip say if the circle / polygon should be flipped in some way
 * @param fill say if the circle / polygon should be filled using triangulation
 * @param sidewaysNormals say if the normals should point sideways rather than upwards
 * @param texOffset a texture coordinate offset
 * @param textureMode the mode of the texture coordinates
 */
inline void addCircle(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, unsigned int numbers, vec4 col, vec3 pos = vec3(0), bool flip = false,
                      bool fill = true, bool sidewaysNormals = false, vec2 texOffset = vec2(0,0), unsigned int textureMode = 0)
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
        vec3 p = vec3(std::sin(c_angle), 0, std::cos(c_angle)) + pos;
        //create the vertex
        Vertex vert = Vertex(p);
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
            if (textureMode == 0)
            {
                //normal circle texturing
                vert.texCoord = vec2(p.x * 0.5 + 0.5, p.z * 0.5 + 0.5) + texOffset;
            }
            else if (textureMode == 1)
            {
                //calculate u
                float u = (float)i*2 / (float)points;
                //check if u is larger than 1
                if (u > 1.f)
                {
                    //then set u to 2 - u
                    u = 2.f - u;
                }
                //1D circle wrapping
                vert.texCoord = vec2(u, 0) + texOffset;
            }
            
        }
        //check if the normals should be sideways
        if (sidewaysNormals)
        {
            //add a sideways normal
            vert.normal = vert.pos;
            vert.normal.normalize();
        }
        else
        {
            //add the normal vector
            vert.normal = vec3(0, flip ? -1 : 1, 0);
        }
        //add the vertex
        vertices->push_back(vert);
    }

    //check if indices should generate
    if (!fill)
    {
        //if not, stop the function
        return;
    }

    //generate the indices
    for (int i = 0; i < (int)points - 2; i++)
    {
        //create a triangle
        indices->push_back(first_ind);
        if (flip)
        {
            indices->push_back(i + 1 + first_ind);
            indices->push_back(i + 2 + first_ind);
        }
        else
        {
            indices->push_back(i + 2 + first_ind);
            indices->push_back(i + 1 + first_ind);
        }
    }
}

//////////////////////
//PRIAVATE VAIRABLES//
//////////////////////

////////////////////////////
//Decalrations for Structs//
////////////////////////////

/////////////
//TRANSFORM//
/////////////

//default constructor
Transform::Transform()
{
    //initalise the object, say that it exists
    this->scale = vec3(1,1,1);
}

//constructor using a position, rotation and a scale on 3 axis
Transform::Transform(vec3 pos, vec3 rot, vec3 scale)
{
    //set the values to the input
    this->pos = pos;
    //set the rotation to the rotation in radians
    this->rot.x = rot.x*GLGE_TO_RADIANS;
    this->rot.y = rot.y*GLGE_TO_RADIANS;
    this->rot.z = rot.z*GLGE_TO_RADIANS;
    this->scale = scale;
}

//constructor using a position, rotation and a uniform scale
Transform::Transform(vec3 pos, vec3 rot, float scale)
{
    //set position amd rotation to the inputs
    this->pos = pos;
    //set the rotation to the rotation in radians
    this->rot.x = rot.x*GLGE_TO_RADIANS;
    this->rot.y = rot.y*GLGE_TO_RADIANS;
    this->rot.z = rot.z*GLGE_TO_RADIANS;
    //set the scale to the input scale on all 3 axis
    this->scale = vec3(scale,scale,scale);
}

//get the transformation matrix for this point
mat4 Transform::getMatrix()
{
    //calculate the translation matrix
    mat4 transl = mat4(1, 0, 0, this->pos.x,
                       0, 1, 0, this->pos.y,
                       0, 0, 1, this->pos.z,
                       0, 0, 0, 1);
    
    //rotation on x axis
    mat4 rotXMat = mat4(1, 0,                     0,                     0,
                        0, std::cos(this->rot.x),-std::sin(this->rot.x), 0,
                        0, std::sin(this->rot.x), std::cos(this->rot.x), 0,
                        0, 0,                     0,                     1);

    //rotation on y axis
    mat4 rotYMat = mat4(std::cos(this->rot.y), 0, std::sin(this->rot.y), 0,
                        0,                     1,                     0, 0,
                       -std::sin(this->rot.y), 0, std::cos(this->rot.y), 0,
                        0,                     0,                     0, 1);

    //rotation on z axis
    mat4 rotZMat = mat4(std::cos(this->rot.z),-std::sin(this->rot.z), 0, 0,
                        std::sin(this->rot.z), std::cos(this->rot.z), 0, 0,
                        0,                     0,                     1, 0,
                        0,                     0,                     0, 1);

    //scaling matrix
    mat4 scaleMat = mat4(this->scale.x, 0, 0, 0,
                         0, this->scale.y, 0, 0,
                         0, 0, this->scale.z, 0,
                         0, 0, 0,             1);

    //return a multiplycation of the matrices
    return transl * scaleMat * ((rotXMat * rotYMat) * rotZMat);
}

mat4 Transform::getRotationMatrix()
{
    //rotation on x axis
    mat4 rotXMat = mat4(1, 0,                     0,                     0,
                        0, std::cos(this->rot.x),-std::sin(this->rot.x), 0,
                        0, std::sin(this->rot.x), std::cos(this->rot.x), 0,
                        0, 0,                     0,                     1);

    //rotation on y axis
    mat4 rotYMat = mat4(std::cos(this->rot.y), 0, std::sin(this->rot.y), 0,
                        0,                     1,                     0, 0,
                       -std::sin(this->rot.y), 0, std::cos(this->rot.y), 0,
                        0,                     0,                     0, 1);

    //rotation on z axis
    mat4 rotZMat = mat4(std::cos(this->rot.z),-std::sin(this->rot.z), 0, 0,
                        std::sin(this->rot.z), std::cos(this->rot.z), 0, 0,
                        0,                     0,                     1, 0,
                        0,                     0,                     0, 1);

    //return the product of the matrices
    return (rotXMat * rotYMat) * rotZMat;
}

//////////
//Vertex//
//////////

//default constructor
Vertex::Vertex()
{
    //initalise the vertex
}

//constructor using vector
Vertex::Vertex(vec3 pos, vec4 color)
{
    //store the inputed position
    this->pos = pos;
    //set a white color
    this->color = color;
    //set the texture coordinate to 0,0
    this->texCoord = vec2(0,0);
}

//constructor using floates
Vertex::Vertex(float x, float y, float z, vec4 color)
{
    //store the inputed position
    this->pos = vec3(x,y,z);
    //set a white color
    this->color = color;
    //set the texture coordinate to 0,0
    this->texCoord = vec2(0,0);
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float a)
{
    //store the inputed position
    this->pos = vec3(x,y,z);
    //set a white color
    this->color = vec4(r,g,b,a);
    //set the texture coordinate to 0,0
    this->texCoord = vec2(0,0);
}

//constructor using vector
Vertex::Vertex(vec3 pos, vec2 texCoord)
{
    //store the inputed position
    this->pos = pos;
    //store the inputed tex coord
    this->texCoord = texCoord;
    //set the color transparent
    this->color = vec4(0,0,0,0);
}

//constructor using floates
Vertex::Vertex(float x, float y, float z, vec2 texCoord)
{
    //store the inputed position
    this->pos = vec3(x,y,z);
    //store the inputed tex coord
    this->texCoord = texCoord;
    //set the color transparent
    this->color = vec4(0,0,0,0);
}

Vertex::Vertex(float x, float y, float z, float tX, float tY)
{
    //store the inputed position
    this->pos = vec3(x,y,z);
    //store the inputed tex coord
    this->texCoord = vec2(tX, tY);
    //set the color transparent
    this->color = vec4(0,0,0,0);
}

Vertex::Vertex(vec3 p, vec3 n)
{
    //store the inputed values
    this->pos = p;
    this->normal = n;
}

Vertex::Vertex(vec3 p, vec2 t, vec3 n)
{
    //store the inputed values
    this->pos = p;
    this->texCoord = t;
    this->normal = n;
}

//a lot of floats
Vertex::Vertex(float x, float y, float z, float tx, float ty, float nx, float ny, float nz)
{
    //store the inputed position values
    this->pos = vec3(x,y,z);
    //store the inputed texture coordinats
    this->texCoord = vec2(tx, ty);
    //store and normalise the inputed normal
    this->normal = vec3(nx,ny,nz);
    this->normal.normalize();
}

//10 floats
Vertex::Vertex(float x, float y, float z, float r, float g, float b, float a, float nx, float ny, float nz)
{
    //store the inputed position values
    this->pos = vec3(x,y,z);
    //store the inputed color
    this->color = vec4(r,g,b,a);
    //store and normalise the inputed normal
    this->normal = vec3(nx,ny,nz);
    this->normal.normalize();
}

//6 floats + vec4
Vertex::Vertex(float x, float y, float z, vec4 color, float nx, float ny, float nz)
{
    //pass to another constructor
    *this = Vertex(x,y,z, color.x, color.y, color.z, color.w, nx,ny,nz);
}

///////////
//CLASSES//
///////////

////////
//MESH//
////////

//default constructor
Mesh::Mesh()
{
    //init the object
}

Mesh::Mesh(unsigned int preset, vec4 color, unsigned int resolution)
{
    //store the indices for the object
    std::vector<unsigned int> indices = {};
    //store the vertices for the object
    std::vector<Vertex> vertices = {};
    
    //check the type of used preset
    switch (preset)
    {
    //check for empty objects
    case GLGE_PRESET_EMPTY:
        {
            //clear the indices
            indices.clear();
            //clear the vertices
            vertices.clear();

            //store the vertices
            this->vertices = vertices;
            //store the indices
            this->indices = indices;
        }
        break;

    //generate a cube
    case GLGE_PRESET_CUBE:
        {
            //set the indices for the cube
            indices = {
                //front and back
                0,2,1,
                1,2,3,
                4,5,6,
                5,7,6,
                //top and bottom
                0+8,1+8,4+8,
                1+8,5+8,4+8,
                2+8,6+8,3+8,
                3+8,6+8,7+8,
                //left and right
                0+16,4+16,2+16,
                2+16,4+16,6+16,
                1+16,3+16,5+16,
                3+16,7+16,5+16
            };

            //check if texture or color should be used
            if (color.w == -1)
            {
                //use texture coordinates

                //load the vertices from an table
                vertices = {
                                 //pos   //tex   //normal
                         //front and back
                         Vertex(-1,-1, 1,  0,0,  0, 0, 1),
                         Vertex( 1,-1, 1,  1,0,  0, 0, 1),
                         Vertex(-1, 1, 1,  0,1,  0, 0, 1),
                         Vertex( 1, 1, 1,  1,1,  0, 0, 1),

                         Vertex(-1,-1,-1,  1,0,  0, 0,-1),
                         Vertex( 1,-1,-1,  0,0,  0, 0,-1),
                         Vertex(-1, 1,-1,  1,1,  0, 0,-1),
                         Vertex( 1, 1,-1,  0,1,  0, 0,-1),

                         //top and bottom
                         Vertex(-1,-1, 1,  0,1,  0,-1, 0),
                         Vertex( 1,-1, 1,  1,1,  0,-1, 0),
                         Vertex(-1, 1, 1,  0,0,  0, 1, 0),
                         Vertex( 1, 1, 1,  1,0,  0, 1, 0),

                         Vertex(-1,-1,-1,  0,0,  0,-1, 0),
                         Vertex( 1,-1,-1,  1,0,  0,-1, 0),
                         Vertex(-1, 1,-1,  0,1,  0, 1, 0),
                         Vertex( 1, 1,-1,  1,1,  0, 1, 0),

                         //left and right
                         Vertex(-1,-1, 1,  1,0, -1, 0, 0),
                         Vertex( 1,-1, 1,  0,0,  1, 0, 0),
                         Vertex(-1, 1, 1,  1,1, -1, 0, 0),
                         Vertex( 1, 1, 1,  0,1,  1, 0, 0),

                         Vertex(-1,-1,-1,  0,0, -1, 0, 0),
                         Vertex( 1,-1,-1,  1,0,  1, 0, 0),
                         Vertex(-1, 1,-1,  0,1, -1, 0, 0),
                         Vertex( 1, 1,-1,  1,1,  1, 0, 0)
                };
            }
            else
            {
                //use color

                //load the vertices
                //load the vertices from an table
                vertices = {
                                 //pos   //color   //normal
                         //front and back
                         Vertex(-1,-1, 1,  color,  0, 0, 1),
                         Vertex( 1,-1, 1,  color,  0, 0, 1),
                         Vertex(-1, 1, 1,  color,  0, 0, 1),
                         Vertex( 1, 1, 1,  color,  0, 0, 1),

                         Vertex(-1,-1,-1,  color,  0, 0,-1),
                         Vertex( 1,-1,-1,  color,  0, 0,-1),
                         Vertex(-1, 1,-1,  color,  0, 0,-1),
                         Vertex( 1, 1,-1,  color,  0, 0,-1),

                         //top and bottom
                         Vertex(-1,-1, 1,  color,  0,-1, 0),
                         Vertex( 1,-1, 1,  color,  0,-1, 0),
                         Vertex(-1, 1, 1,  color,  0, 1, 0),
                         Vertex( 1, 1, 1,  color,  0, 1, 0),

                         Vertex(-1,-1,-1,  color,  0,-1, 0),
                         Vertex( 1,-1,-1,  color,  0,-1, 0),
                         Vertex(-1, 1,-1,  color,  0, 1, 0),
                         Vertex( 1, 1,-1,  color,  0, 1, 0),

                         //left and right
                         Vertex(-1,-1, 1,  color, -1, 0, 0),
                         Vertex( 1,-1, 1,  color,  1, 0, 0),
                         Vertex(-1, 1, 1,  color, -1, 0, 0),
                         Vertex( 1, 1, 1,  color,  1, 0, 0),

                         Vertex(-1,-1,-1,  color, -1, 0, 0),
                         Vertex( 1,-1,-1,  color,  1, 0, 0),
                         Vertex(-1, 1,-1,  color, -1, 0, 0),
                         Vertex( 1, 1,-1,  color,  1, 0, 0)
                };
            }
            //store the vertices
            this->vertices = vertices;
            //store the indices
            this->indices = indices;
        }
        break;

    //generate a circle
    case GLGE_PRESET_CIRCLE:
        {
            //clear the indices and vertices
            this->vertices.clear();
            this->indices.clear();
            //add a circle
            addCircle(&this->vertices, &this->indices, resolution, color);
        }
        break;
    
    //generate a plane
    case GLGE_PRESET_PLANE:
        {
            //generate the indices
            indices = {0,1,2,
                       2,1,3};

            //check if color should be used
            if (color.w == -1)
            {
                //use texture coordinates

                vertices = {
                    Vertex( 1,0, 1,  1,1,  0,1,0), Vertex(-1,0, 1,  0,1,  0,1,0),
                    Vertex( 1,0,-1,  1,0,  0,1,0), Vertex(-1,0,-1,  0,0,  0,1,0)
                };
            }
            else
            {
                //use the color parameter

                vertices = {
                    Vertex( 1,0, 1,  color,  0,1,0), Vertex(-1,0, 1,  color,  0,1,0),
                    Vertex( 1,0,-1,  color,  0,1,0), Vertex(-1,0,-1,  color,  0,1,0)
                };
            }
            //store the vertices
            this->vertices = vertices;
            //store the indices
            this->indices = indices;
        }
        break;

    //generate a uv sphere
    case GLGE_PRESET_SPHERE:
        {
            //generate the list of vertices
            std::vector<Vertex> vertices;
            //generate the list of indices
            std::vector<unsigned int> indices;

            //thanks to Pickachuxxxx for the math. Source : https://gist.github.com/Pikachuxxxx/5c4c490a7d7679824e0e18af42918efc

            //check for 0 to replace it with the default
            if (resolution == 0)
            {
                //use 16 as default
                resolution = 16;
            }

            int latitudes = resolution;
            int longitudes = resolution;

            float radius = 1.f;

            float nx, ny, nz, lengthInv = 1.0f / radius;
            // Temporary vertex

            float deltaLatitude = M_PI / latitudes;
            float deltaLongitude = 2 * M_PI / longitudes;
            float latitudeAngle;
            float longitudeAngle;

            // Compute all vertices first except normals
            for (int i = 0; i <= latitudes; i++)
            {
                latitudeAngle = M_PI / 2 - i * deltaLatitude; /* Starting -pi/2 to pi/2 */
                float xy = radius * cosf(latitudeAngle);    /* r * cos(phi) */
                float z = radius * sinf(latitudeAngle);     /* r * sin(phi )*/

                /*
                * We add (latitudes + 1) vertices per longitude because of equator,
                * the North pole and South pole are not counted here, as they overlap.
                * The first and last vertices have same position and normal, but
                * different tex coords.
                */
                for (int j = 0; j <= longitudes; ++j)
                {
                    longitudeAngle = j * deltaLongitude;

                    Vertex vertex;
                    vertex.pos.x = xy * cosf(longitudeAngle);       /* x = r * cos(phi) * cos(theta)  */
                    vertex.pos.z = xy * sinf(longitudeAngle);       /* y = r * cos(phi) * sin(theta) */
                    vertex.pos.y = z;                               /* z = r * sin(phi) */
                    if (color.w == -1)
                    {
                        vertex.texCoord.x = (float)j/longitudes;        /* s */
                        vertex.texCoord.y = (float)i/latitudes;         /* t */
                    }
                    else
                    {
                        vertex.color = color;
                    }

                    // normalized vertex normal
                    nx = vertex.pos.x * lengthInv;
                    ny = vertex.pos.z * lengthInv;
                    nz = vertex.pos.y * lengthInv;
                    vertex.normal = vec3(nx, ny, nz);
                    vertices.push_back(vertex);
                }
            }

            /*
            *  Indices
            *  k1--k1+1
            *  |  / |
            *  | /  |
            *  k2--k2+1
            */
            unsigned int k1, k2;
            for(int i = 0; i < latitudes; ++i)
            {
                k1 = i * (longitudes + 1);
                k2 = k1 + longitudes + 1;
                // 2 Triangles per latitude block excluding the first and last longitudes blocks
                for(int j = 0; j < longitudes; ++j, ++k1, ++k2)
                {
                    if (i != 0)
                    {
                        indices.push_back(k1);
                        indices.push_back(k2);
                        indices.push_back(k1 + 1);
                    }

                    if (i != (latitudes - 1))
                    {
                        indices.push_back(k1 + 1);
                        indices.push_back(k2);
                        indices.push_back(k2 + 1);
                    }
                }
            }

            //store the final lists
            this->vertices = vertices;
            this->indices = indices;
        }
        break;

    case GLGE_PRESET_ICOSPHERE:
        {
            //check for 0 to replace it with the default
            if (resolution == 0)
            {
                //use 1 as default
                resolution = 1;
            }

            // create 20 triangles of the icosahedron
            std::vector<Vertex> vertices;

            // create 12 vertices of a icosahedron
            float t = (1.0 + std::sqrt(5.0)) / 2.0;

            addVertexOnUintSphere(Vertex(-1,  t,  0), color, &vertices);
            addVertexOnUintSphere(Vertex( 1,  t,  0), color, &vertices);
            addVertexOnUintSphere(Vertex(-1, -t,  0), color, &vertices);
            addVertexOnUintSphere(Vertex( 1, -t,  0), color, &vertices);

            addVertexOnUintSphere(Vertex( 0, -1,  t), color, &vertices);
            addVertexOnUintSphere(Vertex( 0,  1,  t), color, &vertices);
            addVertexOnUintSphere(Vertex( 0, -1, -t), color, &vertices);
            addVertexOnUintSphere(Vertex( 0,  1, -t), color, &vertices);

            addVertexOnUintSphere(Vertex( t,  0, -1), color, &vertices);
            addVertexOnUintSphere(Vertex( t,  0,  1), color, &vertices);
            addVertexOnUintSphere(Vertex(-t,  0, -1), color, &vertices);
            addVertexOnUintSphere(Vertex(-t,  0,  1), color, &vertices);

            //add the indices
            std::vector<unsigned int> indices;

            // 5 faces around point 0
            indices.push_back(11); indices.push_back(0); indices.push_back(5);
            indices.push_back(5); indices.push_back(0); indices.push_back(1);
            indices.push_back(1); indices.push_back(0); indices.push_back(7);
            indices.push_back(7); indices.push_back(0); indices.push_back(10);
            indices.push_back(10); indices.push_back(0); indices.push_back(11);

            // 5 adjacent faces
            indices.push_back(5); indices.push_back(1); indices.push_back(9);
            indices.push_back(11); indices.push_back(5); indices.push_back(4);
            indices.push_back(10); indices.push_back(11); indices.push_back(2);
            indices.push_back(7); indices.push_back(10); indices.push_back(6);
            indices.push_back(1); indices.push_back(7); indices.push_back(8);

            // 5 faces around point 3
            indices.push_back(9); indices.push_back(3); indices.push_back(4);
            indices.push_back(4); indices.push_back(3); indices.push_back(2);
            indices.push_back(2); indices.push_back(3); indices.push_back(6);
            indices.push_back(6); indices.push_back(3); indices.push_back(8);
            indices.push_back(8); indices.push_back(3); indices.push_back(9);

            // 5 adjacent faces
            indices.push_back(9); indices.push_back(4); indices.push_back(5);
            indices.push_back(4); indices.push_back(2); indices.push_back(11);
            indices.push_back(2); indices.push_back(6); indices.push_back(10);
            indices.push_back(6); indices.push_back(8); indices.push_back(7);
            indices.push_back(8); indices.push_back(9); indices.push_back(1);

            // refine triangles
            for (int i = 0; i < (int)resolution; i++)
            {
                std::vector<unsigned int> newIndices;

                for (int t = 0; t < (int)indices.size(); t += 3)
                {
                    Vertex a = vertices[indices[t]];
                    Vertex b = vertices[indices[t+1]];
                    Vertex c = vertices[indices[t+2]];

                    vec3 m0 = (a.pos + b.pos) / vec3(2);
                    vec3 m1 = (b.pos + c.pos) / vec3(2);
                    vec3 m2 = (c.pos + a.pos) / vec3(2);

                    unsigned int m0i = addVertexOnUintSphere(m0, color, &vertices);
                    unsigned int m1i = addVertexOnUintSphere(m1, color, &vertices);
                    unsigned int m2i = addVertexOnUintSphere(m2, color, &vertices);

                    newIndices.push_back(indices[t  ]);newIndices.push_back(m0i);newIndices.push_back(m2i);
                    newIndices.push_back(indices[t+1]);newIndices.push_back(m1i);newIndices.push_back(m0i);
                    newIndices.push_back(indices[t+2]);newIndices.push_back(m2i);newIndices.push_back(m1i);
                    newIndices.push_back(m0i);newIndices.push_back(m1i);newIndices.push_back(m2i);
                }
                indices.clear();
                indices = newIndices;
            }

            //store the generated icosphere
            this->vertices = vertices;
            this->indices = indices;
        }
        break;

    //add a cone
    case GLGE_PRESET_CONE:
        {
            //first, add a circle
            addCircle(&this->vertices, &this->indices, resolution, color, vec3(0,-1,0), true, true, true);
            //add one vertex on top
            //create the vertex at the position
            Vertex vert = Vertex(vec3(0,1,0), vec2(0.5,1), vec3(0,1,0));
            //check if color should be added
            if (color.w != -1)
            {
                //add the color
                vert.color = color;
            }
            //store the index of this vertex
            unsigned int ver_ind = (int)this->vertices.size();
            //add the vertex
            this->vertices.push_back(vert);
            //loop over all vertices
            for (int i = 0; i < (int)ver_ind - 1; i++)
            {
                //add a triangle
                this->indices.push_back(ver_ind);
                this->indices.push_back(i+1);
                this->indices.push_back(i);
            }
            //add the last triangle
            this->indices.push_back(ver_ind);
            this->indices.push_back(0);
            this->indices.push_back(ver_ind-1);
        }
        break;

    case GLGE_PRESET_CYLINDER:
        {
            //first, add a circle
            addCircle(&this->vertices, &this->indices, resolution, color, vec3(0,-1,0), true, true, true);
            //store the amount of vertices per circle
            int circAmount = (int)this->vertices.size();
            //then, add a second circle
            addCircle(&this->vertices, &this->indices, resolution, color, vec3(0,1,0), false, true, true);
            //add a circle of vertices for texture coordinates
            addCircle(&this->vertices, &this->indices, resolution, color, vec3(0,-1,0), false, false, true, vec2(0), 1);
            //add an unfilled circle
            addCircle(&this->vertices, &this->indices, resolution, color, vec3(0,1,0), false, false, true, vec2(0,1), 1);

            //start to fill the edges
            for (int i = (int)this->vertices.size() - circAmount*2; i < (int)this->vertices.size()-(circAmount+1); i++)
            {
                //add a quad
                //add the first tirangle
                this->indices.push_back(i+1);
                this->indices.push_back(i);
                this->indices.push_back(i + circAmount);
                //add the second triangle
                this->indices.push_back(i+1+circAmount);
                this->indices.push_back(i+1);
                this->indices.push_back(i+circAmount);
            }
            int i = (int)this->vertices.size() - circAmount*2;
            //add the last quad
            //add the first tirangle
            this->indices.push_back(i);
            this->indices.push_back(i+(circAmount-1));
            this->indices.push_back(i + circAmount);
            //add the second triangle
            this->indices.push_back(i+(circAmount-1));
            this->indices.push_back(i+(circAmount*2 - 1));
            this->indices.push_back(i+circAmount);
        }
        break;
    
    //default / error case
    default:
        {
            //check if an error should print
            if (glgeErrorOutput)
            {
                //print an error message
                std::cerr << "[GLGE ERROR] Invalid preset used to construct an 3D object\n";
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

//pointer array constructor
Mesh::Mesh(Vertex* vertices, unsigned int* indices, unsigned int sizeVertices, unsigned int sizeIndices)
{
    //convert the pointer arrays to vectors and save them
    this->vertices = std::vector<Vertex>(vertices, vertices + (sizeVertices/sizeof(vertices[0])));
    this->indices = std::vector<unsigned int>(indices, indices + (sizeIndices/sizeof(indices[0])));
}

//vector constructor
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    //save the inputed vectors
    this->vertices = vertices;
    this->indices = indices;
}

Mesh::Mesh(std::string data, int type)
{
    if (type != GLGE_OBJ)
    {
        std::cerr << "Currently, only .obj files are supported" << "\n";
        return;
    }

    //seperate the data into the lines
    std::istringstream iss(data);

    //create an std::vector of 3D vectors for the vertices
    std::vector<vec3> verts;
    //create an vector to store the texture coordinates
    std::vector<vec2> tex;
    //create an vector to store the vertex normals
    std::vector<vec3> normals;

    //create an dictionary to store all created vectors
    std::map<std::string, std::pair<Vertex, unsigned int>> cverts;

    //loop over every line in the data
    for (std::string line; std::getline(iss, line); )
    {
        //check if the line starts with an v (v stands for Vertex)
        if (line[0] == 'v')
        {
            //if the line starts only with an v, it stores vertex position coordinates
            if (line[1] == ' ')
            {
                //create three floats to store the position
                float x,y,z;
                //load the position from the string into the floats
                sscanf(line.c_str(), "v %f %f %f", &x,&y,&z);
                //store the float as an new vertex
                verts.push_back(vec3(x,y,z));
            }
            //if the line starts with an vt it stores vertex texture coordinates
            else if (line[1] == 't')
            {
                //create two floats to store the texture coordinats
                float x,y;
                //load the texture coordinats from the line
                sscanf(line.c_str(), "vt %f %f", &x, &y);
                //store the texture coordinate
                tex.push_back(vec2(x,y));
            }
            //if the line starts with an vn, it stores vertex normals
            else if (line[1] == 'n')
            {
                //create three floats to store the normal
                float nx, ny, nz;
                //load the data form the line
                sscanf(line.c_str(), "vn %f %f %f", &nx, &ny, &nz);
                //store the normal
                normals.push_back(vec3(nx,ny,nz));
            }
        }
        //if the line starts with an f, it is a face. 
        else if (line[0] == 'f')
        {
            //count the amount of spaces in the line and save it in edge
            int edge = count_char(line, ' ');
            //if the edge builds no triangle, step to the next face
            if (edge != 3)
            {
                //jump to the next loop iteration
                continue;
            }
            //count the amount of slashes in the line and save it in count_slash
            int count_slash = count_char(line, '/');
            //if there are no slashes in th eline
            if (count_slash == 0)
            {
                //create 3 ints to store the face conections
                int m[3];
                //read the conections from the file
                sscanf(line.c_str(), "f %d %d %d", &m[2], &m[1], &m[0]);
                //create the three vertices
                for (int i = 0; i < 3; i++)
                {
                    //check if the vertex that should be created allready exists
                    if (cverts.count(std::to_string(m[i])))
                    {
                        //if the vertex allready exists, store the value in the face
                        this->indices.push_back(cverts[std::to_string(m[i])].second);
                    }
                    else
                    {
                        //if the vertex does not exist, create the vertex
                        this->vertices.push_back(Vertex(verts[m[i]]));
                        //store that the vertex exists
                        cverts[std::to_string(m[i])].first = Vertex(verts[m[i]]);
                        cverts[std::to_string(m[i])].second = (unsigned int)this->indices.size()-1;
                        //store the position of the vertex in the face
                        this->indices.push_back(cverts[std::to_string(m[i])].second);
                    }
                }
            }
            //else if there are equal slashes and spaces in the line
            else if (count_slash == edge)
            {
                //create 3 ints
                int v[3];
                //create 3 more ints
                int t[3];
                //read in the line using the 6 integers
                sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &v[2], &t[2], &v[1], &t[1], &v[0], &t[0]);
                //create the three vertices
                for (int i = 0; i < 3; i++)
                {
                    //check if the vertex that should be created allready exists
                    if (cverts.count(std::to_string(v[i])+std::string(",")+std::to_string(t[i])))
                    {
                        //if the vertex allready exists, store the value in the face
                        this->indices.push_back(cverts[std::to_string(v[i])+std::string("v,")+std::to_string(t[i])].second);
                    }
                    else
                    {
                        //if the vertex does not exist, create the vertex
                        this->vertices.push_back(Vertex(verts[v[i]], tex[t[i]]));
                        //store that the vertex exists
                        cverts[std::to_string(v[i])+std::string("v,")+std::to_string(t[i])].first = Vertex(verts[v[i]], tex[t[i]]);
                        cverts[std::to_string(v[i])+std::string("v,")+std::to_string(t[i])].second = (unsigned int)this->vertices.size()-1;
                        //store the position of the vertex in the face
                        this->indices.push_back(cverts[std::to_string(v[i])+std::string("v,")+std::to_string(t[i])].second);
                    }
                }
            }
            //else if the count of slashes is the double of the edge variable
            else if (count_slash == edge * 2)
            {
                //if has_double_slash return true
                if (has_double_slash(line))
                {
                    //create 3 intigers
                    int v[3];
                    //create 3 intigers
                    int n[3];
                    //read the data from the file
                    sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v[2], &n[2], &v[1], &n[1], &v[0], &n[0]);
                    //create the three vertices
                    for (int i = 0; i < 3; i++)
                    {
                        //check if the vertex that should be created allready exists
                        if (cverts.count(std::to_string(v[i])+std::string("v,/,")+std::to_string(n[i])))
                        {
                            //if the vertex allready exists, store the value in the face
                            this->indices.push_back(cverts[std::to_string(v[i])+std::string("v,/,")+std::to_string(n[i])].second);
                        }
                        else
                        {
                            //if the vertex does not exist, create the vertex
                            this->vertices.push_back(Vertex(verts[v[i]], normals[n[i]]));
                            //store that the vertex exists
                            cverts[std::to_string(v[i])+std::string("v,/,")+std::to_string(n[i])].first = Vertex(verts[v[i]], normals[n[i]]);
                            cverts[std::to_string(v[i])+std::string("v,/,")+std::to_string(n[i])].second = (unsigned int)this->vertices.size()-1;
                            //store the position of the vertex in the face
                            this->indices.push_back(cverts[std::to_string(v[i])+std::string("v,/,")+std::to_string(n[i])].second);
                        }
                    }
                }
                //else it has no double slahs, then create an face from pos, tex and normal
                else
                {
                    //create 3 intigers for the position
                    int v0, v1, v2;
                    //create 3 intigers for the texture
                    int t0, t1, t2;
                    //create 3 intigers for the normal
                    int n0, n1, n2;
                    //read the data from the file
                    sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n0, &v1, &t1, &n1, &v2, &t2, &n2);
                    //convert the integers to arrays
                    int v[3] = {v2-1, v1-1, v0-1};
                    int t[3] = {t2-1, t1-1, t0-1};
                    int n[3] = {n2-1, n1-1, n0-1};
                    //create the three vertices
                    for (int i = 0; i < 3; i++)
                    {
                        //check if the vertex that should be created allready exists
                        if (cverts.count(std::to_string(v[i])+std::string(",")+std::to_string(t[i])+std::string(",")+std::to_string(n[i])))
                        {
                            //if the vertex allready exists, store the value in the face
                            this->indices.push_back(cverts[(std::to_string(v[i])+std::string("v,")+std::to_string(t[i])+std::string("t,")+std::to_string(n[i]))].second);
                        }
                        else
                        {
                            //if the vertex does not exist, create the vertex
                            this->vertices.push_back(Vertex(verts[v[i]], tex[t[i]], normals[n[i]]));
                            //store that the vertex exists
                            cverts[std::to_string(v[i])+std::string("v,")+std::to_string(t[i])+std::string("t,")+std::to_string(n[i])].first = Vertex(verts[v[i]], normals[n[i]]);
                            cverts[std::to_string(v[i])+std::string("v,")+std::to_string(t[i])+std::string("t,")+std::to_string(n[i])].second = (unsigned int)this->vertices.size()-1;
                            //store the position of the vertex in the face
                            this->indices.push_back(cverts[std::to_string(v[i])+std::string("v,")+std::to_string(t[i])+std::string("t,")+std::to_string(n[i])].second);
                        }
                    }
                }
            }
        }
    }
}

Mesh::Mesh(const char* file, int type)
{
    std::string data;

    readFile(file, data);

    *this = Mesh(data,type);
}

Mesh::~Mesh()
{
    //clear the vertices
    this->vertices.clear();
    //clear the indices
    this->indices.clear();
}

void Mesh::recalculateNormals()
{
    for (int i = 0; i < (int)this->indices.size()/3; i++)
    {
        vec3 normal = this->vertices[indices[i+1]].pos.cross(this->vertices[indices[i]].pos);
        normal.normalize();

        if (normal == vec3(1/0.f,1/0.f,1/0.f))
        {
            normal = vec3(0,0,0);
        }
        
        for (int j = 0; j < 3; j++)
        {
            bool is0 = this->vertices[indices[i+j]].normal == vec3(0,0,0);            

            if (is0)
            {
                this->vertices[indices[i+j]].normal = normal;
            }
            else
            {
                this->vertices[indices[i+j]].normal.normalize();
                this->vertices[indices[i+j]].normal = (this->vertices[indices[i+j]].normal + normal);
                this->vertices[indices[i+j]].normal.normalize();
            }
        }
    }
}

void Mesh::applyTransform(Transform transform)
{
    //get the transformation matrix
    mat4 transfMat = transform.getMatrix();
    //get the rotation matrix
    mat4 rotMat = transform.getRotationMatrix();

    //loop over all vertices
    for (int i = 0; i < (int)this->vertices.size(); i++)
    {
        //calculate a 4d position vector
        vec4 temp = transfMat * vec4(this->vertices[i].pos,1);
        //store the 3d part of the vector
        this->vertices[i].pos = vec3(temp.x, temp.y, temp.z);
        //calculate a 4d normal vector
        temp = rotMat * vec4(this->vertices[i].normal, 1);
        //store the 3d part of the vector
        this->vertices[i].normal = vec3(temp.x, temp.y, temp.z);
    }
}

Mesh Mesh::join(Mesh mesh)
{
    //create a list of vertices and copy the own vertices into it
    std::vector<Vertex> newVerts = this->vertices;
    //copy the other mesh vertices into it too
    newVerts.insert(newVerts.end(), mesh.vertices.begin(), mesh.vertices.end());
    
    //create a list of indices and copy the own indices into it
    std::vector<unsigned int> newInds = this->indices;
    //loop over the vertices of the other mesh
    for (int i = 0; i < (int)mesh.indices.size(); i++)
    {
        //add a single index from the other mesh to the new mesh and move it up by the amount of own indices
        newInds.push_back(mesh.indices[i] + (int)this->vertices.size());
    }
    //return a new mesh from the new indices and vertices
    return Mesh(newVerts, newInds);
}

void Mesh::joinThis(Mesh mesh)
{
    //pass to the join function
    *this = join(mesh);
}

void Mesh::operator+=(Mesh mesh)
{
    //pass to the joinThis function
    this->joinThis(mesh);
}

Mesh Mesh::operator+(Mesh mesh)
{
    //pass to the join function
    return this->join(mesh);
}