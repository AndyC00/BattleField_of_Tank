#ifndef   Clouds_H 
#define   Clouds_H 

#include "Entity.h"
#include "animatedsprite.h"

#include <vector>


class Clouds
{
public:
	Clouds();
	~Clouds();

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

private:
	Renderer* m_pRenderer;
	std::vector <Entity*> m_clouds;
};

#endif //   Clouds_H