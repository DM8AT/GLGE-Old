#include "CMLDVec4.h"
#include <math.h>

dvec4::dvec4()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

dvec4::dvec4(double x, double y, double z, double w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

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