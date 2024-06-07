#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H


#include <iostream>

//################################
//Configuration Software

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;
const int KILL_DISTANCE = 50;
// Max number of forms : static allocation
const int MAX_FORMS_NUMBER = 2000;

// Animation actualization delay (in ms) => 100 updates per second
const /*Uint32*/int ANIM_DELAY = 1;

const int PHYSIQS_DELAY =1;


const bool USE_STL = true;


struct sceneInput
{
    bool launch,space_down,increaseRope,decreaseRope,rotateLeft,rotateRight,reset,quit,space_up, check;

};



#endif //GLOBAL_VAR_H
