//include the GLGE dependencys
#include "GLGE.h"
#include "GLGE/glgeErrors.h"
#include "GLGE/glgeVars.h"
#include "GLGE/glgeFuncs.h"

//include the OpenGL dependencys
#include <GL/freeglut.h>

//include stbi_image
#define STB_IMAGE_IMPLEMENTATION
#include "GLGE/stb_image.h"

//include the standart librarys
#include <math.h>
#include <cstring>
#include <iostream>

///////////
//CLASSES//
///////////

//default constructor
Triangle::Triangle()
{
    //say that it exists
}

//constructor using colors
Triangle::Triangle(unsigned int indices[3], vec4 colors[3], vec3 normal)
{
    //store the inputed indices
    this->indices[0] = indices[0];
    this->indices[1] = indices[1];
    this->indices[2] = indices[2];

    //store the inputed colors
    this->colors[0] = colors[0];
    this->colors[1] = colors[1];
    this->colors[2] = colors[2];
    //say that colors should be used
    this->useColor = 1.f;

    //store the inputed normal
    this->normal = normal;
}

//constructor using texcoords
Triangle::Triangle(unsigned int indices[3], vec2 texCoords[3], vec3 normal)
{
    //store the inputed indices
    this->indices[0] = indices[0];
    this->indices[1] = indices[1];
    this->indices[2] = indices[2];

    //store the inputed colors
    this->texCoords[0] = texCoords[0];
    this->texCoords[1] = texCoords[1];
    this->texCoords[2] = texCoords[2];
    //say that colors should be used
    this->useColor = 0.f;

    //store the inputed normal
    this->normal = normal;
}

//constructor using single numbers
Triangle::Triangle(unsigned int a, unsigned int b, unsigned int c, vec4 ca, vec4 cb, vec4 cc, vec3 normal)
{
    //store the inputed indices
    this->indices[0] = a;
    this->indices[1] = b;
    this->indices[2] = c;

    //store the inputed colors
    this->colors[0] = ca;
    this->colors[1] = cb;
    this->colors[2] = cc;
    //say that colors should be used
    this->useColor = 1.f;

    //store the inputed normal
    this->normal = normal;
}

//constructor using single numbers for a texture
Triangle::Triangle(unsigned int a, unsigned int b, unsigned int c, vec2 ta, vec2 tb, vec2 tc, vec3 normal)
{
    //store the inputed indices
    this->indices[0] = a;
    this->indices[1] = b;
    this->indices[2] = c;

    //store the inputed colors
    this->texCoords[0] = ta;
    this->texCoords[1] = tb;
    this->texCoords[2] = tc;
    //say that colors should be used
    this->useColor = 0.f;

    //store the inputed normal
    this->normal = normal;
}

////////////////////
//Public functions//
////////////////////

//init function for glge
void glgeInit(int argc, char** argv)
{
    //error check
    bool error = false;
    //if argc = 0, the length of argv is 0 --> output error, but only if error output is enabled
    if (argc == 0)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_ARGC_OUT_OF_BOUNDS, argc);
        }
        error = true;
    }
    //if argv is the nullpointer, output an error if error output is enabled
    if (argv == nullptr)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_NULLPOINTER_INP_INIT);
        }
        error = true;
    }
    //if an error occoured, stop the function
    if (error)
    {
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_GLGE_INIT_ERROR << std::endl;
        }
        //stop the program
        exit(1);
    }

    glutInit(&argc, argv);
}

//first window creation methode
void glgeCreateWindow(const char* name, vec2 size, vec2 pos)
{
    //check for erros in the inputs
    bool error = false;
    //check if the size parameters are correct
    if (size.x <= 0 || size.y <= 0)
    {
        //if the size parameters are invalide, start to print an error
        if (glgeErrorOutput)
        {
            //print diffrent messages depending on the input size
            if (size.x < 0 || size.y < 0)
            {
                printf(GLGE_ERROR_WINDOW_SIZE_NEGATIVE);
            }
            else
            {
                printf(GLGE_ERROR_WINDOW_SIZE_ZERO);
            }
        }
        //say that an error occured
        error = true;
    }
    //check if the name pointer is invalide
    if (name == nullptr)
    {
        //print an error message
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_WINDOW_NAME_NULLPOINTER);
        }
        //say that an error occured
        error = true;
    }
    //check if the position is negative
    if (pos < vec2(0,0))
    {
        //print an error message
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_WINDOW_NEGATIVE_POSITION);
        }
        //say that an error occured
        error = true;
    }
    //if an error occured, return
    if (error)
    {
        //print an error message
        if (glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_WINDOW_INPUT_ERROR << std::endl;
        }
        //stop the program
        exit(1);
    }
    //start the window creation
    createWindow(name, size, pos);
}

