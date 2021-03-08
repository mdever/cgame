#include "Camera.hpp"
#include "Utils.hpp"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

static const float PI = 3.14159265358979323846;

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

Camera& Camera::setPosition(glm::vec3 pos)
{
	this->position = pos;
	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);
	return *this;
}

Camera& Camera::setUp(float x, float y, float z)
{
	this->up = glm::vec3(x, y, z);
	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);
	return *this;
}

Camera& Camera::setUp(glm::vec3 up)
{
	this->up = up;
	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);
	return *this;
}

Camera& Camera::setTarget(float x, float y, float z)
{
	this->target = glm::vec3(x, y, z);
	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);
	return *this;
}

Camera& Camera::setTarget(glm::vec3 target)
{
	this->target = target;
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
	if (std::abs(degrees) < 0.01)
		return *this;

	if (std::abs(degrees) > 10.f) {
		if (degrees > 0) {
			degrees = 10.0f;
		}
		else {
			degrees = -10.0f;
		}
	}

	float r = magnitude(this->target - this->position);
	glm::vec3 forward = normalize(this->target - this->position);
	glm::vec3 up = normalize(this->up);
	glm::vec3 right = glm::cross(forward, up);
	float xFactor;
	float yFactor;
	float theta = degrees * PI / 180.0f;
	float theta2 = (theta / 2.0f) * (1.0f - std::atan((1.0f - std::cos(theta / 2.0f)) / (std::sin(theta / 2.0f))));
	float theta3 = std::atan((1 - std::cos(theta / 2)) / (std::sin(theta / 2)));
	float deltaX = 2 * r * (1 - std::cos(theta / 2.0f)) * std::cos(theta2);
	float deltaY = 2 * r * std::sin(theta / 2.0f) * std::cos(theta3);

	this->position += up * deltaY;
	this->position += forward * deltaX;
	
	glm::vec3 newForward = normalize(this->target - this->position);

	this->up = glm::cross(right, newForward);

	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);

	float newR = magnitude(this->target - this->position);
	wchar_t msg[256];
	swprintf(msg, 256, L"The new radius should be the same: Old Radius: %f, New Radius, %f\n", r, newR);
	WriteToConsole(msg);
	swprintf(msg, 256, L"Difference in radii is %f\n", std::abs(r - newR));
	WriteToConsole(msg);
	return *this;
}

Camera& Camera::moveRadiallyRight(float degrees)
{
	if (std::abs(degrees) < 0.01)
		return *this;

	if (std::abs(degrees) > 10.f) {
		if (degrees > 0) {
			degrees = 10.0f;
		}
		else {
			degrees = -10.0f;
		}
	}

	float r = magnitude(this->target - this->position);
	glm::vec3 forward = normalize(this->target - this->position);
	glm::vec3 up = normalize(this->up);
	glm::vec3 right = glm::cross(forward, up);
	float xFactor;
	float yFactor;
	float theta = degrees * PI / 180.0f;
	float theta2 = (theta / 2.0f) * (1.0f - std::atan((1.0f - std::cos(theta / 2.0f)) / (std::sin(theta / 2.0f))));
	float theta3 = std::atan((1 - std::cos(theta / 2)) / (std::sin(theta / 2)));
	float deltaX = 2 * r * (1 - std::cos(theta / 2.0f)) * std::cos(theta2);
	float deltaY = 2 * r * std::sin(theta / 2.0f) * std::cos(theta3);

	this->position += right * deltaY;
	this->position += forward * deltaX;
	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);

	float newR = magnitude(this->target - this->position);
	wchar_t msg[256];
	swprintf(msg, 256, L"The new radius should be the same: Old Radius: %f, New Radius, %f\n", r, newR);
	WriteToConsole(msg);
	swprintf(msg, 256, L"Difference in radii is %f\n", std::abs(r - newR));
	WriteToConsole(msg);
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

/* Does not work, needs to move up and maintain camera to target line */
Camera& Camera::pedestalUp(float distance)
{
	glm::vec3 upNormalized = this->up / magnitude(this->up);
	this->position += upNormalized * distance;
	this->target += upNormalized * distance;
	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);
	return *this;
}

/* Does not work, needs to move up and maintain camera to target line */
Camera& Camera::truckRight(float distance)
{
	glm::vec3 cameraDirection = this->target - this->position;
	glm::vec3 right = glm::cross(cameraDirection, this->up);
	right = right / magnitude(right);
	this->position += -right * distance;
	this->target += -right * distance;
	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);
	return *this;
}

Camera& Camera::panUp(float degrees)
{
	float theta = glm::radians(degrees);
	glm::vec3 upNormalized = normalize(this->up);
	glm::vec3 toTarget = this->target - this->position;
	glm::vec3 toTargetNormalized = normalize(toTarget);
	float deltaUp = magnitude(toTarget) * std::sin(theta);
	float deltaBack = deltaUp / std::tan((PI - theta) / 2);
	this->target -= deltaUp * upNormalized;
	this->target += deltaBack * toTargetNormalized;
	this->up = glm::rotate(this->up, theta, glm::cross(toTargetNormalized, upNormalized));
	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);
	return *this;
}

Camera& Camera::panRight(float degrees)
{
	float theta = glm::radians(degrees);
	glm::vec3 upNormalized = normalize(this->up);
	glm::vec3 toTarget = this->target - this->position;
	glm::vec3 toTargetNormalized = normalize(toTarget);
	glm::vec3 rightNormalized = normalize(glm::cross(toTargetNormalized, upNormalized));
	float deltaRight = magnitude(toTarget) * std::sin(theta);
	float deltaBack = deltaRight / std::tan((PI - theta) / 2);
	this->target += deltaRight * rightNormalized;
	this->target += deltaBack * toTargetNormalized;
	this->viewMatrix = glm::lookAt(this->position, this->target, this->up);
	return *this;
	return *this;
}
