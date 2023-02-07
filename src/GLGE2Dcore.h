/**
 * @file GLGE2Dcore.hpp
 * @author DM8AT
 * @brief the 2D core for GLGE, everything containing 2D games and GUI
 * @version 0.1
 * @date 2023-01-29
 * 
 * @copyright CC BY (see  https://creativecommons.org/licenses/by/4.0/)
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

    /**
     * @brief update the move matrix
     */
    void update();

    /**
     * @brief update the vertex buffer
     * 
     * @param mesh an optional argument to also asign a new mesh
     */
    void recalculateVertexBuffer(Mesh2D mesh = Mesh2D());

    /**
     * @brief update the index buffer
     * 
     * @param mesh an optinal argument to also asign a new mesh
     */
    void recalculateIndexBuffer(Mesh2D mesh = Mesh2D());

    /**
     * @brief update the complete mesh buffers
     * 
     * @param mesh an optional new mesh
     */
    void recalculateMeshBuffer(Mesh2D mesh = Mesh2D());

    /**
     * @brief assign a new mesh to the object
     * 
     * @param mesh the new mesh data
     */
    void setMesh(Mesh2D mesh);

    /**
     * @brief Get the Mesh from the object
     * 
     * @return Mesh2D the mesh from the object
     */
    Mesh2D getMesh();

    /**
     * @brief Set the Shader
     * 
     * @param path the path and prefix for the shader files. Suffixes are automaticaly .fs and .vs
     */
    void setShader(const char* path);

    /**
     * @brief Set the Shader
     * 
     * @param shader the allready compiled shader
     */
    void setShader(GLuint shader);

    /**
     * @brief Get the Shader
     * 
     * @return GLuint the compiled shader
     */
    GLuint getShader();

    /**
     * @brief Set the Texture for the object
     * 
     * @param file the file to read the texture from
     */
    void setTexture(const char* file);

    /**
     * @brief Set the Texture for the object
     * 
     * @param texture the allready compiled OpenGL texture
     */
    void setTexture(GLuint texture);

    /**
     * @brief Get the Texture from the file
     * 
     * @return GLuint the allready compiled OpenGL texture
     */
    GLuint getTexture();

    /**
     * @brief Set the Transform for the object
     * 
     * @param transform the new transform from the object
     */
    void setTransform(Transform2D transform);

    /**
     * @brief Get the Transform from the object
     * 
     * @return Transform2D the transform from the object
     */
    Transform2D getTransform();

    /**
     * @brief change the position of the object
     * 
     * @param deltaPos the difference in position
     */
    void move(vec2 deltaPos);

    /**
     * @brief change the position of the object
     * 
     * @param deltaX the difference on the x axis
     * @param deltaY the difference on the y axis
     */
    void move(float deltaX, float deltaY);

    /**
     * @brief change the position of the object in a direction
     * 
     * @param speedX the speed on the x axis
     * @param speedY the speed on the y axis
     * @param dir the direction to move the object in
     */
    void move(float speedX, float speedY, float dir);

    /**
     * @brief change the position of the object in the direction it is facig
     * 
     * @param speed the speed to move the object with
     */
    void move(float speed);

    /**
     * @brief Set the position of the object
     * 
     * @param pos the new position for the object
     */
    void setPos(vec2 pos);

    /**
     * @brief Set the position of the object
     * 
     * @param x the new x position for the object
     * @param y the new y position for the object
     */
    void setPos(float x, float y);

    /**
     * @brief Get the position from the object
     * 
     * @return vec2 the position of the object
     */
    vec2 getPos();

    /**
     * @brief rotate the object
     * 
     * @param angle the angle of rotation in degrees
     */
    void rotate(float angle);

    /**
     * @brief Set the rotation of the object
     * 
     * @param dir the direction for the object in degrees
     */
    void setRotation(float dir);

    /**
     * @brief Get the Rotation from the object
     * 
     * @return float the rotation in degrees
     */
    float getRotation();

    /**
     * @brief change the scale of the object
     * 
     * @param scale the scale multiplier
     */
    void scale(vec2 scale);

    /**
     * @brief change the scale of the object
     * 
     * @param scaleX the scale multiplier on the x axis
     * @param scaleY the scale multiplier on the y axis
     */
    void scale(float scaleX, float scaleY);

    /**
     * @brief add something to the scale
     * 
     * @param scale the difference in the scale
     */
    void scaleAdd(vec2 scale);

    /**
     * @brief add something to the scale
     * 
     * @param scaleX the difference in the scale on the x axis
     * @param scaleY the difference in the scale on the y axis
     */
    void scaleAdd(float scaleX, float scaleY);

    /**
     * @brief Set the Scale from the object
     * 
     * @param scale the new scale for the object
     */
    void setScale(vec2 scale);

    /**
     * @brief Set the Scale from the object
     * 
     * @param x the new scale on the x axis
     * @param y the new scale on the y axis
     */
    void setScale(float x, float y);

    /**
     * @brief Get the Scale from the object
     * 
     * @return vec2 the scale from the object
     */
    vec2 getScale();

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
                        0,0,0);
    //store a texture
    GLuint texture;
    //say if the object is static
    bool isStatic;
    //save the length of the vertex and index buffer
    unsigned int VBOLen, IBOLen;

    /**
     * @brief Create the buffers
     */
    void createBuffers();

    /**
     * @brief create the vertex buffer or update it
     * 
     * @param mesh the new mesh
     */
    void updateVertexBuffer();

    /**
     * @brief create the index buffer or update it
     * 
     * @param mesh the new mesh
     */
    void updateIndexBuffer();

    /**
     * @brief setup the shaders
     * 
     * @param vs the vertex shader
     * @param fs the fragment shader
     */
    void shaderSetup(const char* vs, const char* fs);

    /**
     * @brief recalculate the local move matrix
     */
    void recalculateMoveMatrix();
};

