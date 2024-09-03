// Local includes:
#include "sceneballgame.h"
#include "inputsystem.h"
#include "renderer.h"
#include "ball.h"
#include "scene.h"
#include "logmanager.h"
#include "imgui/imgui.h"
#include "soundsystem.h"
#include "game.h"

// Library includes:
#include <vector>
#include <cmath>
#include "fmod.hpp"
#include "fmod_errors.h"


using FMOD::System;
using FMOD::Sound;
using FMOD::Channel;

SceneBallGame::SceneBallGame()
	: m_pRenderer(nullptr),
	m_pPlayerBall(nullptr),
	hitsound1(nullptr),
	hitsound2(nullptr),
	channel(nullptr),
	opening(nullptr)
{
}

SceneBallGame::~SceneBallGame()
{
    delete m_pPlayerBall;
    for (Ball* pBall : m_pGoodBalls)
    {
        delete pBall;
    }
    for (Ball* pBall : m_pBadBalls)
    {
        delete pBall;
    }
}

bool SceneBallGame::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;

	// Initialize player ball
	m_pPlayerBall = new Ball();
	m_pPlayerBall->Initialise(renderer);
	m_pPlayerBall->SetAsPlayer();  // Set the ball as the player ball
	m_pPlayerBall->Position().x = renderer.GetWidth() / 2.0f;
	m_pPlayerBall->Position().y = renderer.GetHeight() / 2.0f;

	// Spawn good balls
	SpawnGoodBalls(25);  // specify the number of good balls to 10

	// Spawn bad balls
	SpawnBadBalls(25);  // specify the number of bad balls to 10

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

void SceneBallGame::SpawnGoodBalls(int number)
{
	for (int i = 0; i < number; ++i)
	{
		Ball* ball = new Ball();
		ball->Initialise(*m_pRenderer);
		ball->SetGood();  // Set the ball as a good ball

		// Set initial position at the top of the screen, with random x
		ball->Position().x = static_cast<float>(rand() % m_pRenderer->GetWidth());
		ball->Position().y = 0;  // Start from the top

		m_pGoodBalls.push_back(ball);
	}
}

void SceneBallGame::SpawnBadBalls(int number)
{
	for (int i = 0; i < number; ++i)
	{
		Ball* ball = new Ball();
		ball->Initialise(*m_pRenderer);
		ball->SetBad();  // Set the ball as a bad ball

		// Set initial position at the bottom of the screen, with random x
		ball->Position().x = static_cast<float>(rand() % m_pRenderer->GetWidth());
		ball->Position().y = static_cast<float>(m_pRenderer->GetHeight());  // Start from the bottom

		m_pBadBalls.push_back(ball);
	}
}

void SceneBallGame::Process(float deltaTime, InputSystem& inputSystem)
{
	Vector2 mousePosition = inputSystem.GetMousePosition();
	m_pPlayerBall->Position() = mousePosition;

	m_pPlayerBall->Process(deltaTime);

	for (auto& goodBall : m_pGoodBalls)
	{
		goodBall->Process(deltaTime);
	}

	for (auto& badBall : m_pBadBalls)
	{
		badBall->Process(deltaTime);
	}

	CheckCollisions();
}

void SceneBallGame::CheckCollisions()
{
	for (auto it = m_pGoodBalls.begin(); it != m_pGoodBalls.end();)
	{
		Ball* goodBall = *it;
		if (BallVsBall(m_pPlayerBall, goodBall))
		{
			m_pPlayerBall->Enlarge();  
			goodBall->Kill();  
			it = m_pGoodBalls.erase(it);
			Game::pSoundsystem->playSound(hitsound1, nullptr, false, &channel);
		}
		else
		{
			++it;
		}
	}

	for (auto it = m_pBadBalls.begin(); it != m_pBadBalls.end();)
	{
		Ball* badBall = *it;
		if (BallVsBall(m_pPlayerBall, badBall))
		{
			m_pPlayerBall->Shrink(); 
			badBall->Kill(); 
			it = m_pBadBalls.erase(it);
			Game::pSoundsystem->playSound(hitsound2, nullptr, false, &channel);
		}
		else
		{
			++it;
		}
	}
}

bool SceneBallGame::BallVsBall(Ball* p1, Ball* p2)
{
	float distanceSquared = (p1->Position() - p2->Position()).LengthSquared();
	float combinedRadius = p1->GetRadius() + p2->GetRadius();
	return distanceSquared <= (combinedRadius * combinedRadius);
}

void SceneBallGame::Draw(Renderer& renderer)
{
	if (m_pPlayerBall && m_pPlayerBall->IsAlive())
	{
		m_pPlayerBall->Draw(renderer);
	}

	for (auto& goodBall : m_pGoodBalls)
	{
		if (goodBall->IsAlive())
		{
			goodBall->Draw(renderer);
		}
	}

	for (auto& badBall : m_pBadBalls)
	{
		if (badBall->IsAlive())
		{
			badBall->Draw(renderer);
		}
	}
}

void SceneBallGame::DebugDraw()
{
	if (m_pPlayerBall && m_pPlayerBall->IsAlive())
	{
		ImGui::Text("Player Ball:");
		m_pPlayerBall->DebugDraw();
	}

	ImGui::Text("Good Balls:");
	for (auto& goodBall : m_pGoodBalls)
	{
		if (goodBall->IsAlive())
		{
			goodBall->DebugDraw();
		}
	}

	ImGui::Text("Bad Balls:");
	for (auto& badBall : m_pBadBalls)
	{
		if (badBall->IsAlive())
		{
			badBall->DebugDraw();
		}
	}
}