//second window creation methode
void glgeCreateWindow(const char* name, int width, int height, vec2 pos)
{
    //check for erros in the inputs
    bool error = false;
    //check if the size parameters are correct
    if (width <= 0 || height <= 0)
    {
        //if the size parameters are invalide, start to print an error
        if (glgeErrorOutput)
        {
            //print diffrent messages depending on the input size
            if (width < 0 || height < 0)
            {
                printf(GLGE_ERROR_WINDOW_SIZE_NEGATIVE);
            }
            else
            {
                printf(GLGE_ERROR_WINDOW_SIZE_ZERO);
            }
        }
        //say that an error occured
        error = true;
    }
    //check if the name pointer is invalide
    if (name == nullptr)
    {
        //print an error message
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_WINDOW_NAME_NULLPOINTER);
        }
        //say that an error occured
        error = true;
    }
    //check if the position is negative
    if (pos < vec2(0,0))
    {
        //print an error message
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_WINDOW_NEGATIVE_POSITION);
        }
        //say that an error occured
        error = true;
    }
    //if an error occured, return
    if (error)
    {
        //print an error message
        if (glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_WINDOW_INPUT_ERROR << std::endl;
        }
        //stop the program
        exit(1);
    }
    //start the window creation
    createWindow(name, vec2(width, height), pos);
}

//set if GLGE should output errors
void glgeSetErrorOutput(bool in)
{
    //set the boolean to controll error output
    glgeErrorOutput = in;
}

//get if GLGE should output errors
bool glgeGetErrorOutput()
{
    //return if GLGE should output errors
    return glgeErrorOutput;
}

//set if GLGE should print warnings
void glgeSetWarningOutput(bool out)
{
    glgeWarningOutput = out;
}

//get if GLGE should print warnings to the console
bool glgeGetWarningOutput()
{
    return glgeWarningOutput;
}

//start the main loop of GLGE
void glgeRunMainLoop()
{
    //start the glut main loop
    glutMainLoop();
}

//first function to set the clear color
void glgeSetClearColor(float r, float g, float b, bool normalise)
{
    //check for errors in the input
    bool error = false;
    //check if the values are negative
    if (r < 0 || g < 0 || b < 0)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_NEGAITVE_COLOR_VAL);
        }
        error = true;
    }

    //if the values need to be normalised, normalise them
    if (normalise)
    {
        r /= 255.f;
        g /= 255.f;
        b /= 255.f;
    }

    //check for out of bound values
    if (r > 1.f || g > 1.f || b > 1.f)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_COLOR_VALUE_OUT_OF_BOUNDS);
        }
        error = true;
    }

    //if an error occured, exit
    if (error)
    {
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_SET_CLEAR_COLOR << std::endl;
        }
        exit(1);
    }

    //finaly, set the clear color
    glClearColor(r,g,b,1.f);
}

//second function to set the clear color
void glgeSetClearColor(vec3 color, bool normalise)
{
    //check for errors in the input
    bool error = false;
    //check if the values are negative
    if (color.x < 0 || color.y < 0 || color.z < 0)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_NEGAITVE_COLOR_VAL);
        }
        error = true;
    }

    //if the values need to be normalised, normalise them
    if (normalise)
    {
        color /= vec3(255,255,255);
    }

    //check for out of bound values
    if (color.x > 1.f || color.y > 1.f || color.z > 1.f)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_COLOR_VALUE_OUT_OF_BOUNDS);
        }
        error = true;
    }

    //if an error occured, exit
    if (error)
    {
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_SET_CLEAR_COLOR << std::endl;
        }
        exit(1);
    }

    //finaly, set the clear color
    glClearColor(color.x,color.y,color.z,1.f);
}

//thired function to set the clear color
void glgeSetClearColor(vec4 color, bool normalise)
{
    //check for errors in the input
    bool error = false;
    //check if the values are negative
    if (color.x < 0 || color.y < 0 || color.z < 0)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_NEGAITVE_COLOR_VAL);
        }
        error = true;
    }

    //if the values need to be normalised, normalise them
    if (normalise)
    {
        color /= vec4(255,255,255,1);
    }

    //check for out of bound values
    if (color.x > 1.f || color.y > 1.f || color.z > 1.f)
    {
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_COLOR_VALUE_OUT_OF_BOUNDS);
        }
        error = true;
    }

    //if an error occured, exit
    if (error)
    {
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_SET_CLEAR_COLOR << std::endl;
        }
        exit(1);
    }

    //finaly, set the clear color
    glClearColor(color.x,color.y,color.z,1.f);
}

