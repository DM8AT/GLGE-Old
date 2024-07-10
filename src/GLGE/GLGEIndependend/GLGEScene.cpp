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
#include "glgePrivDefines.hpp"
#include <fstream>

NamedObject::NamedObject()
{
    //Default constructor
}

NamedObject::NamedObject(std::string name)
{
    //store the name
    this->name = name;
}

NamedObject::~NamedObject()
{
    //clear the data
    this->objData.clear();
}


std::vector<uint8_t>* NamedObject::getData()
{
    //return the object's data
    return &this->objData;
}

std::string NamedObject::getTypeName()
{
    //read the name of the data type
    std::string t = "";
    //store the current read position, begin after the size
    int i = sizeof(uint64_t)/sizeof(char);
    //store the current char
    char curr = this->objData[i];
    //loop unitll a 0-byte is hit
    while (curr != 0x00)
    {
        //store the current element
        t += curr;
        //increase the read position
        i++;
        //read the new character
        curr = this->objData[i];
    }
    //return the type name
    return t;
}

void NamedObject::handleInputData(Data* data, std::string type)
{
    //get the size of the data to store
    uint64_t size = data->getLen() + type.length() + sizeof(uint64_t);
    //malloc enough size to the own data pointer
    this->objData.resize(size + sizeof(size));

    //fill the data pointer with 0
    bzero(this->objData.data(), size + sizeof(size));
    //first, store the size of the whole pack
    *((uint64_t*)this->objData.data()) = size;
    //copy the typename into the data
    std::memcpy((int8_t*)(((uint64_t*)this->objData.data())+1), type.c_str(), type.length());
    ((int8_t*)this->objData.data())[sizeof(uint64_t)/sizeof(int8_t) + type.length()+1] = 0x00;
    //copy the object data into the data
    std::memcpy(((uint8_t*)(((uint64_t*)this->objData.data())+1)) + type.length() + 1, data->getData(), data->getLen());
}