class Camera2D
{
public:
    /**
     * @brief Construct a new Camera 2D
     * default constructor
     */
    Camera2D();

    /**
     * @brief Construct a new Camera 2D
     * 
     * @param transform the transform for the camera
     */
    Camera2D(Transform2D transform);

    /**
     * @brief Construct a new Camera 2D
     * 
     * @param pos 
     * @param rot 
     * @param size 
     */
    Camera2D(vec2 pos, float rot = 0, vec2 size = vec2(1,1));

    /**
     * @brief Construct a new Camera 2D
     * 
     * @param x the x position for the camera
     * @param y the y position for the camera
     * @param rot the rotation of the camera
     * @param size the global scaling
     */
    Camera2D(float x, float y, float rot = 0, vec2 size = vec2(1,1));

    /**
     * @brief Construct a new Camera 2D
     * 
     * @param pos the position for the camera
     * @param sX the global scaling on the x axis
     * @param sY the global scaling on the y axis
     * @param rot the camera rotation
     */
    Camera2D(vec2 pos, float sX, float sY, float rot = 0);

    /**
     * @brief Construct a new Camera 2D
     * 
     * @param x the x position for the camera
     * @param y the y position for the camera
     * @param sX the global scaling on the x axis
     * @param sY the global scaling on the y axis
     * @param rot the camera rotation
     */
    Camera2D(float x, float y, float sX, float sY, float rot = 0);

    /**
     * @brief update the camera matrix
     */
    void update();

    /**
     * @brief Get the camera matrix
     * 
     * @return mat3 the camera matrix
     */
    mat3 getMatrix();

    /**
     * @brief Set the Transform for the camera
     * 
     * @param transform the new transform from the camera
     */
    void setTransform(Transform2D transform);

    /**
     * @brief Get the Transform from the camera
     * 
     * @return Transform2D the transform from the camera
     */
    Transform2D getTransform();

    /**
     * @brief change the position of the camera
     * 
     * @param deltaPos the difference in position
     */
    void move(vec2 deltaPos);

    /**
     * @brief change the position of the camera
     * 
     * @param deltaX the difference on the x axis
     * @param deltaY the difference on the y axis
     */
    void move(float deltaX, float deltaY);

    /**
     * @brief change the position of the camera in a direction
     * 
     * @param speedX the speed on the x axis
     * @param speedY the speed on the y axis
     * @param dir the direction to move the camera in
     */
    void move(float speedX, float speedY, float dir);

    /**
     * @brief change the position of the camera in the direction it is facig
     * 
     * @param speed the speed to move the camera with
     */
    void move(float speed);

    /**
     * @brief Set the position of the camera
     * 
     * @param pos the new position for the camera
     */
    void setPos(vec2 pos);

    /**
     * @brief Set the position of the camera
     * 
     * @param x the new x position for the camera
     * @param y the new y position for the camera
     */
    void setPos(float x, float y);

    /**
     * @brief Get the position from the camera
     * 
     * @return vec2 the position of the camera
     */
    vec2 getPos();

    /**
     * @brief rotate the camera
     * 
     * @param angle the angle of rotation in degrees
     */
    void rotate(float angle);

    /**
     * @brief Set the rotation of the camera
     * 
     * @param dir the direction for the camera in degrees
     */
    void setRotation(float dir);

    /**
     * @brief Get the Rotation from the camera
     * 
     * @return float the rotation in degrees
     */
    float getRotation();

    /**
     * @brief change the scale of the camera
     * 
     * @param scale the scale multiplier
     */
    void scale(vec2 scale);

    /**
     * @brief change the scale of the camera
     * 
     * @param scaleX the scale multiplier on the x axis
     * @param scaleY the scale multiplier on the y axis
     */
    void scale(float scaleX, float scaleY);

    /**
     * @brief add something to the scale
     * 
     * @param scale the difference in the scale
     */
    void scaleAdd(vec2 scale);

    /**
     * @brief add something to the scale
     * 
     * @param scaleX the difference in the scale on the x axis
     * @param scaleY the difference in the scale on the y axis
     */
    void scaleAdd(float scaleX, float scaleY);

    /**
     * @brief Set the Scale from the camera
     * 
     * @param scale the new scale for the camera
     */
    void setScale(vec2 scale);

    /**
     * @brief Set the Scale from the camera
     * 
     * @param x the new scale on the x axis
     * @param y the new scale on the y axis
     */
    void setScale(float x, float y);

    /**
     * @brief Get the Scale from the camera
     * 
     * @return vec2 the scale from the camera
     */
    vec2 getScale();

private:
    //store the camera transform
    Transform2D transf;
    //store the camera matrix
    mat3 camMat = mat3(1,0,0,
                       0,1,0,
                       0,0,1);

    /**
     * @brief recalculate the camera matrix
     */
    void recalculateMatrix();
};

//FUNCTIONS

/**
 * @brief bind the camera for 2D objects
 * 
 * @param camera a pointer to the camera to use
 */
void glgeBindMain2DCamera(Camera2D* camera);

/**
 * @brief get a pointer to the used 2D camera
 * 
 * @return Camera2D* a pointer to the used 2D camera
 */
Camera2D* glgeGetMain2DCamera();

#endif