//bind a display func callback
void glgeBindDisplayFunc(void (*func)())
{
    //check if an error occured
    bool error = false;
    if(func == nullptr)
    {
        //print an error
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_FUNC_IS_NULLPOINTER);
        }
        //say that an error occured
        error = true;
    }

    //if an error occured, exit
    if(error)
    {
        //print an exit message
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_BIND_DISPLAY_CALLBACK << std::endl;
        }
        //stop the program
        exit(1);
    }

    //set the display callback
    glgeDisplayCallback = func;

    //say that an display callback is bound
    glgeHasDisplayCallback = true;
}

//debind a display func callback
void glgeClearDisplayFunc()
{
    //set the display func callback to the nullpointer
    glgeDisplayCallback = nullptr;

    //say that no display function is bound
    glgeHasDisplayCallback = false;
}

//bind a main callback function
void glgeBindMainFunc(void (*func)())
{
    //check if an error occured
    bool error = false;
    if(func == nullptr)
    {
        //print an error
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_FUNC_IS_NULLPOINTER);
        }
        //say that an error occured
        error = true;
    }

    //if an error occured, exit
    if(error)
    {
        //print an exit message
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_BIND_MAIN_CALLBACK << std::endl;
        }
        //stop the program
        exit(1);
    }

    //set the display callback
    glgeMainCallback = func;

    //say that an display callback is bound
    glgeHasMainCallback = true;
}

//unbind the main callback
void glgeClearMainFunc()
{
    //set the main function callback to the nullpointer
    glgeMainCallback = nullptr;

    //say that no main callback is bound
    glgeHasMainCallback = false;
}

void glgeSetMaxFPS(int fps)
{
    //check for errors
    bool error = false;
    //check if the maximal frames per second are less or equal to 0
    if (fps <= 0)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_FPS_LESS_ONE);
        }
        //say that an error occured
        error = true;
    }

    //if an error occured, break
    if (error)
    {
        //output an exit message
        if(glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_SET_MAX_FPS << std::endl;
        }
        //stop the program
        exit(1);
    }

    //set the maximal frames per second
    glgeMaxFPS = fps;
}

//get the current frames per second
int glgeGetCurrentFPS()
{
    //return the current FPS
    return glgeCurrentFPS;
}

//get the maximal frames per second
int glgeGetMaxFPS()
{
    //return the max FPS
    return glgeMaxFPS;
}

//return if GLGE has an additional main function bound
bool glgeHasMainFunc()
{
    return glgeHasMainCallback;
}

//return if GLGE has an additional display function bound
bool glgeHasDisplayFunc()
{
    return glgeHasDisplayCallback;
}

//return if GLGE has a window bound
bool glgeHasWindow()
{
    return glgeHasMainWindow;
}

//set the move matrix name
void glgeSetMoveMatrixName(const char* name)
{
    //check if the input is valide
    if(name == nullptr)
    {
        //output an error message and say where the error occured
        if(glgeErrorOutput)
        {
            printf(GLGE_ERROR_INVALIDE_INPUT);
            std::cerr << GLGE_ERROR_STR_GLGE_SET_MOV_MAT_NAME << std::endl;
        }
        //stop the program
        exit(1);
    }
    //sore the name for the move matrix
    glgeMoveMatrix = (char*)name;
}

//get the momantan name of the move matrix
char* glgeGetMoveMatrixName()
{
    //return the name of the move matrix
    return glgeMoveMatrix;
}

//get the aspect of the window
float glgeGetWindowAspect()
{
    //return the window aspect
    return glgeWindowAspect;
}

//return the pressed keys
Keys glgeGetKeys()
{
    //return the pressed keys
    return glgePressedKeys;
}

//get the delta time
float glgeGetDeltaTime()
{
    //return the delta time
    return glgeDeltaTime;
}

