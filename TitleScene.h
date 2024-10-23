#ifndef _TITLESCENE_H_
#define _TITLESCENE_H_

//local includes:
#include "scene.h"

//library includes:
#include <vector>


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
	std::vector<Sprite*> m_pictures;

private:
	int m_CurrentPicture;

};

#endif // _TITLESCENE_H_
