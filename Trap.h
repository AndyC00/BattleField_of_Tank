#ifndef   Trap_H 
#define   Trap_H 

#include "Entity.h"
#include "animatedsprite.h"

#include <vector>


class Trap: public Entity
{
public:
	Trap();
	~Trap();

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	float GetRadius() const override;

private:
	AnimatedSprite* m_trap;
	Renderer* m_pRenderer;
};

#endif //   Trap_H