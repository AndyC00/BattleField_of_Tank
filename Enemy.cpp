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

Enemy::Enemy(Player* player)
	: Entity()
	, m_pPlayer(player)
	, m_pRenderer(nullptr)
	, m_rotationTimer(0.0f)
	, m_rotationDuration(0.5f) //take 0.5s to rotate
	, m_isRotating(false)
	, m_startAngle(0.0f)
	, m_targetAngle(0.0f)
	, bullet(nullptr)
	, m_bulletTimer(0)
	, m_attackRange(150.0f)
{

}

Enemy::~Enemy()
{
	delete bullet;
	bullet = nullptr;
}

bool Enemy::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;

	float ScreenWidth = m_pRenderer->GetWidth();
	float ScreenHeight = m_pRenderer->GetHeight();

	std::vector<const char*> filenames = {
			"Sprites\\Enemies\\StuG III.png",
			"Sprites\\Enemies\\Panzer III.png",
	};

	int selection = rand() % filenames.size();

	const char* pcFilename = filenames[selection];

	if (!Entity::Initialise(renderer, pcFilename))
	{
		std::cout << "Enemy failed to spawn" << std::endl;
		return false;
	}

	int m_x = (rand() % (renderer.GetWidth() - 50) + 100);
	int m_y = (rand() % (renderer.GetHeight() / 2) + 50);

	m_pSprite->SetScale(0.2f);

	m_pSprite->SetBlueTint(0.85f);
	m_pSprite->SetGreenTint(0.85f);
	m_pSprite->SetRedTint(0.85f);

	m_position = Vector2(m_x, m_y);
	m_velocity = Vector2(0.0f, 0.0f);

	bullet = new Bullet();
	bullet->Initialise(renderer);

	return true;
}

void Enemy::Process(float deltaTime)
{
	if (!IsAlive()) return;

	if (!m_isRotating)
	{
		m_rotationTimer += deltaTime;

		m_isRotating = true;
		m_startAngle = m_pSprite->GetAngle();

		//go back if near the edge of the screen
		if (IsNearBoundary(m_position))
		{
			Vector2 center(ScreenWidth / 2, ScreenHeight / 2);
			Vector2 direction = center - m_position;
			float angleToCenter = atan2(direction.y, direction.x) * 180.0f / M_PI - 90.0f;

			m_targetAngle = angleToCenter;
			m_rotationTimer = 0.0f;
		}

		// rotate every 1.5s
		if (m_rotationTimer >= 1.5f)
		{
			if (IsWithinRange())
			{
				Vector2 direction = m_pPlayer->GetPosition() - m_position;
				m_targetAngle = atan2(direction.y, direction.x) * 180.0f / M_PI - 90.0f;
			}
			else
			{
				//rotate randomly 20-70 degrees every time
				float rotateDegree = 20 + rand() % (70 - 20 + 1);
				m_targetAngle = m_startAngle + ((rand() % 2 == 0) ? rotateDegree : -rotateDegree);
			}
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

	//bullet process:
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

Bullet* Enemy::GetBullet()
{
	return bullet;
}

bool Enemy::IsNearBoundary(Vector2 m_position)
{
	float margin = 35.0f; //the distance to trigger the function

	return (m_position.x <= margin || m_position.x >= ScreenWidth - margin || m_position.y <= margin || m_position.y >= ScreenHeight - margin);
}

bool Enemy::IsWithinRange()
{
	if (m_pPlayer == nullptr)
	{
		std::cout << "Can't find Player!" << std::endl;
		return false;
	}
	Vector2 PlayerPosition = m_pPlayer->GetPosition();

	float deltaX = m_position.x - PlayerPosition.x;
	float deltaY = m_position.y - PlayerPosition.y;
	float distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);

	return distance <= m_attackRange;
}