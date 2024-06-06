// Using SDL, SDL OpenGL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
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

/***************************************************************************/
/* Constants and functions declarations                                    */
/***************************************************************************/
// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Max number of forms : static allocation
const int MAX_FORMS_NUMBER = 150;

// Animation actualization delay (in ms) => 100 updates per second
const Uint32 ANIM_DELAY = 10;

// Starts up SDL, creates window, and initializes OpenGL
bool init(SDL_Window **window, SDL_GLContext *context);

// Initializes matrices and clear color
bool initGL();

// Updating forms for animation
void update(Form *formlist[MAX_FORMS_NUMBER], double delta_t);

// Renders scene to the screen
void render(Form *formlist[MAX_FORMS_NUMBER], const Point &cam_pos, float angleAlphaY, float TranslateX, float TranslateZ);

// Objet : --------------------------------------------------------------
void setupMurDeBrique(Form *formlist[MAX_FORMS_NUMBER], unsigned short &number_of_forms, int Longeur = 5, int largeur = 1, Color col = RED);
// Objet ----------------------------------------------------------------

// Frees media and shuts down SDL
void close(SDL_Window **window);

int createTextureFromImage(const char *filename, GLuint *textureID)
{
    SDL_Surface *imgSurface = IMG_Load(filename);
    if (imgSurface == NULL)
    {
        std::cerr << "Failed to load texture image: " << filename << std::endl;
        return -1;
    }
    else
    {
        // Work out what format to tell glTexImage2D to use...
        int mode;
        if (imgSurface->format->BytesPerPixel == 3) // RGB 24bit
        {
            mode = GL_RGB;
        }
        else if (imgSurface->format->BytesPerPixel == 4) // RGBA 32bit
        {
            mode = GL_RGBA;
        }
        else
        {
            SDL_FreeSurface(imgSurface);
            std::cerr << "Unable to detect the image color format of: " << filename << std::endl;
            return -2;
        }
        // create one texture name
        glGenTextures(1, textureID);

        // tell opengl to use the generated texture name
        glBindTexture(GL_TEXTURE_2D, *textureID);

        // this reads from the sdl imgSurface and puts it into an openGL texture
        glTexImage2D(GL_TEXTURE_2D, 0, mode, imgSurface->w, imgSurface->h, 0, mode, GL_UNSIGNED_BYTE, imgSurface->pixels);

        // these affect how this texture is drawn later on...
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // clean up
        SDL_FreeSurface(imgSurface);
        return 0;
    }
}

Sol *createSTL(Form *forms_list[MAX_FORMS_NUMBER], unsigned short &number_of_forms, const std::string &path, Point pt, Point rotation, GLuint textureid_1, Color Couleur)
{
    Sol *sol = new Sol(Couleur); // Créez un nouvel objet de brique en dehors de la boucle
    if (!sol->loadSTL(path))
    {
        printf("Failed to load STL model!\n");
        // delete sol; // Supprimez l'objet brique si le chargement échoue
    }
    sol->moveAbsolue(pt);
    sol->getAnim().setRotation(rotation); // Déplacez le nouvel objet brique
    sol->setTexture(textureid_1);

    forms_list[number_of_forms] = sol; // Stockez le nouvel objet dans le tableau
    number_of_forms++;
    return sol;
}

void CreateTrees(Form *forms_list[MAX_FORMS_NUMBER], unsigned short &number_of_forms,Point tree)
{

    Sol *Trunk = new Sol(BROWN); // Créez un nouvel objet de brique en dehors de la boucle

    if (!Trunk->loadSTL("Solidworks/trunk_V2Plan.STL"))
    {
        printf("Failed to load STL model!\n");
        // delete sol; // Supprimez l'objet brique si le chargement échoue
    }
    //tree(10, 0, 10); // à re

    Trunk->moveAbsolue(tree);
    Point rot_trunk(-90, 0, 0);
    Trunk->getAnim().setRotation(rot_trunk); // Déplacez le nouvel objet brique
    forms_list[number_of_forms] = Trunk;     // Stockez le nouvel objet dans le tableau
    number_of_forms++;
    Sol *leaf = new Sol(BLACKGREEN); // Créez un nouvel objet de brique en dehors de la boucle
    if (!leaf->loadSTL("Solidworks/Leaf_V4.STL"))
    {
        printf("Failed to load STL model!\n");
        // delete sol; // Supprimez l'objet brique si le chargement échoue
    }
    tree.x -= 0.4;
    tree.y += 2;
    leaf->moveAbsolue(tree);
    Point rot_leaf(90, -30, 0);
    leaf->getAnim().setRotation(rot_leaf); // Déplacez le nouvel objet brique
    forms_list[number_of_forms] = leaf;     // Stockez le nouvel objet dans le tableau
    number_of_forms++;



    // createSTL(forms_list, number_of_forms, "Solidworks/trunk_V2Plan.STL", pt_trunk, rot_trunk, textureid_1, BROWN);
}

