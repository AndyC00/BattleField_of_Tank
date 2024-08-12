//this includes:
#include "scenecheckerboards.h"

//local includes:
#include "renderer.h"
#include "sprite.h"

//library includes:
#include <cassert>

SceneCheckerboards::SceneCheckerboards() :
	m_pCorners{ 0 },
	m_pCentre(0),
	m_angle(0.0f),
	m_rotationSpeed(0.0f)
{
	
}

SceneCheckerboards::~SceneCheckerboards()
{
	for (int k = 0; k < 4; k++)
	{
		delete m_pCorners[k];
		m_pCorners[k] = 0;
	}
	delete m_pCentre;
}

bool SceneCheckerboards::Initialise(Renderer& renderer)
{
	m_pCentre = renderer.CreateSprite("Sprites\\board8x8.png");
	m_pCorners[0] = renderer.CreateSprite("Sprites\\board8x8.png");
	m_pCorners[1] = renderer.CreateSprite("Sprites\\board8x8.png");
	m_pCorners[2] = renderer.CreateSprite("Sprites\\board8x8.png");
	m_pCorners[3] = renderer.CreateSprite("Sprites\\board8x8.png");

	const int BOARD_HALF_WIDTH = m_pCentre->GetWidth() / 2;
	const int BOARD_HALF_HEIGHT = m_pCentre->GetHeight() / 2;
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	m_pCentre->SetX(SCREEN_WIDTH / 2);
	m_pCentre->SetY(SCREEN_HEIGHT / 2);

	//top left white:
	m_pCorners[0]->SetX(BOARD_HALF_WIDTH);
	m_pCorners[0]->SetY(BOARD_HALF_HEIGHT);

	//top right red:
	m_pCorners[1]->SetX(SCREEN_WIDTH - BOARD_HALF_WIDTH);
	m_pCorners[1]->SetY(BOARD_HALF_HEIGHT);
	m_pCorners[1]->SetGreenTint(0.0f);
	m_pCorners[1]->SetBlueTint(0.0f);

	//Bottom right green:
	m_pCorners[2]->SetX(SCREEN_WIDTH - BOARD_HALF_WIDTH);
	m_pCorners[2]->SetY(SCREEN_HEIGHT - BOARD_HALF_HEIGHT);
	m_pCorners[2]->SetRedTint(0.0f);

	//Adding by myself, need to double check:
	//Bottom left blue:
	m_pCorners[3]->SetX(BOARD_HALF_WIDTH);
	m_pCorners[3]->SetY(SCREEN_HEIGHT - BOARD_HALF_HEIGHT);
	m_pCorners[1]->SetBlueTint(0.0f);
	m_pCorners[2]->SetRedTint(0.0f);

	return true;
}
	void SceneCheckerboards::Process(float deltaTime)
	{
		for (int k = 0; k < 4; k++)
		{
			m_pCorners[k]->Process(deltaTime);
		}
		m_angle += m_rotationSpeed * deltaTime;

		m_pCentre->SetAngle(m_angle);
		m_pCentre->Process(deltaTime);
	}
	
	void SceneCheckerboards::Draw(Renderer& renderer)
	{
		for (int k = 0; k < 4; k++)
		{
			m_pCorners[k]->Draw(renderer);
		}
		m_pCentre->Draw(renderer);
	}