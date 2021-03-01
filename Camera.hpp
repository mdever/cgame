#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Camera.hpp"

#include <glm/glm.hpp>

class Camera {
private:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;

public:
	glm::mat4 viewMatrix;

public:
	Camera();
	~Camera();
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up);

	Camera& setPosition(float x, float y, float z);
	Camera& setUp(float x, float y, float z);
	Camera& setTarget(float x, float y, float z);

	Camera& move(glm::vec3 delta);

	Camera& moveRadiallyUp(float degrees);
	Camera& moveRadiallyRight(float degrees);
	Camera& moveForward(float distance);

	Camera& pedestalUp(float distance);
	Camera& truckRight(float distance);

	Camera& panUp(float degrees);
	Camera& panRight(float degrees);

};


#endif // CAMERA_HPP