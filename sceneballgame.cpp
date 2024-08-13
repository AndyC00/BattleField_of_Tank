// Local includes:
#include "sceneballgame.h"
#include "inputsystem.h"
#include "renderer.h"
#include "ball.h"
#include "logmanager.h"

// Library includes:
#include <vector>
#include <cmath>

SceneBallGame::SceneBallGame()
	: m_pRenderer(nullptr),
	m_pPlayerBall(nullptr)
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
	SpawnGoodBalls(10);  // You can specify the number of good balls you want to spawn

	// Spawn bad balls
	SpawnBadBalls(10);  // You can specify the number of bad balls you want to spawn

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
	// ?????????????
	Vector2 mousePos = inputSystem.GetMousePosition();
	m_pPlayerBall->Position() = mousePos;

	// ??????????
	for (auto& ball : m_pGoodBalls)
	{
		ball->Process(deltaTime);
	}
	for (auto& ball : m_pBadBalls)
	{
		ball->Process(deltaTime);
	}

	// ????
	CheckCollisions();

	// ??????
	m_pGoodBalls.erase(std::remove_if(m_pGoodBalls.begin(), m_pGoodBalls.end(),
		[](Ball* ball) { return !ball->IsAlive(); }), m_pGoodBalls.end());
	m_pBadBalls.erase(std::remove_if(m_pBadBalls.begin(), m_pBadBalls.end(),
		[](Ball* ball) { return !ball->IsAlive(); }), m_pBadBalls.end());
}

void SceneBallGame::CheckCollisions()
{
	// ??????
	for (auto& ball : m_pGoodBalls)
	{
		if (BallVsBall(m_pPlayerBall, ball))
		{
			m_pPlayerBall->Enlarge();  // ?????
			ball->Kill();              // ????
		}
	}

	// ??????
	for (auto& ball : m_pBadBalls)
	{
		if (BallVsBall(m_pPlayerBall, ball))
		{
			m_pPlayerBall->Shrink();   // ?????
			ball->Kill();              // ????
		}
	}
}

bool SceneBallGame::BallVsBall(Ball* p1, Ball* p2)
{
	float dist = Vector2::Distance(p1->Position(), p2->Position());
	return dist < (p1->GetRadius() + p2->GetRadius());
}