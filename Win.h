#ifndef _WIN_H_
#define _WIN_H_

//local includes:
#include "scene.h"
#include "InputSystem.h"
#include "game.h"

//Forward declarations:
class Renderer;
class Sprite;

//Class declaration:
class WinScene :public Scene
{
	//member methods:
public:
	WinScene(Game* game);
	virtual ~WinScene();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

protected:

private:
	WinScene(const WinScene& WinScene);
	WinScene& operator = (const WinScene& TitleScene);

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

};

#endif // _WIN_H_
