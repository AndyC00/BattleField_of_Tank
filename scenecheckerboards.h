#ifndef _SCENECHECKERBOARDS_H_
#define _SCENECHECKERBOARDS_H_

//local includes:
#include "scene.h"

//Forward declarations:
class Renderer;
class Sprite;

//Class declaration:
class SceneCheckerboards :public Scene
{
	//member methods:
public:
	SceneCheckerboards();
	virtual ~SceneCheckerboards();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

protected:

private:
	SceneCheckerboards(const SceneCheckerboards& sceneCheckerboards);
	SceneCheckerboards& operator = (const SceneCheckerboards& sceneCheckerboards);

//member data:
public:
	virtual void DebugDraw();
protected:
	Sprite* m_pCorners[4];
	Sprite* m_pCentre;

	float m_angle;
	float m_rotationSpeed;

private:

};

#endif // _SCENECHECKERBOARDS_H_
