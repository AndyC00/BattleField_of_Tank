#include "Entity.h"

#include <vector>


class Trap: public Entity
{
public:
	Trap();
	~Trap();

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

private:
	AnimatedSprite* m_trap;
	Renderer* m_pRenderer;
};
