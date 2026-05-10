#include "CameraA.h"

CameraA::CameraA() {}

CameraA::CameraA(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startMoveSpeed)
{
    // Forzamos la altura a 50 unidades
    position = glm::vec3(startPosition.x, 50.0f, startPosition.z);
    worldUp = startUp;

    // Apuntar constantemente hacia abajo (Eje -Y)
    front = glm::vec3(0.0f, -1.0f, 0.0f);
    moveSpeed = startMoveSpeed;

    update();
}

void CameraA::keyControl(bool* keys, GLfloat deltaTime)
{
    GLfloat velocity = moveSpeed * deltaTime;

    // En una vista cenital, W/S mueven en Z y A/D mueven en X
    if (keys[GLFW_KEY_W])
    {
        position.z -= velocity;
    }
    if (keys[GLFW_KEY_S])
    {
        position.z += velocity;
    }
    if (keys[GLFW_KEY_A])
    {
        position.x -= velocity;
    }
    if (keys[GLFW_KEY_D])
    {
        position.x += velocity;
    }

    // Aseguramos que la altura se mantenga en 50 pase lo que pase
    position.y = 50.0f;
}

void CameraA::update()
{
    // Para una cámara que mira hacia abajo (0,-1,0), el vector "arriba" 
    // debe ser el eje Z negativo para que la vista no se bloquee.
    right = glm::vec3(1.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::mat4 CameraA::calculateViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

CameraA::~CameraA() {}