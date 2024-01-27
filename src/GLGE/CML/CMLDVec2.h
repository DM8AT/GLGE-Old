#ifndef _CMLDVEC2_H_
#define _CMLDVEC2_H_

/**
 * @brief like vec2 but with larger percission
 * 
 */
class dvec2
{
public:
    double x,y;

    /**
     * @brief default construcotr
     */
    dvec2();

    /**
     * @brief Construct a new dvec2
     * 
     * @param xy the value for the x AND y component
     */
    dvec2(double xy);

    /**
     * @brief Construct a new dvec2
     * 
     * @param x the value for the x component
     * @param y the value for the y component
     */
    dvec2(double x, double y);

    /**
     * @brief add two dvec2s
     * 
     * @param v the other dvec2 that should be added with this vector
     * @return dvec2 the sum of the vectors
     */
    dvec2 operator+(dvec2 v);

    /**
     * @brief add a dvec2 to this dvec2
     * 
     * @param v the dvec2 that should be added to this dvec2
     */
    void operator+=(dvec2 v);

    /**
     * @brief subtract two dvec2s
     * 
     * @param v the dvec2 to subtract from this dvec2
     * @return dvec2 the difference of this dvec2 and the other devec2
     */
    dvec2 operator-(dvec2 v);

    /**
     * @brief subtract an other dvec2 from this dvec2
     * 
     * @param v the other dvec2
     */
    void operator-=(dvec2 v);

    /**
     * @brief calculate the dot product of two dvec2s
     * 
     * @param v the other dvec2
     * @return double the dot product of this dvec2 and the other dvec2
     */
    double operator*(dvec2 v);

    /**
     * @brief scale this dvec2 with another dvec2
     * 
     * @param v the other vector to scale this vector with
     */
    void operator*=(dvec2 v);

    /**
     * @brief scale the vector with another dvec2
     * 
     * @param v the INVERSE of the other vector to scale with
     * @return dvec2 the scale of this dvec2 and the inverse of the other dvec2
     */
    dvec2 operator/(dvec2 v);

    /**
     * @brief scale this dvec2 with another dvec2
     * 
     * @param v the INVERSE of the other dvec2 to scale with
     */
    void operator/=(dvec2 v);

    /**
     * @brief check if two dvec2s are equal
     * 
     * @param v the dvec2 to compare with
     * @return true : both dvec2s are equal | 
     * @return false : the dvec2s are not equal
     */
    bool operator==(dvec2 v);

    /**
     * @brief check if an dvec2 is smaller or equal than another dvec2
     * 
     * @param v the dvec2 that should be equal or less than this dvec2
     * @return true : the dvec2 is less or equal than this dvec2 | 
     * @return false : the other dvec2 is larger than this dvec2
     */
    bool operator>=(dvec2 v);

    /**
     * @brief check if an dvec2 is larger or equal than another dvec2
     * 
     * @param v the dvec2 that should be equal or larger than this dvec2
     * @return true : the dvec2 is larger or equal than this dvec2 | 
     * @return false : the other dvec2 is smaler than this dvec2
     */
    bool operator<=(dvec2 v);

    /**dvec4
     * @brief check if an dvec2 is larger than another dvec2
     * 
     * @param v the dvec2 that should be larger than this dvec2
     * @return true : the dvec2 is larger than this dvec2 | 
     * @return false : the other dvec2 is equal or smaler than this dvec2
     */
    bool operator<(dvec2 v);

    /**
     * @brief check if an dvec2 is larger than another dvec2
     * 
     * @param v the dvec2 that should be equal than this dvec2
     * @return true : the dvec2 is larger than this dvec2 | 
     * @return false : the other dvec2 is equal or smaler than this dvec2
     */
    bool operator>(dvec2 v);

    /**
     * @brief calculate the length of this dvec2
     * 
     * @return double the length of the vector
     */
    double length();

    /**
     * @brief normalize this dvec2
     */
    void normalize();

    /**
     * @brief scale this dvec2 with another one
     * 
     * @param v the other dvec2
     * @return dvec2 this vector scaled with the other one
     */
    dvec2 scale(dvec2 v);
};

#endif