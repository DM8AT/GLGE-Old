#include "CMLDVec3.h"
#include <math.h>

dvec3::dvec3()
{
    x = 0;
    y = 0;
    z = 0;
}

dvec3::dvec3(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

dvec3 dvec3::operator+(dvec3 v)
{
    return dvec3(this->x+v.x,this->y+v.y,this->z+v.z);
}

void dvec3::operator+=(dvec3 v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z; 
}

dvec3 dvec3::operator-(dvec3 v)
{
    return dvec3(this->x-v.x,this->y-v.y,this->z-v.z);
}

void dvec3::operator-=(dvec3 v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z; 
}

double dvec3::operator*(dvec3 v)
{
    return (this->x*v.x) + (this->y*v.y) + (this->z*v.z);
}

void dvec3::operator*=(dvec3 v)
{
    this->x *= v.x;
    this->y *= v.y;
    this->z *= v.z; 
}

dvec3 dvec3::operator/(dvec3 v)
{
    return dvec3(this->x/v.x,this->y/v.y,this->z/v.z);
}

void dvec3::operator/=(dvec3 v)
{
    this->x /= v.x;
    this->y /= v.y;
    this->z /= v.z; 
}

bool dvec3::operator==(dvec3 v)
{
    return((this->x==v.x)&&(this->y==v.y)&&(this->z==v.z));
}

bool dvec3::operator!=(dvec3 v)
{
    return((this->x!=v.x)||(this->y!=v.y)||(this->z!=v.z));
}

bool dvec3::operator>=(dvec3 v)
{
    return((this->x>=v.x)&&(this->y>=v.y)&&(this->z>=v.z));
}

bool dvec3::operator<=(dvec3 v)
{
    return((this->x<=v.x)&&(this->y<=v.y)&&(this->z<=v.z));
}

bool dvec3::operator<(dvec3 v)
{
    return((this->x<v.x)&&(this->y<v.y)&&(this->z<v.z));
}

bool dvec3::operator>(dvec3 v)
{
    return((this->x>v.x)&&(this->y>v.y)&&(this->z>v.z));
}

double dvec3::length()
{
    return std::sqrt((this->x*this->x)+(this->y*this->y)+(this->z*this->z));
}

void dvec3::normalize()
{
    double len = this->length();
    this->x /= len;
    this->y /= len;
    this->z /= len;
}

dvec3 dvec3::cross(dvec3 v)
{
    return dvec3((this->y*v.z) - (this->z*v.y), (this->z*v.x) - (this->x*v.z), (this->x*v.y) - (this->y*v.x));
}

dvec3 dvec3::scale(dvec3 v)
{
    return dvec3(this->x*v.x,this->y*v.y,this->z*v.z);
}