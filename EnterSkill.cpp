//local includes:
#include "EnterSkill.h"

//library includes:
#include "renderer.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
#include "vector2.h"


EnterSkill::EnterSkill() :
	m_pRenderer(nullptr),
	spawnInterval(0.0f),
	m_currentBoomerIndex(0),
	planeTimer(0.1f),
	m_isActive(false)
{

}

EnterSkill::~EnterSkill()
{
	for (auto& boomer : m_Boomers)
	{
		delete boomer;
	}
	m_Boomers.clear();
}

bool EnterSkill::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;

	planeFiles = {
	"Sprites\\Planes\\Boomers\\UK_Blenheim.png",
	"Sprites\\Planes\\Boomers\\UK_Lancaster.png",
	"Sprites\\Planes\\Boomers\\UK_Veligton.png",
	"Sprites\\Planes\\Boomers\\US_a26.png",
	"Sprites\\Planes\\Boomers\\US_b17.png",
	"Sprites\\Planes\\Boomers\\USSR_Pe3.png",
	"Sprites\\Planes\\Boomers\\USSR_Tu2.png", };

	return true;
}

void EnterSkill::Process(float deltaTime)
{
	if (!m_isActive)	return;

	spawnInterval -= deltaTime;

	if (spawnInterval <= 0.0f && m_currentBoomerIndex < planeFiles.size())
	{
		SpawnBoomer(m_currentBoomerIndex);
		m_currentBoomerIndex++;
		spawnInterval = planeTimer;
	}

	for (auto it = m_Boomers.begin(); it != m_Boomers.end();)
	{
		Entity* boomer = *it;
		boomer->Process(deltaTime);

		boomer->m_pSprite->SetX(static_cast<int>(boomer->GetPosition().x));
		boomer->m_pSprite->SetY(static_cast<int>(boomer->GetPosition().y));

		if (boomer->GetPosition().y <= -boomer->m_pSprite->GetHeight())
		{
			delete boomer;
			it = m_Boomers.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (m_currentBoomerIndex >= planeFiles.size() && m_Boomers.empty())
	{
		m_isActive = false;
		return;
	}
}

void EnterSkill::Draw(Renderer& renderer)
{
	if (!m_isActive)	return;

	for (auto& boomer : m_Boomers)
	{
		boomer->Draw(renderer);
	}
}

void EnterSkill::SpawnBoomer(int index)
{
	if (index >= planeFiles.size())	return;

	int screenWidth = m_pRenderer->GetWidth();
	int screenHeight = m_pRenderer->GetHeight();

	float offsetX = 150.0f; //distance of each plane

	std::string planeFile = planeFiles[index];

	if (index == 0)
	{
		Entity* boomer = new Entity();
		boomer->m_pSprite = m_pRenderer->CreateSprite(planeFile.c_str());
		boomer->m_pSprite->SetScale(1.5f);

		float startX = static_cast<float>(screenWidth / 2);
		float startY = static_cast<float>(screenHeight + boomer->m_pSprite->GetHeight());

		boomer->GetPosition().x = startX;
		boomer->GetPosition().y = startY;

		float endX = startX;
		float endY = -boomer->m_pSprite->GetHeight();

		Vector2 direction = Vector2(endX, endY) - Vector2(startX, startY);
		direction.Normalise();

		float speed = 220.0f;
		boomer->GetVelocity() = direction * speed;

		m_Boomers.push_back(boomer);
	}
	else
	{
		for (int side = -1; side <= 1; side += 2) // side = -1 (left side), side = 1 (right side)
		{
			Entity* boomer = new Entity();
			boomer->m_pSprite = m_pRenderer->CreateSprite(planeFile.c_str());
			boomer->m_pSprite->SetScale(1.5f);

			float startX = static_cast<float>(screenWidth / 2 + side * index * offsetX);
			float startY = static_cast<float>(screenHeight + boomer->m_pSprite->GetHeight());

			boomer->GetPosition().x = startX;
			boomer->GetPosition().y = startY;

			float endX = startX;
			float endY = -boomer->m_pSprite->GetHeight();

			Vector2 direction = Vector2(endX, endY) - Vector2(startX, startY);
			direction.Normalise();

			float speed = 220.0f;
			boomer->GetVelocity() = direction * speed;

			m_Boomers.push_back(boomer);
		}
	}
}

void EnterSkill::Activate()
{
	m_isActive = true;
	m_currentBoomerIndex = 0;
	spawnInterval = planeTimer;
}