#include "CMLDVec2.h"
#include <math.h>

dvec2::dvec2()
{
    x = 0;
    y = 0;
}

dvec2::dvec2(double xy)
{
    this->x = xy;
    this->y = xy;
}

dvec2::dvec2(double x, double y)
{
    this->x = x;
    this->y = y;
}

dvec2 dvec2::operator+(dvec2 v)
{
    return dvec2(this->x+v.x,this->y+v.y);
}

void dvec2::operator+=(dvec2 v)
{
    this->x += v.x;
    this->y += v.y;
}

dvec2 dvec2::operator-(dvec2 v)
{
    return dvec2(this->x-v.x,this->y-v.y);
}

void dvec2::operator-=(dvec2 v)
{
    this->x -= v.x;
    this->y -= v.y;
}

double dvec2::operator*(dvec2 v)
{
    return (this->x*v.x) + (this->y*v.y);
}

void dvec2::operator*=(dvec2 v)
{
    this->x *= v.x;
    this->y *= v.y;
}

dvec2 dvec2::operator/(dvec2 v)
{
    return dvec2(this->x/v.x,this->y/v.y);
}

void dvec2::operator/=(dvec2 v)
{
    this->x /= v.x;
    this->y /= v.y;
}

bool dvec2::operator==(dvec2 v)
{
    return((this->x==v.x)&&(this->y==v.y));
}

bool dvec2::operator>=(dvec2 v)
{
    return((this->x>=v.x)&&(this->y>=v.y));
}

bool dvec2::operator<=(dvec2 v)
{
    return((this->x<=v.x)&&(this->y<=v.y));
}

bool dvec2::operator<(dvec2 v)
{
    return((this->x<v.x)&&(this->y<v.y));
}

bool dvec2::operator>(dvec2 v)
{
    return((this->x>v.x)&&(this->y>v.y));
}

double dvec2::length()
{
    return std::sqrt((this->x*this->x)+(this->y*this->y));
}

void dvec2::normalize()
{
    double len = this->length();
    this->x /= len;
    this->y /= len;
}

dvec2 dvec2::scale(dvec2 v)
{
    return dvec2(this->x*v.x,this->y*v.y);
}