void Repeat_trees(Form *forms_list[MAX_FORMS_NUMBER], unsigned short &number_of_forms,int Longeur)
{
    float distance_X = 10;
    float entraxe_chateau = 10;
    int Largeur = 2;
    for (int i = 0; i < Largeur; i++)
    {
        for (int j = 0; j < Longeur; j++)
        {
            Point tree((j * distance_X)+10,  0, (i * entraxe_chateau)+2); //en metres)
            CreateTrees(forms_list, number_of_forms,tree);
            // newBrique->getAnim().setPos(pt); // Déplacez le nouvel objet brique
        }
    }

}

void setupMur(Form *forms_list[MAX_FORMS_NUMBER], unsigned short &number_of_forms,int Longeur)
{
    float rayon = 0.800/2;
    int largeur  = 6;

    for (int i = 0; i < largeur; i++)
    {
        for (int j = 0; j < Longeur; j++)
        {

            Sol *Brique = new Sol(GREY); // Créez un nouvel objet de brique en dehors de la boucle
            if (!Brique->loadSTL("Solidworks/brique_800_new_origine.STL"))
            {
                printf("Failed to load STL model!\n");
                // delete sol; // Supprimez l'objet brique si le chargement échoue
            }
            Point pt(5.6, (i * (2*rayon)+rayon)+0, (j * (2.01*rayon))+2.4); //en metres
            Brique->getAnim().setPos(pt); // Déplacez le nouvel objet brique

            forms_list[number_of_forms] = Brique;     // Stockez le nouvel objet dans le tableau
            number_of_forms++; // newBrique->getAnim().setPos(pt); // Déplacez le nouvel objet brique
        }
    }

}

