//local includes:
#include "Enemy.h"
#include "sprite.h"

//library includes:
#include "renderer.h"
#include <cstdlib>

Enemy::Enemy()
	: Entity()
	, m_rotationTimer(0.0f)
{

}

Enemy::~Enemy()
{

}

bool Enemy::Initialise(Renderer& renderer)
{
	if (!Entity::Initialise(renderer))
	{
		printf("Enemy failed to spawn\n");
		return false;
	}

	m_position = Vector2(rand() % 1810, rand() % 1000);

	m_velocity = Vector2(0.0f, 0.0f);

	return true;
}


void Enemy::Process(float deltaTime)
{
	if (IsAlive())
	{
		m_rotationTimer += deltaTime;
		//rotate every 1.5s
		if (m_rotationTimer >= 1.5f)
		{
			// stop while rotate
			m_velocity = Vector2(0.0f, 0.0f);
			RotateRandomly();
			m_rotationTimer = 0.0f;
		}
		else
		{
			m_position += m_velocity * deltaTime;
		}



		Entity::Process(deltaTime);
	}
}

void Enemy::RotateRandomly()
{
	int randomDirection = (rand() % 2 == 0) ? -45 : 45;

	float currentAngle = m_pSprite->GetAngle();

	float newAngle = currentAngle + randomDirection;

	this->Rotate(newAngle);

	float angleInRadians = newAngle * 3.14159f / 180.0f;

	m_velocity.x = cos(angleInRadians) * 10.0f;          // speed control
	m_velocity.y = sin(angleInRadians) * 10.0f;
}