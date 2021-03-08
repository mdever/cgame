#pragma once

#include <list>
#include <memory>

#include "Drawable.hpp"
#include "Camera.hpp"


class Scene
{
private:
	std::list<std::shared_ptr<Drawable>> drawables;
	std::shared_ptr<Camera> camera;

public:
	Scene();
	Scene(Camera& camera);
	Scene(Camera&& camera);

	void addDrawable(std::shared_ptr<Drawable> drawable);
	void removeDrawable(std::shared_ptr<Drawable> drawable);
	void setCamera(std::shared_ptr<Camera> _camera);

	void draw();
};