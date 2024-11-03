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
	std::vector <Entity*> m_clouds;
	Renderer* m_pRenderer;
};
