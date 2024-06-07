// Using SDL, SDL OpenGL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <GL/gl.h>
// Module for space geometry
#include "geometry.h"
// Module for generating and rendering forms
#include "config.h"
#include "forms.h"



class Scene
{
private:
    Form* formlist[MAX_FORMS_NUMBER];
    Form* tempFormlist[MAX_FORMS_NUMBER];//Utiliser pour stocker l'etat temporairement
    int formIndex =0;
    Uint32 current_time, previous_time, elapsed_time;
    SDL_Event event;
    Point camera_position;
    SDL_Window* gWindow =NULL;
    SDL_GLContext gContext;
    float timeSlow = 2.0f;
    bool quit=true;
    bool _initGL();
    bool _initWindow(SDL_Window** window, SDL_GLContext* context);
    bool setupObjects();
    char checkInput();
    bool _useSDL=false;
    void close(SDL_Window** window);
    void checkCollision(int &formIndex, Point &pos, Point &rot, Vector &Fn, reel delta_t);

    void setupMurDeBrique(int Longeur, int largeur, Point initiale, Color col, HitZone size = {((800./2.)/1000.)}, Point rot = Point(0,0,0));

    void setupCatapulte(Point position, Point rot = Point(0,0,0));

    void CreateTrees(Point tree); void Repeat_trees(int Longeur);




public:





    sceneInput input = {false,false,false,false,false,false,false,false,false,false};
    bool _pause = false;

    Scene();

    bool init();
    void render();
    void update(reel delta_t);
    void run();
    bool popForm();
    bool gameOver(){return quit;}
    bool addForm(Form* form);
    // bool removeForm(int index);
    // bool removeForm(Form* form);
    bool useSDL(){return _useSDL;}
};



