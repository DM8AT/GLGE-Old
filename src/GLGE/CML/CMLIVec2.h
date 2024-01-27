#ifndef _CMLivec2_H_
#define _CMLivec2_H_

/**
 * @brief like vec2 but with integers
 * 
 */
class ivec2
{
public:
    int x,y;

    /**
     * @brief default construcotr
     */
    ivec2();

    /**
     * @brief Construct a new ivec2
     * 
     * @param xy the value for the x AND y component
     */
    ivec2(int xy);

    /**
     * @brief Construct a new ivec2
     * 
     * @param x the value for the x component
     * @param y the value for the y component
     */
    ivec2(int x, int y);

    /**
     * @brief add two ivec2s
     * 
     * @param v the other ivec2 that should be added with this vector
     * @return ivec2 the sum of the vectors
     */
    ivec2 operator+(ivec2 v);

    /**
     * @brief add a ivec2 to this ivec2
     * 
     * @param v the ivec2 that should be added to this ivec2
     */
    void operator+=(ivec2 v);

    /**
     * @brief subtract two ivec2s
     * 
     * @param v the ivec2 to subtract from this ivec2
     * @return ivec2 the difference of this ivec2 and the other devec2
     */
    ivec2 operator-(ivec2 v);

    /**
     * @brief subtract an other ivec2 from this ivec2
     * 
     * @param v the other ivec2
     */
    void operator-=(ivec2 v);

    /**
     * @brief calculate the dot product of two ivec2s
     * 
     * @param v the other ivec2
     * @return int the dot product of this ivec2 and the other ivec2
     */
    int operator*(ivec2 v);

    /**
     * @brief scale this ivec2 with another ivec2
     * 
     * @param v the other vector to scale this vector with
     */
    void operator*=(ivec2 v);

    /**
     * @brief scale the vector with another ivec2
     * 
     * @param v the INVERSE of the other vector to scale with
     * @return ivec2 the scale of this ivec2 and the inverse of the other ivec2
     */
    ivec2 operator/(ivec2 v);

    /**
     * @brief scale this ivec2 with another ivec2
     * 
     * @param v the INVERSE of the other ivec2 to scale with
     */
    void operator/=(ivec2 v);

    /**
     * @brief check if two ivec2s are equal
     * 
     * @param v the ivec2 to compare with
     * @return true : both ivec2s are equal | 
     * @return false : the ivec2s are not equal
     */
    bool operator==(ivec2 v);

    /**
     * @brief check if an ivec2 is smaller or equal than another ivec2
     * 
     * @param v the ivec2 that should be equal or less than this ivec2
     * @return true : the ivec2 is less or equal than this ivec2 | 
     * @return false : the other ivec2 is larger than this ivec2
     */
    bool operator>=(ivec2 v);

    /**
     * @brief check if an ivec2 is larger or equal than another ivec2
     * 
     * @param v the ivec2 that should be equal or larger than this ivec2
     * @return true : the ivec2 is larger or equal than this ivec2 | 
     * @return false : the other ivec2 is smaler than this ivec2
     */
    bool operator<=(ivec2 v);

    /**dvec4
     * @brief check if an ivec2 is larger than another ivec2
     * 
     * @param v the ivec2 that should be larger than this ivec2
     * @return true : the ivec2 is larger than this ivec2 | 
     * @return false : the other ivec2 is equal or smaler than this ivec2
     */
    bool operator<(ivec2 v);

    /**
     * @brief check if an ivec2 is larger than another ivec2
     * 
     * @param v the ivec2 that should be equal than this ivec2
     * @return true : the ivec2 is larger than this ivec2 | 
     * @return false : the other ivec2 is equal or smaler than this ivec2
     */
    bool operator>(ivec2 v);

    /**
     * @brief calculate the length of this ivec2
     * 
     * @return float the length of the vector
     */
    float length();

    /**
     * @brief normalize this ivec2
     */
    void normalize();

    /**
     * @brief scale this ivec2 with another one
     * 
     * @param v the other ivec2
     * @return ivec2 this vector scaled with the other one
     */
    ivec2 scale(ivec2 v);
};

#endif