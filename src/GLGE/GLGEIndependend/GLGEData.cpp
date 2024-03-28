/**
 * @file GLGEData.cpp
 * @author DM8AT
 * @brief Implement the Data class for encoding data
 * @version 0.1
 * @date 2024-03-26
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 */

#include "GLGEData.h"

#define SEGMENT_BITS 0x7F
#define CONTINUE_BITS 0x80

Data::Data()
{
    //clear the data
    this->data.clear();
}

Data::~Data()
{
    //free the data vector
    this->data.clear();
}

void Data::writeBool(bool a)
{
    //write a boolean to the message
    this->data.push_back((int8_t)a);
}
void Data::writeByte(int8_t a)
{
    //write a byte to the message
    this->data.push_back(a);
}
void Data::writeUByte(uint8_t a)
{
    //write a unsigned byte (use some pointer magic to handle the sign byte)
    this->data.push_back(*((int8_t*)&a));
}
void Data::writeShort(short a)
{
    //reverse the bit order
    a = (a & 0x00FF) << 8 | (a & 0xFF00) >> 8;
    //create a short to store the number
    short* s = new short(a);
    //write the array to the message
    this->data.insert(this->data.end(), (int8_t*)s, (int8_t*)&s[1]);
}
void Data::writeUShort(unsigned short a)
{
    //reverse the bit order
    a = (a & 0x00FF) << 8 | (a & 0xFF00) >> 8;
    //create a short to store the number
    unsigned short* s = new unsigned short(a);
    //write the array to the message
    this->data.insert(this->data.end(), (int8_t*)s, (int8_t*)&s[1]);
}
void Data::writeInt(int a)
{
    //store the reversed long
    long i = 0;
    //reverse the bit order
    i = (a & 0x000000FF) << (4 - 1)*8 | 
        (a & 0x0000FF00) << (4 - 3)*8 | 
        (a & 0x00FF0000) >> (4 - 3)*8 |
        (a & 0xFF000000) >> (4 - 1)*8;
    //create a short to store the number
    int* p = new int(i);
    //write the array to the message
    this->data.insert(this->data.end(), (int8_t*)p, (int8_t*)(p+1));
}
void Data::writeUInt(unsigned int a)
{
    //store the reversed long
    long i = 0;
    //reverse the bit order
    i = (a & 0x000000FF) << (4 - 1)*8 | 
        (a & 0x0000FF00) << (4 - 3)*8 | 
        (a & 0x00FF0000) >> (4 - 3)*8 |
        (a & 0xFF000000) >> (4 - 1)*8;
    //create a short to store the number
    int* p = new int(i);
    //write the array to the message
    this->data.insert(this->data.end(), (int8_t*)p, (int8_t*)(p+1));
}
void Data::writeLong(long a)
{
    //store the reversed long
    long l = 0;
    //reverse the bit order
    l = (a & 0x00000000000000FF) << (8 - 1)*8 | 
        (a & 0x000000000000FF00) << (8 - 3)*8 | 
        (a & 0x0000000000FF0000) << (8 - 5)*8 | 
        (a & 0x00000000FF000000) << (8 - 7)*8 | 
        (a & 0x000000FF00000000) >> (8 - 7)*8 |
        (a & 0x0000FF0000000000) >> (8 - 5)*8 |
        (a & 0x00FF000000000000) >> (8 - 3)*8 |
        (a & 0xFF00000000000000) >> (8 - 1)*8;
    //create a short to store the number
    long* p = new long(l);
    //write the array to the message
    this->data.insert(this->data.end(), (int8_t*)p, (int8_t*)(p+1));
}
void Data::writeULong(unsigned long a)
{
    //store the reversed long
    long l = 0;
    //reverse the bit order
    l = (a & 0x00000000000000FF) << (8 - 1)*8 | 
        (a & 0x000000000000FF00) << (8 - 3)*8 | 
        (a & 0x0000000000FF0000) << (8 - 5)*8 | 
        (a & 0x00000000FF000000) << (8 - 7)*8 | 
        (a & 0x000000FF00000000) >> (8 - 7)*8 |
        (a & 0x0000FF0000000000) >> (8 - 5)*8 |
        (a & 0x00FF000000000000) >> (8 - 3)*8 |
        (a & 0xFF00000000000000) >> (8 - 1)*8;
    //create a short to store the number
    long* p = new long(l);
    //write the array to the message
    this->data.insert(this->data.end(), (int8_t*)p, (int8_t*)(p+1));
}
void Data::writeFloat(float a)
{
    //pretend the float is an integer
    this->writeInt(*((int*)&a));
}
void Data::writeDouble(double a)
{
    //represent double as long
    this->writeLong(*((long*)&a));
}
void Data::writeString(std::string a)
{
    //write the length as an varint
    this->writeVarInt(a.length());
    //loop over every element in the string
    for (int i = 0; i < (int)a.length(); i++)
    {
        //write the current element to the message
        data.push_back(a[i]);
    }
}
// https://wiki.vg/Data_types#VarInt_and_VarLong
void Data::writeVarInt(int a)
{
    int value = a;
    //make sure to not get stuck in the loop
    for (int i = 0; i < 8; i++)
    {
        if ((value & (~SEGMENT_BITS)) == 0) {
            this->writeByte((int8_t)(value & 0xFF));
            return;
        }
        this->writeByte((value & SEGMENT_BITS) | CONTINUE_BITS);

        value = value >> 7;
        //set the new byte to 0
        ((int8_t*)&value)[3] = 0x00;
    }
    //throw an error : the varint was too long
    std::__throw_runtime_error("VarInt was too long");
    return;
}
void Data::writeVarLong(long a)
{
    long value = a;
    //make sure to not get stuck in the loop
    for (int i = 0; i < 16; i++)
    {
        if ((value & (~((long)SEGMENT_BITS))) == 0) {
            this->writeByte((int8_t)(value & 0xFF));
            return;
        }
        this->writeByte((value & SEGMENT_BITS) | CONTINUE_BITS);

        value = value >> 7;
        //set the new byte to 0
        ((int8_t*)&value)[7] = 0x00;
    }
    //throw an error : the varlong was too long
    std::__throw_runtime_error("VarLong was too long");
    return;
}
void Data::writeVec2(vec2 a)
{
    //write a float for the x component
    this->writeFloat(a.x);
    //write a float for the y-component
    this->writeFloat(a.y);
}
void Data::writeVec3(vec3 a)
{
    //write a float for the x component
    this->writeFloat(a.x);
    //write a float for the y-component
    this->writeFloat(a.y);
    //write a float for the z-component
    this->writeFloat(a.z);
}
void Data::writeVec4(vec4 a)
{
    //write a float for the x component
    this->writeFloat(a.x);
    //write a float for the y-component
    this->writeFloat(a.y);
    //write a float for the z-component
    this->writeFloat(a.z);
    //write a float for the w-component
    this->writeFloat(a.w);
}
void Data::writeMat2(mat2 a)
{
    //loop over the rows
    for (int x = 0; x < 2; x++)
    {
        //loop over every column
        for (int y = 0; y < 2; y++)
        {
            //add a floa for the element
            this->writeFloat(a.m[x][y]);
        }
    }
}
void Data::writeMat3(mat3 a)
{
    //loop over the rows
    for (int x = 0; x < 3; x++)
    {
        //loop over every column
        for (int y = 0; y < 3; y++)
        {
            //add a floa for the element
            this->writeFloat(a.m[x][y]);
        }
    }
}
void Data::writeMat4(mat4 a)
{
    //loop over the rows
    for (int x = 0; x < 4; x++)
    {
        //loop over every column
        for (int y = 0; y < 4; y++)
        {
            //add a floa for the element
            this->writeFloat(a.m[x][y]);
        }
    }
}
void Data::writeQuaternion(Quaternion a)
{
    //pass to an vec4 write
    this->writeVec4(vec4(a.w, a.x,a.y,a.z));
}

