// Local includes:
#include "TankGame.h"
#include "inputsystem.h"
#include "renderer.h"
#include "scene.h"
#include "logmanager.h"
#include "imgui/imgui.h"
#include "soundsystem.h"
#include "game.h"
#include "animatedsprite.h"
#include "texture.h"
#include "Enemy.h"
#include "Entity.h"

// Library includes:
#include <vector>
#include <cmath>
#include "fmod.hpp"
#include "fmod_errors.h"


using FMOD::System;
using FMOD::Sound;
using FMOD::Channel;

SceneTankGame::SceneTankGame()
	: m_pRenderer(nullptr),
	hitsound1(nullptr),
	hitsound2(nullptr),
	channel(nullptr),
	opening(nullptr),
	pAnimatedSprite(nullptr),
	m_pPlayer(nullptr),
	m_pEnemy(nullptr)
{
}

SceneTankGame::~SceneTankGame()
{

	if (hitsound1)
	{
		hitsound1->release();
		hitsound1 = nullptr;
	}
	if (hitsound2)
	{
		hitsound2->release();
		hitsound2 = nullptr;
	}
	if (opening)
	{
		opening->release();
		opening = nullptr;
	}
	for (Enemy* enemy : m_enemies)
	{
		delete enemy;
	}
	m_enemies.clear();
	delete pAnimatedSprite;
	delete m_pPlayer;
	delete m_pEnemy;
}

bool SceneTankGame::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;

	// Initialize player tank
	m_pPlayer = new Entity();
	m_pPlayer->Initialise(renderer);
	m_pPlayer->Position().x = renderer.GetWidth() / 2.0f;
	m_pPlayer->Position().y = renderer.GetHeight() / 2.0f;

	// Spawn enemies
	m_pEnemy = new Enemy();
	m_pEnemy->SpawnEnemies(15);

	//initialise the sound:
	FMOD_RESULT result = Game::pSoundsystem->createSound("sounds\\hit1.wav", FMOD_DEFAULT, &hitsound1);
	if (result != FMOD_OK || hitsound1 == nullptr) {
		printf("Failed to load hit.wav: %s\n", FMOD_ErrorString(result));
	}
	else {
		printf("Successfully loaded hit.wav\n");
	}

	result = Game::pSoundsystem->createSound("sounds\\hit2.mp3", FMOD_DEFAULT, &hitsound2);
	if (result != FMOD_OK || hitsound2 == nullptr) {
		printf("Failed to load hit2.mp3: %s\n", FMOD_ErrorString(result));
	}
	else {
		printf("Successfully loaded hit2.mp3\n");
	}
	Game::pSoundsystem->createSound("sounds\\opening.wav", FMOD_LOOP_NORMAL, &opening);
	Game::pSoundsystem->playSound(opening, nullptr, false, &channel);

	//animated sprite:
	Texture* pTexture = new Texture();
	pTexture->Initialise("Sprites\\explosion.png");

	pAnimatedSprite = new AnimatedSprite();
	pAnimatedSprite->Initialise(*pTexture);
	pAnimatedSprite->SetupFrames(66, 66);
	pAnimatedSprite->SetFrameDuration(0.2f);
	pAnimatedSprite->SetLooping(false);

	return true;
}

void SceneTankGame::Process(float deltaTime, InputSystem& inputSystem)
{
	m_pPlayer->Process(deltaTime);
	for (int i = 0; i < 15; ++i)
	{
		m_pEnemy->m_position = Vector2(rand() % 1810, rand() % 1000); // within 1860x1050 screen resolution
		m_enemies.push_back(m_pEnemy);
	}
	CheckCollisions();

	pAnimatedSprite->Process(deltaTime);
}

void SceneTankGame::CheckCollisions()
{
	
}


void SceneTankGame::Draw(Renderer& renderer)
{
	if (m_pPlayer && m_pPlayer->IsAlive())
	{
		m_pPlayer->Draw(renderer);
	}

	for (auto& enemies : m_enemies)
	{
		if (enemies->IsAlive())
		{
			enemies->Draw(renderer);
		}
	}

	for (auto& enemies : m_enemies)
	{
		if (enemies->IsAlive())
		{
			enemies->Draw(renderer);
		}
	}

	pAnimatedSprite->Draw(renderer);
}

void SceneTankGame::DebugDraw()
{
	if (m_pPlayer && m_pPlayer->IsAlive())
	{
		ImGui::Text("Player Tank:");
		m_pPlayer->DebugDraw();
	}

	ImGui::Text("Enemies:");
	for (auto& enemies : m_enemies)
	{
		if (enemies->IsAlive())
		{
			enemies->DebugDraw();
		}
	}

	pAnimatedSprite->DebugDraw();
}
