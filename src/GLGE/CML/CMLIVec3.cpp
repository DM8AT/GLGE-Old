#include "CMLIVec3.h"
#include <math.h>

ivec3::ivec3()
{
    x = 0;
    y = 0;
    z = 0;
}

ivec3::ivec3(int xyz)
{
    this->x = xyz;
    this->y = xyz;
    this->z = xyz;
}

ivec3::ivec3(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

#ifndef CML_NO_VEC_CASTS
ivec3::ivec3(ivec2 xy, int z)
{
    this->x = xy.x;
    this->y = xy.y;
    this->z = z;
}

ivec3::ivec3(int x, ivec2 yz)
{
    this->x = x;
    this->y = yz.x;
    this->z = yz.y;
}
#endif

ivec3 ivec3::operator+(ivec3 v)
{
    return ivec3(this->x+v.x,this->y+v.y,this->z+v.z);
}

void ivec3::operator+=(ivec3 v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z; 
}

ivec3 ivec3::operator-(ivec3 v)
{
    return ivec3(this->x-v.x,this->y-v.y,this->z-v.z);
}

void ivec3::operator-=(ivec3 v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z; 
}

int ivec3::operator*(ivec3 v)
{
    return (this->x*v.x) + (this->y*v.y) + (this->z*v.z);
}

void ivec3::operator*=(ivec3 v)
{
    this->x *= v.x;
    this->y *= v.y;
    this->z *= v.z; 
}

ivec3 ivec3::operator/(ivec3 v)
{
    return ivec3(this->x/v.x,this->y/v.y,this->z/v.z);
}

void ivec3::operator/=(ivec3 v)
{
    this->x /= v.x;
    this->y /= v.y;
    this->z /= v.z; 
}

bool ivec3::operator==(ivec3 v)
{
    return((this->x==v.x)&&(this->y==v.y)&&(this->z==v.z));
}

bool ivec3::operator!=(ivec3 v)
{
    return((this->x!=v.x)||(this->y!=v.y)||(this->z!=v.z));
}

bool ivec3::operator>=(ivec3 v)
{
    return((this->x>=v.x)&&(this->y>=v.y)&&(this->z>=v.z));
}

bool ivec3::operator<=(ivec3 v)
{
    return((this->x<=v.x)&&(this->y<=v.y)&&(this->z<=v.z));
}

bool ivec3::operator<(ivec3 v)
{
    return((this->x<v.x)&&(this->y<v.y)&&(this->z<v.z));
}

bool ivec3::operator>(ivec3 v)
{
    return((this->x>v.x)&&(this->y>v.y)&&(this->z>v.z));
}

float ivec3::length()
{
    return std::sqrt((this->x*this->x)+(this->y*this->y)+(this->z*this->z));
}

void ivec3::normalize()
{
    int len = this->length();
    this->x /= len;
    this->y /= len;
    this->z /= len;
}

ivec3 ivec3::cross(ivec3 v)
{
    return ivec3((this->y*v.z) - (this->z*v.y), (this->z*v.x) - (this->x*v.z), (this->x*v.y) - (this->y*v.x));
}

ivec3 ivec3::scale(ivec3 v)
{
    return ivec3(this->x*v.x,this->y*v.y,this->z*v.z);
}