//local includes:
#include "Clouds.h"

//library includes:
#include "renderer.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>


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
	int screenWidth = renderer.GetWidth();
	int screenHeight = renderer.GetHeight();

	for (int i = 0; i < 4; ++i)
	{
		Entity* newCloud = new Entity();
		std::string cloudFilename = "Sprites\\Environment\\cloud" + std::to_string(i + 1) + ".png";
		newCloud->m_pSprite = renderer.CreateSprite(cloudFilename.c_str());

		//random scale
		float scale = static_cast<float>(rand() % 50 + 50) / 100.0f;
		newCloud->m_pSprite->SetScale(scale);

		//random position
		int posX = std::rand() % screenWidth;
		int posY = std::rand() % screenHeight;

		newCloud->m_pSprite->SetX(posX);
		newCloud->m_pSprite->SetY(posY);

		newCloud->GetPosition().x = posX;
		newCloud->GetPosition().y = posY;

		newCloud->m_pSprite->SetX(static_cast<int>(posX));
		newCloud->m_pSprite->SetY(static_cast<int>(posY));

		//set the speed & direction (only move on Y axis)
		float speed = static_cast<float>(rand() % 30 + 10);
		float velocityX = (rand() % 2 == 0 ? -1.0f : 1.0f) * speed;

		newCloud->GetVelocity().x = velocityX;
		newCloud->GetVelocity().y = 0.0f;

		m_clouds.push_back(newCloud);
	}

	return true;
}

void Clouds::Process(float deltaTime)
{
	int screenWidth = m_pRenderer->GetWidth();

	for (auto& cloud : m_clouds)
	{
		cloud->Process(deltaTime);

		//check if moving out of the screen
		if (cloud->GetPosition().x < -cloud->m_pSprite->GetWidth() * cloud->m_pSprite->GetScale())
		{
			cloud->GetPosition().x = static_cast<float>(screenWidth);
		}
		else if (cloud->GetPosition().x > screenWidth)
		{
			cloud->GetPosition().x = -cloud->m_pSprite->GetWidth() * cloud->m_pSprite->GetScale();
		}

		cloud->m_pSprite->SetX(static_cast<int>(cloud->GetPosition().x));
		cloud->m_pSprite->SetY(static_cast<int>(cloud->GetPosition().y));
	}
}

void Clouds::Draw(Renderer& renderer)
{
	for (auto& cloud : m_clouds)
	{
		cloud->Draw(renderer);
	}
}