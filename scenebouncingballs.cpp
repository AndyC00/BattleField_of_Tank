// This include:
#include "scenebouncingballs.h"

// Local includes:
#include "renderer.h" 
#include "ball.h"
#include "imgui/imgui.h"

// Library includes:
#include <cassert> 
#include <cstdlib>

SceneBouncingBalls::SceneBouncingBalls()
	: m_pBalls{ 0 }
	, m_iShowCount(0)
{

}

SceneBouncingBalls::~SceneBouncingBalls()
{
	for (int k = 0; k < 100; ++k)
	{
		delete m_pBalls[k]; 
		m_pBalls[k] = 0;
	}
}

bool
SceneBouncingBalls::Initialise(Renderer& renderer)
{
	for (int k = 0; k < 100; ++k)
	{
		m_pBalls[k] = new Ball();
		m_pBalls[k]->Initialise(renderer);

		m_pBalls[k]->RandomiseColour();
	}

	// Always place one ball at the centre...
	m_pBalls[0]->Position().x = renderer.GetWidth() / 2.0f;
	m_pBalls[0]->Position().y = renderer.GetHeight() / 2.0f;
	m_iShowCount = 100; 

	return true;
}

void
SceneBouncingBalls::Process(float deltaTime)
{
	for (int k = 0; k < m_iShowCount; ++k)
	{
		m_pBalls[k]->Process(deltaTime);
	}
}

void
SceneBouncingBalls::Draw(Renderer& renderer)
{
	for (int k = 0; k < m_iShowCount; ++k)
	{
		m_pBalls[k]->Draw(renderer);
	}
}

void SceneBouncingBalls::DebugDraw()
{
	ImGui::Text("Scene: Bouncing Balls");
	ImGui::SliderInt("Show Count", &m_iShowCount, 1, 100);
	static int editBallNumber = 0;
	ImGui::SliderInt("Edit ball", &editBallNumber, 0, 99);

	m_pBalls[editBallNumber]->DebugDraw();
}