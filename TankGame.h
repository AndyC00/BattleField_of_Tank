#ifndef  TANKGAME_H   
#define  TANKGAME_H 

// Local includes:
#include "renderer.h"
#include "scene.h"
#include "inputsystem.h"
#include "animatedsprite.h"

// Library includes:
#include <vector>
#include "fmod.hpp"

// Forward declarations:
class Renderer;
class Entity;
class Enemy;


// Class declaration:
class SceneTankGame : public Scene
{
	// Member methods: 
public:
	SceneTankGame();
	virtual ~SceneTankGame();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);
	virtual void DebugDraw();

protected:
	void CheckCollisions();


private:
	SceneTankGame(const SceneTankGame& SceneTankGame);
	SceneTankGame& operator=(const SceneTankGame& SceneTankGame);

	// Member data: 
public:


protected:
	Renderer* m_pRenderer;
	Entity* m_pPlayer;
	AnimatedSprite* pAnimatedSprite;
	std::vector<Enemy*> m_pEnemies;

private:
	FMOD::Sound* hitsound1;
	FMOD::Sound* hitsound2;
	FMOD::Sound* opening;
	FMOD::Channel* channel;

};

#endif //   TANKGAME_H 
