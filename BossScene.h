#ifndef  BOSSSCENE_H   
#define  BOSSSCENE_H 

// Local includes:
#include "renderer.h"
#include "scene.h"
#include "inputsystem.h"
#include "animatedsprite.h"
#include "game.h"
#include "Bullet.h"
#include "Player.h"
#include "Trap.h"
#include "Clouds.h"
#include "Aircraft.h"
#include "EnterSkill.h"

// Library includes:
#include <vector>
#include "fmod.hpp"

// Forward declarations:
class Renderer;
class Entity;
class Enemy;
class AnimatedSprite;


// Class declaration:
class BossScene : public Scene
{
	// Member methods: 
public:
	BossScene(Game* game, int difficulty);
	virtual ~BossScene();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);
	virtual void DebugDraw();
	void OnExit() override;

protected:
	void CheckCollisions();


private:
	BossScene(const BossScene& SceneTankGame);
	BossScene& operator=(const BossScene& SceneTankGame);
	void UpdateExplosions(float deltaTime);
	void CreateExplosion(float x, float y);
	void UpdateBoom(float deltaTime);
	void CreateBoom(float x, float y);
	void ReceiveDamage(int num);
	void SetButtonOn();
	void SetButtonOff();

	// Member data: 
public:


protected:
	Renderer* m_pRenderer;
	std::vector<AnimatedSprite*> m_explosions;
	std::vector<AnimatedSprite*> m_allFire;
	std::vector<Enemy*> m_pEnemies;
	std::vector<Trap*> m_Traps;
	Sprite* m_pBackground;
	Sprite* m_pSkillButton;

private:
	FMOD::Sound* hitsound1;
	FMOD::Sound* hitsound2;
	FMOD::Sound* opening;
	FMOD::Sound* bgSound;
	FMOD::Sound* boomerSound;
	FMOD::Sound* skillExplosion;
	FMOD::Channel* skillChannel;
	FMOD::Channel* boomerChannel;
	FMOD::Channel* damageReceiveChannel;
	FMOD::Channel* explosionChannel;
	FMOD::Channel* musicChannel;
	FMOD::Channel* bgChannel;

	float skillTimer;
	float skillInterval;
	float waitTimer;
	float waitInterval;
	bool isWaitTimeActive;

	int difficulty;

	Player* m_pPlayer;
	Game* m_game;

	Clouds m_clouds;
	Aircraft m_aircraft;
	EnterSkill m_skill;
};

#endif //   BOSSSCENE_H 
