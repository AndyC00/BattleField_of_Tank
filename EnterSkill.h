#ifndef   EnterSkill_H 
#define   EnterSkill_H 

#include "Entity.h"

#include <vector>
#include <string>

class EnterSkill
{
public:
	EnterSkill();
	~EnterSkill();

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	void Activate();

private:
	void SpawnBoomer(int index);

	Renderer* m_pRenderer;
	std::vector <Entity*> m_Boomers;
	std::vector<std::string> planeFiles;

	float spawnInterval;
	float planeTimer;
	int m_currentBoomerIndex;
	bool m_isActive;
};

#endif //   EnterSkill_H