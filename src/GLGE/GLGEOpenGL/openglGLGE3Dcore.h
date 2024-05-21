/**
 * @file openglGLGE3Dcore.h
 * @author DM8AT
 * @brief define the functions and classes of the 3D core of GLGE
 * @version 0.1
 * @date 2024-01-02
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_3D_CORE_OGL_H_
#define _GLGE_3D_CORE_OGL_H_

//Needed GLGE cores
#include "openglGLGEMaterialCore.h"
#include "openglGLGEShaderCore.h"

//include the library core
#include "openglGLGE.h"

//include the 3D core base
#include "../GLGEIndependend/glge3DcoreDefClasses.h"
//include the data class
#include "../GLGEIndependend/GLGEData.h"

/**
 * @brief store an 3D object
 */
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
     * @brief Construct a new Object object
     * 
     * @param preset the preset to use to generate the object ( starting with GLGE_PRESET_ )
     * @param color the color to use for the constructed object (alpha = -1 to disable color and use texture coordinates)
     * @param resolution the amount of subdivision for subdivided mehses (0 for default)
     * @param transform an optinal transform for the object
     * @param isTransparent say if an object is renderd in the transparent or opaque pass
     * @param isStatic says if the object should move with the camera
     */
    Object(unsigned int preset, vec4 color, unsigned int resolution = 0, Transform transform = Transform(), bool isTransparent = false, bool isStatic = false);

    /**
     * @brief Construct a new Object
     * 
     * @param vertices a pointer array to store the vertices
     * @param indices a pointer array to store the indices
     * @param sizeVertices the size of the vertices pointer
     * @param sizeIndices the size of the indices pointer
     * @param transform an optinal transform for the object
     * @param isTransparent say if an object is renderd in the transparent or opaque pass
     * @param isStatic says if the object should move with the camera
     */
    Object(Vertex* vertices, unsigned int* indices, unsigned int sizeVertices, unsigned int sizeIndices, Transform transform = Transform(), bool isTransparent = false, bool isStatic = false);

    /**
     * @brief Construct a new Object
     * 
     * @param vertices the vertices in an std::vector
     * @param indices the faces in an std::vector
     * @param transform an optinal transform for the object
     * @param isTransparent say if an object is renderd in the transparent or opaque pass
     * @param isStatic says if the object should move with the camera
     */
    Object(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Transform transform = Transform(), bool isTransparent = false, bool isStatic = false);

    /**
     * @brief Construct a new Object
     * 
     * @param mesh the mesh for the Object
     * @param transform an optinal transform for the object
     * @param isTransparent say if an object is renderd in the transparent or opaque pass
     * @param isStatic says if the object should move with the camera
     */
    Object(Mesh mesh, Transform transform = Transform(), bool isTransparent = false, bool isStatic = false);

    /**
     * @brief Construct a new Object
     * 
     * @param file the file to load the data from
     * @param type the type of the file
     * @param transform an optinal transform for the object
     * @param isTransparent say if an object is renderd in the transparent or opaque pass
     * @param isStatic says if the object should move with the camera
     */
    Object(const char* file, int type, Transform transform = Transform(), bool isTransparent = false, bool isStatic = false);

    /**
     * @brief Destroy the Object
     */
    ~Object();

    /**
     * @brief delete the object
     */
    void destroy();

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
     * @param vertexShader the source code for the vertex shader
     * @param fragmentShader the source code for the fragment shader
     */
    void setShader(std::string vertexShader, std::string fragmentShader);

    /**
     * @brief Set the Shader for the object
     * 
     * @param vertexShader the source code for the vertex shader
     * @param fragmentShaderFile the path to the fragment shader
     */
    void setShader(std::string vertexShader, const char* fragmentShaderFile);

    /**
     * @brief Set the Shader for the object
     * 
     * @param shader the allready compiled shader
     */
    void setShader(unsigned int shader);

    /**
     * @brief Get the Shader from the object
     * 
     * @return unsigned int the position of the shader on the graphics card
     */
    unsigned int getShader();

    /**
     * @brief apply a specific transform to the Objct
     * 
     * @param transform the new transform for the Objct
     */
    void setTransform(Transform transform);

    /**
     * @brief Get the Transform from the Objct
     * 
     * @return Transform the current transform of the Objct
     */
    Transform getTransform();

    /**
     * @brief position the Objct new
     * 
     * @param position the new position for the Objct
     */
    void setPos(vec3 position);

    /**
     * @brief position the Objct new
     * 
     * @param x the new x position for the Objct
     * @param y the new y position for the Objct
     * @param z the new z position for the Objct
     */
    void setPos(float x, float y, float z);

    /**
     * @brief move the Objct
     * 
     * @param deltaPosition the difference from the old to the new position
     */
    void move(vec3 deltaPosition);

    /**
     * @brief move the Objct
     * 
     * @param dX the difference on the x axis
     * @param dY the difference on the y axis
     * @param dZ the difference on the z axis
     */
    void move(float dX, float dY, float dZ);

    /**
     * @brief Get the position from the Objct
     * 
     * @return vec3 the position of the Objct
     */
    vec3 getPos();

    /**
     * @brief Set the Rotation of the Objct
     * 
     * @param rotation the new rotation for the Objct
     */
    void setRotation(vec3 rotation);

    /**
     * @brief Set the Rotation of the Objct
     * 
     * @param x the x rotation for the Objct
     * @param y the y rotation for the Objct
     * @param z the z rotation for the Object
     */
    void setRotation(float x, float y, float z);

    /**
     * @brief rotate the Objct
     * 
     * @param deltaRotation the difference for the rotation
     */
    void rotate(vec3 deltaRotation);

    /**
     * @brief rotate the Objct
     * 
     * @param dX the difference of rotation on the x axis
     * @param dY the difference of rotation on the y axis
     * @param dZ the difference of rotation on the z axis
     */
    void rotate(float dX, float dY, float dZ);

    /**
     * @brief Get the rotation from the Objct
     * 
     * @return vec3 the rotation from the Objct
     */
    vec3 getRotation();

    /**
     * @brief Set the Size of the object
     * 
     * @param size the new size for the object
     */
    void setSize(vec3 size);

    /**
     * @brief Set the Size of the object
     * 
     * @param sX the size on the x axis
     * @param sY the size on the y axis
     * @param sZ the size on the z axis
     */
    void setSize(float sX, float sY, float sZ);

    /**
     * @brief Set the Size of the object
     * 
     * @param s the size for all axes
     */
    void setSize(float s);

    /**
     * @brief change the size of the object
     * 
     * @param size the multiplier for the size on each axis
     */
    void scale(vec3 size);

    /**
     * @brief change the size of the object
     * 
     * @param dx the multiplier on the x axis
     * @param dy the multiplier on the y axis
     * @param dz the multiplier on the z axis
     */
    void scale(float dx, float dy, float dz);

    /**
     * @brief change the size of the object
     * 
     * @param s the multiplier for each axis
     */
    void scale(float s);

    /**
     * @brief add something to the scale of the object
     * 
     * @param deltaSize the difference in size on each axis
     */
    void addScale(vec3 deltaSize);

    /**
     * @brief add something to the scale of the object
     * 
     * @param dx the difference in scale on the x axis
     * @param dy the difference in scale on the y axis
     * @param dz the difference in scale on the z axis
     */
    void addScale(float dx, float dy, float dz);

    /**
     * @brief add something to the scale of the object
     * 
     * @param s added to the size on each axis
     */
    void addScale(float s);

    /**
     * @brief Get the Scale from the object
     * 
     * @return vec3 the size of the object
     */
    vec3 getScale();

    /**
     * @brief recalculate the normals of the mesh for the object
     */
    void recalculateNormals();

    /**
     * @brief Set only the mesh for the Object, not recalculates the Buffers
     * 
     * @param mesh the new mesh
     */
    void setOnlyMesh(Mesh mesh);

    /**
     * @brief recalculate the vertex and index buffer
     */
    void recalculateBuffers();

    /**
     * @brief Set the Mesh for the Object
     * 
     * @param mesh the new mesh for the Object
     */
    void setMesh(Mesh mesh);

    /**
     * @brief Get the Mesh from the object
     * 
     * @return Mesh the mesh of the object
     */
    Mesh getMesh();

    /**
     * @brief Set the Material for the object
     * 
     * @param material the new material for the object
     */
    void setMaterial(Material* material);

    /**
     * @brief Get the Material from the object
     * 
     * @return Material* a pointer to the material of the object
     */
    Material* getMaterial();

    /**
     * @brief Get the shader form the object
     * 
     * @return Shader* a pointer to the shader class instance
     */
    Shader* getShaderP();

    /**
     * @brief Set if the object should be handelt as opaque or transparent
     * 
     * @param isTransparent true : the object is transparent | false : the object is opaque
     */
    void setTransparency(bool isTransparent);

    /**
     * @brief Get if the object is handelt as transparent or opaque
     * 
     * @return true : the object is transparent | 
     * @return false : the object is opaque
     */
    bool getTransparency();

    /**
     * @brief Say if the object should only render in the transparent pass
     * 
     * @param transparent true : the object will only render in the transparent pass | false : the object will render in the transparent and opaque pass if it is transparent
     */
    void setFullyTransparent(bool transparent);

    /**
     * @brief Get if the object renders in both passes
     * 
     * @return true : the object will only render in the transparent pass | 
     * @return false : the object will render in the transparent and opaque pass if it is transparent
     */
    bool getFullyTransparent();

    /**
     * @brief encode the data to a single data object
     * 
     * @return Data* a pointer to the encoded data
     */
    Data* encode();
    /**
     * @brief decode the object from a signle data object
     * 
     * @param data the encoded data
     */
    void decode(Data data);

