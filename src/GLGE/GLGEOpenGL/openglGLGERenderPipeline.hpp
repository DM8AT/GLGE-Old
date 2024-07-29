/**
 * @file openglGLGERenderPipeline.hpp
 * @author DM8AT
 * @brief the wraper for the render pipeline. GLGE provides a default, but this class allows full costomization. 
 * @version 0.1
 * @date 2024-06-08
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#ifndef _OGL_GLGE_RENDER_PIPELINE_H_
#define _OGL_GLGE_RENDER_PIPELINE_H_

#include <unordered_map>
#include "openglGLGE.h"

/**
 * @brief a pass that dose nothing
 */
#define GLGE_PASS_NONE 0
/**
 * @brief a pass that draws all solid geometry
 * \par Info
 * This calles the window's draw function with the transparent pass flag set to 'false'
 */
#define GLGE_PASS_DRAW_SOLID 1
/**
 * @brief a pass that draws all transparent geometry 
 * \par Info
 * This calles the window's draw function with the transparent pass flag set to 'true'
 */
#define GLGE_PASS_DRAW_TRANSPARENT 2
/**
 * @brief a pass that executes the bound post processing functions and post processing shaders
 */
#define GLGE_PASS_POST_PROCESSING 3
/**
 * @brief a pass that calculates the lighting for a defferd renderer
 */
#define GLGE_PASS_LIGHING 4
/**
 * @brief a pass that draws the skybox but dosn't clear the screen
 */
#define GLGE_PASS_DRAW_SKYBOX 5
/**
 * @brief perform pre-calculations for shadow mapping
 */
#define GLGE_PASS_SHADOWS 6
/**
 * @brief clear the old frame from the geometry buffer. Run this after shadow mapping and before drawing the geometry
 */
#define GLGE_PASS_CLEAR_G_BUFFER 7
/**
 * @brief copy a color attachment from the framebuffer to the post processing framebuffer \n 
 * use the logic or opperator with an attachment to select the attachment
 */
#define GLGE_PASS_COPY_G_TO_PP 8

/**
 * @brief the default color for a pixel
 */
#define GLGE_G_BUFF_COLOR_ATTACHMENT_ALBEDO 0
/**
 * @brief the normal vector for a pixel
 */
#define GLGE_G_BUFF_COLOR_ATTACHMENT_NORMAL 1
/**
 * @brief the world space position for the pixel
 */
#define GLGE_G_BUFF_COLOR_ATTACHMENT_POSITION 2
/**
 * @brief the roughenss, metallicness and lit factor for the pixel
 */
#define GLGE_G_BUFF_COLOR_ATTACHMENT_RML 3
/**
 * @brief the computed lighting output
 */
#define GLGE_G_BUFF_COLOR_ATTACHMENT_LIT 4
/**
 * @brief the Object ID, the distance and the transparent max accumulation for the pixel
 */
#define GLGE_G_BUFF_COLOR_ATTACHMENT_EIDA 5
/**
 * @brief the solid texture
 */
#define GLGE_G_BUFF_COLOR_ATTACHMENT_SOLID 6
/**
 * @brief the transparent accumulation texture
 */
#define GLGE_G_BUFF_COLOR_ATTACHMENT_TRANS_ACCUM 7

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// POST PROCESSING //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief stores the information on what to execute for one post processing stage local to a post processing stack
 */
struct PostProcessingStage
{
    //store the name of the post processing stage
    std::string name = "NoName";
    //store if a function should execute
    bool isFunc = false;
    //store the index in the buffers
    int index = -1;
};

/**
 * @brief stores all post processing functions and shaders for a post processing pass
 */
class PostProcessingStack
{
public:
    /**
     * @brief Default constructor
     */
    PostProcessingStack() = default;

    /**
     * @brief delete the Post Processing Stack
     */
    ~PostProcessingStack();

    /**
     * @brief add a stage to the post processing stack
     * 
     * @param shader the shader that should execute
     * @param name the name of the stage
     */
    void addStage(Shader* shader, std::string name = "NoName");
    /**
     * @brief add a stage to the post processing stack
     * 
     * @param func a function pointer to a post processing function to execute
     * @param name the name of the stage
     */
    void addStage(Shader(*func)(unsigned int), std::string name = "NoName");

    /**
     * @brief get a post processing stage by a custom name
     * 
     * @param name the name of the stage
     * @return PostProcessingStage* a pointer to the post procesing stage data
     */
    PostProcessingStage* getStage(std::string name);
    /**
     * @brief get a post processing stage by an index
     * 
     * @param index the index in the execution order
     * @return PostProcessingStage* a pointer to the post processing stage data
     */
    PostProcessingStage* getStage(unsigned int index);

    /**
     * @brief Get the Index of the stage with the specified name
     * 
     * @param name the name of the stage
     * @return int the index of the stage
     */
    int getIndexOfStage(std::string name);
    /**
     * @brief Get the Name Of the stage with the specified name
     * 
     * @param index the index of the stage
     * @return std::string the name of the stage
     */
    std::string getNameOfStage(unsigned int index);

    /**
     * @brief delete the first post processing stage with the given name
     * 
     * @param name the name of the stage to delete
     */
    void deleteStage(std::string name);
    /**
     * @brief delete the post processing stage at that index
     * 
     * @param index the index of the stage
     */
    void deleteStage(unsigned int index);

