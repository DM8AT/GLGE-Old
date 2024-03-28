/**
 * @file glge3DcoreDefClasses.h
 * @author DM8AT
 * @brief define the graphics api independand classes for the 3D Core
 * @version 0.1
 * @date 2024-01-09
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license.
 */

#ifndef _GLGE_INTERNAL_3D_CORE_DEF_CLASSES_H_
#define _GLGE_INTERNAL_3D_CORE_DEF_CLASSES_H_

//includes from the default C++ lib
#include <vector>
#include <string>

//include the needed components from CML
//vectors
#include "../CML/CMLVec3.h"
#include "../CML/CMLVec2.h"
//matrices
#include "../CML/CMLMat4.h"

//public defines

//say the type of an model file
#define GLGE_OBJ 1

//define the preset inputs

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

//define the presets

//check if a define for an empty object allreday exists
#ifndef GLGE_PRESET_EMPTY
//generate an empty object without any mesh
#define GLGE_PRESET_EMPTY 0
#endif //GLGE_PRESET_EMPTY creation
//genearte an object with an cube as the mesh
#define GLGE_PRESET_CUBE 1
//genearte an object with an plane as the mesh
#define GLGE_PRESET_PLANE 2
//check if the circle allready exists
#ifndef GLGE_PRESET_CIRCLE
//genearte an object with an circle as the mesh
#define GLGE_PRESET_CIRCLE 3
#endif // GLGE_PRESET_CIRCLE generation
//genearte an object with an sphere as the mesh
#define GLGE_PRESET_SPHERE 4
//genearte an object with an icosphere as the mesh
#define GLGE_PRESET_ICOSPHERE 5
//genearte an object with an cylinder as the mesh
#define GLGE_PRESET_CYLINDER 6
//genearte an object with an cone as the mesh
#define GLGE_PRESET_CONE 7

///////////
//STRUCTS//
///////////

/**
 * @brief a variable type to store position, rotation and scaling information for an object
 * 
 * the transform type is used by objects of the GLGE library to capsule all the movement operations in one varaiable, to not have a lot
 * of different variables to have in an object. 
 */
struct Transform
{
    vec3 pos;
    vec3 rot;
    vec3 scale = vec3(1,1,1);

    /**
     * @brief Construct a new Transform
     * 
     * default constructor
     */
    Transform();

    /**
     * @brief Construct a new Transform
     * 
     * @param position the position data stored in the transform
     * @param rotation the rotation stored in the transform
     * @param scale the size information stored in the transform
     */
    Transform(vec3 position, vec3 rotation, vec3 scale = vec3(1,1,1));

    /**
     * @brief Construct a new Transform
     * 
     * @param position the position of the transform
     * @param rotation the rotation of the transform
     * @param scale the scale on all axis of the transform
     */
    Transform(vec3 position, vec3 rotation, float scale = 1);

    /**
     * @brief Get the Matrix to transform an object to this
     * 
     * @return mat4 transformation matrix
     */
    mat4 getMatrix();

    /**
     * @brief Get the Rotation Matrix
     * 
     * @return mat4 the rotation matrix
     */
    mat4 getRotationMatrix();
};

/**
 * @brief this variable stores information about a single vertex of an Object
 */
struct Vertex
{
    //store the vertex position
    vec3 pos;
    //store the vertex color
    vec4 color;
    //store the texture coordinate
    vec2 texCoord = vec2(1,1);
    //store the normal of the vertex
    vec3 normal = vec3(0,0,0);

    /**
     * @brief Construct a new Vertex
     * 
     * default constructor
     */
    Vertex();

    /**
     * @brief Construct a new Vertex
     * 
     * @param pos the positon for the vertex
     * @param color the vertex color
     */
    Vertex(vec3 pos, vec4 color = vec4(1,1,1,1));

    /**
     * @brief Construct a new Vertex
     * 
     * @param x the x positoin of the vertex
     * @param y the y position of the vertex
     * @param z the z position of the vertex
     * @param color the vertex color
     */
    Vertex(float x, float y, float z, vec4 color = vec4(1,1,1,1));

    /**
     * @brief Construct a new Vertex
     * 
     * @param x the x positoin of the vertex
     * @param y the y position of the vertex
     * @param z the z position of the vertex
     * @param r the red part of the color
     * @param g the green part of the color
     * @param b the blue part of the color
     * @param a the alpha of the color, used for transparency
     */
    Vertex(float x, float y, float z, float r, float g, float b, float a);

    /**
     * @brief Construct a new Vertex
     * 
     * @param pos the positon for the vertex
     * @param texCoord the texture coordinate
     */
    Vertex(vec3 pos, vec2 texCoord);

    /**
     * @brief Construct a new Vertex
     * 
     * @param x the x positoin of the vertex
     * @param y the y position of the vertex
     * @param z the z position of the vertex
     * @param texCoord the texture coordinate
     */
    Vertex(float x, float y, float z, vec2 texCoord);

