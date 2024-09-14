#ifndef _LOSE_H_
#define _LOSE_H_

//local includes:
#include "scene.h"

//Forward declarations:
class Renderer;
class Sprite;

//Class declaration:
class LoseScene :public Scene
{
	//member methods:
public:
	LoseScene();
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

	float m_angle;
	float m_rotationSpeed;

private:

};

#endif // _LOSE_H_
