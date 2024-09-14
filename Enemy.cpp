//local includes:
#include "Enemy.h"
#include "sprite.h"

//library includes:
#include "renderer.h"
#include <cstdlib>

Enemy::Enemy()
	: Entity()
	, m_rotationTimer(0.0f)
	, m_rotationDuration(0.5f) //take 0.5s to rotate
	, m_isRotating(false)
	, m_startAngle(0.0f)
	, m_targetAngle(0.0f)
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
		if (!m_isRotating)
		{
			m_rotationTimer += deltaTime;
			// rotate every 1.5s
			if (m_rotationTimer >= 1.5f)
			{
				m_isRotating = true;
				m_startAngle = m_pSprite->GetAngle();

				m_targetAngle = m_startAngle + ((rand() % 2 == 0) ? -45.0f : 45.0f);
				m_rotationTimer = 0.0f;
			}
			else
			{
				m_position += m_velocity * deltaTime;
			}
		}
		else
		{
			RotateOverTime(deltaTime);
		}

		Entity::Process(deltaTime);
	}
}

void Enemy::RotateOverTime(float deltaTime)
{
	float angleDifference = m_targetAngle - m_startAngle;
	float rotationStep = angleDifference * (deltaTime / m_rotationDuration);

	float currentAngle = m_pSprite->GetAngle();
	currentAngle += rotationStep;

	if ((angleDifference > 0 && currentAngle >= m_targetAngle) || (angleDifference < 0 && currentAngle <= m_targetAngle))
	{
		currentAngle = m_targetAngle; 
		m_isRotating = false;

		float angleInRadians = currentAngle * 3.14159f / 180.0f;
		m_velocity.x = cos(angleInRadians) * 10.0f;  // speed control
		m_velocity.y = sin(angleInRadians) * 10.0f;
	}

	this->Rotate(currentAngle);
}


void Enemy::RotateRandomly()
{
	int randomDirection = (rand() % 2 == 0) ? -45 : 45;

	m_targetAngle = m_pSprite->GetAngle() + randomDirection;
}
