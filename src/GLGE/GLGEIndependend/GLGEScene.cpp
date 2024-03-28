/**
 * @file GLGEScene.cpp
 * @author DM8AT
 * @brief define the stuff for the scene
 * @version 0.1
 * @date 2024-03-25
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license.
 * 
 */

#include "GLGEScene.hpp"

NamedObject::NamedObject()
{
    //Default constructor
}

NamedObject::NamedObject(std::string name)
{
    //store the name
    this->name = name;
}

void NamedObject::handleInputData(Data* data, std::string type)
{
    //get the size of the data to store
    size_t size = data->getLen() + type.length() + sizeof(size_t);
    //malloc enough size to the own data pointer
    this->objData = malloc(size + sizeof(size));

    //fill the data pointer with 0
    bzero(this->objData, size + sizeof(size));
    //first, store the size of the whole pack
    *((size_t*)this->objData) = size;
    //copy the typename into the data
    std::memcpy((int8_t*)(((size_t*)this->objData)+1), type.c_str(), type.length());
    ((int8_t*)this->objData)[sizeof(size_t)/sizeof(int8_t) + type.length()+1] = 0x00;
    //copy the object data into the data
    std::memcpy(((uint8_t*)(((size_t*)this->objData)+1)) + type.length() + 1, data->getData(), data->getLen());
}

Data NamedObject::getOutputData(std::string type)
{
    //read the size of the data
    size_t size = *((size_t*)this->objData);
    //read the name of the data type
    std::string t = "";
    //store the current read position, begin after the size
    int i = sizeof(size_t)/sizeof(char);
    //store the current char
    char curr = ((char*)this->objData)[i];
    //loop unitll a 0-byte is hit
    while (curr != 0x00)
    {
        //store the current element
        t += curr;
        //increase the read position
        i++;
        //read the new character
        curr = ((char*)this->objData)[i];
    }
    //check if the inputed type is not the stored type
    if (type != t)
    {
        //throw an error
        GLGE_THROW_ERROR("Trying to decode object of type \'" + t + "\' as type \'" + type + "\'");
        return Data();
    }
    //calculate the size of the remaining data
    long remain = size - i;
    //check if data remains
    if (remain < 1)
    {
        //if not, return a new object instance
        return Data();
    }
    //create a void* to store the data
    void* data = malloc(remain);
    //test if malloc was successfull
    if (!data)
    {
        //throw an error
        GLGE_THROW_ERROR("malloc failed, is enough ram free?")
    }
    //copy the data into the new pointer
    std::memcpy(data, (int8_t*)((size_t*)this->objData + 1) + type.length() + 1, remain);
    //create a data structure
    Data dat;
    //fill the data
    dat.setData((int8_t*)data, remain);
    //return the data
    return dat;
}

Scene::Scene()
{

}

Scene::Scene(const char* sceneName, std::vector<NamedObject> objects)
{

}

Scene::Scene(const char* sceneFile)
{

}