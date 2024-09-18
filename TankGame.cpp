// Local includes:
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
#include "TankGame.h"
#include "Bullet.h"

// Library includes:
#include <vector>
#include <cmath>
#include "fmod.hpp"
#include "fmod_errors.h"
#include <ctime>
#include "inputsystem.h"

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
	PlayerBullet(nullptr),
	m_pBackground(nullptr)
{
	srand(static_cast<unsigned>(time(0)));
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
	delete pAnimatedSprite;
	delete m_pPlayer;
	for (auto& enemy : m_pEnemies)
	{
		delete enemy;
	}
	m_pEnemies.clear();
	delete PlayerBullet;
	delete m_pBackground;
}

bool SceneTankGame::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;

	//initialise the background pic
	m_pBackground = renderer.CreateSprite("Sprites\\background.png");
	m_pBackground->SetX(1860 / 2);
	m_pBackground->SetY(1060 / 2);

	// Initialize player tank
	m_pPlayer = new Entity();
	m_pPlayer->Initialise(renderer);
	m_pPlayer->GetPosition().x = renderer.GetWidth() / 2.0f;
	m_pPlayer->GetPosition().y = renderer.GetHeight() / 2.0f;
	//initialise player bullet
	PlayerBullet = new Bullet();
	PlayerBullet->Initialise(renderer);
	
	// Spawn a setting number of enemies:
	for (int i = 0; i < 7; i++)
	{
		Enemy* enemy = new Enemy();
		enemy->Initialise(renderer);
		enemy->m_pSprite->SetGreenTint(0.75f);
		m_pEnemies.push_back(enemy);
	}
	
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
	//process the background
	m_pBackground->SetAngle(0);
	m_pBackground->Process(deltaTime);

	//reading input
	ButtonState LKeyState = inputSystem.GetKeyState(SDL_SCANCODE_LEFT);
	ButtonState RKeyState = inputSystem.GetKeyState(SDL_SCANCODE_RIGHT);
	ButtonState sKeyState = inputSystem.GetKeyState(SDL_SCANCODE_SPACE);
	ButtonState FKeyState = inputSystem.GetKeyState(SDL_SCANCODE_UP);

	if (LKeyState == BS_PRESSED)
	{
		printf("key 'left arrow' detected.");
		float currentAngle = m_pPlayer->GetAngle();
		float newAngle = NormalizeAngle(currentAngle - 45.0f);
		m_pPlayer->SetAngle(newAngle);
	}
	if (RKeyState == BS_PRESSED)
	{
		printf("key 'right arrow' detected.");
		float currentAngle = m_pPlayer->GetAngle();
		float newAngle = NormalizeAngle(currentAngle + 45.0f);
		m_pPlayer->SetAngle(newAngle);
	}
	if (sKeyState == BS_PRESSED)
	{
		printf("key 'Space' detected.");
		Game::pSoundsystem->playSound(hitsound1, nullptr, false, &channel);
		Vector2 playerPosition = m_pPlayer->GetPosition();
		float playerAngle = m_pPlayer->GetAngle();
		PlayerBullet->SetPosition(playerPosition, playerAngle);
	}
	if (FKeyState == BS_HELD)
	{
		printf("key 'up arrow' detected.");
		float playerAngle = m_pPlayer->GetAngle();
		float angleInRadians = playerAngle * M_PI / 180.0f + 45.0f;

		const float speed = 20.0f;  // set the speed for the tank

		Vector2 direction(cos(angleInRadians), sin(angleInRadians));

		m_pPlayer->GetPosition() += direction * speed * deltaTime;
	}

	m_pPlayer->Process(deltaTime);

	PlayerBullet->Process(deltaTime);

	for (auto& enemy : m_pEnemies)
	{
		enemy->Process(deltaTime);
	}

	CheckCollisions();

	pAnimatedSprite->Process(deltaTime);

	//judge if all enemies destroyed
	bool allEnemiesDestroyed = true;
	for (const auto& enemy : m_pEnemies)
	{
		if (enemy->IsAlive())
		{
			allEnemiesDestroyed = false;
			break;
		}
	}
	if (allEnemiesDestroyed)
	{
		(*m_sceneIndex) += 2;
	}
}

void SceneTankGame::CheckCollisions()
{
	if (m_pPlayer && m_pPlayer->IsAlive())
	{
		for (auto& enemy : m_pEnemies)
		{
			if (enemy->IsAlive() && m_pPlayer->IsCollidingWith(*enemy))
			{
				m_pPlayer->SetDead();

				Game::pSoundsystem->playSound(hitsound2, nullptr, false, &channel);

				pAnimatedSprite->SetX(static_cast<int>(m_pPlayer->GetPosition().x));
				pAnimatedSprite->SetY(static_cast<int>(m_pPlayer->GetPosition().y));
				pAnimatedSprite->Animate();
			}

			if (m_pPlayer->IsCollidingWithBullet(enemy->GetBullet())) 
			{
				Game::pSoundsystem->playSound(hitsound2, nullptr, false, &channel);

				pAnimatedSprite->SetX(static_cast<int>(m_pPlayer->GetPosition().x));
				pAnimatedSprite->SetY(static_cast<int>(m_pPlayer->GetPosition().y));
				pAnimatedSprite->Animate();

				(*m_sceneIndex)++;
				break;
			}

			if (enemy->IsAlive() && enemy->IsCollidingWithBullet(PlayerBullet))
			{
				enemy->SetDead();

				Game::pSoundsystem->playSound(hitsound2, nullptr, false, &channel);

				pAnimatedSprite->SetX(static_cast<int>(enemy->GetPosition().x));
				pAnimatedSprite->SetY(static_cast<int>(enemy->GetPosition().y));
				pAnimatedSprite->Animate();
			}

		}

	}
}

void SceneTankGame::OnSceneChange(int* sceneIndex) 
{
	m_sceneIndex = sceneIndex;
}

void SceneTankGame::Draw(Renderer& renderer)
{
	m_pBackground->Draw(renderer);

	if (m_pPlayer && m_pPlayer->IsAlive())
	{
		m_pPlayer->Draw(renderer);
	}
	PlayerBullet->Draw(renderer);

	for (auto& enemies : m_pEnemies)
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
	
}

float SceneTankGame::NormalizeAngle(float angle)
{
	while (angle >= 360.0f)
		angle -= 360.0f;
	while (angle < 0.0f)
		angle += 360.0f;
	return angle;
}