/***************************************************************************/
/* MAIN Function                                                           */
/***************************************************************************/
int main(int argc, char *args[])
{
    // The window we'll be rendering to
    SDL_Window *gWindow = NULL;
    int angleAlphaY;
    float TranslateX;
    float TranslateZ;
    float angle_lancement = 10;
    float CoordX = 9.6, CoordY = 2, CoordZ = 10;
    GLuint textureid_1, textureid_2;
    int RotY = 180;
    int Longeur = 12;
    int largeur = 6;

    createTextureFromImage("resources/images/earth_texture.jpg", &textureid_1);
    createTextureFromImage("resources/images/tiles.bmp", &textureid_2);
    // OpenGL context
    SDL_GLContext gContext;
    printf("Hello World\n");

    // Start up SDL and create window
    if (!init(&gWindow, &gContext))
    {
        std::cerr << "Failed to initialize!" << std::endl;
    }
    else
    {
        // Main loop flag
        bool quit = false;
        Uint32 current_time, previous_time, elapsed_time;

        // Event handler
        SDL_Event event;

        // Camera position
        Point camera_position(0, -6, 19);

        // The forms to render
        Form *forms_list[MAX_FORMS_NUMBER];
        unsigned short number_of_forms = 0, i;
        for (i = 0; i < MAX_FORMS_NUMBER; i++)
        {
            forms_list[i] = NULL;
        }

        // Create here specific forms and add them to the list...
        // Don't forget to update the actual number_of_forms !
        Point pt_chateau(0, 0, 6.5);
        Point rotation_chateau(0, 180, 0);
        createSTL(forms_list, number_of_forms, "Solidworks/Castle_new_origine.stl", pt_chateau, rotation_chateau, textureid_1, GREY);
        Point rotation_catapulte(0, 90, 0);
        Point pt_catapulte(40, 0, 6.5);
        createSTL(forms_list, number_of_forms, "Solidworks/Catapulte_ASSEMBLY_without_sppon.STL", pt_catapulte, rotation_catapulte, textureid_1, ORANGE);
        Point pt_spoon(41.3, 1.4, 5.5); // à re
        Point rot_spoon(90, 120, 0);
        Sol *spoon = createSTL(forms_list, number_of_forms, "Solidworks/catapulte_test.STL", pt_spoon, rot_spoon, textureid_1, RED);

        Point pt_balle(20, 0, 20); // à re
        Point rot_balle(-90, 0, 0);
        createSTL(forms_list, number_of_forms, "Solidworks/Boule.STL", pt_balle, rot_balle, textureid_1, BROWN);
        Point pt_nuage(20, 40, 20); // à re
        Point rot_nuage(-90, 0, 0);
        createSTL(forms_list, number_of_forms, "Solidworks/nuage.STL", pt_nuage, rot_nuage, textureid_1, BROWN);
        Repeat_trees(forms_list, number_of_forms,4);
        setupMur(forms_list, number_of_forms,12);
        // Point pt_trees(20,0,20;)
        // The forms to render
        // setupMurDeBrique(forms_list, number_of_forms);
        // Get first "current time"
        previous_time = SDL_GetTicks();
        // While application is running
        while (!quit)
        {
            //printf(" X, = %f  Y = %f Z = %f \n", CoordX, CoordY, CoordZ);
            // printf("camera x = %f, camera y  = %f, camera z %f \n", camera_position.x, camera_position.y, camera_position.z);
            //  Handle events on queue
            while (SDL_PollEvent(&event) != 0)
            {
                int x = 0, y = 0;
                SDL_Keycode key_pressed = event.key.keysym.sym;

                switch (event.type)
                {
                // User requests quit
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    // Handle key pressed with current mouse position
                    SDL_GetMouseState(&x, &y);
                    switch (key_pressed)
                    {
                    // Quit the program when 'q' or Escape keys are pressed
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_LEFT:
                        angleAlphaY++;
                        printf("%f\n", angleAlphaY);
                        break;
                    case SDLK_RIGHT:
                        angleAlphaY--;
                        printf("Right\n");
                        break;
                    case SDLK_UP:
                        camera_position.y += 1;
                        printf("Up\n");
                        break;
                    case SDLK_DOWN:
                        camera_position.y -= 1;
                        printf("Down\n");
                        break;
                    case SDLK_q:
                        TranslateX++;
                        break;
                    case SDLK_d:
                        TranslateX--;
                        printf("D\n");
                        break;
                    case SDLK_z:
                        camera_position.z -= 1;

                        printf("zoom\n");
                        break;
                    case SDLK_e:
                        camera_position.z -= 1;
                        printf("Dezoom\n");
                        break;
                    case SDLK_w:
                        TranslateZ++;
                        printf("zoom\n");
                        break;
                    case SDLK_x:
                        TranslateZ--;
                        printf("Dezoom\n");
                        break;
                    case SDLK_r:
                        camera_position.x = 0;
                        camera_position.y = 1.0;
                        camera_position.z = 100.0;
                        printf("R\n");
                        break;
                    case SDLK_m:
                        printf("m\n");
                        break;
                    case SDLK_l:
                        printf("l\n");
                        break;
                    case SDLK_p:
                        CoordY += 0.2;
                        printf("m\n");
                        break;
                    case SDLK_o:
                        CoordY -= 0.2;
                        printf("l\n");
                        break;
                    case SDLK_1:
                        RotY -= 1;
                        //spoon->getAnim().setRotation(Point(90, RotY, 0));
                        printf("\n");
                        break;
                    case SDLK_2:
                        RotY += 1;
                        //spoon->getAnim().setRotation(Point(90, RotY, 0));
                        printf("l\n");
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }
            }

            // Update the scene
            current_time = SDL_GetTicks(); // get the elapsed time from SDL initialization (ms)
            elapsed_time = current_time - previous_time;
            if (elapsed_time > ANIM_DELAY)
            {
                previous_time = current_time;
                update(forms_list, 1e-3 * elapsed_time); // International system units : seconds
            }

            // Render the scene
            render(forms_list, camera_position, angleAlphaY, TranslateX, TranslateZ);
            // Update window screen
            SDL_GL_SwapWindow(gWindow);
        }
    }

    // Free resources and close SDL
    close(&gWindow);

    return 0;
}

/***************************************************************************/
/* Functions implementations                                               */
/***************************************************************************/
bool init(SDL_Window **window, SDL_GLContext *context)
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else
    {
        // Use OpenGL 2.1
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        // Create window
        *window = SDL_CreateWindow("TP intro OpenGL / SDL 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if (*window == NULL)
        {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            // Create context
            *context = SDL_GL_CreateContext(*window);
            if (*context == NULL)
            {
                std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else
            {
                // Use Vsync
                if (SDL_GL_SetSwapInterval(1) < 0)
                {
                    std::cerr << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
                }

                // Initialize OpenGL
                if (!initGL())
                {
                    std::cerr << "Unable to initialize OpenGL!" << std::endl;
                    success = false;
                }
            }
        }
    }

    return success;
}

bool initGL()
{
    bool success = true;
    GLenum error = GL_NO_ERROR;

    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport : use all the window to display the rendered scene
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Fix aspect ratio and depth clipping planes
    gluPerspective(40.0, (GLdouble)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0, 3000.0);

    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Initialize clear color : black with no transparency
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Activate Z-Buffer
    glEnable(GL_DEPTH_TEST);

    // Lighting basic configuration and activation
    const GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    const GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    const GLfloat light_position[] = {2.0f, 5.0f, 5.0f, 0.0f};

    const GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
    const GLfloat mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    const GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    const GLfloat high_shininess[] = {100.0f};

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    // Check for error
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "Error initializing OpenGL!  " << gluErrorString(error) << std::endl;
        success = false;
    }

    return success;
}

void update(Form *formlist[MAX_FORMS_NUMBER], double delta_t)
{
    // Position et taille du sol (assumées constantes)
    Point sizeSol(50, 0, 50);                        // Sol de taille 50 m x 0 m x 50 m
    Point posSol(-sizeSol.x / 2, 0, -sizeSol.z / 2); // Position du sol

    // Mettre à jour la liste de formes
    unsigned short i = 0;
    while (formlist[i] != NULL)
    {
        switch (formlist[i]->getTypeForm())
        {
        case BRIQUE:
        {
            Point pos = formlist[i]->getAnim().getPos();
            Vector speed = formlist[i]->getAnim().getSpeed();
            // Vérifiez la position de la brique par rapport au sol
            if (pos.y <= posSol.y + sizeSol.y)
            {
                // Collision détectée : ajustez la position et la Force de contre reaction de la brique sur le sol
                pos.y = posSol.y + sizeSol.y;
                Vector force_contre_reaction(0.0, formlist[i]->g * formlist[i]->getMasse(), 0.0); // Force de contre reaction
                // formlist[i]->setFn(force_contre_reaction);
                // speed.y = -1/formlist[i]->getMasse() *speed.y;//0;
                // formlist[i]->getAnim().setPos(pos);
                // formlist[i]->getAnim().setSpeed(speed);
            }
        }
        break;
        case SOL:
        {
        }
        break;
        default:
            break;
        }
        formlist[i]->update(delta_t);
        i++;
    }
}

void render(Form *formlist[MAX_FORMS_NUMBER], const Point &cam_pos, float angleAlphaY, float TranslateX, float TranslateZ)
{
    // Clear color buffer and Z-Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // glFrustum(-1.0, 1.0, -1.0, 1.0, 60.0, 100.0);
    //  Set the camera position and parameters
    gluLookAt(cam_pos.x, cam_pos.y, cam_pos.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // Isometric view
    glRotated(-45, 0, 1, 0);
    glRotated(30, 1, 0, -1);
    glRotated(angleAlphaY, 0, 1, 0);
    glTranslated(TranslateX, 0, 0);
    glTranslated(0, 0, TranslateZ);
    // X, Y and Z axis
    glPushMatrix(); // Preserve the camera viewing point for further forms
    // Render the coordinates system
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(1, 0, 0);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 1, 0);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 0, 1);
    }
    glEnd();
    glPopMatrix(); // Restore the camera viewing point for next object

    // Render the list of forms
    unsigned short i = 0;
    while (formlist[i] != NULL)
    {
        // printf("Type %d\n", formlist[i]->getTypeForm());
        glPushMatrix(); // Preserve the camera viewing point for further forms
        formlist[i]->render();
        glPopMatrix(); // Restore the camera viewing point for next object
        i++;
    }
}

