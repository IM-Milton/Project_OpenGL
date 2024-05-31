#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include "geometry.h"

class Animation
{
private:
    double phi, theta; // Azimuthal and polar angles for local coordinate system orientation
    Vector acc, spd; //  Instantaneous acceleration and speed
    Point pos; // Instantaneous position of the local coordinate system origin
    Point rotation; // Angles de rotation en degrées sur les axes X, Y, Z

    float _masse;
    HitZone _sizeObjet;//La place que prend l'objet dans les trois axes

    //Le coefficient de restitution est défini comme le rapport de la vitesse relative après l'impact à la vitesse relative avant l'impact
    //Il depend du type de materiau de l'objet, il est entre 0 et 1
    // Solide 1  Solide 2	 e
    //     bois	    bois	1/2
    //     liège	liège	5/9
    //     ivoire	ivoire	8/9
    //     verre	verre	15/16
    //     acier	acier	19/20
    //Il est donc propre à chaque objet et doit donc etre saisie à la creation de l'objet, par simplification
    float coef_restitution;

public:
    Animation(double ph = 0.0, double th = 0.0,
              Vector accel = Vector(0.0, 0.0, 0.0),
              Vector speed = Vector(0.0, 0.0, 0.0),
              Point p = Point(0.0, 0.0, 0.0)
              );
    double getPhi() const {return phi;}
    double getTheta() const {return theta;}
    void setPhi(double agl) {phi = agl;}
    void setTheta(double agl) {theta = agl;}
    Vector getAccel() const {return acc;}
    Vector getSpeed() const {return spd;}
    void setAccel(Vector vect) {acc = vect;}
    void setSpeed(Vector vect) {spd = vect;}
    Point getPos() const {return pos;}
    void setPos(Point pt) {pos = pt;}
    void setPosRelative(Point pt) {setPos(getPos() + pt);}
    //Rotation en degrées
    Point getRotation() const { return rotation; }
    //Rotation en degrées
    void setRotation(Point rot) { rotation = rot; }
    void setMasse(float kg) {_masse = kg;}
    //En kg
    float getMasse(){return _masse;}
    void setSize(const HitZone size) {_sizeObjet = size;}
    //La place que prend l'objet dans les trois axes en metre
    //Hitzone
    HitZone getSize() const {return _sizeObjet;}
    void setCoefRestitution(float e){coef_restitution = e;}
    //Le coefficient de restitution est défini comme le rapport de la vitesse relative après l'impact à la vitesse relative avant l'impact
    //Il depend du type de materiau de l'objet, il est entre 0 et 1
    // Solide 1  Solide 2	 e
    //     bois	    bois	1/2
    //     liège	liège	5/9
    //     ivoire	ivoire	8/9
    //     verre	verre	15/16
    //     acier	acier	19/20
    //Il est donc propre à chaque objet et doit donc etre saisie à la creation de l'objet
    float getCoefRestitution() const {return coef_restitution;}
};





#endif // ANIMATION_H_INCLUDED
