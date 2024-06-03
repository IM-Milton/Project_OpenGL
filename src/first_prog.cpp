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

/***************************************************************************/
/* Constants and functions declarations                                    */
/***************************************************************************/
// Screen dimension constants
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

// Max number of forms : static allocation
const int MAX_FORMS_NUMBER = 150;

// Animation actualization delay (in ms) => 100 updates per second
const Uint32 ANIM_DELAY = 10;

// Starts up SDL, creates window, and initializes OpenGL
bool init(SDL_Window** window, SDL_GLContext* context);

// Initializes matrices and clear color
bool initGL();

// Updating forms for animation
void update(Form* formlist[MAX_FORMS_NUMBER], reel delta_t);

// Renders scene to the screen
void render(Form* formlist[MAX_FORMS_NUMBER], const Point &cam_pos);

//Objet : --------------------------------------------------------------
void setupMurDeBrique(Form* formlist[MAX_FORMS_NUMBER], unsigned short &number_of_forms, int Longeur=5, int largeur=1, Color col = RED);
//Objet ----------------------------------------------------------------

// Frees media and shuts down SDL
void close(SDL_Window** window);

/***************************************************************************/
/* MAIN Function                                                           */
/***************************************************************************/
int main(int argc, char* args[])
{
    // The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    // OpenGL context
    SDL_GLContext gContext;
    printf("Hello World\n");

    // Start up SDL and create window
    if( !init(&gWindow, &gContext))
    {
        std::cerr << "Failed to initialize!" << std::endl;
    }
    else
    {
        // Main loop flag
        bool quit = false;
        Uint32 current_time, previous_time, elapsed_time, slowMotion = 5;

        // Event handler
        SDL_Event event;

        // Camera position
        Point camera_position(1, 1.0, 0.0);

        // The forms to render
        Form* forms_list[MAX_FORMS_NUMBER];
        unsigned short number_of_forms = 0, i;
        for (i=0; i<MAX_FORMS_NUMBER; i++)
        {
            forms_list[i] = NULL;
        }

        // Create here specific forms and add them to the list...
        // Don't forget to update the actual number_of_forms !
        // Cube *pFace = NULL;
        // pFace = new Cube(Vector(1,0,0), Vector(0,1,0), Point(-0.5, -0.5, -0.5), 1, 1, ORANGE);
        // forms_list[number_of_forms] = pFace;
        // number_of_forms++;

        Sol *sol = new Sol(GREEN); // Créez un nouvel objet de brique en dehors de la boucle
        if (!sol->loadSTL("Solidworks/sol.STL")){
            printf("Failed to load sol.STL model!\n");
            delete sol; // Supprimez l'objet brique si le chargement échoue
        }
        HitZone size = {10000};
        sol->getAnim().setSize(size);
        Point rot(0, 0, 0);
        sol->getAnim().setRotation(rot); // Déplacez le nouvel objet brique
        Point pt(-size.rayon/2, 0, -size.rayon/2);
        sol->getAnim().setPos(pt); // Déplacez le nouvel objet brique
        sol->setPhysics(false); 
        forms_list[number_of_forms] = sol; // Stockez le nouvel objet dans le tableau
        number_of_forms++;

        // The forms to render

        setupMurDeBrique(forms_list, number_of_forms);

        // Point a(0.3,0.2,0.1), b(0.2,0.1,0.0);
        // Point ptEst = a - b;
        // std::cerr << "pt  " << ptEst << std::endl;

        // Get first "current time"
        previous_time = SDL_GetTicks();
        // While application is running
        while(!quit)
        {
            // Handle events on queue
            while(SDL_PollEvent(&event) != 0)
            {
                int x = 0, y = 0;
                SDL_Keycode key_pressed = event.key.keysym.sym;

                switch(event.type)
                {
                // User requests quit
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    // Handle key pressed with current mouse position
                    SDL_GetMouseState( &x, &y );
                    switch(key_pressed)
                    {
                    // Quit the program when 'q' or Escape keys are pressed
                    case SDLK_q:
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_s://slow motion
                        slowMotion += 1;
                        break;
                    case SDLK_d://fast motion
                        slowMotion -= 1;
                        break;
                    case SDLK_z://zoom
                        camera_position.z -= 1;
                        break;
                    case SDLK_e:
                        camera_position.z += 1;
                    break;
                    case SDLK_r://restart

                        break;
                    case SDLK_LEFT:
                        camera_position.x -= 1;
                        printf("Left\n");
                    break;
                    case SDLK_RIGHT:
                      camera_position.x += 1;
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
                update(forms_list, 1e-3 * elapsed_time/(slowMotion*1.)); // International system units : seconds
            }

            // Render the scene
            render(forms_list, camera_position);
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
bool init(SDL_Window** window, SDL_GLContext* context)
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
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
        *window = SDL_CreateWindow( "TP intro OpenGL / SDL 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        if( *window == NULL )
        {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            // Create context
            *context = SDL_GL_CreateContext(*window);
            if( *context == NULL )
            {
                std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else
            {
                // Use Vsync
                if(SDL_GL_SetSwapInterval(1) < 0)
                {
                    std::cerr << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
                }

                // Initialize OpenGL
                if( !initGL() )
                {
                    std::cerr << "Unable to initialize OpenGL!"  << std::endl;
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
    gluPerspective(40.0, (GLdouble)SCREEN_WIDTH/SCREEN_HEIGHT, 1.0, 100.0);


    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Initialize clear color : black with no transparency
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    // Activate Z-Buffer
    glEnable(GL_DEPTH_TEST);


    // Lighting basic configuration and activation
    const GLfloat light_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
    const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

    const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess[] = { 100.0f };

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


    // Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        std::cerr << "Error initializing OpenGL!  " << gluErrorString( error ) << std::endl;
        success = false;
    }

    return success;
}

reel angleDeChuteObjet(reel angle, reel inclinaisionPlan) {
    // Assurer que l'angle est positif entre 0 et 360 degrés
    angle = fmod(angle, 360.);
    if (angle < 0.) {angle += 360.;}
    // Assigner un angle de chute approprié en fonction de l'angle donné
    if (angle < 45. || angle > 315.) {angle = 0.;}
    else if (angle >= 45. && angle < 135.) {angle = 90.;}
    else if (angle >= 135. && angle < 225.) {angle = 180.;}
    else {angle = 270.;}
    return angle + inclinaisionPlan;//On ajout l'angle du plan
}

void angleDeChute(Point &rot, Point rotPlan, Point &posObjet, reel rayonObjet){
    //On doit potentiellement avancé la pos de l'objet par rapport à la correction en angle qu'on fait
    //La rotation sur l'axe z influe sur la position en x
    //La rotation sur l'axe x influe sur la position en z
    reel anglex = angleDeChuteObjet(rot.x, rotPlan.x);
    // rot.y = angleDeChuteObjet(rot.y, rotPlan.y);//On laisse l'angle y inchangé
    reel anglez = angleDeChuteObjet(rot.z, rotPlan.z);

    rayonObjet /= 2.;
    reel offesetx = (rayonObjet)*sin(rot.y*M_PI/180.), offesetz = (rayonObjet)*cos(rot.y*M_PI/180.);
    if(anglez > rot.z){//Alors il faut avancer sur pos.x
        posObjet.x = posObjet.x + offesetx;
    }
    else if(anglez < rot.z){
        posObjet.x = posObjet.x - offesetx;
    }
    if(anglex > rot.x){//Alors il faut avancer sur pos.z
        posObjet.z = posObjet.z + offesetz;
    }
    else if(anglex < rot.x){
        posObjet.z = posObjet.z - offesetz;
    }

    rot = Point(anglex, rot.y, anglez);
}

void detectionCollision(Form* formlist[MAX_FORMS_NUMBER], unsigned short PForm, Point &pos, Point &rot, Vector &Fn){
    // Position et taille du sol (assumées constantes)
    Point sizeSol(50, 0, 50); // Sol de taille 50 m x 0 m x 50 m
    Point posSol(-sizeSol.x / 2, 0, -sizeSol.z / 2); // Position du sol
    Point rotSol(0,0,0);

    Plan planSol(Vector(1, 0, 0), Vector(0, 0, 1)); // Vecteurs pour le plan du sol
    planSol.normal = Vector(1, 0, 0) ^ Vector(0, 0, 1);

    reel masse = formlist[PForm]->getAnim().getMasse();
    reel rayon = formlist[PForm]->getAnim().getSize().rayon;
    pos = formlist[PForm]->getAnim().getPos();
    rot = formlist[PForm]->getAnim().getRotation();
    Fn = formlist[PForm]->getFn();
    Vector speed = formlist[PForm]->getAnim().getSpeed();
    int nbObstacle = 0;
    unsigned short j = 0;
    while (formlist[j] != NULL)
    {
        // printf("j = %d\n", j);
        if(!(j == PForm)){
            switch (formlist[j]->getTypeForm())
            {
            case SOL:
            {
                Vector normal = planSol.normal;
                reel distancetoPlane = (normal.x * pos.x + normal.y * pos.y + normal.z * pos.z) /
                                    sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

                // Calculer les coordonnées de I
                Point I = ((distancetoPlane * normal));
                I = pos - I;
                // std::cout << "Coordonnes de I: " << I << std::endl;
                // // Calculer la distance entre C et I
                reel distanceCI = distance(pos, I);

                // Afficher les résultats

                // Définir le rayon de la sphère
                // reel radius = 2;
                // Vérifier la collision
                if (distanceCI < rayon)
                {
                    // std::cout << "Collision avec le plan." << std::endl;
                    // std::cout << "Coordonnes avant de pos: " << pos << std::endl;
                    // Correction de la position pour éviter l'interpénétration
                    Point correction = (rayon - distanceCI) * normal;
                    pos = pos - correction;
                    Vector Fimpact(0.0, 0.0, 0.0);
                    // Fimpact = -masse / (0.02 * 2 * sqrt(masse)) * speed; // Doit prendre en compte l'angle l'objet pour faire un cos phi
                    Fimpact = (-masse*formlist[PForm]->getAnim().getCoefRestitution()/0.015) * speed;
                    Fn = Fimpact;

                    angleDeChute(rot, rotSol, pos, rayon);

                    // printf("rot.x: %f, rot.y: %f, rot.z: %f\n", rot.x, rot.y, rot.z);
                    nbObstacle++;
                }
            }
            break;
            case BRIQUE:
            {
                reel masseObs = formlist[j]->getAnim().getMasse();
                reel rayonObs = formlist[j]->getAnim().getSize().rayon;
                Point posObs = formlist[j]->getAnim().getPos();
                Point rotObs = formlist[j]->getAnim().getRotation();
                Vector speedObs = formlist[PForm]->getAnim().getSpeed();

                // Vérifiez la position de la brique 1 par rapport à la brique 2
                reel dx = pos.x - posObs.x;
                reel dy = pos.y - posObs.y;
                reel dz = pos.z - posObs.z;
                Point I = Point(dx, dy, dz);
                // // Calculer la distance entre brique 1 et brique 2
                // Vérifier la collision
                reel dL = (rayon + rayonObs); // Distance limite
                if ((abs(I.x) < dL) && (abs(I.y) < dL) && (abs(I.z) < dL))
                {
                    // printf("Collision\n");
                    std::cout << "Collision avec la brique " << pos << posObs << I << std::endl;
                    // Correction de la position pour éviter l'interpénétration
                    // Point correction = (rayon + rayonObs - distanceCI);
                    // // pos = pos - correction;
                    pos.y = posObs.y + dL;//((dL)/cos(rotObs.z*M_PI/180.)) + ((dL)/cos(rotObs.x*M_PI/180.)) ;//* cos(rotObs.z);
                    Vector Fimpact(0.0, 1.0, 0.0);
                    Fimpact = (-masse*formlist[PForm]->getAnim().getCoefRestitution()/0.01) * speed + masse*formlist[PForm]->g*Fimpact; //Doit prendre en compte l'angle l'objet pour faire un cos phi
                    Fn = Fimpact;
                    angleDeChute(rot, rotObs, pos, rayon);
                    // printf("rot.x: %f, rot.y: %f, rot.z: %f\n", rot.x, rot.y, rot.z);
                    nbObstacle ++;
                }
            }
            break;
            default:
                break;
            }
        }
        j++;
    }

    if (!nbObstacle)
    {
        Fn = 0;
        // printf("No obstacle found\n");
    }
}

void update(Form* formlist[MAX_FORMS_NUMBER], reel delta_t) {

    // Mettre à jour la liste de formes
    unsigned short i = 0;
    // Form* parallelFormlist[MAX_FORMS_NUMBER];
    // while (formlist[i] != NULL) {
    //     parallelFormlist[i] = formlist[i];
    //     i++;
    // }

    i = 0;
    while (formlist[i] != NULL) {
        if (formlist[i]->getPhysics())// Alors concerné par la physique
        {
            switch (formlist[i]->getTypeForm()) {
                case BRIQUE:
                {     
                    Point pos = formlist[i]->getAnim().getPos();
                    Point rot = formlist[i]->getAnim().getRotation();
                    Vector Fn = formlist[i]->getFn();
                    

                    // Vérifiez la position de la brique par rapport au sol

                    detectionCollision(formlist, i, pos, rot, Fn);
                    
                    formlist[i]->getAnim().setPos(pos);
                    formlist[i]->getAnim().setRotation(rot);
                    formlist[i]->setFn(Fn);
                }break;
                case SOL:{

                }break;
                default:
                    break;
            }
            formlist[i]->update(delta_t);
            // std::cout << "Coordonnes apres de pos: " << formlist[i]->getAnim().getPos() << std::endl;
        }
        
        i++;
    }
    i = 0;
}

void render(Form* formlist[MAX_FORMS_NUMBER], const Point &cam_pos)
{
    // Clear color buffer and Z-Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Set the camera position and parameters
    gluLookAt(cam_pos.x,cam_pos.y,cam_pos.z, 0.0,0.0,0.0, 0.0,1.0,0.0);
    // Isometric view
    glRotated(-45, 0, 1, 0);
    glRotated(30, 1, 0, -1);

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
    while(formlist[i] != NULL)
    {
        // printf("Type %d\n", formlist[i]->getTypeForm());
        glPushMatrix(); // Preserve the camera viewing point for further forms
        formlist[i]->render();
        glPopMatrix(); // Restore the camera viewing point for next object
        i++;
    }
}

//Objet : --------------------------------
void setupMurDeBrique(Form* formlist[MAX_FORMS_NUMBER], unsigned short &number_of_forms, int Longeur, int largeur, Color col) {
    static const HitZone size = {((200./2.)/1000.)};//Brique cubique de taille de 200 mm de long exprimé en metre

    Brique *brique = new Brique(col); // Créez un nouvel objet de brique en dehors de la boucle
    if (!brique->loadSTL("Solidworks/brique.STL")){
        printf("Failed to load brique.STL model!\n");
        delete brique; // Supprimez l'objet brique si le chargement échoue
        return;
    }
    brique->getAnim().setMasse(60./1000);//12.8 kg
    brique->getAnim().setSize(size);
    brique->getAnim().setCoefRestitution(1);
    printf("Size Objet : size rayon =  %2.1f\n", size.rayon);

    Brique* newBrique = new Brique(*brique); // Créez un nouvel objet brique à chaque itération
    newBrique->getAnim().setPos(Point(0,size.rayon,0));
    newBrique->getAnim().setRotation(Point(20.,0,20));
    newBrique->setPhysics(false);
    // newBrique->getAnim().setSpeed(Vector(0, 10.0, 0.0));
    newBrique->getAnim().setSize(size);
    newBrique->setColor(WHITE);

    Point pt(0, 2, 0);
    Point rot(0.,0,0);//en degrees
    brique->getAnim().setRotation(rot);
    brique->getAnim().setPos(pt); // Déplacez le nouvel objet brique
    brique->getAnim().setSpeed(Vector(0, 0.0, 0.0));//Vitesse ou force initiale
    brique->setPhysics();
    // brique.set
    formlist[number_of_forms] = brique; // Stockez le nouvel objet dans le tableau
    number_of_forms++;
    formlist[number_of_forms] = newBrique; // Stockez le nouvel objet dans le tableau
    number_of_forms++;
    
    

    // for (int i = 0; i < largeur; i++) {
    //     for (int j = 0; j < Longeur; j++) {
    //         Brique* newBrique = new Brique(*brique); // Créez un nouvel objet brique à chaque itération
    //         Point pt(j * size.x, i * size.y, 0.0 * size.x); //en metres
    //         newBrique->getAnim().setPos(pt); // Déplacez le nouvel objet brique
    //         formlist[number_of_forms] = newBrique; // Stockez le nouvel objet dans le tableau
    //         number_of_forms++;
    //     }
    // }
    // delete brique; // Supprimez l'objet brique une fois que vous avez terminé avec lui
}



//Objet ----------------------------------

void close(SDL_Window** window)
{
    //Destroy window
    SDL_DestroyWindow(*window);
    *window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}
