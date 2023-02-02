/**
 * @file GLGE3Dcore.h
 * @author DM8AT
 * @brief this is the 3D core for GLGE. It is used to create 3D graphics and such things. DEPENDECYS: GLGE.h
 * @version 0.1
 * @date 2022-12-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

//check for double include 
#ifndef _GLGE_3D_CORE_H_
#define _GLGE_3D_CORE_H_

//include glew
#include <GL/glew.h>
#include <vector>

//include the needed components from CML
//vectors
#include "GLGE/CML/CMLVec3.h"
#include "GLGE/CML/CMLVec2.h"
//matrices
#include "GLGE/CML/CMLMat4.h"

//include the library core
#include "GLGE.h"

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
};

/**
 * @brief this variable stores information about a single vertex of an Object
 */
struct Vertex
{
    //store the vertex position
    vec3 pos;
    //store the texture coordinate
    vec2 texCoord;
    //store the vertex color
    vec4 color;
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
};

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
};

class Object
{
public:
    /**
     * @brief Construct a new Object
     * 
     * default constructor
     */
    Object();

    /**
     * @brief Construct a new Object
     * 
     * @param vertices a pointer array to store the vertices
     * @param indices a pointer array to store the indices
     * @param sizeVertices the size of the vertices pointer
     * @param sizeIndices the size of the indices pointer
     * @param transform an optinal transform for the object
     * @param isStatic says if the object should move with the camera
     */
    Object(Vertex* vertices, unsigned int* indices, unsigned int sizeVertices, unsigned int sizeIndices, Transform transform = Transform(), bool isStatic = false);

    /**
     * @brief Construct a new Object
     * 
     * @param vertices the vertices in an std::vector
     * @param indices the faces in an std::vector
     * @param transform an optinal transform for the object
     * @param isStatic says if the object should move with the camera
     */
    Object(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Transform transform = Transform(), bool isStatic = false);

    /**
     * @brief draw the object to the screen
     */
    void draw();

    /**
     * @brief update things like the move matrix
     */
    void update();

    /**
     * @brief Set the Shader for the object
     * 
     * @param vs the file name for the vertex shader
     * @param fs the file name for the fragment shader
     */
    void setShader(const char* vs, const char* fs);

    /**
     * @brief Set the Shader for the object
     * 
     * @param filePrefix the prefix for the shader files (PATH+FILE PREFIX) suffixe: .vs and .fs
     */
    void setShader(const char* filePrefix);

    /**
     * @brief Set the Shader for the object
     * 
     * @param shader the allready compiled shader
     */
    void setShader(GLuint shader);

    /**
     * @brief Get the Shader from the object
     * 
     * @return GLuint the position of the shader on the graphics card
     */
    GLuint getShader();

private:
    //store the transform for the object
    Transform transf;
    //store a mesh
    Mesh mesh;
    //store the vertex and index buffer
    GLuint VBO ,IBO;
    //save the shader
    GLint shader;
    //store the move matrix location
    GLuint moveMatLoc;
    //the local matrix to make the object correct
    mat4 moveMat = mat4(1,0,0,0,
                        0,1,0,0,
                        0,0,1,0,
                        0,0,0,1);
    //store a texture
    GLuint texture;
    //store if the object is static
    bool isStatic;
    //store the length of the index and vertex vuffer
    unsigned int VBOLen, IBOLen;

    //compile the draw list
    void compileBuffers();

    //recalculate the move matrix
    void recalculateMoveMatrix();

    //compile the shader and save it
    void shaderSetup(const char* vs, const char* fs);
};

/**
 * @brief a Camera used for 3D projection, position usw
 */
class Camera
{
public:
    /**
     * @brief default constructor
     */
    Camera();

    /**
     * @brief Construct a new Camera
     * 
     * @param FOV the field of view for the camera
     * @param near the near cliping plane for the camera
     * @param far the far cliping plane for the camera
     * @param transform the position and rotation of the camera. Scaling is ignored. 
     */
    Camera(float FOV, float near = 0.1, double far = 10.0, Transform transform = Transform());

