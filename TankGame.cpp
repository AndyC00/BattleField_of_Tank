// Local includes:
#include "inputsystem.h"
#include "renderer.h"
#include "scene.h"
#include "imgui/imgui.h"
#include "soundsystem.h"
#include "game.h"
#include "animatedsprite.h"
#include "texture.h"
#include "Enemy.h"
#include "Entity.h"
#include "TankGame.h"
#include "Bullet.h"
#include "Player.h"

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
	m_pPlayer(nullptr),
	channel(nullptr),
	opening(nullptr),
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
	delete m_pPlayer;
	for (auto& pAnimatedSprite : m_explosions)
	{
		delete pAnimatedSprite;
	}
	m_explosions.clear();
	for (auto& enemy : m_pEnemies)
	{
		delete enemy;
	}
	m_pEnemies.clear();
	for (auto& trap : m_Traps)
	{
		delete trap;
	}
	m_Traps.clear();
	delete m_pBackground;
}

bool SceneTankGame::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;

	//initialise the background pic
	m_pBackground = renderer.CreateSprite("Sprites\\Scene\\background.png");
	m_pBackground->SetX(1860 / 2);
	m_pBackground->SetY(1060 / 2);

	// Initialize player tank
	m_pPlayer = new Player();
	m_pPlayer->Initialise(renderer);
	m_pPlayer->SetPosition(static_cast<int>(renderer.GetWidth() / 2.0f), static_cast<int>(renderer.GetHeight() / 2.0f));

	// Spawn a setting number of enemies:
	for (int i = 0; i < 9; i++)
	{
		Enemy* enemy = new Enemy();
		enemy->Initialise(renderer);
		enemy->m_pSprite->SetGreenTint(0.75f);
		m_pEnemies.push_back(enemy);
	}
	for (int i = 0; i < 5; i++)
	{
		Trap* trap = new Trap();
		trap->Initialise(renderer);
		m_Traps.push_back(trap);
	}

	//initialise the clouds:
	m_clouds.Initialise(renderer);

	//initialise the planes:
	m_aircraft.Initialise(renderer);

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

	return true;
}

void SceneTankGame::Process(float deltaTime, InputSystem& inputSystem)
{
	//process the background
	m_pBackground->SetAngle(0);
	m_pBackground->Process(deltaTime);

	m_pPlayer->Process(deltaTime, inputSystem);

	for (auto& enemy : m_pEnemies)
	{
		enemy->Process(deltaTime);
	}

	for (auto& trap : m_Traps)
	{
		trap->Process(deltaTime);
	}

	m_clouds.Process(deltaTime);

	m_aircraft.Process(deltaTime);

	CheckCollisions();

	//processing animated sprite
	UpdateExplosions(deltaTime);

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
	if (!m_pPlayer) return;
	if (!m_pPlayer->IsAlive()) return;

	//collision check with enemies
	for (auto& enemy : m_pEnemies)
	{
		if (enemy->IsAlive() && m_pPlayer->IsCollidingWith(*enemy))
		{
			Game::pSoundsystem->playSound(hitsound2, nullptr, false, &channel);

			CreateExplosion(m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y);

			ReceiveDamage(1);
		}

		if (m_pPlayer->IsCollidingWithBullet(enemy->GetBullet()))
		{
			Game::pSoundsystem->playSound(hitsound2, nullptr, false, &channel);

			CreateExplosion(m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y);

			ReceiveDamage(1);
		}

		if (enemy->IsAlive() && enemy->IsCollidingWithBullet(m_pPlayer->GetBullet()))
		{
			enemy->SetDead();

			Game::pSoundsystem->playSound(hitsound2, nullptr, false, &channel);

			CreateExplosion(enemy->GetPosition().x, enemy->GetPosition().y);
		}
	}

	//collision check with traps:
	for (auto& trap : m_Traps)
	{
		if (m_pPlayer->IsCollidingWith(*trap))
		{
			trap->SetDead();

			Game::pSoundsystem->playSound(hitsound2, nullptr, false, &channel);

			CreateExplosion(m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y);

			ReceiveDamage(1);
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

	for (auto& enemies : m_pEnemies)
	{
		if (enemies->IsAlive())
		{
			enemies->Draw(renderer);
		}
	}

	for (auto& trap : m_Traps)
	{
		if (trap->IsAlive())
		{
			trap->Draw(renderer);
		}
	}

	for (auto& explosion : m_explosions)
	{
		explosion->Draw(renderer);
	}

	m_aircraft.Draw(renderer);

	m_clouds.Draw(renderer);
}

void SceneTankGame::DebugDraw()
{

}

void SceneTankGame::UpdateExplosions(float deltaTime)
{
	for (auto it = m_explosions.begin(); it != m_explosions.end();)
	{
		AnimatedSprite* explosion = *it;

		explosion->Process(deltaTime);

		if (!explosion->IsAnimating())
		{
			delete explosion;
			it = m_explosions.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void SceneTankGame::CreateExplosion(float x, float y)
{
	AnimatedSprite* newExplosion = m_pRenderer->CreateAnimatedSprite("Sprites\\explosion.png");
	newExplosion->SetupFrames(66, 66);
	newExplosion->SetFrameDuration(0.1f);
	newExplosion->SetLooping(false);
	newExplosion->SetX(static_cast<int>(x));
	newExplosion->SetY(static_cast<int>(y));
	newExplosion->Animate();
	m_explosions.push_back(newExplosion);
}

void SceneTankGame::ReceiveDamage(int num)
{
	m_pPlayer->TakeDamage(num);

	if (m_pPlayer->GetLives() <= 0)
	{
		(*m_sceneIndex)++;
	}
}