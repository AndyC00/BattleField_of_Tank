//local includes:
#include "Enemy.h"
#include "sprite.h"
#include "Bullet.h"

//library includes:
#include "renderer.h"
#include <cstdlib>
#include <cmath>
#include <iostream>

const int bulletTimerTotal = 2;

Enemy::Enemy()
	: Entity()
	, m_rotationTimer(0.0f)
	, m_rotationDuration(0.5f) //take 0.5s to rotate
	, m_isRotating(false)
	, m_startAngle(0.0f)
	, m_targetAngle(0.0f)
	, bullet(nullptr)
	, m_bulletTimer(0)
{

}

Enemy::~Enemy()
{
	delete bullet;
}

bool Enemy::Initialise(Renderer& renderer)
{
	//std::vector<const char*> filenames = {
	//		"Sprites\\Enemies\\hummel.png",
	//		"Sprites\\Enemies\\panther.png",
	//		"Sprites\\Enemies\\panzer4.png",
	//		"Sprites\\Enemies\\stug3.png",
	//		"Sprites\\Enemies\\tiger.png",
	//};

	//int selection = rand() % filenames.size();

	//const char* pcFilename = filenames[selection];

	const char* pcFilename = "Sprites\\Tanks\\tank.png";

	if (!Entity::Initialise(renderer, pcFilename))
	{
		std::cout << "Enemy failed to spawn" << std::endl;
		return false;
	}

	int m_x = ((rand() % 2 == 0) ? (rand() % 890 + 10) : (rand() % 890 + 910));
	int m_y = ((rand() % 2 == 0) ? (rand() % 490 + 10) : (rand() % 480 + 550));

	m_pSprite->SetScale(0.2f);
	m_position = Vector2(m_x, m_y);
	m_velocity = Vector2(0.0f, 0.0f);

	bullet = new Bullet();
	bullet->Initialise(renderer);

	return true;
}

void Enemy::Process(float deltaTime)
{
	if (IsAlive())
	{
		if (!m_isRotating)
		{
			m_rotationTimer += deltaTime;

			//go back if near the edge of the screen
			if (IsNearBoundary(m_position))
			{
				m_isRotating = true;
				m_startAngle = m_pSprite->GetAngle();

				Vector2 center(930.0f, 530.0f); // the center of the screen (1860/2, 1060/2)
				Vector2 direction = center - m_position;
				float angleToCenter = atan2(direction.y, direction.x) * 180.0f / M_PI - 90.0f;

				m_targetAngle = angleToCenter;
				m_rotationTimer = 0.0f;
			}

			// rotate every 1.5s
			if (m_rotationTimer >= 1.5f)
			{
				m_isRotating = true;
				m_startAngle = m_pSprite->GetAngle();
				//rotate 45 degrees every time
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

		if (m_bulletTimer < 0)
		{
			bullet->SetPosition(m_position, m_pSprite->GetAngle());
			m_bulletTimer = bulletTimerTotal;
		}
		else
		{
			bullet->Process(deltaTime);
			m_bulletTimer -= deltaTime;
		}

		Entity::Process(deltaTime);
	}
}

void Enemy::Draw(Renderer& renderer)
{
	bullet->Draw(renderer);
	Entity::Draw(renderer);
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
		float angleInRadians = currentAngle * M_PI / 180.0f + 90.0f;
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

Bullet* Enemy::GetBullet()
{
	return bullet;
}

bool Enemy::IsNearBoundary(Vector2 m_position)
{
	float margin = 35.0f; //the distance to trigger the function

	return (m_position.x <= margin || m_position.x >= 1860.0f - margin ||
		m_position.y <= margin || m_position.y >= 1060.0f - margin);
}