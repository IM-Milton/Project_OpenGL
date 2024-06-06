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
    glTranslated(org.x, org.y, org.z);
    glColor3f(col.r, col.g, col.b);

}


Sphere::Sphere(double r, Color cl)
{
    radius = r;
    col = cl;
}


void Sphere::update(double delta_t)
{
    // Complete this part
}


void Sphere::render()
{
    GLUquadric *quad;
    quad = gluNewQuadric();

    // Complete this part
    Form::render(); // Position the form and assign its color

    // Mise en route de la texture associee
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    gluQuadricTexture(quad,texture_id);
    gluQuadricNormals(quad,GLU_SMOOTH);

    gluSphere(quad, radius, 20, 20);

    gluDeleteQuadric(quad);

    // Ne plus appliquer la texture pour la suite
    glDisable(GL_TEXTURE_2D);
}


Cube_face::Cube_face(Vector v1, Vector v2, Point org, double l, double w, Color cl)
{
    vdir1 = 1.0 / v1.norm() * v1;
    vdir2 = 1.0 / v2.norm() * v2;
    anim.setPos(org);
    length = l;
    width = w;
    col = cl;

}


void Cube_face::update(double delta_t)
{
    // Complete this part
    delta_t = delta_t + 1 ;
}


void Cube_face::render()
{
 // Les quatre coins de la face
    Point p1 = Point();
    Point p2 = p1, p3, p4 = p1;
    p2.translate(length*vdir1);
    p3 = p2;
    p3.translate(width*vdir2);
    p4.translate(width*vdir2);

    // Autorisation de la texture choisie a la creation de la face (cf main())
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Actions communes a toutes les formes
    Form::render();
    glBegin(GL_QUADS);
    {
        //All subsequent vertices will have an associated texture coordinate (coord x y).
        glTexCoord2f(0.0f, 0.0f); //All subsequent vertices will have an associated texture coordinate of (0,0)
        glVertex3d(p1.x, p1.y, p1.z);
        glTexCoord2f(1.0f, 0.0f); //All subsequent vertices will have an associated texture coordinate of (1,0)
        glVertex3d(p2.x, p2.y, p2.z);
        glTexCoord2f(1.0f, 1.0f); //All subsequent vertices will have an associated texture coordinate of (0,0)
        glVertex3d(p3.x, p3.y, p3.z);
        glTexCoord2f(0.0f, 1.0f); //All subsequent vertices will have an associated texture coordinate of (0,0)
        glVertex3d(p4.x, p4.y, p4.z);
    }
    glEnd();

    // Ne plus appliquer la texture pour la suite
    glDisable(GL_TEXTURE_2D);
}
