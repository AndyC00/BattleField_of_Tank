#ifndef  SCENEBALLGAME_H   
#define  SCENEBALLGAME_H 

// Local includes:
#include "scene.h"
#include "inputsystem.h"

// Library includes:
#include <vector>
#include "fmod.hpp"

// Forward declarations:
class Renderer; 
class Ball;

// Class declaration:
class SceneBallGame : public Scene
{
// Member methods: 
public:
	SceneBallGame();
	virtual ~SceneBallGame();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);


	virtual void DebugDraw();

protected:
	void SpawnGoodBalls(int number);
	void SpawnBadBalls(int number);

	void CheckCollisions();
	bool BallVsBall(Ball* p1, Ball* p2);

private:
	SceneBallGame(const SceneBallGame& sceneballgame);
	SceneBallGame& operator=(const SceneBallGame& sceneballgame);

	// Member data: 
public:


protected:
	Renderer* m_pRenderer;

	std::vector<Ball*> m_pGoodBalls;
	std::vector<Ball*> m_pBadBalls;

	Ball* m_pPlayerBall; 

private:
	FMOD::Sound* hitsound1;
	FMOD::Sound* hitsound2;
	FMOD::Channel* channel;
};

#endif //   SCENEBALLGAME_H 