int8_t* Data::getData()
{
    return this->data.data();
}
size_t Data::getLen()
{
    return this->data.size();
}

void Data::setData(std::vector<int8_t> data)
{
    this->data.clear();
    this->data = data;
}
void Data::setData(int8_t* data, size_t len)
{
    this->data.clear();
    this->data.insert(this->data.end(), data, data+len);
}

bool Data::readBool()
{
    //get the first byte of the message
    int8_t byte = this->data[0];
    //delete the first byte of the message
    this->data.erase(data.begin(), data.begin()+1);
    //return the byte interpreted as an bool
    return (bool)byte;
}
int8_t Data::readByte()
{
    //get the first byte of the message
    int8_t byte = this->data[0];
    //delete the first byte of the message
    this->data.erase(data.begin(), data.begin()+1);
    //return the byte
    return byte;
}
uint8_t Data::readUByte()
{
    //get the first byte of the message
    int8_t byte = this->data[0];
    //delete the first byte of the message
    this->data.erase(data.begin(), data.begin()+1);
    //return the byte interpreted as an unsigned int8_t
    return (unsigned int8_t)byte;
}
short Data::readShort()
{
    //construct a short from the first two element of the message
    short data = (this->data[0] << 4) | this->data[1];
    //delete the first two elements of the message
    this->data.erase(this->data.begin(), this->data.begin()+2);
    //return the short
    return data;
}
unsigned short Data::readUShort()
{
    //construct an unsigned short from the first two element of the message
    unsigned short data = (this->data[0] << 4) | this->data[1];
    //delete the first two elements of the message
    this->data.erase(this->data.begin(), this->data.begin()+2);
    //return the unsigned short
    return data;
}
int Data::readInt()
{
    //construct an integer from the first four element of the message
    int data = (this->data[0] << 12) | (this->data[1] << 8) | (this->data[2] << 4) | this->data[3];
    //delete the first four elements of the message
    this->data.erase(this->data.begin(), this->data.begin()+4);
    //return the integer
    return data;
}
unsigned int Data::readUInt()
{
    //construct an unsigned integer from the first four element of the message
    unsigned int data = (this->data[0] << 12) | (this->data[1] << 8) | (this->data[2] << 4) | this->data[3];
    //delete the first four elements of the message
    this->data.erase(this->data.begin(), this->data.begin()+4);
    //return the unsigned integer
    return data;
}
long Data::readLong()
{
    //construct a long from the first eigt element of the message
    int data = (this->data[0] << 28) | (this->data[1] << 24) | (this->data[2] << 20) | (this->data[3] << 16) | 
              (this->data[4] << 12) | (this->data[5] << 8)  | (this->data[6] << 4) | this->data[7];
    //delete the first eight elements of the message
    this->data.erase(this->data.begin(), this->data.begin()+8);
    //return the long
    return data;
}
unsigned long Data::readULong()
{
    //construct a long from the first eigt element of the message
    unsigned int data = (this->data[0] << 28) | (this->data[1] << 24) | (this->data[2] << 20) | (this->data[3] << 16) | 
                       (this->data[4] << 12) | (this->data[5] << 8)  | (this->data[6] << 4) | this->data[7];
    //delete the first eight elements of the message
    this->data.erase(this->data.begin(), this->data.begin()+8);
    //return the unsigned long
    return data;
}
float Data::readFloat()
{
    //store an integer
    int a = 0;
    //loop over the vector and read the float
    for (int i = 0; i < 4; i++)
    {
        //store the current element
        ((int8_t*)&a)[(3-i)] = this->data[i];
    }
    //delete the content
    this->data.erase(this->data.begin(), this->data.begin()+4);
    //return a float
    return *((float*)&a);
}
double Data::readDouble()
{
    //store a long
    long a = 0;
    //loop over the vector and read the float
    for (int i = 0; i < 8; i++)
    {
        //store the current element
        ((int8_t*)&a)[(7-i)] = this->data[i];
    }
    //delete the content
    this->data.erase(this->data.begin(), this->data.begin()+8);
    //return a float
    return *((double*)&a);
}

