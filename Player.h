#ifndef _PLAYER_H_
#define _PLAYER_H_

//local includes:
#include "Entity.h"
#include "InputSystem.h"
#include "sprite.h"

//library includes
#include "Vector2.h"
#include <vector>
#include <cmath>
#include "fmod.hpp"
#include "fmod_errors.h"

class Sprite;

class Player : public Entity
{
public:
    Player();
    ~Player();

// Member methods:
    bool Initialise(Renderer& renderer);
    void Process(float deltaTime, InputSystem& inputSystem);
    void Attack(Renderer& renderer);
    void Draw(Renderer& renderer);
    int GetLives() const;
    void SetDead();
    void TakeDamage(int damage);
    float NormalizeAngle(float angle);
    Bullet* GetBullet();
    void SetPosition(int x, int y);


protected:

private:


// Member data:
public:
    bool m_bAlive;
    Vector2 m_facingDirection;  // The direction the player is facing
    float m_invincibilityRemaining;
    float m_moveSpeed;
    int m_currentWeapon;
    int m_lives;  // Player's remaining lives
    float m_attackCooldownRemaining;
    float m_attackCooldown;
    Bullet* PlayerBullet;

protected:

private:
    FMOD::Sound* hitsound1;
    FMOD::Channel* channel;
};

#endif // _PLAYER_H_