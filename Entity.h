#ifndef   ENTITY_H 
#define   ENTITY_H 

// Local includes:
#include "entity.h"
#include "vector2.h"

// Forward declarations:
class Renderer; 
class Sprite;

// Class declaration:
class Entity
{
	// Member methods: 
public:
	Entity();
	~Entity();

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	void Rotate(float direction); 
	bool IsAlive() const;
	void SetDead();
	float GetRadius();
	Vector2 GetFacingDirection();

	Vector2& GetPosition(); 
	Vector2& GetVelocity();

	bool IsCollidingWith(Entity& toCheck); 

protected:

private:
	Entity(const Entity& entity);
	Entity& operator=(const Entity& entity);

	// Member data: 
public:

protected:
	Sprite* m_pSprite;
	Vector2 m_position;
	Vector2 m_velocity; 
	bool m_bAlive;

private:
	
};

#endif //   ENTITY_H 
