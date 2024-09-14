#include "Enemy.h"
#include "renderer.h"
#include "vector2.h"
#include <cstdlib>

Enemy::Enemy()
{
    
}

Enemy::~Enemy()
{
    for (Enemy* enemy : m_enemies)
    {
        delete enemy;
    }
    m_enemies.clear();
}

bool Enemy::Initialise(Renderer& renderer, int numEnemies)
{
    for (int i = 0; i < numEnemies; ++i)
    {
        Enemy* newEnemy = new Enemy();
        if (!newEnemy->Initialise(renderer, numEnemies))
        {
            return false;
        }
        newEnemy->m_position = Vector2(rand() % 1810, rand() % 1000); // within 1860x1050 screen resolution
        m_enemies.push_back(newEnemy);
    }

    return true;
}

void Enemy::Process(float deltaTime)
{
    for (Enemy* enemy : m_enemies)
    {
        enemy->RotateRandomly();

        // Move with velocity
        enemy->m_position += enemy->m_velocity * deltaTime;

        // Call base class Process for other logic
        enemy->Entity::Process(deltaTime);
    }
}

void Enemy::RotateRandomly()
{
    // Randomly choose to rotate -45 or +45 degrees
    int randomDirection = (rand() % 2 == 0) ? -45 : 45;
    this->Rotate(static_cast<float>(randomDirection));
}

void Enemy::SpawnEnemies(int numEnemies)
{
    // Called to spawn enemies
    for (int i = 0; i < numEnemies; ++i)
    {
        // Create and initialise enemy instances
        Enemy* newEnemy = new Enemy();
        newEnemy->m_position = Vector2(rand() % 1810, rand() % 1000);
        m_enemies.push_back(newEnemy);
    }
}