    /**
     * @brief update the view matrix
     */
    void update();

    /**
     * @brief recalculate the projection matrix
     */
    void recalculateProjection();

    /**
     * @brief Get the View Matrix for the camera
     * 
     * @return mat4 the view matrix for the camera
     */
    mat4 getViewMatrix();

    /**
     * @brief Get the Projection Matrix for the camera
     * 
     * @return mat4 the projection matrix for the camera
     */
    mat4 getProjectionMatrix();

    /**
     * @brief apply a specific transform to the camera
     * 
     * @param transform the new transform for the camera
     */
    void applyTransform(Transform transform);

    /**
     * @brief position the camera new
     * 
     * @param position the new position for the camera
     */
    void set(vec3 position);

    /**
     * @brief position the camera new
     * 
     * @param x the new x position for the camera
     * @param y the new y position for the camera
     * @param z the new z position for the camera
     */
    void set(float x, float y, float z);

    /**
     * @brief move the camera
     * 
     * @param deltaPosition the difference from the old to the new position
     */
    void move(vec3 deltaPosition);

    /**
     * @brief move the camera
     * 
     * @param dX the difference on the x axis
     * @param dY the difference on the y axis
     * @param dZ the difference on the z axis
     */
    void move(float dX, float dY, float dZ);

    /**
     * @brief Set the Rotation of the camera
     * 
     * @param rotation the new rotation for the camera
     */
    void setRotation(vec2 rotation);

    /**
     * @brief Set the Rotation of the camera
     * 
     * @param x the x rotation for the camera
     * @param y the y rotation for the camera
     */
    void setRotation(float x, float y);

    /**
     * @brief rotate the camera
     * 
     * @param deltaRotation the difference for the rotation
     */
    void rotate(vec2 deltaRotation);

    /**
     * @brief rotate the camera
     * 
     * @param dX the difference of rotation on the x axis
     * @param dY the difference of rotation on the y axis
     */
    void rotate(float dX, float dY);

    /**
     * @brief set the field of view of the camera
     * 
     * @param fov the new field of view for the camera
     */
    void setFOV(float fov);

    /**
     * @brief change the size of the field of view
     * 
     * @param deltaFOV the difference in the size of the field of view
     */
    void sizeFOV(float deltaFOV);

private:
    //store the right direction of the camera
    vec4 right = vec4(1,0,0,0);
    //store the left direction of the camera
    vec4 left = vec4(-1,0,0,0);
    //store the up direction of the camera
    vec4 up = vec4(0,1,0,0);
    //store the down direction of the camera
    vec4 down = vec4(0,-1,0,0);
    //store the forward direction of the camera
    vec4 forward = vec4(0,0,1,0);
    //store the backwards direction of the camera
    vec4 backward = vec4(0,0,-1,0);
    //store the position of the camera in camera space
    vec4 position = vec4(0,0,0,1);
    //store the field of view for the camera
    float fov = 90.f;
    //store the near cliping plane
    float near = 0.1f;
    //store the far cliping plane
    double far = 10.0;
    //store the transform of the camera
    Transform transf;

    //store the view matrix
    mat4 viewMatrix;
    //store the projection matrix
    mat4 projectionMatrix;

    //calculate the view matrix
    mat4 calculateViewMatrix();

    //calculate the projection matrix
    mat4 calculateProjectionMatrix();
};

/////////////
//FUNCTIONS//
/////////////

/**
 * @brief bind a camera to use by GLGE to use
 * 
 * @param camera a pointer to the camera so parameters can be changed later
 */
void glgeBindCamera(Camera* camera);

/**
 * @brief initalise the 3D core. Call directly after createing the window
 * 
 * this function initalises all values to draw 3D things
 */
void glgeInit3DCore();

#endif