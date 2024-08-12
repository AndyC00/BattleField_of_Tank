// This include:
#include "ball.h"

// Local includes:
#include "renderer.h" 
#include "sprite.h" 
#include "inlinehelpers.h"

// Library includes:
#include <cassert> 
#include <cstdlib>

// Static members:
float Ball::sm_fBoundaryWidth = 0.0f; 
float Ball::sm_fBoundaryHeight = 0.0f;

Ball::Ball(): 
	m_pSprite(0)
{

}

Ball::~Ball()
{
	delete m_pSprite; 
	m_pSprite = 0;
}

bool
Ball::Initialise(Renderer& renderer)
{
	m_pSprite = renderer.CreateSprite("Sprites\\ball.png");

	RandomiseSize();

	const float MAX_SPEED = 250.0f;
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);

	m_position.x = static_cast<float>(GetRandom(EDGE_LIMIT, SCREEN_WIDTH - EDGE_LIMIT));
	m_position.y = static_cast<float>(GetRandom(EDGE_LIMIT, SCREEN_HEIGHT - EDGE_LIMIT));

	m_velocity.x = GetRandomPercentage() * MAX_SPEED * GetPositiveOrNegative();
	m_velocity.y = GetRandomPercentage() * MAX_SPEED * GetPositiveOrNegative();

	ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	return true;
}

void
Ball::Process(float deltaTime)
{
	if (m_position.x >= (m_boundaryHigh.x))
	{
		m_position.x = m_boundaryHigh.x;
		m_velocity.x *= -1.0f;
	}
	else if (m_position.x <= (m_boundaryLow.x))
	{
		m_position.x = m_boundaryLow.x;
		m_velocity.x *= -1.0f;
	}

	if (m_position.y >= (m_boundaryHigh.y))
	{
		m_position.y = m_boundaryHigh.y;
		m_velocity.y *= -1.0f;
	}
	else if (m_position.y <= (m_boundaryLow.y))
	{
		m_position.y = m_boundaryLow.y;
		m_velocity.y *= -1.0f;
	}

	m_position += m_velocity * deltaTime;
	m_pSprite->SetX(static_cast<int>(m_position.x));
	m_pSprite->SetY(static_cast<int>(m_position.y));
	m_pSprite->Process(deltaTime);
}

void Ball::Draw(Renderer & renderer)
{
	//I assigned it to true to avoid printing error
	if (m_bAlive = true)
	{
		m_pSprite->Draw(renderer);
	}
}

Vector2& Ball::Position()
{
	return m_position;
}

void Ball::RandomiseColour()
{
	m_pSprite->SetRedTint(GetRandomPercentage()); 
	m_pSprite->SetGreenTint(GetRandomPercentage()); 
	m_pSprite->SetBlueTint(GetRandomPercentage());
}

void Ball::RandomiseSize()
{
	float scale = GetRandomPercentage();
	scale *= 1.25f;
	m_pSprite->SetScale(scale);
}

void Ball::ComputeBounds(int width, int height)
{
	m_boundaryLow.x = (m_pSprite->GetWidth() / 2.0f); 
	m_boundaryLow.y = (m_pSprite->GetHeight() / 2.0f);

	m_boundaryHigh.x = width - (m_pSprite->GetWidth() / 2.0f); 
	m_boundaryHigh.y = height - (m_pSprite->GetHeight() / 2.0f);
}
