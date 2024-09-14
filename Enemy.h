#include "Entity.h"

#include <vector>
#include <memory>
#include "Bullet.h"

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();

	// Initialise multiple enemies
	bool Initialise(Renderer& renderer);

	void Process(float deltaTime);

	// Function to spawn enemies at random positions
	void SpawnEnemies(int numEnemies);

	// Randomly rotate the enemy
	void RotateRandomly();

	void RotateOverTime(float deltaTime);

	void ShootBullet();

	void Draw(Renderer& renderer);
	Bullet* GetBullet();

private:
	float m_rotationTimer;
	float m_rotationDuration;
	bool m_isRotating;
	float m_startAngle;
	float m_targetAngle;
	float m_bulletTimer;
	float m_bulletInterval;

	Bullet* bullet;
};
