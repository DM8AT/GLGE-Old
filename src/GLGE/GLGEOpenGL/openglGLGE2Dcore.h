/**
 * @file openglGLGE2Dcore.h
 * @author DM8AT
 * @brief define the 2D core of GLGE
 * @version 0.1
 * @date 2024-01-02
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _GLGE_2D_CORE_OGL_H_
#define _GLGE_2D_CORE_OGL_H_

//include the base classes for the 2D core
#include "../GLGEIndependend/glge2DcoreDefClasses.h"
//include the base
#include "openglGLGE.h"

/**
 * @brief an 2D object
 */
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
     * @brief Construct a new 2D Object
     * 
     * @param mesh the mesh for the object
     * @param transform the transform of the object
     * @param isStatic say if the object should stay on the same screen position relative to the world
     */
    Object2D(Mesh2D mesh, Transform2D transform = Transform2D(), bool isStatic = false);

    /**
     * @brief Construct a new 2D object
     * 
     * @param preset the preset for the objcet (starting with GLGE_PRESET_)
     * @param resolution the resolution for the preset
     * @param color the color for the object (w = -1 for texture coordinates)
     * @param transform the transform of the object
     * @param isStatic say if the object should stay on the same screen position relative to the world
     */
    Object2D(unsigned int preset, unsigned int resolution, vec4 color, Transform2D transform = Transform2D(), bool isStatic = false);

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
    void setShader(unsigned int shader);

    /**
     * @brief Set the Shader
     * 
     * @param vertexShader the source code for the vertex shader
     * @param fragmentShader the source code for the fragment shader
     */
    void setShader(std::string vertexShader, std::string fragmentShader);

    /**
     * @brief Get the Shader
     * 
     * @return unsigned int the compiled shader
     */
    unsigned int getShader();

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
    void setTexture(unsigned int texture);

    /**
     * @brief deletes the own texture and sets it to 0
     */
    void deleteTexture();

    /**
     * @brief Get the Texture from the file
     * 
     * @return unsigned int the allready compiled OpenGL texture
     */
    unsigned int getTexture();

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

    /**
     * @brief Set the anchor position for the objct
     * 
     * @param anchor the anchor
     */
    void setAnchor(vec2 anchor);

    /**
     * @brief Set the anchor position for the objct
     * 
     * @param x the x part of the anchor
     * @param y the y part of the anchor
     */
    void setAnchor(float x, float y);

    /**
     * @brief Get the anchor position from the object
     * 
     * @return vec2 the anchor position
     */
    vec2 getAnchor();

    /**
     * @brief Set the Static mode of the object
     * 
     * @param stat true : the object will be static | false : the object will be free to move arround
     */
    void setStatic(bool stat);

    /**
     * @brief Get the Static mode from the object
     * 
     * @return true : the object is static
     * @return false : the object is free to move arround
     */
    bool getStatic();

    /**
     * @brief endoce the object to some object data
     * 
     * @return glgeObjectData the encoded object data
     */
    Data* encode();

    /**
     * @brief set the object to some object data
     * 
     * @param data the data to set the object to
     */
    void decode(Data data);

    /**
     * @brief a hook into the encoding process to use if you don't want to override the original function | TIMESTAMP: before return
     * 
     * @param data a pointer to the encoded data AFTER the encoding process
     */
    virtual void encodeHook(Data* data);

    /**
     * @brief a hook into the encoding process to use if you don't want to override the original function | TIMESTAMP: before start
     * 
     * @param data a pointer to the encoded data BEFORE the encoding process
     */
    virtual void encodeHookEarly(Data* data);

    /**
     * @brief a hook into the decoding process to use if you don't want to override the original function | TIMESTAMP: before return
     * 
     * @param data a pointer to the left data AFTER the decoding process
     */
    virtual void decodeHook(Data* data);

    /**
     * @brief a hook into the decoding process to use if you don't want to override the original function | TIMESTAMP: before start
     * 
     * @param data a pointer to the entire data BEFORE the decoding process
     */
    virtual void decodeHookEarly(Data* data);

protected:
    //store the anchor position for the 2D Object
    vec2 anchor = vec2(0);
    //store the mesh for the object
    Mesh2D mesh;
    //store the transform of the object
    Transform2D transf;
    //store the vertex and index buffer
    unsigned int VBO=0, IBO=0;
    //save the shader
    int shader = 0;
    //store the move matrix location
    unsigned int moveMatLoc = 0;
    //the local matrix to make the object correct
    mat3 moveMat = mat3(1,0,0,
                        0,1,0,
                        0,0,0);
    //store a texture
    unsigned int texture = 0;
    //say if the object is static
    bool isStatic = false;
    //save the length of the vertex and index buffer
    unsigned int VBOLen=0, IBOLen=0;
    //store the object UUID
    unsigned int id = 0;
    //store the window the buffers where created in
    int windowID = -1;

    /**
     * @brief perform any setup scripts for the object that are not mesh-related
     */
    void superConstructor();

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

