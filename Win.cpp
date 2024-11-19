//this includes:
#include "Win.h"

//local includes:
#include "renderer.h"
#include "sprite.h"
#include "TankGame.h"

#include "imgui/imgui.h"

//library includes:
#include <cassert>
#include <cstdlib>

using FMOD::System;
using FMOD::Sound;
using FMOD::Channel;

WinScene::WinScene(Game* game) :
	m_pCentre(0),
	m_quit(0),
	m_restart(0),
	m_angle(0.0f),
	m_rotationSpeed(0.0f),
	m_pRenderer(nullptr),
	winningChannel(nullptr),
	winningSound(nullptr),
	m_game(game)
{

}

WinScene::~WinScene()
{
	delete m_pCentre;
	m_pCentre = 0;
	delete m_quit;
	m_quit = 0;
	delete m_restart;
	m_restart = 0;

	if (winningSound)
	{
		winningSound->release();
		winningSound = nullptr;
	}
	if (winningChannel)
	{
		winningChannel->stop();
		winningChannel = nullptr;
	}
}

bool WinScene::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;

	m_pCentre = renderer.CreateSprite("Sprites\\Scene\\win.png");
	m_quit = renderer.CreateSprite("Sprites\\Buttons\\QButton.png");
	m_restart = renderer.CreateSprite("Sprites\\Buttons\\RButton.png");

	const int BOARD_HALF_WIDTH = m_pCentre->GetWidth() / 2;
	const int BOARD_HALF_HEIGHT = m_pCentre->GetHeight() / 2;
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	m_pCentre->SetX(SCREEN_WIDTH / 2);
	m_pCentre->SetY(SCREEN_HEIGHT / 2);
	m_pCentre->SetScale(1.05f);

	m_quit->SetX(SCREEN_WIDTH / 6);
	m_quit->SetY(SCREEN_HEIGHT / 6);

	m_restart->SetX(SCREEN_WIDTH / 6);
	m_restart->SetY(SCREEN_HEIGHT / 6 + 120);

	//sound:
	FMOD_RESULT result = Game::pSoundsystem->createSound("sounds\\winningSound.wav", FMOD_DEFAULT, &winningSound);

	if (result != FMOD_OK || winningSound == nullptr)
	{
		printf("Failed to load winningSound.wav: %s\n", FMOD_ErrorString(result));
	}
	else
	{
		printf("Successfully loaded winningSound.wav\n");
	}

	return true;
}

void WinScene::Process(float deltaTime, InputSystem& inputSystem)
{
	if (!winningChannel)
	{
		Game::pSoundsystem->playSound(winningSound, nullptr, false, &winningChannel);
	}

	m_angle += m_rotationSpeed * deltaTime;

	m_pCentre->SetAngle(m_angle);
	m_pCentre->Process(deltaTime);

	m_quit->Process(deltaTime);
	m_restart->Process(deltaTime);

	ButtonState RKeyState = inputSystem.GetKeyState(SDL_SCANCODE_R);
	ButtonState QKeyState = inputSystem.GetKeyState(SDL_SCANCODE_Q);

	if (RKeyState == BS_PRESSED)
	{
		//Restart the Playing Scene
		int sceneTankGameIndex = 1; // The buffer of the TankGame Scene

		for (int i = 0; i < 3; i++)
		{
			Scene* oldScene = m_game->GetSceneAt(sceneTankGameIndex + i);
			oldScene->OnExit();
			oldScene = nullptr;	//the oldScene will be deleted in the __debugbreak(). No idea why
		}

		for (int i = 1; i <= 3; ++i)
		{
			int difficulty = i * 2; // Difficulty levels: 2, 4, 6

			SceneTankGame* newScene = new SceneTankGame(m_game, difficulty);
			newScene->OnSceneChange(m_sceneIndex);
			newScene->Initialise(*m_pRenderer);

			m_game->SetSceneAt(i, newScene);
		}

		*m_sceneIndex = sceneTankGameIndex;
	}

	if (QKeyState == BS_PRESSED)
	{
		exit(0);
	}
}

void WinScene::Draw(Renderer& renderer)
{
	m_pCentre->Draw(renderer);
	m_quit->Draw(renderer);
	m_restart->Draw(renderer);
}

void WinScene::DebugDraw()
{
	ImGui::Text("Scene: Winning Title");
	ImGui::InputFloat("Rotation speed", &m_rotationSpeed); float scale = m_pCentre->GetScale();
	ImGui::SliderFloat("Demo scale", &scale, 0.0f, 2.0f, "%.3f"); m_pCentre->SetScale(scale);

	int position[2];
	position[0] = m_pCentre->GetX(); position[1] = m_pCentre->GetY();
	ImGui::InputInt2("Demo position", position); m_pCentre->SetX(position[0]);
	m_pCentre->SetY(position[1]);

	float tint[4];
	tint[0] = m_pCentre->GetRedTint();
	tint[1] = m_pCentre->GetGreenTint();
	tint[2] = m_pCentre->GetBlueTint();
	tint[3] = m_pCentre->GetAlpha();
	ImGui::ColorEdit4("Demo tint", tint);
	m_pCentre->SetRedTint(tint[0]);
	m_pCentre->SetGreenTint(tint[1]);
	m_pCentre->SetBlueTint(tint[2]);
	m_pCentre->SetAlpha(tint[3]);
}
