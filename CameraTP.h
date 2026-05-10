#pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <glfw3.h>

class CameraTP
{
public:
	CameraTP();
	CameraTP(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();

	void establecerObjetivo(glm::vec3* objetivo, float distancia, float altura) {
		posicionMain  = objetivo;
		distanciaP = distancia;
		alturaP = altura;
		TPV = true;
	}

	~CameraTP();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	glm::vec3* posicionMain;
	float distanciaP;
	float alturaP;
	bool TPV;

	void update();
};

