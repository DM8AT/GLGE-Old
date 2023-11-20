#include "CMLDVec4.h"
#include <math.h>

dvec4::dvec4()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

dvec4::dvec4(double xyzw)
{
    this->x = xyzw;
    this->y = xyzw;
    this->z = xyzw;
    this->w = xyzw;
}

dvec4::dvec4(double x, double y, double z, double w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

#ifndef CML_NO_VEC_CASTS

dvec4::dvec4(dvec3 xyz, double w)
{
    this->x = xyz.x;
    this->y = xyz.y;
    this->z = xyz.z;
    this->w = w;
}

dvec4::dvec4(double x, dvec3 yzw)
{
    this->x = x;
    this->y = yzw.x;
    this->z = yzw.y;
    this->w = yzw.z;
}

dvec4::dvec4(dvec2 xy, double z, double w)
{
    this->x = xy.x;
    this->y = xy.y;
    this->z = z;
    this->w = w;
}

dvec4::dvec4(double x, dvec2 yz, double w)
{
    this->x = x;
    this->y = yz.x;
    this->z = yz.y;
    this->w = w;
}

dvec4::dvec4(double x, double y, dvec2 zw)
{
    this->x = x;
    this->y = y;
    this->z = zw.x;
    this->w = zw.y;
}

dvec4::dvec4(dvec2 xy, dvec2 zw)
{
    this->x = xy.x;
    this->y = xy.y;
    this->z = zw.x;
    this->w = zw.y;
}

#endif

dvec4 dvec4::operator+(dvec4 v)
{
    return dvec4(this->x+v.x,this->y+v.y,this->z+v.z,this->w+w);
}

void dvec4::operator+=(dvec4 v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z; 
    this->w += v.w;
}

dvec4 dvec4::operator-(dvec4 v)
{
    return dvec4(this->x-v.x,this->y-v.y,this->z-v.z,this->w-v.w);
}

void dvec4::operator-=(dvec4 v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    this->w -= v.w;
}

double dvec4::operator*(dvec4 v)
{
    return (this->x*v.x) + (this->y*v.y) + (this->z*v.z) + (this->w*v.w);
}

void dvec4::operator*=(dvec4 v)
{
    this->x *= v.x;
    this->y *= v.y;
    this->z *= v.z;
    this->w *= v.w;
}

dvec4 dvec4::operator/(dvec4 v)
{
    return dvec4(this->x/v.x,this->y/v.y,this->z/v.z,this->w/v.w);
}

void dvec4::operator/=(dvec4 v)
{
    this->x /= v.x;
    this->y /= v.y;
    this->z /= v.z;
    this->w /= v.w;
}

bool dvec4::operator==(dvec4 v)
{
    return((this->x==v.x)&&(this->y==v.y)&&(this->z==v.z)&&(this->w==v.w));
}

bool dvec4::operator>=(dvec4 v)
{
    return((this->x>=v.x)&&(this->y>=v.y)&&(this->z>=v.z)&&(this->w>=v.w));
}

bool dvec4::operator<=(dvec4 v)
{
    return((this->x<=v.x)&&(this->y<=v.y)&&(this->z<=v.z)&&(this->w<=v.w));
}

bool dvec4::operator<(dvec4 v)
{
    return((this->x<v.x)&&(this->y<v.y)&&(this->z<v.z)&&(this->w<v.w));
}

bool dvec4::operator>(dvec4 v)
{
    return((this->x>v.x)&&(this->y>v.y)&&(this->z>v.z)&&(this->w>v.w));
}

double dvec4::length()
{
    return std::sqrt((this->x*this->x)+(this->y*this->y)+(this->z*this->z)+(this->w*this->w));
}

void dvec4::normalize()
{
    double len = this->length();
    this->x /= len;
    this->y /= len;
    this->z /= len;
    this->w /= len;
}

dvec4 dvec4::scale(dvec4 v)
{
    return dvec4(this->x*v.x,this->y*v.y,this->z*v.z,this->w*v.w);
}