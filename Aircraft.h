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
	void SpawnAxisPlane();

	float planeTimer;
	float axisPlaneTimer;

private:
	Renderer* m_pRenderer;
	std::vector <Entity*> m_alliesPlanes;
	std::vector <Entity*> m_axisPlanes;
	std::vector<std::string> planeFiles;
	std::vector<std::string> axisPlaneFiles;

	float spawnInterval;
	float axisSpawnInterval;
};
