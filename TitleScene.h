#ifndef _TITLESCENE_H_
#define _TITLESCENE_H_

//local includes:
#include "scene.h"

//Forward declarations:
class Renderer;
class Sprite;

//Class declaration:
class TitleScene:public Scene
{
	//member methods:
public:
	TitleScene();
	virtual ~TitleScene();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

protected:

private:
	TitleScene(const TitleScene& TitleScene);
	TitleScene& operator = (const TitleScene& TitleScene);

	//member data:
public:
	virtual void DebugDraw();

protected:
	Sprite* m_pCentre;

	float m_angle;
	float m_rotationSpeed;

private:

};

#endif // _TITLESCENE_H_
