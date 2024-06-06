#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include "geometry.h"

enum SHAPE_ID{
    NONE,
    BRIQUE,
    CATAPULTE_CHASSIS,
    PLAN,
    STATIC,
    SPHERE,
    CATAPULTE_BRAS
};

class Color
{
public:
    reel r, g, b;
    Color(reel rr = 1.0f, reel gg = 1.0f, reel bb = 1.0f) {r=rr; g=gg; b=bb;}

};


// Constant Colors
const Color RED(1.0f, 0.0f, 0.0f);
const Color BLUE(0.0f, 0.0f, 1.0f);
const Color GREEN(0.0f, 1.0f, 0.0f);
const Color YELLOW(1.0f, 1.0f, 0.0f);
const Color WHITE(1.0f, 1.0f, 1.0f);
const Color ORANGE(1.0f, 0.65f, 0.0f);

class Animation
{
private:
    reel phi, theta; // Azimuthal and polar angles for local coordinate system orientation
    Vector acc, spd, spdRot; //  Instantaneous acceleration and speed
    Point pos; // Instantaneous position of the local coordinate system origin
    Point rotation; // Angles de rotation en degrées sur les axes X, Y, Z

    reel _masse;
    HitZone _sizeObjet;//La place que prend l'objet dans les trois axes

    SHAPE_ID _id;
    Color col;
    //Le coefficient de restitution est défini comme le rapport de la vitesse relative après l'impact à la vitesse relative avant l'impact
    //Il depend du type de materiau de l'objet, il est entre 0 et 1
    // Solide 1  Solide 2	 e
    //     bois	    bois	1/2
    //     liège	liège	5/9
    //     ivoire	ivoire	8/9
    //     verre	verre	15/16
    //     acier	acier	19/20
    //Il est donc propre à chaque objet et doit donc etre saisie à la creation de l'objet, par simplification
    reel coef_restitution;
public:
    Animation(reel ph = 0.0, reel th = 0.0,
              Vector accel = Vector(0.0, 0.0, 0.0),
              Vector speed = Vector(0.0, 0.0, 0.0),
              Point p = Point(0.0, 0.0, 0.0)
              );

    void setTypeForm(SHAPE_ID id) {_id = id;}
    SHAPE_ID getTypeForm() {return _id;}
    void setColor(Color cl) {col = cl;}
    Color& getColor() {return col;}

    reel getPhi() const {return phi;}
    reel getTheta() const {return theta;}
    void setPhi(reel agl) {phi = agl;}
    void setTheta(reel agl) {theta = agl;}

    //En m/s²
    Vector getAccel() const {return acc;}
    //En m/s
    Vector getSpeed() const {return spd;}
    //En m/s²
    void setAccel(Vector vect) {acc = vect;}
    //En m/s
    void setSpeed(Vector vect) {spd = vect;}
    //En métres
    Point getPos() const {return pos;}
    //En métres
    void setPos(Point pt) {pos = pt;}
    //En métres
    void setPosRelative(Point pt) {
        setPos(getPos() + pt);
    }

    //En degrés par secondes
    Vector getSpeedRotation(){
        return spdRot;
    }
    //En degrés par secondes
    void setSpeedRotation(Vector vect){
        spdRot = vect;
    }
    //Rotation en degrés
    Point getRotation() const { return rotation; }
    //Rotation en degrés
    void setRotation(Point rot) { rotation = rot; }
    void setRotRelative(Point rot) { rotation = rotation + rot; }
    void setMasse(reel kg) {_masse = kg;}
    //En kg
    reel getMasse(){return _masse;}
    void setSize(const HitZone size) {_sizeObjet = size;}
    //La place que prend l'objet dans les trois axes en metre
    //Hitzone
    HitZone getSize() const {return _sizeObjet;}
    void setCoefRestitution(reel e){coef_restitution = e;}
    //Le coefficient de restitution est défini comme le rapport de la vitesse relative après l'impact à la vitesse relative avant l'impact
    //Il depend du type de materiau de l'objet, il est entre 0 et 1
    // Solide 1  Solide 2	 e
    //     bois	    bois	1/2
    //     liège	liège	5/9
    //     ivoire	ivoire	8/9
    //     verre	verre	15/16
    //     acier	acier	19/20
    //Il est donc propre à chaque objet et doit donc etre saisie à la creation de l'objet
    reel getCoefRestitution() const {return coef_restitution;}
};

#endif // ANIMATION_H_INCLUDED
