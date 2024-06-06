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

struct Vertex {
    float x, y, z;
};
struct Triangle {
    Vertex normal;
    Vertex vertices[3];
};

class ModelSTL
{
    protected:
        Animation anim;

        Point globalPosition;
        Vector globalRotation;
    public :

        ModelSTL(){}

        Animation& getAnim() {return anim;}
        void setAnim(Animation ani) {anim = ani;}

        std::vector<Triangle> triangleSTL;//Pour le STL
        bool loadSTL(const std::string& path);

        void render();
        bool isLoaded(){return triangleSTL.size();}
};

// Generic class to render and animate an object
class Form
{
protected:
    Animation anim;

    //Partie Physique : ------------------------------------------------
    bool etatPhysique = true, etatEffetCollision = true;//Activer la physique ou non sur cette objet
    Vector _Fn;
    Plan plan;
    //Partie Physique Fin -----------------------------------------------
public:
    ModelSTL modelSTL;
    Animation& getAnim() {return anim;}
    void setAnim(Animation ani) {anim = ani;}

    void setPhysics(bool activer = true){etatPhysique = activer;}
    bool getPhysics() {return etatPhysique;}

    void setCollisionEffect(bool activer = true){etatEffetCollision = activer;}
    bool getCollisionEffect() {return etatEffetCollision;}

    Plan& getPlan() {return plan;}//Utiliser si la form est un plan, comme le sol
    void setPlan(Plan pl){plan = pl;}

    // This method should update the anim object with the corresponding physical model
    // It has to be done in each inherited class, otherwise all forms will have the same movements !
    // Virtual method for dynamic function call
    // Pure virtual to ensure all objects have their physics implemented
    virtual void update(reel delta_t) = 0;
    // Virtual method : Form is a generic type, only setting color and reference position
    virtual void render();

    void physique(reel delta_t);

    //Partie Physique : ------------------------------------------------
    const reel g = 9.81; // Accélération gravitationnelle en m/s^2

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
// longueur :	500 mm
// largeur :	200 mm
// hauteur :	200 mm
// poids :	18.4 Kilo(s) (merci google)
class Brique : public Form
{
public:
    Brique(Color cl = Color(), reel masse = 18.4, HitZone size = {200}, char* url = NULL) {
        anim.setTypeForm(BRIQUE);
        anim.setColor(cl);
        anim.setSpeedRotation(0);
        anim.setMasse(masse);//En kg
        anim.setSize(size);
        setFn(Vector(0.0, 1*anim.getMasse()*g, 0.0));
        if(url!=NULL)
        {
            modelSTL = ModelSTL();
            modelSTL.loadSTL(url);
            // anim.setPos(anim.getPos());
            // anim.setRotation(anim.getRotation());
        }
    }
    void render();
    void update(reel delta_t);
};


class Sphere : public Form
{
public:
    Sphere(Color cl = Color(), reel masse = 18.4, HitZone size = {200}, char* url = NULL) {
        anim.setTypeForm(SPHERE);
        anim.setColor(cl);
        anim.setSpeedRotation(0);
        anim.setMasse(masse);//En kg
        anim.setSize(size);
        setFn(Vector(0.0, 0.0, 0.0));
        if(url!=NULL)
        {
            modelSTL = ModelSTL();
            modelSTL.loadSTL(url);
            // anim.setPos(anim.getPos());
            // anim.setRotation(anim.getRotation());
        }
    }
    void render();
    void update(reel delta_t);
};

class PlanForm : public Form
{
public:
    PlanForm(Color cl = Color(), char* url = NULL) {
        anim.setTypeForm(PLAN);
        anim.setColor(cl);
        anim.setMasse(1);
        plan.setVector(Vector(0.0, 0.0, 0.0), Vector(0.0, 0.0, 0.0));
        setPlan(plan);
        if(url!=NULL)
        {
            modelSTL = ModelSTL();
            modelSTL.loadSTL(url);
            // anim.setPos(anim.getPos());
            // anim.setRotation(anim.getRotation());
        }
    }
    void render();
    void update(reel delta_t);
};

class staticForm : public Form
{
public:
    staticForm(Color cl = Color(), Point pos = Point(0,0,0), Point rot = Point(0,0,0), char* url = NULL) {
        anim.setTypeForm(STATIC);
        anim.setColor(cl);
        anim.setMasse(1);
        anim.setPos(pos);
        anim.setRotation(rot);
        setPhysics(false);
        if(url!=NULL)
        {
            modelSTL = ModelSTL();
            modelSTL.loadSTL(url);
            // anim.setPos(anim.getPos());
            // anim.setRotation(anim.getRotation());
        }
    }
    void render();
    void update(reel delta_t);
};

class Catapulte : public Form
{
    public:
    Catapulte(Color cl = Color(), Point pos = Point(0,0,0), Point rot = Point(0,0,0), char* url = NULL) {
        anim.setTypeForm(CATAPULTE_BRAS);
        anim.setColor(cl);
        anim.setMasse(1);
        anim.setPos(pos);
        anim.setRotation(rot);
        setPhysics(false);
        if(url!=NULL)
        {
            modelSTL = ModelSTL();
            modelSTL.loadSTL(url);
            // anim.setPos(anim.getPos());
            // anim.setRotation(anim.getRotation());
        }
    }
    void render();
    void update(reel delta_t);
};
#endif // FORMS_H_INCLUDED