private:
    /**
     * @brief the same as the buffer for the object data on the GPU
     */
    struct ObjectData
    {
        //the model matrix of the object
        mat4 modelMat;
        //the rotation matrix of the object
        mat4 rotMat;
        //the objects uuid
        unsigned int uuid;
    };
    //store the transform for the object
    Transform transf;
    //store a mesh
    Mesh mesh;
    //store the vertex and index buffer
    unsigned int VBO ,IBO;
    //store if the object is transparent or opaque
    bool isTransparent = false;
    //save the shader
    Shader shader;
    //the local matrix to make the object correct
    mat4 camMat = mat4(1,0,0,0,
                        0,1,0,0,
                        0,0,1,0,
                        0,0,0,1);
    //store if the object is static
    bool isStatic;
    //store the length of the index and vertex vuffer
    unsigned int VBOLen, IBOLen;
    //store the material of the object
    Material* mat = 0;
    //store the position of the light positions
    std::vector<unsigned int> lightPosLocs;
    //store the position of the light colors
    std::vector<unsigned int> lightColLocs;
    //store the position of the light data
    std::vector<unsigned int> lightDatLocs;
    //store the position of the light directions
    std::vector<unsigned int> lightDirLocs;
    //store the position for the used lights
    unsigned int usedLigtsPos;
    //store the position for the shadow maps
    unsigned int shadowMapSamplerLoc;
    //store the own ubo (uniform buffer object)
    unsigned int ubo = 0;
    //store the position of the uniform block index
    int uboIndex = -1;
    //store the own object data
    ObjectData objData;
    //store if the object is fully transparent
    bool fullyTransparent = false;
    //store the index of the window the object is used in
    int windowIndex = -1;

    //compile the draw list
    void compileBuffers();

    //recalculate the move matrix
    void recalculateMatrices();

    //compile the shader and save it
    void shaderSetup(const char* vs, const char* fs);

    //get all uniforms from the shader
    void getUniforms();

    //get the light specific uniforms from the shader
    void getLightUniforms();
    //load the light uniforms to the shader
    void loadLights();

    /**
     * @brief the draw function if this is a shadow pass
     */
    void shadowDraw();
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
     * @brief Get the rotation matrix for the camera
     * 
     * @return mat4 the rotation matrix
     */
    mat4 getRotMat();

    /**
     * @brief Get the Transformation matrix
     * 
     * @return mat4 the transformation matrix
     */
    mat4 getTransformMat();

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
     * @brief Get the Transform from the camera
     * 
     * @return Transform the current transform of the camera
     */
    Transform getTransform();

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
     * @brief Get the position from the camera
     * 
     * @return vec3 the position of the camera
     */
    vec3 getPos();

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
     * @param dZ the difference of rotation on the z axis
     */
    void rotate(float dX, float dY, float dZ);

    /**
     * @brief Get the rotation from the camera
     * 
     * @return vec3 the rotation from the camera
     */
    vec3 getRotation();

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

    /**
     * @brief get the field of view from the camera
     * 
     * @return float the current fov from the camera
     */
    float getFOV();

    /**
     * @brief Get the Far Plane of the camera
     * 
     * @return float the far cliping plane
     */
    float getFarPlane();

    /**
     * @brief Get a pointer to the rotation matrix of the camera
     * 
     * @return float* a pointer to the camera rotation matrix
     */
    float* getRotMatPointer();

    /**
     * @brief Get a pointer to the projection matrix
     * 
     * @return float* a pointer to the projection matrix
     */
    float* getProjectionMatrixPointer();

    /**
     * @brief Get a pointer to the transformation matrix
     * 
     * @return float* a pointer to the transformation matrix
     */
    float* getTransformMatPointer();
    
    /**
     * @brief Get a pointer to the view matrix
     * 
     * @return float* a pointer to the view matrix
     */
    float* getViewMatrixPointer();

    /**
     * @brief Get the view matrix
     * 
     * @return mat4 the complete view matrix
     */
    mat4 getViewMatrix();

    /**
     * @brief Set the Window Index for the camera (this function is called if an camera is bound to an window, no need to call it again afterwards)
     * 
     * @param windowIndex the window index
     */
    void setWindowIndex(unsigned int windowIndex);

    /**
     * @brief Get the Window index from the window the camera is bound to
     * 
     * @return unsigned int the index of the window
     */
    unsigned int getWindowIndex();

    /**
     * @brief make the camera ready to draw from this perspective
     */
    void readyForDraw();

private:
    /**
     * @brief the data that should be send to the GPU
     */
    struct CameraData
    {
        //store the whole camera matrix
        mat4 camMat;
        //store the projection matrix
        mat4 projMat;
        //store the transformation matrix
        mat4 transMat;
        //store the rotation matrix
        mat4 rotMat;
        //store the camera position
        vec3 pos;
        //store the camera rotation
        vec3 rot;
        //store the near clipping plane
        float near;
        //store the far clipping plane
        float far;
        //store the field of view
        float fov;
    };
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
    //store the transform of the camera
    Transform transf;
    //store the window index the camera is bound to
    unsigned int windowIndex = -1;
    //store the camera data
    CameraData camData;
    //store the ubo
    unsigned int ubo;

    //store the location of the exposure
    unsigned int expLoc;
    //store the exposure
    float exposure = 0.1;

    //calculate the view matrix
    void calculateViewMatrix();

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