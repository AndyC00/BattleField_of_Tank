//local includes:
#include "Aircraft.h"

//library includes:
#include "renderer.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
#include "vector2.h"


Aircraft::Aircraft() :
	m_pRenderer(nullptr),
	planeTimer(3.5f)
{

}

Aircraft::~Aircraft()
{
	for (auto& plane : m_planes)
	{
		delete plane;
	}
	m_planes.clear();
}

bool Aircraft::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;

	planeTimer = spawnInterval;

		planeFiles = {
		"Sprites\\Planes\\allies\\UK_Beaufighter.png",
		"Sprites\\Planes\\allies\\UK_Spitfire.png",
		"Sprites\\Planes\\allies\\UK_typhoon.png",
		"Sprites\\Planes\\allies\\US_p38.png",
		"Sprites\\Planes\\allies\\US_p40.png",
		"Sprites\\Planes\\allies\\US_p47.png",
		"Sprites\\Planes\\allies\\US_p51.png",
		"Sprites\\Planes\\allies\\USSR_Il2.png",
		"Sprites\\Planes\\allies\\USSR_La5.png",
		"Sprites\\Planes\\allies\\USSR_Lagg3.png"	};

	for (const auto& filePath : planeFiles) 
	{
		renderer.CreateSprite(filePath.c_str());
	}

	return true;
}

void Aircraft::Process(float deltaTime)
{
	planeTimer -= deltaTime;
	if (planeTimer <= 0.0f)
	{
		SpawnPlane();
		planeTimer = spawnInterval;
	}

	for (auto it = m_activePlanes.begin(); it != m_activePlanes.end();)
	{
		Entity* plane = *it;
		plane->Process(deltaTime);

		plane->m_pSprite->SetX(static_cast<int>(plane->GetPosition().x));
		plane->m_pSprite->SetY(static_cast<int>(plane->GetPosition().y));

		if (plane->GetPosition().y <= -plane->m_pSprite->GetHeight())
		{
			delete plane;
			it = m_activePlanes.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Aircraft::Draw(Renderer& renderer)
{
	for (auto& plane : m_activePlanes)
	{
		plane->Draw(renderer);
	}
}

void Aircraft::SpawnPlane()
{
	int screenWidth = m_pRenderer->GetWidth();
	int screenHeight = m_pRenderer->GetHeight();

	int planeTypeIndex = rand() % planeFiles.size();

	Entity* newPlane = new Entity();
	newPlane->m_pSprite = m_pRenderer->CreateSprite(planeFiles[planeTypeIndex].c_str());
	newPlane->m_pSprite->SetScale(1.0f);

	//start position:
	float startX = static_cast<float>(rand() % (screenWidth/2) + (screenWidth / 4));
	float startY = static_cast<float>(screenHeight + newPlane->m_pSprite->GetHeight());

	newPlane->GetPosition().x = startX;
	newPlane->GetPosition().y = startY;

	//destination position
	float endX = static_cast<float>(rand() % (screenWidth / 2) + (screenWidth / 4));
	float endY = -newPlane->m_pSprite->GetHeight();

	Vector2 direction = Vector2(endX, endY) - Vector2(startX, startY);
	direction.Normalise();

	//set the speed
	float speed = static_cast<float>(rand() % 30 + 80);
	newPlane->GetVelocity() = direction * speed;

	m_activePlanes.push_back(newPlane);
}
