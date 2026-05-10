#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class CameraF
{
public:
	CameraF();
	// El constructor recibe la posición de la cámara y el punto exacto al que debe mirar (target)
	CameraF(glm::vec3 startPosition, glm::vec3 lookAtTarget, glm::vec3 upVector);

	// No incluimos keyControl ni mouseControl para asegurar que sea fija

	glm::vec3 getCameraPosition() { return position; }
	glm::vec3 getCameraDirection() { return front; }
	glm::mat4 calculateViewMatrix();

	~CameraF();

private:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
	glm::vec3 front;
};