// Objet : --------------------------------
void setupMurDeBrique(Form *formlist[MAX_FORMS_NUMBER], unsigned short &number_of_forms, int Longeur, int largeur, Color col)
{
    static const Point size(500. / 1000, 200. / 1000, 200. / 1000); // Brique de taille de 500 mm , 200 mm, 200 mm, exprimé en metre

    Brique *brique = new Brique(col); // Créez un nouvel objet de brique en dehors de la boucle
    if (!brique->loadSTL("Solidworks/brique.STL"))
    {
        printf("Failed to load brique.STL model!\n");
        delete brique; // Supprimez l'objet brique si le chargement échoue
        return;
    }
    brique->setMasse(18.4); // kg
    brique->setSize(size);
    printf("Size Objet : size X = %2.1f, size Y = %2.1f, size Z = %2.1f\n", size.x, size.y, size.z);
    Point pt(0, 0, 0);
    Point rot(45, 0, 0); // en degrees
    brique->getAnim().setRotation(rot);
    brique->moveAbsolue(pt); // Déplacez le nouvel objet brique
    // brique.set
    formlist[number_of_forms] = brique; // Stockez le nouvel objet dans le tableau
    number_of_forms++;

    // for (int i = 0; i < largeur; i++) {
    //     for (int j = 0; j < Longeur; j++) {
    //         Brique* newBrique = new Brique(*brique); // Créez un nouvel objet brique à chaque itération
    //         Point pt(j * size.x, i * size.y, 0.0 * size.x); //en metres
    //         newBrique->moveAbsolue(pt); // Déplacez le nouvel objet brique
    //         formlist[number_of_forms] = newBrique; // Stockez le nouvel objet dans le tableau
    //         number_of_forms++;
    //     }
    // }
    // delete brique; // Supprimez l'objet brique une fois que vous avez terminé avec lui
}

// Objet ----------------------------------

void close(SDL_Window **window)
{
    // Destroy window
    SDL_DestroyWindow(*window);
    *window = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}
