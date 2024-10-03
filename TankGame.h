#ifndef  TANKGAME_H   
#define  TANKGAME_H 

// Local includes:
#include "renderer.h"
#include "scene.h"
#include "inputsystem.h"
#include "animatedsprite.h"
#include "game.h"
#include "Bullet.h"

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
	float NormalizeAngle(float angle);
	void UpdateExplosions(float deltaTime);
	void CreateExplosion(float x, float y);

	// Member data: 
public:


protected:
	Renderer* m_pRenderer;
	Entity* m_pPlayer;
	std::vector<AnimatedSprite*> m_explosions;
	std::vector<Enemy*> m_pEnemies;
	Sprite* m_pBackground;

private:
	FMOD::Sound* hitsound1;
	FMOD::Sound* hitsound2;
	FMOD::Sound* opening;
	FMOD::Channel* channel;

	int* m_sceneIndex;
	Bullet* PlayerBullet;
};

#endif //   TANKGAME_H 
