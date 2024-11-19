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
	m_pBackground(nullptr),
	m_pSkillButton(nullptr),
	waitTimer(0.75f),		//the time wait for the plane
	waitInterval(0),
	isWaitTimeActive(false),
	skillTimer(0.5f),
	skillInterval(4.0f)		//how often able to use skill
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
	for (auto& pAnimatedSprite : m_explosions)
	{
		delete pAnimatedSprite;
	}
	m_explosions.clear();
	for (auto& fire : m_allFire)
	{
		delete fire;
	}
	m_allFire.clear();
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
	m_pBackground = nullptr;
	delete m_pSkillButton;
	m_pSkillButton = nullptr;
	delete m_pPlayer;
	m_pPlayer = nullptr;
}

bool SceneTankGame::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;

	waitInterval = waitTimer;

	//initialise the background pic
	std::vector<const char*> filenames = {
			"Sprites\\Scene\\background1.png",
			"Sprites\\Scene\\background2.png",
			"Sprites\\Scene\\background3.png",
			"Sprites\\Scene\\background4.png",
			"Sprites\\Scene\\background5.png",
	};

	int selection = rand() % filenames.size();
	m_pBackground = renderer.CreateSprite(filenames[selection]);

	m_pBackground->SetX(m_pRenderer->GetWidth() / 2);
	m_pBackground->SetY(m_pRenderer->GetHeight() / 2);
	m_pBackground->SetScale(1.05f);

	m_pSkillButton = renderer.CreateSprite("Sprites\\Buttons\\SkillButton.png");
	m_pSkillButton->SetX(m_pRenderer->GetWidth() / 8);
	m_pSkillButton->SetY(m_pRenderer->GetHeight() / 7);
	SetButtonOff();

	// Initialize player tank
	m_pPlayer = new Player();
	m_pPlayer->Initialise(renderer);
	m_pPlayer->SetPosition(static_cast<int>(renderer.GetWidth() / 2.0f), static_cast<int>(renderer.GetHeight() * 0.8f));

	// Spawn a set random number of enemies:
	for (int i = 0; i < rand() % 5 + 4; i++)
	{
		Enemy* enemy = new Enemy(m_pPlayer);
		enemy->Initialise(renderer);
		m_pEnemies.push_back(enemy);
	}
	for (int i = 0; i < rand() % 3 + 3; i++)
	{
		Trap* trap = new Trap();
		trap->Initialise(renderer);
		m_Traps.push_back(trap);
	}

	//initialise the clouds:
	m_clouds.Initialise(renderer);

	//initialise the planes:
	m_aircraft.Initialise(renderer);

	//initialise the skill assets:
	m_skill.Initialise(renderer);

	//initialise the sound:
	FMOD_RESULT result = Game::pSoundsystem->createSound("sounds\\hit1.wav", FMOD_DEFAULT, &hitsound1);
	if (result != FMOD_OK || hitsound1 == nullptr) {
		printf("Failed to load hit.wav: %s\n", FMOD_ErrorString(result));
	}
	else {
		printf("Successfully loaded hit.wav\n");
	}

	result = Game::pSoundsystem->createSound("sounds\\hit2.wav", FMOD_DEFAULT, &hitsound2);
	if (result != FMOD_OK || hitsound2 == nullptr) {
		printf("Failed to load hit2.wav: %s\n", FMOD_ErrorString(result));
	}
	else {
		printf("Successfully loaded hit2.wav\n");
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

	m_pSkillButton->SetAngle(0);
	m_pSkillButton->Process(deltaTime);

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
	UpdateBoom(deltaTime);

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

	m_skill.Process(deltaTime);

	//dealing with skill:
	ButtonState ENKeyState = inputSystem.GetKeyState(SDL_SCANCODE_RETURN);

	if (skillTimer > 0.0f)
	{
		skillTimer -= deltaTime;
	}
	else
	{
		SetButtonOn();
	}

	if (skillTimer <= 0.0f && ENKeyState == BS_PRESSED)
	{
		SetButtonOff();
		skillTimer = skillInterval;

		//use the skill:
		m_skill.Activate();

		isWaitTimeActive = true;
		waitTimer = waitInterval;
	}

	if (isWaitTimeActive)
	{
		waitTimer -= deltaTime;

		if (waitTimer <= 0.0f)
		{
			for (int i = 0; i < 13; i++)
			{
				int positionX = m_pPlayer->GetPosition().x + ((rand() % 2 == 0) ? (rand() % 400) : (-rand() % 400));
				int positionY = m_pPlayer->GetPosition().y + ((rand() % 2 == 0) ? (rand() % 400) : (-rand() % 400));
				CreateBoom(positionX, positionY);
			}
			for (auto& enemy : m_pEnemies)
			{
				if (!enemy->IsAlive())	continue;

				Vector2 playerPosition = m_pPlayer->GetPosition();
				Vector2 enemyPosition = enemy->GetPosition();

				float distance = (playerPosition - enemyPosition).Length();

				if (distance < 400.0f)
				{
					CreateExplosion(enemy->GetPosition().x, enemy->GetPosition().y);
					enemy->SetDead();
				}
			}
			isWaitTimeActive = false;
		}
	}

	m_skill.Process(deltaTime);

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

	for (auto& fire : m_allFire)
	{
		fire->Draw(renderer);
	}

	m_aircraft.Draw(renderer);

	m_clouds.Draw(renderer);

	m_pSkillButton->Draw(renderer);

	m_skill.Draw(renderer);
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

void SceneTankGame::CreateBoom(float x, float y)
{
	AnimatedSprite* newFire = m_pRenderer->CreateAnimatedSprite("Sprites\\fireExplosion.png");
	newFire->SetupFrames(128, 128);
	newFire->SetFrameDuration(0.1f);
	newFire->SetLooping(false);
	newFire->SetX(static_cast<int>(x));
	newFire->SetY(static_cast<int>(y));
	newFire->Animate();
	m_allFire.push_back(newFire);
}

void SceneTankGame::UpdateBoom(float deltaTime)
{
	for (auto it = m_allFire.begin(); it != m_allFire.end();)
	{
		AnimatedSprite* fire = *it;

		fire->Process(deltaTime);

		if (!fire->IsAnimating())
		{
			delete fire;
			it = m_allFire.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void SceneTankGame::ReceiveDamage(int num)
{
	m_pPlayer->TakeDamage(num);

	if (m_pPlayer->GetLives() <= 0)
	{
		(*m_sceneIndex)++;
	}
}

void SceneTankGame::SetButtonOn()
{
	m_pSkillButton->SetBlueTint(1.0f);
	m_pSkillButton->SetGreenTint(1.0f);
	m_pSkillButton->SetRedTint(1.0f);
}

void SceneTankGame::SetButtonOff()
{
	m_pSkillButton->SetBlueTint(0.6f);
	m_pSkillButton->SetGreenTint(0.6f);
	m_pSkillButton->SetRedTint(0.6f);
}