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
	spawnInterval(0),
	axisSpawnInterval(0),
	planeTimer(4.0f),	//how often to spawn an allies' or USSR plane
	axisPlaneTimer(3.5f)//how often to spawn an axis' plane
{

}

Aircraft::~Aircraft()
{
	for (auto& plane : m_alliesPlanes)
	{
		delete plane;
	}
	m_alliesPlanes.clear();

	for (auto& axisPlane : m_axisPlanes)
	{
		delete axisPlane;
	}
	m_axisPlanes.clear();
}

bool Aircraft::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;

	//allies' & USSR planes:
	spawnInterval = planeTimer;

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
	"Sprites\\Planes\\allies\\USSR_Lagg3.png" };


	//axies' planes:
	axisSpawnInterval = axisPlaneTimer;

	axisPlaneFiles = {
		"Sprites\\Planes\\axis\\GER_bf109.png",
		"Sprites\\Planes\\axis\\GER_bf110.png",
		"Sprites\\Planes\\axis\\GER_FW190.png",
		"Sprites\\Planes\\axis\\JAP_a6m.png",
		"Sprites\\Planes\\axis\\JAP_Ki21.png",
		"Sprites\\Planes\\axis\\JAP_Ki45.png",
		"Sprites\\Planes\\axis\\JAP_Ki51.png",
		"Sprites\\Planes\\axis\\JAP_Ki61.png"
	};

	return true;
}

void Aircraft::Process(float deltaTime)
{
	planeTimer -= deltaTime;
	axisPlaneTimer -= deltaTime;

	if (planeTimer <= 0.0f)
	{
		SpawnPlane();
		planeTimer = spawnInterval;
	}

	if (axisPlaneTimer <= 0.0f)
	{
		SpawnAxisPlane();
		axisPlaneTimer = axisSpawnInterval;
	}

	for (auto it = m_alliesPlanes.begin(); it != m_alliesPlanes.end();)
	{
		Entity* plane = *it;
		plane->Process(deltaTime);

		plane->m_pSprite->SetX(static_cast<int>(plane->GetPosition().x));
		plane->m_pSprite->SetY(static_cast<int>(plane->GetPosition().y));

		if (plane->GetPosition().y <= -plane->m_pSprite->GetHeight())
		{
			delete plane;
			it = m_alliesPlanes.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = m_axisPlanes.begin(); it != m_axisPlanes.end();)
	{
		Entity* axisPlane = *it;
		axisPlane->Process(deltaTime);

		axisPlane->m_pSprite->SetX(static_cast<int>(axisPlane->GetPosition().x));
		axisPlane->m_pSprite->SetY(static_cast<int>(axisPlane->GetPosition().y));

		if (axisPlane->GetPosition().y >= m_pRenderer->GetHeight() + axisPlane->m_pSprite->GetHeight())
		{
			delete axisPlane;
			it = m_axisPlanes.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Aircraft::Draw(Renderer& renderer)
{
	for (auto& plane : m_alliesPlanes)
	{
		plane->Draw(renderer);
	}

	for (auto& axisPlane : m_axisPlanes)
	{
		axisPlane->Draw(renderer);
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
	float startX = static_cast<float>(rand() % (screenWidth / 2) + (screenWidth / 8));
	float startY = static_cast<float>(screenHeight + newPlane->m_pSprite->GetHeight());

	newPlane->GetPosition().x = startX;
	newPlane->GetPosition().y = startY;

	//destination position
	float endX = static_cast<float>(rand() % (screenWidth / 2) + (screenWidth / 8));
	float endY = -newPlane->m_pSprite->GetHeight();

	Vector2 direction = Vector2(endX, endY) - Vector2(startX, startY);
	direction.Normalise();

	//set the speed
	float speed = static_cast<float>(rand() % 30 + 130);
	newPlane->GetVelocity() = direction * speed;

	m_alliesPlanes.push_back(newPlane);
}

void Aircraft::SpawnAxisPlane()
{
	int screenWidth = m_pRenderer->GetWidth();
	int screenHeight = m_pRenderer->GetHeight();

	int axisPlaneTypeIndex = rand() % axisPlaneFiles.size();

	Entity* axisPlane = new Entity();
	axisPlane->m_pSprite = m_pRenderer->CreateSprite(axisPlaneFiles[axisPlaneTypeIndex].c_str());
	axisPlane->m_pSprite->SetScale(-1.0f);

	float startX = static_cast<float>(rand() % (screenWidth / 2) + (screenWidth / 8));
	float startY = -axisPlane->m_pSprite->GetHeight();

	axisPlane->GetPosition().x = startX;
	axisPlane->GetPosition().y = startY;

	float endX = static_cast<float>(rand() % (screenWidth / 2) + (screenWidth / 8));
	float endY = static_cast<float>(screenHeight + axisPlane->m_pSprite->GetHeight());

	Vector2 direction = Vector2(endX, endY) - Vector2(startX, startY);
	direction.Normalise();

	float speed = static_cast<float>(rand() % 30 + 130);
	axisPlane->GetVelocity() = direction * speed;

	m_axisPlanes.push_back(axisPlane);
}
