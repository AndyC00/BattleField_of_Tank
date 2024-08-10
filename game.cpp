// This include: 
#include "game.h"
#include "sprite.h"

// Library includes: 
#include "renderer.h" 
#include "logmanager.h"

// Static Members:
Game* Game::sm_pInstance = 0;
Game& Game::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Game();
	}

	return (*sm_pInstance);
}

void Game::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

Game::Game()
	: m_pRenderer(0)
	, m_bLooping(true)
{

}

Game::~Game()
{
	delete m_pRenderer;
	m_pRenderer = 0;
	delete m_pCheckerboard1;
	m_pCheckerboard1 = 0;
	delete m_pCheckerboard2;
	m_pCheckerboard2 = 0;
	delete m_pCheckerboard3;
	m_pCheckerboard3 = 0;
	delete m_pCheckerboard4;
	m_pCheckerboard4 = 0;
	delete m_pCheckerboard5;
	m_pCheckerboard5 = 0;
}

void Game::Quit()
{
	m_bLooping = false;
}

bool Game::Initialise()
{
	int bbWidth = 1860;
	int bbHeight = 1050;

	m_pRenderer = new Renderer();
	if (!m_pRenderer->Initialise(true, bbWidth, bbHeight))
	{
		LogManager::GetInstance().Log("Renderer failed to initialise!");
		return false;
	}

	//load sprite and draw
	m_pCheckerboard1 = m_pRenderer->CreateSprite("Sprites\\board8x8.png");

	//set X and Y
	m_pCheckerboard1->SetX(930);
	m_pCheckerboard1->SetY(525);

	//setting the color for the sprite
	m_pCheckerboard1->SetRedTint(255);
	m_pCheckerboard1->SetGreenTint(255);
	m_pCheckerboard1->SetBlueTint(255);

	//set angles for the sprite:
	m_pCheckerboard1->SetAngle(45);

	//change scale for the sprite:
	m_pCheckerboard1->SetScale(1.5);

	//loading more sprites:
	m_pCheckerboard2 = m_pRenderer->CreateSprite("Sprites\\board8x8.png");
	m_pCheckerboard2->SetX(290); 
	m_pCheckerboard2->SetY(221);
	m_pCheckerboard2->SetRedTint(0);
	m_pCheckerboard2->SetGreenTint(0);
	m_pCheckerboard2->SetBlueTint(255);

	m_pCheckerboard3 = m_pRenderer->CreateSprite("Sprites\\board8x8.png");
	m_pCheckerboard3->SetX(1570);
	m_pCheckerboard3->SetY(829);
	m_pCheckerboard3->SetRedTint(255);
	m_pCheckerboard3->SetGreenTint(0);
	m_pCheckerboard3->SetBlueTint(0);

	m_pCheckerboard4 = m_pRenderer->CreateSprite("Sprites\\board8x8.png");
	m_pCheckerboard4->SetX(290);
	m_pCheckerboard4->SetY(829);
	m_pCheckerboard4->SetRedTint(0);
	m_pCheckerboard4->SetGreenTint(255);
	m_pCheckerboard4->SetBlueTint(0);

	m_pCheckerboard5 = m_pRenderer->CreateSprite("Sprites\\board8x8.png");
	m_pCheckerboard5->SetX(1570);
	m_pCheckerboard5->SetY(221);
	m_pCheckerboard5->SetRedTint(122);
	m_pCheckerboard5->SetGreenTint(122);
	m_pCheckerboard5->SetBlueTint(122);


	bbWidth = m_pRenderer->GetWidth();
	bbHeight = m_pRenderer->GetHeight();

	m_iLastTime = SDL_GetPerformanceCounter();
	m_pRenderer->SetClearColour(0, 255, 255); 

	return true;
}
bool Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;
	// TODO: Process input here! 

	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		continue;
	}
	if (m_bLooping)
	{
		Uint64 current = SDL_GetPerformanceCounter();
		float deltaTime = (current - m_iLastTime) / static_cast<float>(SDL_GetPerformanceFrequency());
		m_iLastTime = current;
		m_fExecutionTime += deltaTime;
		Process(deltaTime);

#ifdef USE_LAG
		m_fLag += deltaTime;
		int innerLag = 0;
		while (m_fLag >= stepSize)
		{
			Process(stepSize);

			m_fLag -= stepSize;

			++m_iUpdateCount;
			++innerLag;
		}
#endif //USE_LAG

		Draw(*m_pRenderer);
	}
	return m_bLooping;
}

void
Game::Process(float deltaTime)
{
	ProcessFrameCounting(deltaTime);
	// TODO: Add game objects to process here!
	m_pCheckerboard1->Process(deltaTime);
	m_pCheckerboard2->Process(deltaTime);
	m_pCheckerboard3->Process(deltaTime);
	m_pCheckerboard4->Process(deltaTime);
	m_pCheckerboard5->Process(deltaTime);
}

void
Game::Draw(Renderer& renderer)
{
	++m_iFrameCount;
	renderer.Clear();

	// TODO: Add game objects to draw here!
	m_pCheckerboard1->Draw(renderer);
	m_pCheckerboard2->Draw(renderer);
	m_pCheckerboard3->Draw(renderer);
	m_pCheckerboard4->Draw(renderer);
	m_pCheckerboard5->Draw(renderer);

	renderer.Present();
}
void
Game::ProcessFrameCounting(float deltaTime)
{
	// Count total simulation time elapsed:
	m_fElapsedSeconds += deltaTime;
	// Frame Counter:
	if (m_fElapsedSeconds > 1.0f)
	{
		m_fElapsedSeconds -= 1.0f;
		m_iFPS = m_iFrameCount;
		m_iFrameCount = 0;
	}
}