#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED

#include <iostream>

typedef double reel;

class Coordinates
{
public:
    reel x, y, z;
    Coordinates(reel xx=0, reel yy=0, reel zz=0) {x=xx; y=yy; z=zz;}
};

// Declaration in order to use it within Point methods
class Vector;

class Point : public Coordinates
{
public:
    // Point constructor calls the base class constructor and do nothing more
    Point(reel xx=0, reel yy=0, reel zz=0) : Coordinates(xx, yy, zz) {}
    void translate(const Vector &);

    Point operator+(Point& v) const {
        return Point(x + v.x, y + v.y, z + v.z);
    }

    Point operator-(Point& v) const {
        return Point(x - v.x, y - v.y, z - v.z);
    }
};

typedef struct HitZone
{
    reel rayon;//Rayon de la sphere
}HitZone;

class Vector : public Coordinates
{
public:
    // Instantiates a Vector from its coordinates
    Vector(reel xx=0, reel yy=0, reel zz=0) : Coordinates(xx, yy, zz) {}
    // Or with two points
    Vector(Point, Point);
    // Compute the vector norm
    reel norm();
    Vector integral(reel delta_t);
    // Overloaded standard operators
    void operator+=(const Vector &v);


    reel dot(const Vector& v) const;
    operator Point() const {
        return Point(x, y, z);
    }
};

// Compute the distance between two points
reel distance(Point p1, Point p2);

// Overloaded standard operators
std::ostream& operator<<(std::ostream& os, const Coordinates& coord);
Vector operator+(const Vector &v1, const Vector &v2);
Vector operator-(const Vector &v);
Vector operator-(const Vector &v1, const Vector &v2);
Vector operator*(const reel &k, const Vector &v);
// Scalar product
reel operator*(const Vector &v1, const Vector &v2);
// Vector product
Vector operator^(const Vector &v1, const Vector &v2);

Vector rotateAroundX(const Vector& v, reel theta);
Vector rotateAroundY(const Vector& v, reel theta);
Vector rotateAroundZ(const Vector& v, reel theta);

Point operator+(const Vector &v, Point &p);
Point operator+(Point &p, const Vector &v);
Point operator-(const Vector &v, Point &p);
Point operator-(Point &p, const Vector &v);
// Point operator+(Point &p1, Point &p2);
// Point operator-(Point &p1, Point &p2);

class Plan
{
public:
    Vector v1, v2, normal;
    Plan(Vector v1, Vector v2){
        v1 = v1;
        v2 = v2;
        normal = v1 ^ v2;
    }
    Plan(){
        v1 = 0;
        v2 = 0;
        normal = v1 ^ v2;
    }
};

#endif // GEOMETRY_H_INCLUDED
