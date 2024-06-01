#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED

#include <iostream>

class Coordinates
{
public:
    double x, y, z;
    Coordinates(double xx=0, double yy=0, double zz=0) {x=xx; y=yy; z=zz;}
};

// Declaration in order to use it within Point methods
class Vector;

class Point : public Coordinates
{
public:
    // Point constructor calls the base class constructor and do nothing more
    Point(double xx=0, double yy=0, double zz=0) : Coordinates(xx, yy, zz) {}
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
    float rayon;//Rayon de la sphere
}HitZone;

class Vector : public Coordinates
{
public:
    // Instantiates a Vector from its coordinates
    Vector(double xx=0, double yy=0, double zz=0) : Coordinates(xx, yy, zz) {}
    // Or with two points
    Vector(Point, Point);
    // Compute the vector norm
    double norm();
    Vector integral(double delta_t);
    // Overloaded standard operators
    void operator+=(const Vector &v);

    
    double dot(const Vector& v) const;
    operator Point() const {
        return Point(x, y, z);
    }
};

// Compute the distance between two points
double distance(Point p1, Point p2);

// Overloaded standard operators
std::ostream& operator<<(std::ostream& os, const Coordinates& coord);
Vector operator+(const Vector &v1, const Vector &v2);
Vector operator-(const Vector &v);
Vector operator-(const Vector &v1, const Vector &v2);
Vector operator*(const double &k, const Vector &v);
// Scalar product
double operator*(const Vector &v1, const Vector &v2);
// Vector product
Vector operator^(const Vector &v1, const Vector &v2);

Vector rotateAroundX(const Vector& v, double theta);
Vector rotateAroundY(const Vector& v, double theta);
Vector rotateAroundZ(const Vector& v, double theta);

Point operator+(const Vector &v, Point &p);
Point operator+(Point &p, const Vector &v);
Point operator-(const Vector &v, Point &p);
Point operator-(Point &p, const Vector &v);


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
