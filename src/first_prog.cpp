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
#include "forms.h"
#include "Scene.h"

/***************************************************************************/
/* MAIN Function                                                           */
/***************************************************************************/
int main(int argc, char* args[])
{
    // The window we'll be rendering to
    Scene* mainScene = new Scene();
    // std::thread update(Scene::updateLoop, mainScene);
    mainScene->init();
    while(!mainScene->gameOver())
    {
        mainScene->run();
    }

    return 0;
}



