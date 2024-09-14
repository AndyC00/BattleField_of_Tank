#ifndef _WIN_H_
#define _WIN_H_

//local includes:
#include "scene.h"

//Forward declarations:
class Renderer;
class Sprite;

//Class declaration:
class WinScene :public Scene
{
	//member methods:
public:
	WinScene();
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

	float m_angle;
	float m_rotationSpeed;

private:

};

#endif // _WIN_H_
