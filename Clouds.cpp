//local includes:
#include "Clouds.h"


//library includes:
#include "renderer.h"
#include <cstdlib>
#include <cmath>
#include <iostream>


Clouds::Clouds():
	m_pRenderer(nullptr)
{

}

Clouds::~Clouds()
{
	for (auto& cloud : m_clouds)
	{
		delete cloud;
	}
	m_clouds.clear();
}

bool Clouds::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;

	Entity* newCloud1 = new Entity();
	newCloud1->m_pSprite = renderer.CreateSprite("Sprites\\Environment\\cloud1.png");
	m_clouds.push_back(newCloud1);

	Entity* newCloud2 = new Entity();
	newCloud2->m_pSprite = renderer.CreateSprite("Sprites\\Environment\\cloud2.png");
	m_clouds.push_back(newCloud2);

	Entity* newCloud3 = new Entity();
	newCloud3->m_pSprite = renderer.CreateSprite("Sprites\\Environment\\cloud3.png");
	m_clouds.push_back(newCloud3);

	Entity* newCloud4 = new Entity();
	newCloud4->m_pSprite = renderer.CreateSprite("Sprites\\Environment\\cloud4.png");
	m_clouds.push_back(newCloud4);

	return true;
}

void Clouds::Process(float deltaTime)
{
	for (auto& cloud : m_clouds)
	{
		cloud->Process(deltaTime);
	}
}

void Clouds::Draw(Renderer& renderer)
{
	for (auto& cloud : m_clouds)
	{
		cloud->Draw(renderer);
	}
}