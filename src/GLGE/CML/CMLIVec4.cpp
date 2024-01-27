#include "CMLIVec4.h"
#include <math.h>

ivec4::ivec4()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

ivec4::ivec4(int xyzw)
{
    this->x = xyzw;
    this->y = xyzw;
    this->z = xyzw;
    this->w = xyzw;
}

ivec4::ivec4(int x, int y, int z, int w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

#ifndef CML_NO_VEC_CASTS

ivec4::ivec4(ivec3 xyz, int w)
{
    this->x = xyz.x;
    this->y = xyz.y;
    this->z = xyz.z;
    this->w = w;
}

ivec4::ivec4(int x, ivec3 yzw)
{
    this->x = x;
    this->y = yzw.x;
    this->z = yzw.y;
    this->w = yzw.z;
}

ivec4::ivec4(ivec2 xy, int z, int w)
{
    this->x = xy.x;
    this->y = xy.y;
    this->z = z;
    this->w = w;
}

ivec4::ivec4(int x, ivec2 yz, int w)
{
    this->x = x;
    this->y = yz.x;
    this->z = yz.y;
    this->w = w;
}

ivec4::ivec4(int x, int y, ivec2 zw)
{
    this->x = x;
    this->y = y;
    this->z = zw.x;
    this->w = zw.y;
}

ivec4::ivec4(ivec2 xy, ivec2 zw)
{
    this->x = xy.x;
    this->y = xy.y;
    this->z = zw.x;
    this->w = zw.y;
}

#endif

ivec4 ivec4::operator+(ivec4 v)
{
    return ivec4(this->x+v.x,this->y+v.y,this->z+v.z,this->w+w);
}

void ivec4::operator+=(ivec4 v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z; 
    this->w += v.w;
}

ivec4 ivec4::operator-(ivec4 v)
{
    return ivec4(this->x-v.x,this->y-v.y,this->z-v.z,this->w-v.w);
}

void ivec4::operator-=(ivec4 v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    this->w -= v.w;
}

int ivec4::operator*(ivec4 v)
{
    return (this->x*v.x) + (this->y*v.y) + (this->z*v.z) + (this->w*v.w);
}

void ivec4::operator*=(ivec4 v)
{
    this->x *= v.x;
    this->y *= v.y;
    this->z *= v.z;
    this->w *= v.w;
}

ivec4 ivec4::operator/(ivec4 v)
{
    return ivec4(this->x/v.x,this->y/v.y,this->z/v.z,this->w/v.w);
}

void ivec4::operator/=(ivec4 v)
{
    this->x /= v.x;
    this->y /= v.y;
    this->z /= v.z;
    this->w /= v.w;
}

bool ivec4::operator==(ivec4 v)
{
    return((this->x==v.x)&&(this->y==v.y)&&(this->z==v.z)&&(this->w==v.w));
}

bool ivec4::operator>=(ivec4 v)
{
    return((this->x>=v.x)&&(this->y>=v.y)&&(this->z>=v.z)&&(this->w>=v.w));
}

bool ivec4::operator<=(ivec4 v)
{
    return((this->x<=v.x)&&(this->y<=v.y)&&(this->z<=v.z)&&(this->w<=v.w));
}

bool ivec4::operator<(ivec4 v)
{
    return((this->x<v.x)&&(this->y<v.y)&&(this->z<v.z)&&(this->w<v.w));
}

bool ivec4::operator>(ivec4 v)
{
    return((this->x>v.x)&&(this->y>v.y)&&(this->z>v.z)&&(this->w>v.w));
}

float ivec4::length()
{
    return std::sqrt((this->x*this->x)+(this->y*this->y)+(this->z*this->z)+(this->w*this->w));
}

void ivec4::normalize()
{
    int len = this->length();
    this->x /= len;
    this->y /= len;
    this->z /= len;
    this->w /= len;
}

ivec4 ivec4::scale(ivec4 v)
{
    return ivec4(this->x*v.x,this->y*v.y,this->z*v.z,this->w*v.w);
}