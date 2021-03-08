#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/GL.h>
#include <memory>

#include "Scene.hpp"

Scene::Scene() {}

Scene::Scene(Camera& camera)
{
}

Scene::Scene(Camera&& camera)
{
}

void Scene::addDrawable(std::shared_ptr<Drawable> drawable)
{
	this->drawables.push_back(drawable);
}

void Scene::removeDrawable(std::shared_ptr<Drawable> drawable)
{
	this->drawables.remove(drawable);
}

void Scene::setCamera(std::shared_ptr<Camera> _camera)
{
	this->camera = _camera;
}

void Scene::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	for (auto const& drawable : this->drawables)
	{
		drawable->draw();
	}
}