std::string Data::readString()
{
    //store the string to return
    std::string str;
    //get the number of elements in the string
    int len = this->readVarInt();
    //get as many elements as they are in the string
    for (int i = 0; i < len; i++)
    {
        //write the element to tthe string
        str += this->readByte();
    }
    //return the finished string
    return str;
}
// https://wiki.vg/Data_types#VarInt_and_VarLong
int Data::readVarInt()
{
    int value = 0;
    int position = 0;
    int8_t currentByte;

    for (int i = 0; i < 8; i++)
    {
        currentByte = this->readByte();
        value |= (currentByte & SEGMENT_BITS) << position;

        if ((currentByte & CONTINUE_BITS) == 0) return value;

        position += 7;

        if (position >= 32)
        {
            std::__throw_runtime_error("VarInt is too big");
        }
    }

    return value;

}
long Data::readVarLong()
{
    long value = 0;
    int position = 0;
    int8_t currentByte;

    for (int i = 0; i < 16; i++)
    {
        currentByte = this->readByte();
        value |= (long) (currentByte & SEGMENT_BITS) << position;

        if ((currentByte & CONTINUE_BITS) == 0) return value;

        position += 7;

        if (position >= 64)
        {
            std::__throw_runtime_error("VarLong is too big");
        }
    }

    return value;
}
vec2 Data::readVec2()
{
    //read a float for the x value and one for the y value
    return vec2(this->readFloat(), this->readFloat());
}
vec3 Data::readVec3()
{
    //read a float for the x value, one for the y value and one for the z value
    return vec3(this->readFloat(), this->readFloat(), this->readFloat());
}
vec4 Data::readVec4()
{
    //read a float for the x value, one for the y value, one for the z value and one for the w value
    return vec4(this->readFloat(), this->readFloat(), this->readFloat(), this->readFloat());
}
mat2 Data::readMat2()
{
    //create a matrix
    mat2 a;
    //loop over the rows
    for (int x = 0; x < 2; x++)
    {
        //loop over every column
        for (int y = 0; y < 2; y++)
        {
            //add a floa for the element
            a.m[x][y] = this->readFloat();
        }
    }
    //return the matrix
    return a;
}
mat3 Data::readMat3()
{
    //create a matrix
    mat3 a;
    //loop over the rows
    for (int x = 0; x < 3; x++)
    {
        //loop over every column
        for (int y = 0; y < 3; y++)
        {
            //add a floa for the element
            a.m[x][y] = this->readFloat();
        }
    }
    //return the matrix
    return a;
}
mat4 Data::readMat4()
{
    //create a matrix
    mat4 a;
    //loop over the rows
    for (int x = 0; x < 4; x++)
    {
        //loop over every column
        for (int y = 0; y < 4; y++)
        {
            //add a floa for the element
            a.m[x][y] = this->readFloat();
        }
    }
    //return the matrix
    return a;
}
Quaternion Data::readQuaternion()
{
    //read a float for the angle and a vector 3 for the position
    return Quaternion(this->readFloat(), this->readVec3());
}