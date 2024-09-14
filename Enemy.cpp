#include "Enemy.h"

#include "renderer.h"
#include <cstdlib>

Enemy::Enemy()
    : Entity()
{

}

Enemy::~Enemy()
{

}

bool Enemy::Initialise(Renderer& renderer, int numEnemies)
{

    for (int i = 0; i < numEnemies; ++i)
    {
        std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
        if (!newEnemy->Entity::Initialise(renderer))
        {
            m_enemies.clear();
            return false;
        }

        newEnemy->m_position = Vector2(rand() % 1810, rand() % 1000);

        m_enemies.push_back(std::move(newEnemy));
    }

    return true;
}


void Enemy::Process(float deltaTime)
{
	for (auto& enemy : m_enemies)
	{
        if (enemy->IsAlive())
        {
            enemy->RotateRandomly();

            enemy->m_position += enemy->m_velocity * deltaTime;

            enemy->Entity::Process(deltaTime);
        }
	}
}

void Enemy::RotateRandomly()
{
	int randomDirection = (rand() % 2 == 0) ? -45 : 45;
	this->Rotate(static_cast<float>(randomDirection));
}