    /**
     * @brief swap two post processing stages
     * 
     * @param nameFirst the name of the first stage
     * @param nameSecond the name of the second stage
     */
    void swapStage(std::string nameFirst, std::string nameSecond);
    /**
     * @brief swap two post processing stages
     * 
     * @param indexFirst the index of the first stage
     * @param indexSecond the index of the second stage
     */
    void swapStage(unsigned int indexFirst, unsigned int indexSecond);

    /**
     * @brief move the first stage with that name to the specified index
     * 
     * @param name the name of the stage
     * @param index the index to move to
     */
    void moveStage(std::string name, unsigned int index);
    /**
     * @brief move the stage from the given index to the specified index
     * 
     * @param from the index of the location to move from
     * @param to the index of the location to move to
     */
    void moveStage(unsigned int from, unsigned int to);

    /**
     * @brief Get the post processing shader bound to the specific stage
     * 
     * @param stage a pointer to the stage to get the shader from
     * @return Shader* a pointer to the shader or NULL if a function is bound
     */
    Shader* getShaderOfStage(PostProcessingStage* stage);
    /**
     * @brief Get the post processing function bound to the specific stage
     * 
     * @param stage a pointer to the stage to get the function from
     * @return Shader(*)() a function pointer to the shader function
     */
    Shader(*getFunctionOfStage(PostProcessingStage* stage))(unsigned int);

    /**
     * @brief Set the Shader for a post processing stage
     * 
     * @param stage a pointer to the stage to modify
     * @param shader the shader for the stage
     * @param force says if the shader should be set if the stage used a function before
     */
    void setShaderOfStage(PostProcessingStage* stage, Shader* shader, bool force = false);
    /**
     * @brief Set the Function for a post processing stage
     * 
     * @param stage a poitner to the stage to modify
     * @param func the function for the stage
     * @param force says if the function should be set if the stage used a shader before
     */
    void setFunctionOfStage(PostProcessingStage* stage, Shader(*func)(unsigned int), bool force = false);

    /**
     * @brief get a post processing stage
     * 
     * @param index the index of the stage to get
     * @return PostProcessingStage* a pointer to the post processing stage
     */
    PostProcessingStage* operator[](int index);

    /**
     * @brief Get the amount of bound post processing stages
     * 
     * @return int the amount of stages
     */
    int getStageCount();

private:
    //stores all post processing functions
    std::vector<Shader(*)(unsigned int)> ppFunctions = {};
    //stores poitners to all post processing shaders
    std::vector<Shader*> ppShaders = {};
    //stores the information what should execute
    std::vector<PostProcessingStage> stages = {};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RENDER PIPELINE //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief defines the layout of a render pipeline stage
 */
struct Stage
{
    //store the stage name
    std::string name = "NoName";
    //store the pass identifier for the render pipeline pass
    unsigned int pass = GLGE_PASS_NONE;
    //store a pre-execution callback
    void(*preExecCallback)() = NULL;
    //store a post-execution callback
    void(*postExecCallback)() = NULL;
    //store the name of a post processing stack for a post processing pass
    std::string ppsName = "";
};

/**
 * @brief wrapper for a render pipeline
 */
class RenderPipeline
{
public:
    /**
     * @brief Default constructor
     */
    RenderPipeline() = default;

    /**
     * @brief Create a new render pipeline
     * 
     * @param stages all the stages for the render pipeline and theyre names
     */
    RenderPipeline(std::vector<Stage> stages);

    /**
     * @brief delete the render pipeline
     */
    ~RenderPipeline();

    /**
     * @brief Get all stages of the render pipeline
     * 
     * @return std::vector<Stage*>* a pointer to the internal stages
     */
    std::vector<Stage>* getStages();

    /**
     * @brief sets a stages parameters or adds the stage if it dosn't exist
     * 
     * @param name the name of the stage
     * @param stage a stage for the render pipeline pass
     */
    void setStage(std::string name, Stage stage);

    /**
     * @brief Get the stage trough its name
     * 
     * @param name the name of the stage
     * @return Stage* a pointer to the stage data
     */
    Stage* getStage(std::string name);

    /**
     * @brief set the post processing stack belonging to a post processin stage
     * 
     * @param stage the stage to modify
     * @param pps the post processing stack to bind to the stage
     * @param name the name of the post processing stack
     */
    void setPPStagePPS(Stage* stage, PostProcessingStack* pps, std::string name);

    /**
     * @brief get the post processing stack belonging to a stage
     * 
     * @param stage the stage to get the data from
     * @return PostProcessingStack* the post processing stack or NULL, if none is bound or stage isn't a post processing stage
     */
    PostProcessingStack* getPPStagePPS(Stage* stage);

    /**
     * @brief executes the render pipeline
     */
    void execute();

    /**
     * @brief says if the render pipeline is currently executing
     * 
     * @return true : the render pipeline is currently executing | 
     * @return false : the render pipeline is not executing
     */
    bool isExecuting();

    /**
     * @brief get the stage at the specified index in the pipeline
     * 
     * @param index the index in the rendering process
     * @return Stage* a pointer to the stage
     */
    Stage* operator[](unsigned int index);

private: 
    //store all stages for the render pipeline
    std::vector<Stage> stages = {};
    //store all post processing stacks
    std::unordered_map<std::string, PostProcessingStack*> ppStacks = {};
    //stores if the render pipeline is currently active
    bool isCurrent = false;

    /**
     * @brief draw a post processing stack
     * 
     * @param ppsStack a pointer to the post processing stack to use
     */
    void drawPPS(PostProcessingStack* ppsStack);

};

#endif