#include "CameraF.h"

CameraF::CameraF() {}

CameraF::CameraF(glm::vec3 startPosition, glm::vec3 lookAtTarget, glm::vec3 upVector)
{
	position = startPosition;
	target = lookAtTarget;
	up = upVector;

	front = glm::normalize(target - position);
}

glm::mat4 CameraF::calculateViewMatrix()
{
	return glm::lookAt(position, target, up);
}

CameraF::~CameraF() {}