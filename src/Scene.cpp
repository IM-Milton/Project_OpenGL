#include "Scene.h"
#include "forms.h"
#include "config.h"

Scene::Scene()
{

    if( !_initWindow(&gWindow, &gContext))
    {// Start up SDL and create window
        std::cerr << "Failed to initialize!" << std::endl;
    }
    for (int i=0; i<MAX_FORMS_NUMBER; i++)
    {
        formlist[i] = NULL;
    }

}

bool Scene::init()
{

    for (int i=0; i<MAX_FORMS_NUMBER; i++)
    {
        if(formlist[i] != NULL){
            delete formlist[i] ;
            formlist[i] = NULL;
        }
    }

    formIndex =0;


    camera_position=Point(0, 1.0, 20.0);

    quit=false;
    setupObjects();

}



bool Scene::setupObjects() // Initialisation des objets
{
    // Create here specific forms and add them to the list...
    // Don't forget to update the actual number_of_forms !
    // Cube *pFace = NULL;
    // pFace = new Cube(Vector(1,0,0), Vector(0,1,0), Point(-0.5, -0.5, -0.5), 1, 1, ORANGE);
    // forms_list[number_of_forms] = pFace;
    // number_of_forms++;

    // Brique *brique = new Brique(GREEN,18.4, size,"Solidworks/brique.STL");
    // if(!brique->modelSTL.isLoaded())
    // {
    //     printf("Failed to load brique.STL model!\n");
    // }
    // brique->getAnim().setPos(Point(1,1,1));
    // addForm(brique);

    PlanForm *sol = new PlanForm(GREEN, "Solidworks/sol.STL"); // Créez un nouvel objet de brique en dehors de la boucle
        if (!sol->modelSTL.isLoaded()){
            printf("Failed to load sol.STL model!\n");
            delete sol; // Supprimez l'objet brique si le chargement échoue
        }
        HitZone size = {10000};
        sol->getAnim().setSize(size);
        sol->getPlan().setVector(Vector(0, 0, 1),Vector(1, 0, 0));
        Point rot(0, 0, 0);
        sol->getAnim().setRotation(rot); // Déplacez le nouvel objet brique
        sol->getPlan().rotateVectors(rot);
        Point pt(-size.rayon/2, 0, -size.rayon/2);
        sol->getAnim().setPos(pt); // Déplacez le nouvel objet brique
        sol->setPhysics(false);
    addForm(sol);
    Sphere *sphere = new Sphere(YELLOW,200,{0.5});
    sphere->getAnim().setPos(Point(1.5,0,15));
    sphere->getAnim().setSpeed(Vector(0, 0, -10.0));
    sphere->getAnim().setSpeedRotation(Vector(0, 360*100, 0.0));
    Sphere* newS = new Sphere(*sphere);
    newS->getAnim().setPos(Point(0.5,0,15+2));
    // sphere->setCollisionEffect(false);
    addForm(sphere);
    addForm(newS);
    staticForm *chateau = new staticForm(RED, Point(0, 0, 0), Point(0, 180, 0), "Solidworks/chateau.STL");
    if (!chateau->modelSTL.isLoaded()){
        printf("Failed to load chateau.STL model!\n");
        delete chateau; // Supprimez l'objet brique si le chargement échoue
    }
    // addForm(chateau);

    setupMurDeBrique(10,6, Point(13.65,0,0),RED);

    if(USE_STL)
    {
        printf("afficher les STL");
    }

}

