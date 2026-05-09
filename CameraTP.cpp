#include "CameraTP.h"

CameraTP::CameraTP() {}

CameraTP::CameraTP(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    front = glm::vec3(0.0f, 0.0f, -1.0f);

    moveSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;

    update();
}

void CameraTP::keyControl(bool* keys, GLfloat deltaTime)
{
    GLfloat velocity = moveSpeed * deltaTime;

    // Vector para caminar (plano XZ)
    glm::vec3 walkForward = glm::normalize(glm::vec3(front.x, 0.0f, front.z));

    // W y S: Moverse adelante/atrás en el suelo
    if (keys[GLFW_KEY_W])
    {
        position += walkForward * velocity;
    }
    if (keys[GLFW_KEY_S])
    {
        position -= walkForward * velocity;
    }

    // A y D: Moverse a los lados (strafe)
    if (keys[GLFW_KEY_A])
    {
        position -= right * velocity;
    }
    if (keys[GLFW_KEY_D])
    {
        position += right * velocity;
    }

    // --- NUEVOS CONTROLES VERTICALES ---

}

void CameraTP::mouseControl(GLfloat xChange, GLfloat yChange)
{
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }

    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    update();
}

glm::mat4 CameraTP::calculateViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

glm::vec3 CameraTP::getCameraPosition()
{
    return position;
}


glm::vec3 CameraTP::getCameraDirection()
{
    return glm::normalize(front);
}

void CameraTP::update()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}


CameraTP::~CameraTP()
{}
