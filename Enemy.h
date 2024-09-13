#include "Entity.h"
#include <vector>

class Enemy : public Entity
{
public:
    Enemy();
    ~Enemy();

    // Initialise multiple enemies
    bool Initialise(Renderer& renderer, int numEnemies);

    // Override the Process function to handle specific movement for enemies
    void Process(float deltaTime) override;

    // Function to spawn enemies at random positions
    void SpawnEnemies(int numEnemies);

    // Randomly rotate the enemy
    void RotateRandomly();

private:
    std::vector<Enemy*> m_enemies; // List of all enemy instances
};
