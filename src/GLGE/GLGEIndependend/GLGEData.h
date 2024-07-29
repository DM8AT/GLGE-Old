/**
 * @file GLGEData.h
 * @author DM8AT
 * @brief The Data class can store a lot of data in an single array. This is usefull for storing or sending data. 
 * The encoding way is inspired on how minecraft encodes data. 
 * before sending it to a server. 
 * @version 0.1
 * @date 2024-03-26
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 */

#ifndef _GLGE_ENCODE_H_
#define _GLGE_ENCODE_H_

#include <vector>
#include <string>
#include "../CML/CML.h"
#include <cstdint>

/**
 * @brief store a lot of data in an single vector
 * @warning using a read function will delete it in the stored data
 */
class Data
{
private:
    /**
     * @brief store the encoded data in an vector
     */
    std::vector<int8_t> data;
public:
    /**
     * @brief Construct a new class to the data
     */
    Data();
    /**
     * @brief Destroy the class instance
     */
    ~Data();

    /**
     * @brief write a boolean to the data
     * 
     * @param a the boolean to add
     */
    void writeBool(bool a);
    /**
     * @brief write a byte to the data
     * 
     * @param a the byte to add
     */
    void writeByte(int8_t a);
    /**
     * @brief write an unsigned byte to the data
     * 
     * @param a the unsigned byte to add
     */
    void writeUByte(uint8_t a);
    /**
     * @brief write a short integer to the data
     * 
     * @param a the short integer to add
     */
    void writeShort(short a);
    /**
     * @brief write an unsigned short integer to the data
     * 
     * @param a the unsigned short integer to add
     */
    void writeUShort(unsigned short a);
    /**
     * @brief write an integer to the data
     * 
     * @param a the integer to add
     */
    void writeInt(int a);
    /**
     * @brief write an unsigned integer to the data
     * 
     * @param a the unsigned integer to add
     */
    void writeUInt(unsigned int a);
    /**
     * @brief write a long integer to the data
     * 
     * @param a the long integer to add
     */
    void writeLong(long a);
    /**
     * @brief write an unsigned long integer to the data
     * 
     * @param a the unsigned long integer to add
     */
    void writeULong(unsigned long a);
    /**
     * @brief write a float to the data
     * 
     * @param a the float to add
     */
    void writeFloat(float a);
    /**
     * @brief write a double to the data
     * 
     * @param a the double to add
     */
    void writeDouble(double a);
    /**
     * @brief write a string to the data
     * 
     * @param a the string to add
     */
    void writeString(std::string a);
    /**
     * @brief write an integer to the data and try to save some space
     * 
     * @param a the integer to add
     */
    void writeVarInt(int a);
    /**
     * @brief write a long integer to the data and try to save some space
     * 
     * @param a the long integer to add
     */
    void writeVarLong(long a);
    /**
     * @brief write a vector 2 to the data
     * 
     * @param a the vector 2 to add
     */
    void writeVec2(vec2 a);
    /**
     * @brief write a vector 3 to the data
     * 
     * @param a the vector 3 to add
     */
    void writeVec3(vec3 a);
    /**
     * @brief write a vector 4 to the data
     * 
     * @param a the vector 4 to add
     */
    void writeVec4(vec4 a);
    /**
     * @brief write a 2 by 2 matrix to the data
     * 
     * @param a the 2 by 2 matrix to add
     */
    void writeMat2(mat2 a);
    /**
     * @brief write a 3 by 3 matrix to the data
     * 
     * @param a the 3 by 3 matrix to add
     */
    void writeMat3(mat3 a);
    /**
     * @brief write a 4 by 4 matrix to the data
     * 
     * @param a the 4 by 4 matrix to add
     */
    void writeMat4(mat4 a);
    /**
     * @brief write a quaternion to the data
     * 
     * @param a the quaternion to add
     */
    void writeQuaternion(Quaternion a);
    /**
     * @brief write an abituary amount of data
     * 
     * @param data the data to write
     * @param size the size of the data
     */
    void writeBytes(uint8_t* data, size_t size);

    /**
     * @brief read a bool from the stored data
     * 
     * @return bool the read bool
     */
    bool readBool();
    /**
     * @brief read a byte from the stored data
     * 
     * @return int8_t the read byte
     */
    int8_t readByte();
    /**
     * @brief read an unsigned byte from the stored data
     * 
     * @return unsigned int8_t the read unsigned byte
     */
    uint8_t readUByte();
    /**
     * @brief read a short integer from the stored data
     * 
     * @return short the read short integer
     */
    short readShort();
    /**
     * @brief read an unsigned short integer from the stored data
     * 
     * @return unsigned short the read unsigned short integer
     */
    unsigned short readUShort();
    /**
     * @brief read an integer from the stored data
     * 
     * @return int the read integer
     */
    int readInt();
    /**
     * @brief read an unsigned integer from the stored data
     * 
     * @return unsigned int the read unsigned integer
     */
    unsigned int readUInt();
    /**
     * @brief read a long integer from the stored data
     * 
     * @return long the read long integer
     */
    long readLong();
    /**
     * @brief read an unsigned long integer from the stored data
     * 
     * @return unsigned long the read unsigned long integer
     */
    unsigned long readULong();
    /**
     * @brief read a float from the stored data
     * 
     * @return float the read float
     */
    float readFloat();
    /**
     * @brief read a double from the store data
     * 
     * @return double the read double
     */
    double readDouble();
    /**
     * @brief read a string from the stored data
     * 
     * @return std::string the read string
     */
    std::string readString();
    /**
     * @brief read an integer where some storage space was saved from the data
     * 
     * @return int the read integer
     */
    int readVarInt();
    /**
     * @brief read a long integer where some storage space was saved from the data
     * 
     * @return long the read long integer
     */
    long readVarLong();
    /**
     * @brief read a vector 2 from the stored data
     * 
     * @return vec2 the read vector 2
     */
    vec2 readVec2();
    /**
     * @brief read a vector 3 from the stored data
     * 
     * @return vec3 the read vector 3
     */
    vec3 readVec3();
    /**
     * @brief read a vector 4 from the stored data
     * 
     * @return vec4 the read vector 4
     */
    vec4 readVec4();
    /**
     * @brief read a 2 by 2 matrix from the stored data
     * 
     * @return mat2 the read 2 by 2 matrix
     */
    mat2 readMat2();
    /**
     * @brief read a 3 by 3 matrix from the stored data
     * 
     * @return mat3 the read 3 by 3 matrix
     */
    mat3 readMat3();
    /**
     * @brief read a 4 by 4 matrix from the stored data
     * 
     * @return mat4 the read 4 by 4 matrix
     */
    mat4 readMat4();
    /**
     * @brief read a quaternion from the stored data
     * 
     * @return Quaternion the read quaternion
     */
    Quaternion readQuaternion();
    /**
     * @brief read an abituary amount of data
     * 
     * @param size the size of the data to read
     * @return uint8_t* the read data
     */
    uint8_t* readBytes(size_t size);

    /**
     * @brief Get the raw data
     * 
     * @return int8_t* a pointer to the raw data
     */
    int8_t* getData();
    /**
     * @brief Get the amount of stored data in bytes
     * 
     * @return size_t the amount of stored data in bytes
     */
    size_t getLen();
    /**
     * @brief Set the raw data for this object
     * 
     * @param data an std::vector of some data, split up in bytes
     */
    void setData(std::vector<int8_t> data);
    /**
     * @brief Set the raw data for this object
     * 
     * @param data the raw data in an pointer
     * @param len the length of the data in bytes
     */
    void setData(int8_t* data, size_t len);
};


#endif