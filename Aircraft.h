#include "Entity.h"
#include "animatedsprite.h"

#include <vector>
#include <string>

class Aircraft
{
public:
	Aircraft();
	~Aircraft();

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	void SpawnPlane();

	float planeTimer;

private:
	Renderer* m_pRenderer;
	std::vector <Entity*> m_planes;
	std::vector <Entity*> m_activePlanes;
	std::vector<std::string> planeFiles;

	float spawnInterval;
};
