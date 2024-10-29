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
    float m_invincibilityRemaining;
    Bullet* PlayerBullet;
    int HP;
    int CurrentSprite;

protected:

private:
    FMOD::Sound* deadsound;
    FMOD::Sound* hitsound1;
    FMOD::Sound* engineSound;
    FMOD::Channel* channelEngineBackward;
    FMOD::Channel* channelEngineLeft;
    FMOD::Channel* channelEngineRight;
    FMOD::Channel* channelEngineForward;
    FMOD::Channel* channelFire;

    float rotationSpeed;
    float m_currentSpeed;
    float m_maxSpeed;
    float m_acceleration;

    std::vector<Sprite*> TankStates;
};

#endif // _PLAYER_H_