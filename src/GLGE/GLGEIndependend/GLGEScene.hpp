/**
 * @file GLGEScene.hpp
 * @author DM8AT
 * @brief a wraper for scenes that can be stored and loaded into files. So the whole scene setup dosn't need to be part of the code
 * @version 0.1
 * @date 2024-03-25
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license.
 */

#ifndef _GLGE_SCENE_H_
#define _GLGE_SCENE_H_

//includes from default library
#include <unordered_map>
#include <vector>
#include <string>
#include <cstring>
#include <typeinfo>

//includes from GLGE
#include "glgePrivDefines.hpp"
#include "GLGEData.h"

///////////
//CLASSES//
///////////

/**
 * @brief store one object an the corresponding name
 */
class NamedObject
{
public:
    /**
     * @brief Default constructor
     */
    NamedObject();

    /**
     * @brief Construct a named object
     * 
     * @param name the name of the object
     */
    NamedObject(std::string name);

    /**
     * @brief Get the object as an object
     * 
     * @tparam T the object to use
     * @return T the interpreted object
     */
    template<typename T> T getObject()
    {
        //create the object to return
        T ret;
        //use the data in the decode function to construct the object
        ret.decode(this->getOutputData(std::string(typeid(T).name())));
        //return the finished object
        return ret;
    }

    /**
     * @brief Set the object to another object
     * 
     * @tparam T the object to use
     * @param obj an instance of the object
     */
    template<typename T> void setObject(T obj)
    {
        //get the encoded obejct data
        Data* data = obj.encode();
        //get the type of the object
        std::string type = std::string(typeid(obj).name());
        //store the data
        this->handleInputData(data, type);
        //delete the data
        delete data;
    }

    /**
     * @brief set the whole object
     * 
     * @tparam T the object to use
     * @param obj an instance of the object
     * @param name the name for the object
     */
    template<typename T> void set(T obj, std::string name)
    {
        //set the name
        this->name = name;
        //set the object
        this->setObject<T>(obj);
    }

    /**
     * @brief the name of the object
     */
    std::string name;

private:
    /**
     * @brief store the data of the object
     */
    void* objData;
    /**
     * @brief store the inputed data from an object encoding
     * 
     * @param data a pointer to the data to store
     * @param type the type name to store
     */
    void handleInputData(Data* data, std::string type);
    /**
     * @brief Get the data used to decode the object
     * 
     * @param type the type name to decode to
     * 
     * @return Data the data for object decoding
     */
    Data getOutputData(std::string type);
};

//default includes

/**
 * @brief a class to store the objects of an scene
 */
class Scene
{
public:
    /**
     * @brief Default constructor
     */
    Scene();

    /**
     * @brief Construct a new scene from existing objects with predefined names
     * 
     * @param sceneName the name of the new scene
     * @param objects a list of all the objects with theyre names
     */
    Scene(const char* sceneName, std::vector<NamedObject> objects);

    /**
     * @brief Load a scene from an file
     * 
     * @param sceneFile the file to load the scene from
     */
    Scene(const char* sceneFile);

private:
    //store all objects
    std::unordered_map<std::string, NamedObject> objs;
};

#endif