//add a shader to an shader program
void glgeAddShader(GLuint shaderProgram, const char* shadertext, GLenum shaderType)
{
    //create a new shader with the inputed type
    GLuint shaderObj = glCreateShader(shaderType);

    //check if the shader object could be created
    if (shaderObj == 0)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            std::cout << GLGE_ERROR_COULD_NOT_CREATE_SHADER << shaderType << std::endl;
            //print where the error occured
            std::cerr << GLGE_ERROR_STR_OBJECT_ADD_SHADER << std::endl;
        }
        //stop the script
        exit(1);
    }

    //set a GLchar to the inputed text
    const GLchar* p[1];
    p[0] = shadertext;

    //store the length of the text
    GLint lengths[1];
    lengths[0] = strlen(shadertext);

    //set the shader source code
    glShaderSource(shaderObj, 1, p, lengths);

    //compile to the new shader object
    glCompileShader(shaderObj);

    //check for compiling errors
    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

    //if there was an error, print a message and exit
    if (!success)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            //create an info log to store the error created by open gl
            GLchar InfoLog[1024];
            glGetShaderInfoLog(shaderObj, 1024, NULL, InfoLog);
            //print the message
            printf(GLGE_ERROR_SHADER_COMPILE_ERROR, shaderType, InfoLog);
            //print where the error occured
            std::cerr << GLGE_ERROR_STR_OBJECT_ADD_SHADER << std::endl;
        }
        //stop the script
        exit(1);
    }

    //attach the shader object to the final program
    glAttachShader(shaderProgram, shaderObj);
}

//get a uniform variable from a shader
GLint glgeGetUniformVar(GLuint program, const char* name)
{
    //create the output variable
    GLint ret;
    //set the output variable to the uniform variable in the shader
    ret = glGetUniformLocation(program, name);
    //if the id is -1, output an error
    if (ret == -1)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_UNIFORM_VAR_NOT_FOUND, name);
            //say where the error occured
            std::cerr << GLGE_ERROR_STR_OBJECT_GET_UNIFORM_VARIABLE << std::endl;
        }
        //stop the program
        exit(1);
    }
    //if no error occured, return the id of the uniform variable
    return ret;
}

//compile and add the shaders to this object
GLuint glgeCompileShader(const char* fileNameVS, const char* fileNameFS)
{
    //create a new shader program
    GLuint shaderProgram = glCreateProgram();

    //check if the shader could be created
    if (shaderProgram == 0)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            printf(GLGE_ERROR_COULD_NOT_CREATE_SHADER);
            //say where the error occured
            std::cerr << GLGE_ERROR_STR_OBJECT_COMPILE_SHADERS << std::endl;
        }
        //stop the program
        exit(1);
    }

    //create strings for the shaders
    std::string vs, fs;

    //read the files
    if (!readFile(fileNameVS, vs))
    {
        //output an error message
        if (glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_OBJECT_COMPILE_SHADERS << std::endl;
        }
        //stop the script
        exit(1);
    }
    //add the shader program from the first file
    glgeAddShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);

    //read the second file
    if (!readFile(fileNameFS, fs))
    {
        //if the file can't be read, output an error message
        if (glgeErrorOutput)
        {
            std::cerr << GLGE_ERROR_STR_OBJECT_COMPILE_SHADERS << std::endl;
        }
        //stop the program
        exit(1);
    }
    //add the shader program from the second file
    glgeAddShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    //create an variable to check for success
    GLint success = 0;
    //setup an error log
    GLchar ErrorLog[1024] = {0};

    //link the shader program
    glLinkProgram(shaderProgram);

    //get the program iv from the shader
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    //check if the program linking was no success
    if (success == 0)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            //get the error from open gl and output it with an custom message
            glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
            printf(GLGE_ERROR_SHADER_VALIDATE_ERROR, ErrorLog);
        }
        //stop the program
        exit(1);
    }

    //check if the program is valide
    glValidateProgram(shaderProgram);
    //get the program iv again
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    //check for success
    if (!success)
    {
        //output an error message
        if (glgeErrorOutput)
        {
            //get the error from open gl and output it with an custom message
            glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
            printf(GLGE_ERROR_SHADER_VALIDATE_ERROR, ErrorLog);
        }
        //stop the program
        exit(1);
    }

    //say open GL to use the shader program
    glUseProgram(shaderProgram);
    //return the shader program in the GLGE Object
    return shaderProgram;
}

GLuint glgeTextureFromFile(const char* name, vec2* sP)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(name, &width, &height, &nrChannels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture file: " << name << std::endl;
    }

    //store the size if the input is not NULL
    if (sP != NULL)
    {
        sP->x = width;
        sP->y = height;
    }

    //clear memory and return the texture
    stbi_image_free(data);
    return texture;
}

vec2 glgeGetTextureSize(const char* name)
{
    int w, h, c;
    unsigned char* data = stbi_load(name, &w, &h, &c, 0);
    if(!data)
    {
        std::cout << "Failed to load texture file: " << name << std::endl;
    }
    stbi_image_free(data);
    return vec2(h,w);
}