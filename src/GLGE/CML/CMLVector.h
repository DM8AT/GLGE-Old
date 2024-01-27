#ifndef _CMLVECTOR_H_
#define _CMLVECTOR_H_
#pragma once
#include <string>
#include <vector>

/**
 * @brief a float vector of varrying length
 * 
 */
class vec
{
public:
    /**
     * @brief default constructor
     */
    vec();

    /**
     * @brief Construct a new vector
     * 
     * @param vals a vector of floats to construct this vector from
     */
    vec(std::vector<float> vals);

    /**
     * @brief get the size of this vector (the dimension of the vector)
     * 
     * @return int the amount of floats in the vector
     */
    int size();

    /**
     * @brief get a singe float from the vector
     * 
     * @param id the dimension for the float
     * @return float the requested float
     */
    float getComponent(int id);

    /**
     * @brief get all the floats stored in the vector as an std::vector<float>
     * 
     * @return std::vector<float> a vector containing all the floats
     */
    std::vector<float> getContent();

    /**
     * @brief Set the value at an specific id
     * 
     * @param i the id to set the value at
     * @param v the value at that id
     */
    void setId(int i, float v);

    /**
     * @brief replace all values behind a starting point
     * 
     * @param vals the values to replace with
     * @param start the starting index
     */
    void set(std::vector<float> vals, int start);

    /**
     * @brief convert the vector to an string
     * 
     * @return std::string the stirng, ready to be printed
     */
    std::string print();

    /**
     * @brief sum two vectors
     * 
     * @param v the other vector
     * @return vec the sum of this and the other vector
     */
    vec operator+(vec v);
    /**
     * @brief sum this and another vector
     * 
     * @param v the other vector
     */
    void operator+=(vec v);

    /**
     * @brief subtract an vector from another vector
     * 
     * @param v the other vector
     * @return vec the difference of the two vectors
     */
    vec operator-(vec v);
    /**
     * @brief subtract an other vector from this vector
     * 
     * @param v the other vector
     */
    void operator-=(vec v);

    /**
     * @brief calculate the dot product of this and another vector
     * 
     * @param v the other vector
     * @return float the dot product of the two vectors
     */
    float operator*(vec v);

    /**
     * @brief divide this vector with another vector
     * 
     * @param v the other vector
     * @return vec the resault of the division
     */
    vec operator/(vec v);
    /**
     * @brief divide this vector with an scalar
     * 
     * @param v the scalar to divide with
     * @return vec the resault of the division
     */
    vec operator/(float v);

    /**
     * @brief divide this vector with another vector
     * 
     * @param v the other vector
     */
    void operator/=(vec v);
    /**
     * @brief divide this vector with an scalar
     * 
     * @param v the scalar to divide with
     */
    void operator/=(float v);

    /**
     * @brief calculate the length of the vector
     * 
     * @return float the length of the vector
     */
    float length();

private:
    //store the content of the vector
    std::vector<float> val;
};

#endif