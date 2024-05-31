#ifndef FORMS_H_INCLUDED
#define FORMS_H_INCLUDED

#include "geometry.h"
#include "animation.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <GL/gl.h>



//l'echelle est en metre. Soit 1 egale 1 metre


enum SHAPE_ID{
    BRIQUE,
    CATAPULTE_CHASSIS,
    SOL
};

class Color
{
public:
    float r, g, b;
    Color(float rr = 1.0f, float gg = 1.0f, float bb = 1.0f) {r=rr; g=gg; b=bb;}
};

// Constant Colors
const Color RED(1.0f, 0.0f, 0.0f);
const Color BLUE(0.0f, 0.0f, 1.0f);
const Color GREEN(0.0f, 1.0f, 0.0f);
const Color YELLOW(1.0f, 1.0f, 0.0f);
const Color WHITE(1.0f, 1.0f, 1.0f);
const Color ORANGE(1.0f, 0.65f, 0.0f);

struct Vertex {
    float x, y, z;
};

struct Triangle {
    Vertex normal;
    Vertex vertices[3];
};

// Generic class to render and animate an object
class Form
{
protected:
    Color col;
    Animation anim;
    SHAPE_ID _id;

    //Partie Physique : ------------------------------------------------
    Vector _Fn;
    Plan plan;
    //Partie Physique Fin -----------------------------------------------
public:

    Animation& getAnim() {return anim;}
    void setAnim(Animation ani) {anim = ani;}
    void setID(SHAPE_ID id) {_id = id;}
    SHAPE_ID getTypeForm() {return _id;}

    Plan& getPlan() {return plan;}//Utiliser si la form est un plan, comme le sol
    void setPlan(Plan pl){plan = pl;}

    std::vector<Triangle> triangleSTL;//Pour le STL
    bool loadSTL(const std::string& path);
    // This method should update the anim object with the corresponding physical model
    // It has to be done in each inherited class, otherwise all forms will have the same movements !
    // Virtual method for dynamic function call
    // Pure virtual to ensure all objects have their physics implemented
    virtual void update(double delta_t) = 0;
    // Virtual method : Form is a generic type, only setting color and reference position
    virtual void render();

    void setTriangles(const std::vector<Triangle>& tris) {
        triangleSTL = tris;
    }
    void getTriangles(std::vector<Triangle>& tr){
        tr = triangleSTL;
    }
    void setColor(Color cl) {col = cl;}

    //Partie Physique : ------------------------------------------------
    const float g = 9.81; // Accélération gravitationnelle en m/s^2

    Vector getFg(){
        //Doit dependre de la position de l'objet, sa rotation etc
        Vector Fg(0.0, -1*anim.getMasse()*g, 0.0); // Force de gravité dirigée vers le bas
        return Fg;
    }
    //force perpendiculaire à la surface du sol qui empêche l'objet de passer à travers le sol.
    void setFn(const Vector &Fn){
        _Fn = Fn;
    }
    //force perpendiculaire à la surface du sol qui empêche l'objet de passer à travers le sol.
    Vector getFn(){
        return _Fn;
    }
    //Partie Physique Fin -----------------------------------------------
};

// Quel est le poids d'un parpaing de 20 par 20 par 50 ?
// Parpaing creux 20x20x50 NF DB
// longueur:	500 mm
// largeur :	200 mm
// hauteur :	200 mm
// poids   :	18.4 Kilo(s) (merci google)
class Brique : public Form
{
public:
    Brique(Color cl = Color(), float masse = 18.4, HitZone size = {200}){
        setID(BRIQUE);
        col = cl;
        anim.setMasse(masse);//En kg
        anim.setSize(size);
        setFn(Vector(0.0, 0.0, 0.0));
    }

    void render();
    void update(double delta_t);
};

class Sol : public Form
{
public:
    Sol(Color cl = Color()) {
        setID(SOL);
        col = cl;
        anim.setMasse(1);
        plan.v1 = Vector(0.0, 0.0, 0.0);
        plan.v2 = Vector(0.0, 0.0, 0.0);
        plan.normal = plan.v1 ^ plan.v2;
        setPlan(plan);
    }
    void render();
    void update(double delta_t);
};
#endif // FORMS_H_INCLUDED