#include <cmath>
#include "geometry.h"

void Point::translate(const Vector &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

Vector::Vector(Point p1, Point p2)
{
    x = p2.x - p1.x;
    y = p2.y - p1.y;
    z = p2.z - p1.z;
}

reel Vector::norm()
{
    reel norm;

    norm = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

    return norm;
}



/**
 * @brief Computes the dot product of the current vector with another vector.
 * 
 * @param v The other vector to compute the dot product with.
 * @return The dot product of the current vector and the other vector.
 * 
 * The dot product is a scalar value that represents the cosine of the angle between two vectors.
 * It is calculated as the sum of the product of corresponding components of the two vectors.
 * 
 * The formula for the dot product is: dot(v1, v2) = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z
 * 
 * This method is useful for various applications such as calculating the angle between vectors,
 * checking for orthogonality, and performing vector projections.
 */
reel Vector::dot(const Vector& v) const {
    return x * v.x + y * v.y + z * v.z;
}

Vector Vector::integral(reel delta_t)
{
    Vector res;

    res.x = delta_t * x;
    res.y = delta_t * y;
    res.z = delta_t * z;

    return res;
}

void Vector::operator+=(const Vector &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}



reel distance(Point p1, Point p2)
{
    Vector vect(p1, p2);

    return vect.norm();
}


// Overloaded standard operators
std::ostream& operator<<(std::ostream& os, const Coordinates& coord)
{
    os << '(' << coord.x << ", " << coord.y << ", " << coord.z << ')';
    return os;
}

Vector operator+(const Vector &v1, const Vector &v2)
{
    Vector res = v1;

    res.x += v2.x;
    res.y += v2.y;
    res.z += v2.z;

    return res;
}

Vector operator-(const Vector &v)
{
    Vector res;

    res.x = -v.x;
    res.y = -v.y;
    res.z = -v.z;

    return res;
}

Vector operator-(const Vector &v1, const Vector &v2)
{
    Vector res = -v2;

    res = res + v1;

    return res;
}

Vector operator*(const reel &k, const Vector &v)
{
    Vector res = v;

    res.x *= k;
    res.y *= k;
    res.z *= k;

    return res;
}

// Scalar product
reel operator*(const Vector &v1, const Vector &v2)
{
    reel res;

    res = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

    return res;
}

// Vector product
Vector operator^(const Vector &v1, const Vector &v2)
{
    Vector res;

    res.x = v1.y * v2.z - v1.z * v2.y;
    res.y = v1.z * v2.x - v1.x * v2.z;
    res.z = v1.x * v2.y - v1.y * v2.x;

    return res;
}

// Fonctions de rotation
Vector rotateAroundX(const Vector& v, reel theta) {
    reel cosTheta = cos(theta);
    reel sinTheta = sin(theta);
    return Vector(
        v.x,
        v.y * cosTheta - v.z * sinTheta,
        v.y * sinTheta + v.z * cosTheta
    );
}

Vector rotateAroundY(const Vector& v, reel theta) {
    reel cosTheta = cos(theta);
    reel sinTheta = sin(theta);
    return Vector(
        v.x * cosTheta + v.z * sinTheta,
        v.y,
        -v.x * sinTheta + v.z * cosTheta
    );
}

Vector rotateAroundZ(const Vector& v, reel theta) {
    reel cosTheta = cos(theta);
    reel sinTheta = sin(theta);
    return Vector(
        v.x * cosTheta - v.y * sinTheta,
        v.x * sinTheta + v.y * cosTheta,
        v.z
    );
}


Point operator+(const Vector &v, Point &p){
    Point res = v;
    res = res + p;
    return res;
}

Point operator+(Point &p, const Vector &v){
    Point res = v;
    res = res + p;
    return res;
}

Point operator-(const Vector &v, Point &p){
    Point res = v;
    res = res - p;
    return res;
}

Point operator-(Point &p, const Vector &v){
    Point res = v;
    res = res - p;
    return res;
}

// Point operator+(Point &p1, Point &p2) {
//     return Point(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
// }
    
// Point operator-(Point &p1, Point &p2) {
//     return Point(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
// }