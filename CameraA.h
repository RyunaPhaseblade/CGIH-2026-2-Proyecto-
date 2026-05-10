#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <glfw3.h>

class CameraA
{
public:
    CameraA();
    CameraA(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startMoveSpeed);

    // Movimiento solo en X y Z
    void keyControl(bool* keys, GLfloat deltaTime);

    // El mouse no hará nada en esta cámara
    void mouseControl(GLfloat xChange, GLfloat yChange) {}

    glm::vec3 getCameraPosition() { return position; }
    glm::vec3 getCameraDirection() { return front; }
    glm::mat4 calculateViewMatrix();

    ~CameraA();

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat moveSpeed;

    void update();
};