/**
 * @brief a camera to scroll around the 2D viewport
 */
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

//////////////////
// BUTTON CLASS //
//////////////////

/**
 * @brief a simple button pre-set to handle circular and rectangle buttons
 */
class Button : public Object2D
{
public:
    /**
     * @brief Construct a new Button
     * 
     * default constructor
     */
    Button();

    /**
     * @brief Construct a new Button
     * 
     * @param texture the texture to use for the button
     * @param transf the transform for the button
     */
    Button(const char* texture, Transform2D transf = Transform2D());

    /**
     * @brief Construct a new Button
     * 
     * @param width the width of the button
     * @param height the height of the button
     * @param color the color of the button
     * @param transf the transform for the button
     */
    Button(unsigned int width, unsigned int height, vec4 color, Transform2D transf = Transform2D());

    /**
     * @brief Construct a new Button
     * 
     * @param size the size of the button
     * @param color the color of the button
     * @param transf the transform for the button
     */
    Button(vec2 size, vec4 color, Transform2D transf = Transform2D());

    /**
     * @brief Construct a new Button
     * 
     * @param radius the radius for a circle
     * @param res the resolution of the circle
     * @param color the color of the button
     * @param transf the transform for the button
     */
    Button(float radius, unsigned int res, vec4 color, Transform2D transf = Transform2D());

    /**
     * @brief update the object
     */
    void update();

    /**
     * @brief get if the button is currently clicked
     * 
     * @return true : the button is clicked | 
     * @return false : the button is not clicked
     */
    bool isClicked();

    /**
     * @brief get if the button is hoverd
     * 
     * @return true : the button is hoverd | 
     * @return false : the button is not hoverd
     */
    bool isHoverd();

    /**
     * @brief get if the button was clicked this tick
     * 
     * @return true : the button was clicked this tick | 
     * @return false : the button was not clicked this tick
     */
    bool clickThisTick();

    /**
     * @brief get if the mouse moved over the button in this tick
     * 
     * @return true : the hovering started this tick | 
     * @return false : the hovering did not start this tick
     */
    bool hoverThisTick();

    /**
     * @brief get if the click stopped this tick
     * 
     * @return true : the click stopped this tick | 
     * @return false : the click did not stop this tick
     */
    bool clickStopThisTick();

    /**
     * @brief get if the hovering stopped this tick
     * 
     * @return true : the hovering did stop this tick | 
     * @return false : the hovering did not stop this tick
     */
    bool hoverStopThisTick();

    /**
     * @brief a hook into the encoding process
     * 
     * @param data the encoded data
     */
    void encodeHook(Data* data) override;

    /**
     * @brief a hook into the decoding process
     * 
     * @param data the data to decode
     */
    void decodeHook(Data* data) override;

protected:
    /**
     * @brief store if the button was clicked last tick
     */
    bool clickLastTick = false;
    /**
     * @brief store if the button was hoverd last tick
     */
    bool hoverLastTick = false;
    /**
     * @brief store if the button was clicked this tick
     */
    bool hoverThis = false;
    /**
     * @brief store if the button was hoverd this tick
     */
    bool clickThis = false;
    /**
     * @brief store if the button was released this tick
     */
    bool clickStopThis = false;
    /**
     * @brief store if the hovering was stopped this tick
     */
    bool hoveringStopedThis = false;
    /**
     * @brief store if a circle is used
     */
    bool isCircle = false;
    /**
     * @brief store the width and height or radius
     */
    vec2 size = vec2(0);
};

/**
 * @brief a simple class to render text with SDL2
 */
class Text : public Object2D
{
public:
    /**
     * @brief Construct a new Text
     * 
     * default constructor
     */
    Text();

    /**
     * @brief Construct a new Text
     * 
     * @param text the text to render
     * @param font the font for the text
     * @param color the color of the text
     * @param fontSize the font size of the text
     * @param transf the transform for the text
     * @param dynamicMesh true : the mesh will be recalculated every time the words change | false : the mesh won't change
     */
    Text(const char* text, const char* font, vec4 color = vec4(0,0,0,1), int fontSize = 30, Transform2D transf = Transform2D(), bool dynamicMesh = true);

    /**
     * @brief Get the text from this text element
     * 
     * @return std::string the text
     */
    std::string getText();

