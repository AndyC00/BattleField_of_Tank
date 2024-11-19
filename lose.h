#ifndef _LOSE_H_
#define _LOSE_H_

//local includes:
#include "scene.h"
#include "InputSystem.h"
#include "game.h"

#include "fmod.hpp"
#include "soundsystem.h"

//Forward declarations:
class Renderer;
class Sprite;

//Class declaration:
class LoseScene :public Scene
{
	//member methods:
public:
	LoseScene(Game* game);
	virtual ~LoseScene();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

protected:

private:
	LoseScene(const LoseScene& LoseScene);
	LoseScene& operator = (const LoseScene& LoseScene);

	//member data:
public:
	virtual void DebugDraw();

protected:
	Sprite* m_pCentre;
	Sprite* m_quit;
	Sprite* m_restart;

	float m_angle;
	float m_rotationSpeed;
	Renderer* m_pRenderer;

private:
	Game* m_game;

	FMOD::Sound* loseSound;
	FMOD::Channel* loseChannel;
};

#endif // _LOSE_H_