    /**
     * @brief Construct a new Vertex
     * 
     * @param x the x positoin of the vertex
     * @param y the y position of the vertex
     * @param z the z position of the vertex
     * @param tX the texture x coordinate (or: s, v)
     * @param tY the texture y coordinate (or: r, u)
     */
    Vertex(float x, float y, float z, float tX, float tY);

    /**
     * @brief Construct a new Vertex
     * 
     * @param pos the position of the vertex
     * @param normal the normal of the vertex
     */
    Vertex(vec3 pos, vec3 normal);

    /**
     * @brief Construct a new Vertex
     * 
     * @param pos the position of the vertex
     * @param texCoord the texture coordinate of the vertex
     * @param normal the normal of the vertex
     */
    Vertex(vec3 pos, vec2 texCoord, vec3 normal);

    /**
     * @brief Construct a new Vertex from a lot of floats
     * 
     * @param x the x position of the vertex
     * @param y the y position of the vertex
     * @param z the z position of the vertex
     * @param tx the texture x coordinate (or: s, v)
     * @param ty the texture y coordinate (or: r, u)
     * @param nx the normal on the x axis
     * @param ny the normal on the y axis
     * @param nz the normal on the z axis
     */
    Vertex(float x, float y, float z, float tx, float ty, float nx, float ny, float nz);

    /**
     * @brief Construct a new Vertex from 10 floats
     * 
     * @param x the x position of the vertex
     * @param y the y position of the vertex
     * @param z the z position of the vertex
     * @param r the red part of the color
     * @param g the green part of the color
     * @param b the blue part of the color
     * @param a the alpha of the color, used for transparency
     * @param nx the normal on the x axis
     * @param ny the normal on the y axis
     * @param nz the normal on the z axis
     */
    Vertex(float x, float y, float z, float r, float g, float b, float a, float nx, float ny, float nz);

    /**
     * @brief Construct a new Vertex from 6 floats and one vec4
     * 
     * @param x the x position of the vertex
     * @param y the y position of the vertex
     * @param z the z position of the vertex
     * @param color the color for the vertex
     * @param nx the normal on the x axis
     * @param ny the normal on the y axis
     * @param nz the normal on the z axis
     */
    Vertex(float x, float y, float z, vec4 color, float nx, float ny, float nz);
};

/**
 * @brief store the mesh for a 3D object
 */
class Mesh
{
public:
    //store the vertices for the object
    std::vector<Vertex> vertices;
    //store the indices for the object
    std::vector<unsigned int> indices;

    /**
     * @brief defalut constructor
     */
    Mesh();

    /**
     * @brief Construct a new Mesh
     * 
     * @param preset the preset to use
     * @param color the color for the mesh (alpha = -1 for texture coordinates)
     * @param resolution the amount of subdivision for subdivided meshes (0 for defaults)
     */
    Mesh(unsigned int preset, vec4 color, unsigned int resolution = 0);

    /**
     * @brief Construct a new Mesh
     * 
     * @param vertices a pointer array to store the vertices
     * @param indices a pointer array to store the indices
     * @param sizeVertices the size of the vertices pointer
     * @param sizeIndices the size of the indices pointer
     */
    Mesh(Vertex* vertices, unsigned int* indices, unsigned int sizeVertices, unsigned int sizeIndices);

    /**
     * @brief Construct a new Mesh
     * 
     * @param vertices the vertices in an std::vector
     * @param indices the indices in an std::vector
     */
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    /**
     * @brief Construct a new Mesh
     * 
     * Supported formats:
     * .obj
     * 
     * @param data the data from an for the mesh
     * @param type the type of the data (what file format was used)
     */
    Mesh(std::string data, int type);

    /**
     * @brief Construct a new Mesh
     * 
     * Supported formats: 
     * .obj
     * 
     * @param file the file to read the data from
     * @param type the type of the file
     */
    Mesh(const char* file, int type);

    /**
     * @brief recalculate the normal vectors from the mesh in clockwise order
     */
    void recalculateNormals();

    /**
     * @brief apply the transform to the mesh data
     * 
     * @param transform the transform to apply
     */
    void applyTransform(Transform transform);

    /**
     * @brief join two meshes
     * 
     * @param mesh the other mesh
     * @return Mesh the joined mesh
     */
    Mesh join(Mesh mesh);

    /**
     * @brief join an mesh to this mesh
     * 
     * @param mesh the other mesh
     */
    void joinThis(Mesh mesh);

    /**
     * @brief join another mesh to this mesh
     * 
     * @param mesh the other mesh
     */
    void operator+=(Mesh mesh);

    /**
     * @brief join two meshes
     * 
     * @param mesh the other mesh
     * @return Mesh the joined mesh
     */
    Mesh operator+(Mesh mesh);
};

#endif