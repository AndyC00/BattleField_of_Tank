#ifndef  TANKGAME_H   
#define  TANKGAME_H 

// Local includes:
#include "renderer.h"
#include "scene.h"
#include "inputsystem.h"
#include "animatedsprite.h"
#include "game.h"
#include "Bullet.h"
#include "Player.h"
#include "Trap.h"

// Library includes:
#include <vector>
#include "fmod.hpp"

// Forward declarations:
class Renderer;
class Entity;
class Enemy;
class AnimatedSprite;


// Class declaration:
class SceneTankGame : public Scene
{
	// Member methods: 
public:
	SceneTankGame();
	virtual ~SceneTankGame();

	void OnSceneChange(int* sceneIndex);
	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);
	virtual void DebugDraw();

protected:
	void CheckCollisions();


private:
	SceneTankGame(const SceneTankGame& SceneTankGame);
	SceneTankGame& operator=(const SceneTankGame& SceneTankGame);
	void UpdateExplosions(float deltaTime);
	void CreateExplosion(float x, float y);
	void ReceiveDamage(int num);

	// Member data: 
public:


protected:
	Renderer* m_pRenderer;
	std::vector<AnimatedSprite*> m_explosions;
	std::vector<Enemy*> m_pEnemies;
	std::vector<Trap*> m_Traps;
	Sprite* m_pBackground;

private:
	FMOD::Sound* hitsound1;
	FMOD::Sound* hitsound2;
	FMOD::Sound* opening;
	FMOD::Channel* channel;

	Player* m_pPlayer;

	int* m_sceneIndex;
};

#endif //   TANKGAME_H 
