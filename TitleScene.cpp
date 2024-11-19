//this includes:
#include "TitleScene.h"

//local includes:
#include "renderer.h"
#include "sprite.h"

#include "imgui/imgui.h"
#include "inputsystem.h"

//library includes:
#include <cassert>

using FMOD::System;
using FMOD::Sound;
using FMOD::Channel;

TitleScene::TitleScene():
	m_CurrentPicture(0),
	titleChannel(nullptr),
	titleSound(nullptr)
{

}

TitleScene::~TitleScene()
{
	for (auto& m_picture : m_pictures)
	{
		delete m_picture;
	}
	m_pictures.clear();

	if (titleSound)
	{
		titleSound->release();
		titleSound = nullptr;
	}
	if (titleChannel)
	{
		titleChannel->stop();
		titleChannel = nullptr;
	}
}

bool TitleScene::Initialise(Renderer& renderer)
{
	m_pictures.push_back(renderer.CreateSprite("Sprites\\Scene\\opening.png"));
	m_pictures.push_back(renderer.CreateSprite("Sprites\\Scene\\Story1.png"));
	m_pictures.push_back(renderer.CreateSprite("Sprites\\Scene\\Story2.png"));
	m_pictures.push_back(renderer.CreateSprite("Sprites\\Scene\\Story3.png"));
	m_pictures.push_back(renderer.CreateSprite("Sprites\\Scene\\Story4.png"));
	m_pictures.push_back(renderer.CreateSprite("Sprites\\Scene\\instruction.png"));

	for (auto& m_picture : m_pictures)
	{
		const int BOARD_HALF_WIDTH = m_picture->GetWidth() / 2;
		const int BOARD_HALF_HEIGHT = m_picture->GetHeight() / 2;
		const int SCREEN_WIDTH = renderer.GetWidth();
		const int SCREEN_HEIGHT = renderer.GetHeight();

		m_picture->SetX(SCREEN_WIDTH / 2);
		m_picture->SetY(SCREEN_HEIGHT / 2);
		m_picture->SetScale(1.05f);
	}

	//sound:
	Game::pSoundsystem->createSound("sounds\\TitleMusic.wav", FMOD_LOOP_NORMAL, &titleSound);

	return true;
}
void TitleScene::Process(float deltaTime, InputSystem& inputSystem)
{
	if (!titleChannel)
	{
		Game::pSoundsystem->playSound(titleSound, nullptr, false, &titleChannel);
	}

	m_pictures[m_CurrentPicture]->Process(deltaTime);

	if (inputSystem.GetMouseButtonState(SDL_BUTTON_LEFT) == BS_PRESSED)
	{
		if (m_CurrentPicture < m_pictures.size() - 1)
		{
			m_CurrentPicture++;
		}
		else
		{
			(*m_sceneIndex)++;
			OnExit();
		}
	}
}

void TitleScene::Draw(Renderer& renderer)
{
	m_pictures[m_CurrentPicture]->Draw(renderer);
}

void TitleScene::OnExit()
{
	if (titleSound)
	{
		titleSound->release();
		titleSound = nullptr;
	}
	if (titleChannel)
	{
		titleChannel->stop();
		titleChannel = nullptr;
	}
}

void TitleScene::DebugDraw()
{
	
}
