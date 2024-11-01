//local includes:
#include "Trap.h"


//library includes:
#include "renderer.h"
#include <cstdlib>
#include <cmath>
#include <iostream>


Trap::Trap()
	: Entity(),
	m_pRenderer(nullptr),
	m_trap(nullptr)
{

}

Trap::~Trap()
{

}

bool Trap::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;
	m_trap = m_pRenderer->CreateAnimatedSprite("Sprites\\trap.png");

	m_trap->SetupFrames(112, 60);
	m_trap->SetScale(1.0f);
	m_trap->SetFrameDuration(0.1f);
	m_trap->SetLooping(true);
	m_trap->Animate();

	int m_x = ((rand() % 2 == 0) ? (rand() % 890 + 10) : (rand() % 890 + 910));
	int m_y = ((rand() % 2 == 0) ? (rand() % 490 + 10) : (rand() % 480 + 550));

	m_position = Vector2(m_x, m_y);

	m_trap->SetX(static_cast<int>(m_x));
	m_trap->SetY(static_cast<int>(m_y));

	return true;
}

void Trap::Process(float deltaTime)
{
	if (!IsAlive()) return;

	m_trap->Process(deltaTime);
}

void Trap::Draw(Renderer& renderer)
{
	m_trap->Draw(renderer);
}

float Trap::GetRadius() const
{
	if (IsAlive())
	{
		int frameWidth = 112;
		int frameHeight = 60;

		float scale = m_trap->GetScale();

		float actualWidth = frameWidth * scale;
		float actualHeight = frameHeight * scale;

		float radius = (actualWidth + actualHeight) / 4.0f;

		return radius;
	}
}