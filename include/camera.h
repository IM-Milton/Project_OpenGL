#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>


class camera
{
    public:
        camera();
        glm::mat4 getMatrixView() const;

        void MouseLook(int xmouse, int ymouse);
        void MoveForward(float speed);
        void MoveBackward(float speed);
        void MoveLeft(float speed);
        void MoveRight(float speed);


    protected:
        glm::vec3 mEye;
        glm::vec3 mViewDirection;
        glm::vec3 mUpVector;


    private:
};

#endif // CAMERA_H
