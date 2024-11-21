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
#include "soundsystem.h"
#include "TankGame.h"
#include "Enemy.h"
#include "Entity.h"

//scenes include:
#include "TitleScene.h"
#include "lose.h"
#include "Win.h"

// Library includes: 
#include "renderer.h" 
#include "logmanager.h"
#include "imgui/imgui_impl_sdl2.h"
#include "fmod.hpp"
#include "fmod.h"
#include "fmod_errors.h"

#include <vector>
#include <SDL_ttf.h>

//namespace using:
using FMOD::System;
using FMOD::Sound;
using FMOD::Channel;
using FMOD::ChannelGroup;

// Static Members:
Game* Game::sm_pInstance = 0;
SoundSystem* Game::pSoundsystem = nullptr;

void SceneChange(int& currentScene)
{
	currentScene++;
}

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

Game::Game() :
	m_pRenderer(0),
	m_bLooping(true),
	m_iCurrentScene(0),
	m_pInputSystem(0)
{
	pSoundsystem = new SoundSystem();
}

Game::~Game()
{
	delete m_pRenderer;
	m_pRenderer = 0;

	delete m_pInputSystem;
	m_pInputSystem = 0;

	delete pSoundsystem;
	pSoundsystem = 0;

	delete m_pZapPow[0];
	m_pZapPow[0] = 0;
}

void Game::Quit()
{
	m_bLooping = false;
}

bool Game::Initialise()
{
	//load soundsystem:
	pSoundsystem->init();

	int bbWidth = 1792;
	int bbHeight = 1024;

	//Instantiate a input system pointer with new
	m_pInputSystem = new InputSystem();
	m_pInputSystem->Initialise();

	m_pRenderer = new Renderer();
	//create game in window or not:
	if (!m_pRenderer->Initialise(true, bbWidth, bbHeight))
	{
		LogManager::GetInstance().Log("Renderer failed to initialise!");
		return false;
	}

	TitleScene* pScene1 = nullptr;
	pScene1 = new TitleScene();
	pScene1->OnSceneChange(&m_iCurrentScene);
	pScene1->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene1);

	SceneTankGame* pScene = nullptr;
	for (int i = 1; i <= 3; i++)
	{
		int difficulty = i * 2;

		SceneTankGame* pScene = new SceneTankGame(this, difficulty);
		pScene->OnSceneChange(&m_iCurrentScene);
		pScene->Initialise(*m_pRenderer);
		m_scenes.push_back(pScene);
	}

	Scene* pScene5 = nullptr;
	pScene5 = new LoseScene(this);
	pScene5->OnSceneChange(&m_iCurrentScene);
	pScene5->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene5);

	Scene* pScene6 = nullptr;
	pScene6 = new WinScene(this);
	pScene6->OnSceneChange(&m_iCurrentScene);
	pScene6->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene6);

	// text renderer at last:
	// Load static text textures into the Texture Manager... 
	m_pRenderer->CreateStaticText("left Click to Continue", 35);

	// Generate sprites that use the static text textures... 
	m_pZapPow[0] = m_pRenderer->CreateSprite("left Click to Continue");
	m_pZapPow[0]->SetX(300);
	m_pZapPow[0]->SetY(80);
	m_pZapPow[0]->SetAngle(0);

	bbWidth = m_pRenderer->GetWidth();
	bbHeight = m_pRenderer->GetHeight();

	m_iLastTime = SDL_GetPerformanceCounter();

	//set the color of the background
	m_pRenderer->SetClearColour(0, 0, 0);

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

		pSoundsystem->update();

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
	m_scenes[m_iCurrentScene]->Process(deltaTime, *m_pInputSystem);


	if (m_pInputSystem->GetKeyState(SDL_SCANCODE_ESCAPE) == BS_PRESSED)
	{
		exit(0);
	}

	//right click mouse to move to next scene:
	//if (m_pInputSystem->GetMouseButtonState(SDL_BUTTON_RIGHT) == BS_PRESSED && m_iCurrentScene < m_scenes.size() - 1)
	//{
	//	m_iCurrentScene++;
	//}

	/*ButtonState leftArrowState = (m_pInputSystem->GetKeyState(SDL_SCANCODE_LEFT));

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
	}*/
}

void Game::DebugDraw()
{
	// assigned to be true by myself because it hasn't been return by functions
	if (m_bShowDebugWindow = false)
	{
		bool open = true;
		ImGui::Begin("Debug Window", &open, ImGuiWindowFlags_MenuBar);
		ImGui::Text("My Game Frame", "2024, S2");

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

	//draw the fonts:
	m_pZapPow[0]->Draw(renderer);


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

Scene* Game::GetSceneAt(int index) const
{
	if (index >= 0 && index < m_scenes.size())
	{
		return m_scenes[index];
	}
	return nullptr;
}

void Game::SetSceneAt(int index, Scene* scene)
{
	if (index >= 0 && index < m_scenes.size())
	{
		m_scenes[index] = scene;
	}
}

void Game::SwitchScene(int newSceneIndex)
{
	if (m_iCurrentScene != newSceneIndex)
	{
		m_scenes[m_iCurrentScene]->OnExit();

		m_iCurrentScene = newSceneIndex;
	}
}

int Game::GetSceneSize()
{
	return m_scenes.size();
}