#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

camera::camera()
{
    mEye = glm::vec3(0.0f, 0.0f, 0.0f);

    mViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);

    mUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
}


glm::mat4 camera::getMatrixView() const{
    return glm::lookAt(mEye, mViewDirection, mUpVector);
}


void camera::MouseLook(int xmouse, int ymouse){
    std::cout << "mouse :" << xmouse<< " ," << ymouse << std::endl;
};

void camera::MoveForward(float speed){
    mEye.z -= speed;
}

void camera::MoveBackward(float speed){
    mEye.z += speed;
}

void camera::MoveLeft(float speed){
    mEye.x -= speed;
}

void camera::MoveRight(float speed){
    mEye.x += speed;
}