    /**
     * @brief Get the path to the used font
     * 
     * @return std::string the path to the font
     */
    std::string getFont();

    /**
     * @brief Set the text for this text element
     * 
     * @param text the new text for the text object
     */
    void setText(std::string text);

    /**
     * @brief Set the font for this text element
     * 
     * @param fontPath the path to the font file
     */
    void setFont(std::string fontPath);

    /**
     * @brief Get the font scale
     * 
     * @return int the font scale
     */
    unsigned int getFontSize();

    /**
     * @brief Set the font scale factor
     * 
     * @param fontScale the new font scale
     */
    void setFontSize(unsigned int fontScale);

    /**
     * @brief get the color of the text
     * 
     * @return vec4 the text color
     */
    vec4 getTextColor();

    /**
     * @brief Set the color for the text
     * 
     * @param color the new text color
     */
    void setTextColor(vec4 color);

    /**
     * @brief Get the texture containing the text
     * 
     * @return int an OpenGL texture pointer for the texture
     */
    int getTextTexture();

    /**
     * @brief Get if dynamic meshing is activated
     * 
     * @return true : the mesh will be recalculated every time the words change | 
     * @return false : the mesh won't change
     */
    bool getDynamicMeshing();

    /**
     * @brief Set if dynamic meshing is activated or not
     * 
     * @param dynamicMesh true : the mesh will be recalculated every time the words change | false : the mesh won't change
     */
    void setDynamicMeshing(bool dynamicMesh);

    /**
     * @brief encode the needed data to store this object
     * 
     * @param data the data to store in
     */
    void encodeHook(Data* data) override;

    /**
     * @brief decode the data to generate this object
     * 
     * @param data the data to use
     */
    void decodeHook(Data* data) override;

protected:
    //the text the object is displaying
    std::string text;
    //store the size of the texture
    vec2 texSize;
    //store the font size
    unsigned int fontSize = 30;
    //store the color of the text
    vec4 color;
    //store the font
    std::string font;
    //store if dynamic meshing is activated
    bool dynamicMesh = true;

    //update the texture
    void updateTexture();
};

/**
 * @brief A class to handle simple text input
 */
class TextInput : public Text
{
public:

    /**
     * @brief default constructor
     * 
     */
    TextInput();

    /**
     * @brief Construct a new Text Input box
     * 
     * @param text the default text
     * @param font the font for the text
     * @param color the color of the text
     * @param fontSize the font size of the text
     * @param transf the transform for the text
     */
    TextInput(const char* text, const char* font, vec4 color = vec4(0,0,0,1), int fontSize = 30, Transform2D transf = Transform2D());

    /**
     * @brief update the text input box
     */
    void update();

    /**
     * @brief get if the object is currently focused
     * 
     * @return true : the object accepts text input | 
     * @return false : the object is just text
     */
    bool isFocused();

    /**
     * @brief get if the object will cleare once it is focused
     * 
     * @return true : the object will clear | 
     * @return false : the object wont clear
     */
    bool isEmpty();

    /**
     * @brief Set a function that should be called every time something is typed
     * 
     * @param func the function to call
     */
    void setOnTypeFunction(void (*func)());

    /**
     * @brief Get the function that should be called every time something is typed
     */
    void (*getOnTypeFunc())();

    /**
     * @brief Set a function that should be called if the text input is enterd
     * 
     * @param func the function to call
     */
    void setOnEnterFunction(void (*func)());

    /**
     * @brief Get the function that should be called if the text input is enterd
     */
    void (*getOnEnterFunc())();

    /**
     * @brief Set a function that should be called if the text input is exited
     * 
     * @param func the function to call
     */
    void setOnExitFunction(void (*func)());

    /**
     * @brief Get the function that should be called if the text input is exited
     */
    void (*getOnExitFunc())();

    /**
     * @brief encode the needed data to store this object
     * 
     * @param data the data to store in
     */
    void encodeHook(Data* data) override;

    /**
     * @brief decode the data to generate this object
     * 
     * @param data the data to use
     */
    void decodeHook(Data* data) override;
    
protected:
    //store the current courser position
    int cursourPos = 0;
    //store if the box is focused
    bool focused = false;
    //store if the text is empty
    bool empty = false;
    //store the on tick function
    void (*onTypeFunc)() =  NULL;
    //store the on enter function
    void (*onEnterFunc)() =  NULL;
    //store the on exit function
    void (*onExitFunc)() =  NULL;
};

//FUNCTIONS

/**
 * @brief initalise the 2D core functions
 */
void glgeInit2DCore();

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