#ifndef   BALL_H 
#define   BALL_H 

// Local includes:
#include "vector2.h"

// Forward declarations:
class Renderer;
class Sprite;

// Class declaration:
class Ball
{
	// Member methods:
public:
	Ball();
	~Ball();

	bool Initialise(Renderer& renderer); 
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

	void RandomiseColour(); 
	void RandomiseSize();

	Vector2& Position();

protected:
	void ComputeBounds(int width, int height);

private:
	Ball(const Ball& ball);
	Ball& operator=(const Ball& ball);

	// Member data: 
public:

protected:
	Vector2 m_position; 
	Vector2 m_velocity;

	Vector2 m_boundaryLow; 
	Vector2 m_boundaryHigh;

	Sprite* m_pSprite; 
	bool m_bAlive;

	static float sm_fBoundaryWidth; 
	static float sm_fBoundaryHeight;

private:

};

#endif //   BALL_H 