Data NamedObject::getOutputData(std::string type)
{
    //read the size of the data
    uint64_t size = *((uint64_t*)this->objData.data());
    //read the name of the data type
    std::string t = "";
    //store the current read position, begin after the size
    int i = sizeof(uint64_t)/sizeof(char);
    //store the current char
    char curr = this->objData[i];
    //loop unitll a 0-byte is hit
    while (curr != 0x00)
    {
        //store the current element
        t += curr;
        //increase the read position
        i++;
        //read the new character
        curr = this->objData[i];
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
    std::memcpy(data, (int8_t*)((uint64_t*)this->objData.data() + 1) + type.length() + 1, remain);
    //create a data structure
    Data dat;
    //fill the data
    dat.setData((int8_t*)data, remain);
    //return the data
    return dat;
}

Scene::Scene()
{ /*Default constructor*/ }

Scene::Scene(const char* sceneName, std::vector<NamedObject*> objects)
{
    //store the name
    this->name = std::string(sceneName);
    //store all objects
    this->storeObjects(objects);
}

Scene::Scene(const char* sceneFile)
{
    //store the name
    this->name = sceneFile;
}

Scene::~Scene()
{
    //clear the objects
    this->objs.clear();
}

void Scene::storeObjects(std::vector<NamedObject*> objs)
{
    //loop over all objects
    for (NamedObject* obj : objs)
    {
        //read the object name
        std::string name = obj->name;
        //store the object in a new position in the map
        this->objs[name] = obj;
    }
}

void Scene::safeFile()
{
    //create a new file
    std::fstream file;
    //open a file
    file.open((this->name + ".glges").c_str(), std::fstream::binary | std::fstream::out);
    //check if the file is open
    if (!file.is_open())
    {
        //throw an error
        GLGE_THROW_ERROR("Cound't create scene file")
        //stop the function
        return;
    }
    //clear the file
    file.clear();

    //specify that this is a GLGE file
    file << "GLGE";
    //specify that this is a scene file
    file << "Scene";
    //specify the version id 1
    file << "001";

    //store an iterator for the own object vector
    std::unordered_map<std::string, NamedObject*>::iterator it;
    //loop over all objects
    for (it = this->objs.begin(); it != this->objs.end(); ++it)
    {
        //get the object name
        std::string name = it->first;
        //check if the name is short enough
        if (name.size() > UINT8_MAX)
        {
            //throw an error
            GLGE_THROW_ERROR("The name " + name + " is too long to be stored in an scene file")
        }
        //store the length of the name
        uint8_t nLen = (uint8_t)name.size();
        //get the object data
        std::vector<uint8_t>* data = it->second->getData();
        //get the length of the data
        uint32_t len = (uint32_t)(data->size() + (int)name.size() + sizeof(nLen));
        //store the object size
        file.write((char*)&len, sizeof(len));
        //store the length of the name
        file.write((char*)&nLen, sizeof(nLen));
        //store the name
        file << name;
        //store the object data
        file.write((char*)data->data(), len);
    }

    //close the file
    file.close();
}

void Scene::set(NamedObject* obj)
{
    //access the named object and store it
    this->objs[obj->name] = obj;
}

void Scene::load()
{
    //store the corresponding file
    std::fstream file;
    //open the corresponding scene file
    file.open((this->name + ".glges"));
    //check if the file is open
    if (!file.is_open())
    {
        //throw an error
        GLGE_THROW_ERROR("Cound't open scene file for scene " + this->name + ".glges")
        //stop the function
        return;
    }

    //store the header
    char head[10];
    //get the file header
    file.read(head, sizeof(head)-1);
    //add a 0-Byte to the end
    head[9] = 0x00;
    //check if the file header is correct
    if (std::string(head) != "GLGEScene")
    {
        //throw an error
        GLGE_THROW_ERROR("The scene file for the scene " + this->name + " is not a valid scene file")
        //stop the function
        return;
    }
    
    //store the version number
    char vers[4];
    //get the version from the file
    file.read(vers, sizeof(vers)-1);
    //add a 0-Byte to the end
    vers[3] = 0x00;
    //check the version
    if (atoi(vers) > 1)
    {
        //throw an error
        GLGE_THROW_ERROR("The Scene was created in a newer version of GLGE. Can't load the scene file.")
        //stop the function
        return;
    }

    while (!(file.peek() == EOF))
    {
        //store the amount of data to read
        uint32_t len = 0;
        //get the length of the new data
        file.read((char*)&len, sizeof(len));
        //store the length of the name
        uint8_t nLen = 0;
        //read the length of the name
        file.read((char*)&nLen, sizeof(nLen));
        //create a string to store the name
        std::string name;
        //make enough space for the name
        name.resize(nLen);
        //read the name
        file.read(name.data(), nLen);
        //store the main data
        std::vector<uint8_t> data;
        //resize to enough size for everything
        data.resize(len);
        //get all the data
        file.read((char*)data.data(), len);

        //create a new NamedObject
        NamedObject* obj = new NamedObject(name);
        //make enough size for the data
        obj->getData()->resize(len);
        //set the data for the named object
        memcpy(obj->getData()->data(), data.data(), len);
        //store the named object
        this->objs[name] = obj;
    }

    //close the file
    file.close();
}

NamedObject* Scene::get(std::string name)
{
    //return the named object
    return this->objs[name];
}

std::vector<NamedObject*> Scene::getAllObjects()
{
    //create a new vector
    std::vector<NamedObject*> ret;
    //make enough space in the vector
    ret.resize(this->objs.size());
    //store an iterator for the objects
    std::unordered_map<std::string, NamedObject*>::iterator it;
    //store the current element id
    int i = 0;
    //loop over the map
    for (it = this->objs.begin(); it != this->objs.end(); ++it)
    {
        //store the map element
        ret[i] = it->second;
        //increment i
        i++;
    }

    //return all the elements
    return ret;
}