void Scene::setupMurDeBrique(int Longeur, int largeur, Point initiale, Color col) {
    static const HitZone size = {((200./2.)/1000.)};//Brique cubique de taille de 200 mm de long exprimé en metre

    Brique *brique = new Brique(col, 12.8, size, "Solidworks/brique_new_origine.STL"); // Créez un nouvel objet de brique en dehors de la boucle
    if (!brique->modelSTL.isLoaded()){
        printf("Failed to load brique.STL model!\n");
        // delete brique; // Supprimez l'objet brique si le chargement échoue
        return;
    }
    brique->getAnim().setMasse(12.8); // 12.8 kg
    brique->getAnim().setSize(size);
    brique->getAnim().setCoefRestitution(0.2);
    //brique->getAnim().setSpeedRotation(Vector(0, 360*10, 0.0));
    printf("Size Objet : size rayon =  %2.1f\n", size.rayon);

    Brique* newB = new Brique(*brique); // Créez un nouvel objet brique à chaque itération
    newB->getAnim().setPos(Point(-1,5,0));
    newB->getAnim().setRotation(Point(30.,0,0));
    newB->setPhysics();
    // newB->getAnim().setSpeed(Vector(-10.0, 5.0, -10.0));
    // newB->getAnim().setSpeedRotation(Vector(0, 360*10, 0.0));
    newB->getAnim().setSize(size);
    newB->getAnim().setColor(WHITE);

    // Point pt(0,2,0);
    // Point rot(0.,0,0);//en degrees
    // brique->getAnim().setRotation(rot);
    // brique->getAnim().setPos(pt); // Déplacez le nouvel objet brique
    // brique->getAnim().setSpeed(Vector(0, 0.0, 0));//Vitesse ou force initiale
    // // brique->getAnim().setSpeedRotation(Vector(0, 360*10, 0.0));
    // brique->setPhysics(true);
    // // brique.set
    // addForm(brique);

    addForm(newB);

    for (int i = 0; i < largeur; i++) {
        for (int j = 0; j < Longeur; j++) {
            Brique* newBrique = new Brique(*brique); // Créez un nouvel objet brique à chaque itération
            Point pt(j * (2.01*size.rayon), i * (2*size.rayon)+size.rayon, 0); //en metres
            newBrique->getAnim().setPos(pt); // Déplacez le nouvel objet brique
            newBrique->setPhysics(true);
            addForm(newBrique); // Stockez le nouvel objet dans le tableau
        }
    }
    delete brique; // Supprimez l'objet brique une fois que vous avez terminé avec lui

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
// experience
void projectOntoAxis(const std::vector<Vector>& vertices, const Vector& axis, float& min, float& max) {
    min = max = vertices[0] * axis;
    for (const auto& vert : vertices) {
        float projection = vert * axis;
        if (projection < min) min = projection;
        if (projection > max) max = projection;
    }
}

bool SATCollision(const std::vector<Vector>& vertices1, const std::vector<Vector>& vertices2, const std::vector<Vector>& axes) {
    for (const auto& axis : axes) {
        float minA, maxA, minB, maxB;
        projectOntoAxis(vertices1, axis, minA, maxA);
        projectOntoAxis(vertices2, axis, minB, maxB);
        if (maxA < minB || maxB < minA) {
            return false;
        }
    }
    return true;
}

std::vector<Vector> getVertices(const Vector& pos, const Vector& halfSize, const Point& rot) {
    std::vector<Vector> vertices(8);
    vertices[0] = Vector{halfSize.x, halfSize.y, halfSize.z};
    vertices[1] = Vector{halfSize.x, halfSize.y, -halfSize.z};
    vertices[2] = Vector{halfSize.x, -halfSize.y, halfSize.z};
    vertices[3] = Vector{halfSize.x, -halfSize.y, -halfSize.z};
    vertices[4] = Vector{-halfSize.x, halfSize.y, halfSize.z};
    vertices[5] = Vector{-halfSize.x, halfSize.y, -halfSize.z};
    vertices[6] = Vector{-halfSize.x, -halfSize.y, halfSize.z};
    vertices[7] = Vector{-halfSize.x, -halfSize.y, -halfSize.z};

    for (auto& vert : vertices) {
        rotateVector(vert, rot);
        vert = vert + pos;
    }

    return vertices;
}

std::vector<Vector> getAxes(const Point& rot) {
    std::vector<Vector> axes(3);
    axes[0] = Vector{1, 0, 0};
    axes[1] = Vector{0, 1, 0};
    axes[2] = Vector{0, 0, 1};

    for (auto& axis : axes) {
        rotateVector(axis, rot);
    }

    return axes;
}
// experience
void Scene::checkCollision(int &formIndex, Point &pos, Point &rot, Vector &Fn, reel delta_t)
{
    static int i = 0, nbObstacle = 0;
    static reel masse, rayon, e;
    static Vector vector_pos, speed;
    masse = formlist[formIndex]->getAnim().getMasse();
    rayon = formlist[formIndex]->getAnim().getSize().rayon;
    pos = formlist[formIndex]->getAnim().getPos();
    vector_pos = Vector(pos.x, pos.y, pos.z);
    rot = formlist[formIndex]->getAnim().getRotation();
    Fn = formlist[formIndex]->getFn();
    speed = formlist[formIndex]->getAnim().getSpeed();
    e = formlist[formIndex]->getAnim().getCoefRestitution();

    SHAPE_ID id = formlist[formIndex]->getAnim().getTypeForm();

    nbObstacle = 0;
    i = 0;
    bool sphereDetected = false;
    while(formlist[i]!=NULL && !sphereDetected)
    {
        if(i!=formIndex) {
            static reel masseObs, rayonObs;
            static Point posObs, rotObs;
            static Vector vector_posObs, speedObs;
            masseObs = formlist[i]->getAnim().getMasse();
            rayonObs = formlist[i]->getAnim().getSize().rayon;
            posObs = formlist[i]->getAnim().getPos();
            vector_posObs = Vector(posObs.x, posObs.y, posObs.z);
            rotObs = formlist[i]->getAnim().getRotation();
            speedObs = formlist[i]->getAnim().getSpeed();
            switch (formlist[i]->getAnim().getTypeForm())
            {
            case PLAN:
            {
                static reel distancetoPlane;
                static Vector normal, v1, v2;
                normal = formlist[i]->getPlan().getNormal();
                v1 = formlist[i]->getPlan().v1;
                v2 = formlist[i]->getPlan().v2;
                distancetoPlane = (normal.x * pos.x + normal.y * pos.y + normal.z * pos.z) /
                                    sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
                // printf("dist : %f\n", distancetoPlane);

                // Calculer les coordonnées de I
                // Point I = ((distancetoPlane * normal));
                // I = pos - I;
                // // std::cout << "Coordonnes de I: " << I << std::endl;
                // // // Calculer la distance entre objet et I
                // reel dist = distance(pos, I);

                // Afficher les résultats

                // Définir le rayon de la sphère
                // reel radius = 2;
                // Vérifier la collision
                if (distancetoPlane <= rayon)
                {
                    static reel comp1, comp2, comp_speed_1, comp_speed_2;
                    // std::cout << "Collision avec le plan." << std::endl;

                    comp1 = vector_pos * v1;
                    comp2 = vector_pos * v2;
                    vector_pos = rayon * normal + comp1 * v1 + comp2 * v2;
                    formlist[formIndex]->getAnim().setPos(vector_pos);
                    pos = formlist[formIndex]->getAnim().getPos();
                    // printf("comp1 : %f << comp2 : %f  <<  ",comp1, comp2 );
                    // std::cout << "Coordonnes de pos: " << pos << vector_pos << v1 << v2 << std::endl;

                    // Fimpact = -masse / (0.02 * 2 * sqrt(masse)) * speed + masse*formlist[formIndex]->g*Fimpact; // Doit prendre en compte l'angle l'objet pour faire un cos phi
                    Fn = masse*formlist[formIndex]->g * normal;

                    comp_speed_1 = speed * v1;
                    comp_speed_2 = speed * v2;
                    speed = comp_speed_1 * v1 + comp_speed_2 * v2 - e * (speed * normal) * normal;
                    formlist[formIndex]->getAnim().setSpeed(speed);

                    angleDeChute(rot, formlist[i]->getAnim().getRotation(), pos, rayon);

                    // printf("rot.x: %f, rot.y: %f, rot.z: %f\n", rot.x, rot.y, rot.z);
                    nbObstacle++;
                }
            }
            break;
            case BRIQUE:
            {
                if(id==SPHERE)
                {
                    break;
                }
                static Vector halfSize1, halfSize2;
                halfSize1 = Vector{rayon, rayon, rayon};
                halfSize2 = Vector{rayonObs, rayonObs, rayonObs};

                auto vertices1 = getVertices(vector_pos, halfSize1, rot);
                auto vertices2 = getVertices(vector_posObs, halfSize2, rotObs);

                auto axes1 = getAxes(rot);
                auto axes2 = getAxes(rotObs);

                std::vector<Vector> axes;
                axes.insert(axes.end(), axes1.begin(), axes1.end());
                axes.insert(axes.end(), axes2.begin(), axes2.end());

                for (const auto& axis1 : axes1) {
                    for (const auto& axis2 : axes2) {
                        axes.push_back(Vector{
                            axis1.y * axis2.z - axis1.z * axis2.y,
                            axis1.z * axis2.x - axis1.x * axis2.z,
                            axis1.x * axis2.y - axis1.y * axis2.x
                        });
                    }
                }

                if (SATCollision(vertices1, vertices2, axes)) { // 2 briques en collision
                    static Vector normal, v1, v2;
                    static reel dist, comp1, comp2, comp_speed_1, comp_speed_2;
                    // printf("Collision\n");
                    normal = vector_pos - vector_posObs;

                    // Vector correction = (halfSize1 + halfSize2) - (vector_pos - vector_posObs);
                    // pos = (vector_pos - correction);

                    v1 = Vector{1, 0, 0}; // Axe X
                    v2 = Vector{0, 1, 0}; // Axe Y
                    rotateVector(v1, rotObs);
                    rotateVector(v2, rotObs);

                    dist = distance(pos, posObs);
                    comp1 = vector_pos * v1;
                    comp2 = vector_pos * v2;
                    vector_pos = ((rayon + rayonObs) - dist) * normal + comp1 * v1 + comp2 * v2;// + halfSize1 * normal ;
                    // pos = Point(vector_pos.x,vector_pos.y,vector_pos.z);
                    formlist[formIndex]->getAnim().setPos(vector_pos);
                    pos = formlist[formIndex]->getAnim().getPos();

                    Fn.y = masse * formlist[formIndex]->g;


                    // formlist[formIndex]->getAnim().setSpeed(0);


                    // normal.normalize();
                    // Vector acc = 1./formlist[formIndex]->getAnim().getMasse() * formlist[formIndex]->getFg();
                    // comp_speed_1 = (speed- acc.integral(delta_t)) * normal;
                    comp_speed_1 = (speed) * normal;
                    comp_speed_2 = speedObs * normal;

                    speed = comp_speed_1 * normal + comp_speed_2 * normal - e * (speed * normal) * normal;

                    speed =  speed - formlist[formIndex]->getAnim().getSpeed() ;
                    // formlist[formIndex]->getAnim().setSpeed(speed);
                    Vector acc = (1./delta_t *speed);
                    Vector Fn = formlist[formIndex]->getFn() + acc*formlist[formIndex]->getAnim().getMasse();
                    //formlist[formIndex]->setFn(Fn);

                    angleDeChute(rot, rotObs, pos, rayon);
                    nbObstacle++;
                }
            }
            break;
            case SPHERE:
            {
                static Vector halfSize1, halfSize2;
                halfSize1 = Vector{rayon, rayon, rayon};
                halfSize2 = Vector{rayonObs, rayonObs, rayonObs};

                auto vertices1 = getVertices(vector_pos, halfSize1, rot);
                auto vertices2 = getVertices(vector_posObs, halfSize2, rotObs);

                auto axes1 = getAxes(rot);
                auto axes2 = getAxes(rotObs);

                std::vector<Vector> axes;
                axes.insert(axes.end(), axes1.begin(), axes1.end());
                axes.insert(axes.end(), axes2.begin(), axes2.end());

                for (const auto& axis1 : axes1) {
                    for (const auto& axis2 : axes2) {
                        axes.push_back(Vector{
                            axis1.y * axis2.z - axis1.z * axis2.y,
                            axis1.z * axis2.x - axis1.x * axis2.z,
                            axis1.x * axis2.y - axis1.y * axis2.x
                        });
                    }
                }

                if (SATCollision(vertices1, vertices2, axes)) {


                    formlist[formIndex]->getAnim().setColor(ORANGE);

                    static Vector normal, v1, v2;
                    static reel dist, comp1, comp2, comp_speed_1, comp_speed_2;
                    // printf("Collision\n");
                    normal = vector_pos - vector_posObs;

                    // Vector correction = (halfSize1 + halfSize2) - (vector_pos - vector_posObs);
                    // pos = (vector_pos - correction);

                    v1 = Vector{1, 0, 0}; // Axe X
                    v2 = Vector{0, 1, 0}; // Axe Y
                    rotateVector(v1, rotObs);
                    rotateVector(v2, rotObs);

                    dist = distance(pos, posObs);
                    comp1 = vector_pos * v1;
                    comp2 = vector_pos * v2;
                    vector_pos = ((rayon + rayonObs) - dist) * normal + comp1 * v1 + comp2 * v2;// + halfSize1 * normal ;

                    if(formlist[i]->getCollisionEffect())
                    {
                        formlist[formIndex]->getAnim().setPos(vector_pos );
                        pos = formlist[formIndex]->getAnim().getPos();

                        Fn.y = masse * formlist[formIndex]->g;
                        // formlist[formIndex]->getAnim().setSpeed(0);
                        comp_speed_1 = 10*speed * normal;
                        comp_speed_2 = 10*speedObs * normal;

                        speed = comp_speed_1 * normal + comp_speed_2 * normal - e * (speed * normal) * normal;
                        formlist[formIndex]->getAnim().setSpeed(speed);
                        angleDeChute(rot, rotObs, pos, rayon);
                    }

                    sphereDetected=true;
                    nbObstacle++;
                }
            }
            break;
            default:
                break;
            }
        }


        i++;
    }
    if (!nbObstacle)
    {
        Fn = 0;
        // printf("No obstacle found\n");
    }

}

void Scene::update(reel delta_t){
    static int i = 0;
    // Mettre à jour la liste de formes
    // while (formlist[i] != NULL) {
    // {
    //     // Brique* newBrique = new Brique(*brique); // Créez un nouvel objet brique à chaque itération
    //     tempFormlist[i] = new Form(*formlist[i]);
    //     i++;
    // }

    i = 0;
    if(_pause)
    {
        //printf("on est en pause: %d\n",inpu);
        return;
    }
    while (formlist[i] != NULL) {
        if (formlist[i]->getPhysics())// Alors concerné par la physique
        {
            static Point pos, rot; static Vector Fn;
            formlist[i]->update(delta_t/timeSlow);///timeSlow);


            switch (formlist[i]->getAnim().getTypeForm()) {
                case BRIQUE:
                {
                    // pos = formlist[i]->getAnim().getPos();
                    // rot = formlist[i]->getAnim().getRotation();
                    // Fn = formlist[i]->getFn();

                    // Vérifiez la position de la brique par rapport au sol

                    checkCollision(i, pos, rot, Fn, delta_t);

                    formlist[i]->getAnim().setPos(pos);
                    formlist[i]->getAnim().setRotation(rot);
                    formlist[i]->setFn(Fn);
                }break;
                case PLAN:{

                }break;
                case SPHERE:{
                    checkCollision(i,pos,rot,Fn, delta_t);

                }break;
                default:
                    break;
            }
            reel dist = distance(Point(0,0,0),formlist[i]->getAnim().getPos());
            if(dist>KILL_DISTANCE)// || formlist[i]->getAnim().getPos().y>10)
            {
                formlist[i]->setPhysics(false);
                formlist[i]->modelSTL.triangleSTL = std::vector<Triangle>();

            }
            // std::cout << "Coordonnes apres de pos: " << formlist[i]->getAnim().getPos() << std::endl;
        }
        i++;
    }
}

void Scene::render()
{
    // Clear color buffer and Z-Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Set the camera position and parameters
    gluLookAt(camera_position.x,camera_position.y,camera_position.z, 0.0,0.0,0.0, 0.0,1.0,0.0);
    // Isometric view
    glRotated(-45, 0, 1, 0);
    glRotated(30, 1, 0, -1);

    // X, Y and Z axis

    // glPushMatrix(); // Preserve the camera viewing point for further forms
    // // Render the coordinates system
    // glBegin(GL_LINES);
    // {
    //     glColor3f(1.0f, 0.0f, 0.0f);
    //     glVertex3i(0, 0, 0);
    //     glVertex3i(1, 0, 0);
    //     glColor3f(0.0f, 1.0f, 0.0f);
    //     glVertex3i(0, 0, 0);
    //     glVertex3i(0, 1, 0);
    //     glColor3f(0.0f, 0.0f, 1.0f);
    //     glVertex3i(0, 0, 0);
    //     glVertex3i(0, 0, 1);
    // }
    // glEnd();
    // glPopMatrix(); // Restore the camera viewing point for next object

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



void Scene::run()
{
    previous_time = SDL_GetTicks();
    while(!gameOver())
    {
        // printf("Running");
        checkInput();
    // Update the scene

        current_time = SDL_GetTicks(); // get the elapsed time from SDL initialization (ms)
        elapsed_time = current_time - previous_time;
        if (elapsed_time > ANIM_DELAY)
        {
            previous_time = current_time;
            update(1e-3 *elapsed_time); // International system units : seconds
        }

         // Render the scene
        render();
        // Update window screen
        SDL_GL_SwapWindow(gWindow);

    }
    close(&gWindow);
}

void Scene::close(SDL_Window** window)
{
    //Destroy window
    SDL_DestroyWindow(*window);
    *window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

bool Scene::addForm(Form *form)
{
    formlist[formIndex] = form;
    formIndex = (formIndex + 1)%MAX_FORMS_NUMBER;

    return true;
}

char Scene::checkInput()
{
    static int nbSpaces = 0;
// Handle events on queue
    while(SDL_PollEvent(& event) != 0)
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
                    case SDLK_z://zoom
                    {

                    
                        camera_position.z -= 1;

                        int i=0;
                        while (formlist[i]!=NULL) 
                        {
                            // Brique* newBrique = new Brique(*brique); // Créez un nouvel objet brique à chaque itération
                            if(formlist[i]->getAnim().getTypeForm() == SPHERE)
                            {
                                bool state =  formlist[i]->getCollisionEffect();
                                formlist[i]->setCollisionEffect(!state);
                            }
                            i++;
                        }
                        break;
                    }
                    case SDLK_e:
                        camera_position.z += 1;
                    break;
                    case SDLK_r://restart
                        input.reset = true;
                        init();
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


                    case SDLK_SPACE:
                        input.space_down = true;
                        if(nbSpaces == 0 || nbSpaces == 2)
                        {
                            nbSpaces++;
                        }


                        printf("pause down\n");
                        break;

                    default:
                        break;
                    }

                    break;

                case SDL_KEYUP:
                    switch (key_pressed)
                    {
                    case SDLK_SPACE:
                        input.space_up = true;
                        nbSpaces ++;


                        printf("pause up\n");

                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }

    }



    if(nbSpaces>=4){
        _pause = false;
        nbSpaces = 0;
    }else if (nbSpaces>=1)
    {
        _pause = true;
    }




    if(input.decreaseRope)
    {
        //Diminue la force de la corde.
    }
    if(input.increaseRope)
    {
        //Augmente la force de la corde.
    }
    if(input.launch)
    {
        //Fais le lancement de la boule.
    }
    if(input.reset)
    {
        //reset le jeu.

    }
    if(input.quit)
    {
        //quitte le jeu
    }
    if(input.rotateLeft)
    {
        //tourne la catapulte vers la gauche.
    }
    if(input.rotateRight)
    {
        //tourne la catapulte vers la droite.
    }
}






bool removeForm(int index)
{


}

bool removeForm(Form* form);

bool Scene::popForm()
{
    delete(formlist[formIndex]);
    formlist[formIndex] = NULL;
    if(formIndex>0)
    {
        formIndex--;
    }
}





bool Scene::_initWindow(SDL_Window** window, SDL_GLContext* context)
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
                if( !_initGL() )
                {
                    std::cerr << "Unable to initialize OpenGL!"  << std::endl;
                    success = false;
                }
            }
        }
    }

    return success;
}

bool Scene::_initGL()
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
