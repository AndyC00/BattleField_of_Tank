#include "Entity.h"

#include <vector>
#include <memory>
#include "Bullet.h"
#include "Player.h"

class Boss: public Entity
{
public:
	Boss(Player* player);
	~Boss();

	// Initialise multiple enemies
	bool Initialise(Renderer& renderer);

	void Process(float deltaTime);

	// Randomly rotate the enemy
	void RotateRandomly();

	void RotateOverTime(float deltaTime);

	void Draw(Renderer& renderer);
	Bullet* GetBullet();

private:
	float m_rotationTimer;
	float m_rotationDuration;
	bool m_isRotating;
	float m_startAngle;
	float m_targetAngle;
	float m_bulletTimer;
	float m_attackRange;
	float ScreenWidth;
	float ScreenHeight;

	bool IsNearBoundary(Vector2 m_position);
	bool IsWithinRange();

	Bullet* bullet;
	Player* m_pPlayer;
	Renderer* m_pRenderer;
};
