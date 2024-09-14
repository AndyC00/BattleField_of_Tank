#ifndef   GAME_H 
#define   GAME_H 

//Library includes:
#include <vector>

#include "inputsystem.h"
#include "soundsystem.h"

// Forward declarations: 
class Renderer;
class Sprite;
class Scene;

class Game
{
	// Member methods:
public:
	//declare the font:
	Sprite* m_pZapPow[4];

	static Game& GetInstance(); 
	static void DestroyInstance();

	bool Initialise(); 
	bool DoGameLoop(); 
	void Quit();

protected:
	void Process(float deltaTime); 
	void Draw(Renderer& renderer);

	void ProcessFrameCounting(float deltaTime);

private:
	Game();
	~Game();
	Game(const Game& game);
	Game& operator = (const Game& game);

	// Member data: 
public:
	void ToggleDebugWindow();
	static SoundSystem* pSoundsystem;

protected:
	bool m_bShowDebugWindow;

	void DebugDraw();

	static Game* sm_pInstance; 
	Renderer* m_pRenderer;

	//Scene:
	std::vector<Scene*> m_scenes;
	int m_iCurrentScene;

	__int64 m_iLastTime; 
	float m_fExecutionTime; 
	float m_fElapsedSeconds; 
	int m_iFrameCount;
	int m_iFPS;

#ifdef USE_LAG 
	float m_fLag;
	int m_iUpdateCount;
#endif // USE_LAG

	bool m_bLooping;

private:
	InputSystem* m_pInputSystem;
};

#endif //   GAME_H 
