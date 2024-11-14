//This include:
#include "scene.h"

//local include:
#include "renderer.h"

//Library includes:
#include <cassert>

Scene::Scene()
{

}

Scene::~Scene()
{

}

bool Scene::Initialise(Renderer& renderer)
{
	return true;
}

void Scene::OnSceneChange(int* sceneIndex)
{
	m_sceneIndex = sceneIndex;
}