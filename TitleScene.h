#ifndef _TITLESCENE_H_
#define _TITLESCENE_H_

//local includes:
#include "scene.h"
#include "game.h"

//library includes:
#include <vector>

#include "fmod.hpp"
#include "soundsystem.h"

//Forward declarations:
class Renderer;
class Sprite;

//Class declaration:
class TitleScene: public Scene
{
	//member methods:
public:
	TitleScene();
	virtual ~TitleScene();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);
	void OnExit() override;

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

	FMOD::Sound* titleSound;
	FMOD::Channel* titleChannel;
};

#endif // _TITLESCENE_H_
