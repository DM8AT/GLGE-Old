/**
 * @file openglGLGERenderPipeline.cpp
 * @author DM8AT
 * @brief implements the render pipeline class from file openglGLGERenderPipeline.hpp
 * @version 0.1
 * @date 2024-06-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <algorithm>
#include "openglGLGERenderPipeline.hpp"
#include "openglGLGEVars.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// POST PROCESSING //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PostProcessingStack::~PostProcessingStack()
{
    //delete the stages
    this->stages.clear();
    //delete the shaders
    this->ppShaders.clear();
    //delete the functions
    this->ppFunctions.clear();
}

void PostProcessingStack::addStage(Shader* shader, std::string name)
{
    //create the stage data
    PostProcessingStage stage = {
        //store the name
        name: name,
        //specify that it is not a function
        isFunc: false,
        //store the shader index
        index: (int)this->ppShaders.size()
    };
    //store the shader
    this->ppShaders.push_back(shader);
    //store the stage data
    this->stages.push_back(stage);
}
void PostProcessingStack::addStage(Shader(*func)(unsigned int), std::string name)
{
    //create the stage data
    PostProcessingStage stage = {
        //store the name
        name: name,
        //specify that it is not a function
        isFunc: true,
        //store the shader index
        index: (int)this->ppFunctions.size()
    };
    //store the shader
    this->ppFunctions.push_back(func);
    //store the stage data
    this->stages.push_back(stage);
}

PostProcessingStage* PostProcessingStack::getStage(std::string name)
{
    //get the index of the name
    int i = this->getIndexOfStage(name);
    //if the name was found return the pointer else null
    return ((i == -1) ? NULL : &this->stages[i]);
}
PostProcessingStage* PostProcessingStack::getStage(unsigned int index)
{
    //return a pointer to the stage at that index
    return &this->stages[index];
}

int PostProcessingStack::getIndexOfStage(std::string name)
{
    //loop over all stages
    for (int i = 0; i < (int)this->stages.size(); i++)
    {
        //check the current stage name
        if (this->stages[i].name == name)
        {
            //return the index
            return i;
        }
    }

    //if it wasn't found return -1
    return -1;
}
std::string PostProcessingStack::getNameOfStage(unsigned int index)
{
    //return the name of the stage
    return this->stages[index].name;
}

void PostProcessingStack::deleteStage(std::string name)
{
    //get the index of the stage
    int i = this->getIndexOfStage(name);
    //if the stage wasn't found, return
    if (i == -1) { return; }
    //else, delete the stage
    this->stages.erase(this->stages.begin() + i);
}
void PostProcessingStack::deleteStage(unsigned int index)
{
    //delete the stage
    this->stages.erase(this->stages.begin() + index);
}

void PostProcessingStack::swapStage(std::string nameFirst, std::string nameSecond)
{
    //get the index of the first stage
    int iF = this->getIndexOfStage(nameFirst);
    //if the stage wasn't found, stop
    if (iF == -1) { return; }
    //get the index of the second stage
    int iS = this->getIndexOfStage(nameSecond);
    //if the stage wasn't found, return
    if (iS == -1) { return; }
    //swap the two stages
    this->swapStage(iF, iS);
}
void PostProcessingStack::swapStage(unsigned int indexFirst, unsigned int indexSecond)
{
    //get the stage at the first index
    PostProcessingStage stage = this->stages[indexFirst];
    //write the stage at the second index to the stage at the first index
    this->stages[indexFirst] = this->stages[indexSecond];
    //write the first stage at the second index
    this->stages[indexSecond] = stage;
}

void PostProcessingStack::moveStage(std::string name, unsigned int index)
{
    //get the index of the stage
    int i = this->getIndexOfStage(name);
    //if the stage wasn't found, stop
    if (i == -1) { return; }
    //move the stages
    this->moveStage(i, index);
}
void PostProcessingStack::moveStage(unsigned int from, unsigned int to)
{
    //check if the from location is bigger than the to location
    if (from > to)
    {
        //if that is true, rotate to negative direction
        std::rotate(this->stages.rend() - from - 1, this->stages.rend() - from, this->stages.rend() - to);
    }
    else
    {        
        //else rotate in a positive direction
        std::rotate(this->stages.begin() + from, this->stages.begin() + from + 1, this->stages.begin() + to + 1);
    }
}

Shader* PostProcessingStack::getShaderOfStage(PostProcessingStage* stage)
{
    //check if the stage uses shaders
    if (stage->isFunc)
    {
        //if not, return null
        return NULL;
    }
    else
    {
        //else, return the shader
        return this->ppShaders[stage->index];
    }
}
Shader(*PostProcessingStack::getFunctionOfStage(PostProcessingStage* stage))(unsigned int)
{
    //check if the stage uses a function
    if (stage->isFunc)
    {
        //if it dose, return the function
        return this->ppFunctions[stage->index];
    }
    else
    {
        //else, return NULL
        return NULL;
    }
}

void PostProcessingStack::setShaderOfStage(PostProcessingStage* stage, Shader* shader, bool force)
{
    //check if the stage used a function
    if (stage->isFunc)
    {
        //check if force is enabled
        if (force)
        {
            //delete the bound function
            this->ppFunctions.erase(this->ppFunctions.begin() + stage->index);
            //loop over all stages
            for (int i = 0; i < (int)this->stages.size(); i++)
            {
                //if the to manipulate stage is the current stage, skip it
                if (&this->stages[i] == stage) { continue; }
                //check if the stage uses a function that index is greater than the own
                if (this->stages[i].isFunc && (this->stages[i].index > stage->index))
                {
                    //decrement the stages index
                    this->stages[i].index--;
                }
            }

            //say that the stage now uses a shader
            stage->isFunc = false;
            //store the index of the shader
            stage->index = (int)this->ppShaders.size();
            //store the shader for the stage
            this->ppShaders.push_back(shader);
            //stop the function
            return;
        }
        else
        {
            //stop the function
            return;
        }
    }

    //replace the old shader with the new shader
    this->ppShaders[stage->index] = shader;
}
void PostProcessingStack::setFunctionOfStage(PostProcessingStage* stage, Shader(*func)(unsigned int), bool force)
{
    //check if the stage used a function
    if (stage->isFunc)
    {
        //replace the old function pointer
        this->ppFunctions[stage->index] = func;
    }

    //check if force is enabled
    if (force)
    {
        //delete the bound function
        this->ppShaders.erase(this->ppShaders.begin() + stage->index);
        //loop over all stages
        for (int i = 0; i < (int)this->stages.size(); i++)
        {
            //if the to manipulate stage is the current stage, skip it
            if (&this->stages[i] == stage) { continue; }
            //check if the stage uses a function that index is greater than the own
            if ((!this->stages[i].isFunc) && (this->stages[i].index > stage->index))
            {
                //decrement the stages index
                this->stages[i].index--;
            }
        }

        //say that the stage now uses a function
        stage->isFunc = true;
        //store the index of the function
        stage->index = (int)this->ppFunctions.size();
        //store the function for the stage
        this->ppFunctions.push_back(func);
        //stop the function
        return;
    }
    else
    {
        //stop the function
        return;
    }
}

PostProcessingStage* PostProcessingStack::operator[](int index)
{
    //check if that index is in range
    if (index < (int)this->stages.size())
    {
        //return a pointer to the stage
        return &this->stages[index];
    }
    //else, return null
    return NULL;
}

int PostProcessingStack::getStageCount()
{
    //return the count
    return (int)this->stages.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RENDER PIPELINE //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


RenderPipeline::RenderPipeline(std::vector<Stage> stages)
{
    //store the inputed stage map
    this->stages = stages;
}

RenderPipeline::~RenderPipeline()
{
    //delete all stages
    this->stages.clear();
    //delete all post processing stacks
    for (std::unordered_map<std::string, PostProcessingStack*>::iterator it = this->ppStacks.begin(); it != this->ppStacks.end(); ++it)
    {
        //delete the post processing stack
        delete it->second;
    }
    //clear the stack of post processing stacks
    this->ppStacks.clear();
}

std::vector<Stage>* RenderPipeline::getStages()
{
    //return a pointer to all stages
    return &this->stages;
}

void RenderPipeline::setStage(std::string name, Stage stage)
{
    //loop over the stages
    for (int i = 0; i < (int)this->stages.size(); i++)
    {
        //check if the stage is the searched one
        if (this->stages[i].name == name)
        {
            //set the stage
            this->stages[i] = stage;
            //stop the function
            return;
        }
    }
    //correct the stages name
    stage.name = name;
    //if it wasn't found, add the stage
    this->stages.push_back(stage);
}

Stage* RenderPipeline::getStage(std::string name)
{
    //loop over the stages
    for (int i = 0; i < (int)this->stages.size(); i++)
    {
        //check if the stage is the searched one
        if (this->stages[i].name == name)
        {
            //return a pointer to the stage
            return &this->stages[i];
        }
    }
    //if it wasn't found, return null
    return NULL;
}

void RenderPipeline::setPPStagePPS(Stage* stage, PostProcessingStack* pps, std::string name)
{
    //check if the post processing stack is null
    if (!pps)
    {
        //throw an error
        GLGE_THROW_ERROR("Can't use NULL as a post processing stack")
        //stop the function
        return;
    }
    //check if the stage is NULL
    if (!stage)
    {
        //just add the post processing stack
        this->ppStacks[name] = pps;
        return;
    }
    //check if the stage is a post processing stage
    if (stage->pass != GLGE_PASS_POST_PROCESSING)
    {
        //throw an error
        GLGE_THROW_ERROR("Tried to bind a post processing stack to a stage that isn't for post processing")
        //stop the function
        return;
    }
    //store the post processing stack
    this->ppStacks[name] = pps;
    //store the name of the post processing stack in the stage
    stage->ppsName = name;
}

PostProcessingStack* RenderPipeline::getPPStagePPS(Stage* stage)
{
    //check if the stage is a post processing stage
    if (stage->pass != GLGE_PASS_POST_PROCESSING)
    {
        //throw an error
        GLGE_THROW_ERROR("Tried to get a post processing stack from a stage that isn't for post processing")
        //stop the function
        return NULL;
    }

    std::unordered_map<std::string, PostProcessingStack*>::iterator it;
    //iterate over the map
    for (it = this->ppStacks.begin(); it != this->ppStacks.end(); ++it)
    {
        //check if the element name is the one searched
        if (it->first == stage->ppsName)
        {
            //return the post processing stack
            return it->second;
        }
    }

    //if it wasn't found, return NULL
    return NULL;
}

void RenderPipeline::execute()
{
    //say that this pipeline is now executing
    this->isCurrent = true;

    //loop over all stages
    for (int i = 0; i < (int)this->stages.size(); i++)
    {
        //extract the current stage
        Stage stage = this->stages[i];

        //first, check if a callback exists
        if (stage.preExecCallback)
        {
            //call the pre-execution function
            (*stage.preExecCallback)();
        }
        //check wich stage to execute
        switch (stage.pass)
        {
        //check if this is a pass with no build-in functionality
        case GLGE_PASS_NONE:
            break;
        
        //check if this is a draw pass for the solid geometry
        case GLGE_PASS_DRAW_SOLID:
            //draw the solid geometry
            glgeWindows[glgeCurrentWindowIndex]->drawSolid();
            break;

        //check if this is a pass to draw the skybock
        case GLGE_PASS_DRAW_SKYBOX:
            //draw the skybox
            glgeWindows[glgeCurrentWindowIndex]->drawSkybox();
            break;

        //check if this is a draw pass for the transparent geometry
        case GLGE_PASS_DRAW_TRANSPARENT:
            //draw the transparent geometry
            glgeWindows[glgeCurrentWindowIndex]->drawTransparent();
            break;

        //check if this is a pass to do the lighting calculation
        case GLGE_PASS_LIGHING:
            //draw the lighting
            glgeWindows[glgeCurrentWindowIndex]->drawLighting();
            break;

        //check if this is a post processing pass
        case GLGE_PASS_POST_PROCESSING:
            //draw a post processing stack
            this->drawPPS(this->ppStacks[stage.ppsName]);
            break;

        //check if this is the shadow pass
        case GLGE_PASS_SHADOWS:
            //perform the shadow mapping
            glgeWindows[glgeCurrentWindowIndex]->shadowPass();
            break;

        //check if this is to clear the g-buffer
        case GLGE_PASS_CLEAR_G_BUFFER:
            //perform the clearing
            glgeWindows[glgeCurrentWindowIndex]->clearGBuff();
            break;
        
        case (GLGE_PASS_COPY_G_TO_PP | GLGE_G_BUFF_COLOR_ATTACHMENT_ALBEDO):
        case (GLGE_PASS_COPY_G_TO_PP | GLGE_G_BUFF_COLOR_ATTACHMENT_NORMAL):
        case (GLGE_PASS_COPY_G_TO_PP | GLGE_G_BUFF_COLOR_ATTACHMENT_POSITION):
        case (GLGE_PASS_COPY_G_TO_PP | GLGE_G_BUFF_COLOR_ATTACHMENT_RML):
        case (GLGE_PASS_COPY_G_TO_PP | GLGE_G_BUFF_COLOR_ATTACHMENT_LIT):
        case (GLGE_PASS_COPY_G_TO_PP | GLGE_G_BUFF_COLOR_ATTACHMENT_EIDA):
        case (GLGE_PASS_COPY_G_TO_PP | GLGE_G_BUFF_COLOR_ATTACHMENT_SOLID):
        case (GLGE_PASS_COPY_G_TO_PP | GLGE_G_BUFF_COLOR_ATTACHMENT_TRANS_ACCUM):
            //copy the buffer, mask out the pass identifier
            glgeWindows[glgeCurrentWindowIndex]->copyGToPPFramebuffer(stage.pass & (~GLGE_PASS_COPY_G_TO_PP));
            break;

        default:
            //throw an error
            GLGE_THROW_ERROR("A stage in the render pipeline has an undefined identifier. Defaulting to stage GLGE_PASS_NONE")
            break;
        }

        //lastly, check if a callback exists
        if (stage.postExecCallback)
        {
            //call the post-execution function
            (*stage.postExecCallback)();
        }
    }

    //say that the pipeline finished
    this->isCurrent = false;
}

bool RenderPipeline::isExecuting()
{
    //return if this is the currently executing pipeline
    return this->isCurrent;
}

Stage* RenderPipeline::operator[](unsigned int index)
{
    //return the stage at that index
    return &this->stages[index];
}

void RenderPipeline::drawPPS(PostProcessingStack* ppsStack)
{
    //iterate over the post processing stacks
    for (int i = 0; i < ppsStack->getStageCount(); i++)
    {
        //get the current post processing stage
        PostProcessingStage* stage = (*ppsStack)[i];
        //check if the stage is a function or shader
        if (stage->isFunc)
        {
            //get the target for the post processing
            unsigned int postProcessingTarget = glgeWindows[glgeCurrentWindowIndex]->getPostProcessingTexture();
            //get the shader from the function
            Shader shader = (*(ppsStack->getFunctionOfStage(stage)))(postProcessingTarget);
            //draw the post processing shader
            glgeWindows[glgeCurrentWindowIndex]->drawPPShader(&shader);
        }
        else
        {
            //get and draw the shader and draw it
            glgeWindows[glgeCurrentWindowIndex]->drawPPShader(ppsStack->getShaderOfStage(stage));
        }
    }
}