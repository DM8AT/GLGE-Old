#include "CMLIVec2.h"
#include <math.h>

ivec2::ivec2()
{
    x = 0;
    y = 0;
}

ivec2::ivec2(int xy)
{
    this->x = xy;
    this->y = xy;
}

ivec2::ivec2(int x, int y)
{
    this->x = x;
    this->y = y;
}

ivec2 ivec2::operator+(ivec2 v)
{
    return ivec2(this->x+v.x,this->y+v.y);
}

void ivec2::operator+=(ivec2 v)
{
    this->x += v.x;
    this->y += v.y;
}

ivec2 ivec2::operator-(ivec2 v)
{
    return ivec2(this->x-v.x,this->y-v.y);
}

void ivec2::operator-=(ivec2 v)
{
    this->x -= v.x;
    this->y -= v.y;
}

int ivec2::operator*(ivec2 v)
{
    return (this->x*v.x) + (this->y*v.y);
}

void ivec2::operator*=(ivec2 v)
{
    this->x *= v.x;
    this->y *= v.y;
}

ivec2 ivec2::operator/(ivec2 v)
{
    return ivec2(this->x/v.x,this->y/v.y);
}

void ivec2::operator/=(ivec2 v)
{
    this->x /= v.x;
    this->y /= v.y;
}

bool ivec2::operator==(ivec2 v)
{
    return((this->x==v.x)&&(this->y==v.y));
}

bool ivec2::operator>=(ivec2 v)
{
    return((this->x>=v.x)&&(this->y>=v.y));
}

bool ivec2::operator<=(ivec2 v)
{
    return((this->x<=v.x)&&(this->y<=v.y));
}

bool ivec2::operator<(ivec2 v)
{
    return((this->x<v.x)&&(this->y<v.y));
}

bool ivec2::operator>(ivec2 v)
{
    return((this->x>v.x)&&(this->y>v.y));
}

float ivec2::length()
{
    return std::sqrt((this->x*this->x)+(this->y*this->y));
}

void ivec2::normalize()
{
    int len = this->length();
    this->x /= len;
    this->y /= len;
}

ivec2 ivec2::scale(ivec2 v)
{
    return ivec2(this->x*v.x,this->y*v.y);
}