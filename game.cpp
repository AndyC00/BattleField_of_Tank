// This include: 
#include "game.h"
#include "sprite.h"
#include "scenecheckerboards.h"
#include "scenebouncingballs.h"
#include "sceneballgame.h"
#include "scene.h"
#include "ball.h"
#include "inputsystem.h"
#include "animatedsprite.h"

// Library includes: 
#include "renderer.h" 
#include "logmanager.h"
#include "imgui/imgui_impl_sdl2.h"

#include <vector>


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

Game::Game():
	m_pRenderer(0),
	m_bLooping(true),
	m_pInputSystem(0)
{

}

Game::~Game()
{
	delete m_pRenderer;
	m_pRenderer = 0;

	delete m_pInputSystem;
    m_pInputSystem = 0;
	//deleting created sprites:

}

void Game::Quit()
{
	m_bLooping = false;
}

bool Game::Initialise()
{
	int bbWidth = 1860;
	int bbHeight = 1050;

	//Instantiate a input system pointer with new
	m_pInputSystem = new InputSystem();
	m_pInputSystem->Initialise();

	m_pRenderer = new Renderer();
	if (!m_pRenderer->Initialise(true, bbWidth, bbHeight))
	{
		LogManager::GetInstance().Log("Renderer failed to initialise!");
		return false;
	}

	//creating the scene:
	Scene* pScene = 0;
	pScene = new SceneCheckerboards();
	pScene->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene);
	m_iCurrentScene = 0;

	//creating another scene for bouncing balls:
	/*Scene* pScene2 = 0;
	pScene2 = new SceneBouncingBalls();
	pScene2->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene2);
	m_iCurrentScene = 0;*/

	//creating the scene3 for the ball game:
	/*Scene* pScene3 = 0;
	pScene3 = new SceneBallGame();
	pScene3->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene3);
	m_iCurrentScene = 0;*/

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

	m_pInputSystem->ProcessInput();

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

void Game::Process(float deltaTime)
{
	ProcessFrameCounting(deltaTime);
	// TODO: Add game objects to process here!
	m_scenes[m_iCurrentScene]->Process(deltaTime);

	ButtonState leftArrowState = (m_pInputSystem->GetKeyState(SDL_SCANCODE_LEFT));

	if (leftArrowState == BS_PRESSED)
	{
		LogManager::GetInstance().Log("Left arrow key pressed.");
	}
	else if (leftArrowState == BS_RELEASED)
	{
		LogManager::GetInstance().Log("Left arrow key released.");
	}

	int result = m_pInputSystem->GetMouseButtonState(SDL_BUTTON_LEFT);

	if (result == BS_PRESSED)
	{
		LogManager::GetInstance().Log("Left mouse button pressed.");
	}
	else if (result == BS_RELEASED)
	{
		LogManager::GetInstance().Log("Left mouse button released.");
	}


}

void Game::DebugDraw()
{
	if (m_bShowDebugWindow)
	{
		bool open = true;
		ImGui::Begin("Debug Window", &open, ImGuiWindowFlags_MenuBar);
		ImGui::Text("COMP710 GP Framework (%s)", "2022, S2");

		if (ImGui::Button("Quit"))
		{
			Quit();
		}

		ImGui::SliderInt("Active scene", &m_iCurrentScene, 0, m_scenes.size() - 1, "%d");
		m_scenes[m_iCurrentScene]->DebugDraw();

		ImGui::End();
	}
}

void Game::Draw(Renderer& renderer)
{
	++m_iFrameCount;
	renderer.Clear();

	// TODO: Add game objects to draw here!
	m_scenes[m_iCurrentScene]->Draw(renderer);

	DebugDraw();

	renderer.Present();
}

void Game::ProcessFrameCounting(float deltaTime)
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

void Game::ToggleDebugWindow()
{
	m_bShowDebugWindow = !m_bShowDebugWindow;

	m_pInputSystem->ShowMouseCursor(m_bShowDebugWindow);
}