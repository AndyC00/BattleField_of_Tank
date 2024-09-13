#include "enemy.h"
#include "renderer.h"
#include "vector2.h"
#include <cstdlib>
#include <ctime>

Enemy::Enemy()
{
    // Seed random once for the entire game
    srand(static_cast<unsigned>(time(0)));
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
        if (!newEnemy->Initialise(renderer))
        {
            return false;
        }
        // Spawn at random position within screen
        newEnemy->m_position = Vector2(rand() % 800, rand() % 600); // Assuming 800x600 screen resolution
        m_enemies.push_back(newEnemy);
    }

    return true;
}

void Enemy::Process(float deltaTime)
{
    for (Enemy* enemy : m_enemies)
    {
        // Random rotation
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
        newEnemy->m_position = Vector2(rand() % 800, rand() % 600); // Random position on screen
        m_enemies.push_back(newEnemy);
    }
}
