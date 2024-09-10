#ifndef _INSTRUCTIONSCENE_H_
#define _INSTRUCTIONSCENE_H_

//local includes:
#include "scene.h"

//Forward declarations:
class Renderer;
class Sprite;

//Class declaration:
class InstructionScene :public Scene
{
	//member methods:
public:
	InstructionScene();
	virtual ~InstructionScene();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

protected:

private:
	InstructionScene(const InstructionScene& InstructionScene);
	InstructionScene& operator = (const InstructionScene& InstructionScene);

	//member data:
public:
	virtual void DebugDraw();

protected:
	Sprite* m_pCentre;

	float m_angle;
	float m_rotationSpeed;

private:

};

#endif // _INSTRUCTIONSCENE_H_
