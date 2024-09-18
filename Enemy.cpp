//local includes:
#include "Enemy.h"
#include "sprite.h"
#include "Bullet.h"

//library includes:
#include "renderer.h"
#include <cstdlib>
#include <cmath>

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
	if (!Entity::Initialise(renderer))
	{
		printf("Enemy failed to spawn\n");
		return false;
	}

	int m_x = ((rand() % 2 == 0) ? (rand() % 910 + 10) : (rand() % 910 + 940));
	int m_y = ((rand() % 2 == 0) ? (rand() % 910 + 10) : (rand() % 910 + 940));

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