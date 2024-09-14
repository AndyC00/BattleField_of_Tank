#include "Entity.h"

#include <vector>
#include <memory>

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

private:
	float m_rotationTimer;
	float m_rotationDuration;
	bool m_isRotating;
	float m_startAngle;
	float m_targetAngle;
};
