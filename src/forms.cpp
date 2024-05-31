#include <cmath>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include "forms.h"

void Form::update(double delta_t)
{
    // Nothing to do here, animation update is done in child class method
}

void Form::render()
{
    // Point of view for rendering
    // Common for all Forms
    Point org = anim.getPos();
    Point rot = anim.getRotation();

    glTranslated(org.x, org.y, org.z);

    glRotated(rot.x, 1.0, 0.0, 0.0); // Rotation autour de l'axe X
    glRotated(rot.y, 0.0, 1.0, 0.0); // Rotation autour de l'axe Y
    glRotated(rot.z, 0.0, 0.0, 1.0); // Rotation autour de l'axe Z
    
    glColor3f(col.r, col.g, col.b);
    
    // Render the form
    // Add your rendering code here
}

bool Form::loadSTL(const std::string& path) {
    // printf("Loading STL file %s\n", path );
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return false;
    }

    // Read the header
    char header[80] = "";
    file.read(header, 80);

    // Read number of triangles
    uint32_t numTriangles;
    file.read(reinterpret_cast<char*>(&numTriangles), sizeof(uint32_t));

    // Read triangles
    triangleSTL.resize(numTriangles);
    uint32_t i = 0;
    printf("Starting triangles read from file\n");
    for (i = 0; i < numTriangles; ++i) {
        file.read(reinterpret_cast<char*>(&triangleSTL[i]), sizeof(Triangle));
        // Skip attribute byte count
        file.ignore(2);
    }
    printf("numTriangles : %d\n", i);

    return true;
}

void Brique::render() {
    // Render the STL model
    Form::render();
    
    if(!triangleSTL.size()){//Le stl n'a pas été chargée donc on doit dessiner à la main la brique
        //Alors on affiche une brique normal de base
        //Enzo doit faire une brique de 500/1000 de longeur, 200/1000 de largeur et 200/1000 de profondeur
        // printf("Triangle vide !! Doit donc dessiner la brique à la main %d\n");
            Form::render();

            glBegin(GL_QUADS);
            {
                glVertex3d(0, 1, 0);
                glVertex3d(1, 1, 0);
                glVertex3d(1, 1, 1);
                glVertex3d(0, 1, 1);
            }
            glEnd();
    }else{
        glBegin(GL_TRIANGLES);
        int nbTriangles = 0;
        for (const auto& tri : triangleSTL) {
            glNormal3f(tri.normal.x, tri.normal.y, tri.normal.z);
            for (const auto& vert : tri.vertices) {
                glVertex3f(vert.x, vert.y, vert.z);
            }
            nbTriangles++;
        }
        glEnd();
    }
}

void Brique::update(double delta_t) {
    // Calcul du PFD 
    
    Vector sumForce = getFg() + getFn();
    //Determination de l'acceleration à partir du PFD
    //Somme des force = masse * acc
    //acc = Somme des force / masse
    float masse = anim.getMasse();
    Vector acc(sumForce.x/masse, sumForce.y/masse, sumForce.z/masse);
    anim.setAccel(acc);
    //Intergrer pour avoir la vitesse
    Vector speed = anim.getSpeed() + anim.getAccel().integral(delta_t);//+v0;
    anim.setSpeed(speed);
    
    // Mettez à jour la position en fonction de la vitesse et du temps
    // On intergre la vitesse pour obtenir le delta position qu'on vient rajouter à notre position actuelle
    // anim.setPosRelative(anim.getSpeed().integral(delta_t));
    Point position = speed.integral(delta_t);
    anim.setPosRelative(position);

    printf("accel : x:%3.2f y:%3.2f z:%3.2f  ;;  speed : x:%3.2f y:%3.2f z:%3.2f ;; position : x:%3.2f y:%3.2f z:%3.2f \n",
                                        anim.getAccel().x, anim.getAccel().y, anim.getAccel().z, 
                                        anim.getSpeed().x, anim.getSpeed().y, anim.getSpeed().z,
                                        anim.getPos().x, anim.getPos().y, anim.getPos().z
                                        );
}

void Sol::render() {
    // Render the STL model
    Form::render();
    
    if(!triangleSTL.size()){//Le stl n'a pas été chargée donc on doit dessiner à la main la brique
        //Alors on affiche une brique normal de base
        //Enzo doit faire une brique de 500/1000 de longeur, 200/1000 de largeur et 200/1000 de profondeur
        // printf("Triangle vide !! Doit donc dessiner la brique à la main %d\n");
            Form::render();

            glBegin(GL_QUADS);
            {
                glVertex3d(0, 1, 0);
                glVertex3d(1, 1, 0);
                glVertex3d(1, 1, 1);
                glVertex3d(0, 1, 1);
            }
            glEnd();
    }else{
        glBegin(GL_TRIANGLES);
        int nbTriangles = 0;
        for (const auto& tri : triangleSTL) {
            glNormal3f(tri.normal.x, tri.normal.y, tri.normal.z);
            for (const auto& vert : tri.vertices) {
                glVertex3f(vert.x, vert.y, vert.z);
            }
            nbTriangles++;
        }
        glEnd();
    }
}

void Sol::update(double delta_t) {
    
}