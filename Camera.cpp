#include "Camera.hpp"
#include "Utils.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
	this->position = glm::vec3(0.0f, 0.0f, 1.0f);
	this->target = glm::vec3(0.0f, 0.0f, 0.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);

	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);
}

Camera::~Camera()
{
}

Camera::Camera(glm::vec3 thePosition, glm::vec3 theTarget, glm::vec3 theUp)
	: position(thePosition), target(theTarget), up(theUp)
{
	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);
}

Camera& Camera::setPosition(float x, float y, float z)
{
	this->position = glm::vec3(x, y, z);
	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);
	return *this;
}

Camera& Camera::setUp(float x, float y, float z)
{
	this->up = glm::vec3(x, y, z);
	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);
	return *this;
}

Camera& Camera::setTarget(float x, float y, float z)
{
	this->target = glm::vec3(x, y, z);
	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);
	return *this;
}

Camera& Camera::move(glm::vec3 delta)
{
	this->position = this->position + delta;
	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);
	return *this;
}

Camera& Camera::moveRadiallyUp(float degrees)
{
	// TODO: insert return statement here
	return *this;
}

Camera& Camera::moveRadiallyRight(float degrees)
{
	// TODO: insert return statement here
	return *this;
}

Camera& Camera::moveForward(float distance)
{
	glm::vec3 forwardDirection = this->target - this->position;
	forwardDirection = forwardDirection / magnitude(forwardDirection);
	this->position += forwardDirection * distance;
	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);
	return *this;
}

Camera& Camera::pedestalUp(float distance)
{
	// TODO: insert return statement here
	return *this;
}

Camera& Camera::truckRight(float distance)
{
	// TODO: insert return statement here
	return *this;
}

Camera& Camera::panUp(float degrees)
{
	// TODO: insert return statement here
	return *this;
}

Camera& Camera::panRight(float degrees)
{
	// TODO: insert return statement here
	return *this;
}