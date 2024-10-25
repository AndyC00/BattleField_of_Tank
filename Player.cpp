//local includes:
#include "Player.h"
#include "Entity.h"
#include "renderer.h"
#include "Bullet.h"
#include "game.h"

//library includes:
#include <iostream>

using FMOD::System;
using FMOD::Sound;
using FMOD::Channel;

Player::Player()
    : Entity(),
    m_lives(5),
    m_invincibilityRemaining(0.0f),
	hitsound1(nullptr),
	deadsound(nullptr),
	engineSound(nullptr),
	channelEngineLeft(nullptr),
	channelEngineRight(nullptr),
	channelEngineForward(nullptr),
	channelFire(nullptr),
    PlayerBullet(nullptr),
    m_bAlive(true),
	m_currentSpeed(0.0f),
	m_maxSpeed(20.0f),		//Maximum speed can achieve
	m_acceleration(20.0f)	//accelerations to achieve maximum speed
{

}

Player::~Player()
{
    delete  PlayerBullet;
	if (deadsound)
	{
		deadsound->release();
		deadsound = nullptr;
	}
	if (hitsound1)
	{
		hitsound1->release();
		hitsound1 = nullptr;
	}
	if (engineSound)
	{
		engineSound->release();
		engineSound = nullptr;
	}
}

bool Player::Initialise(Renderer& renderer)
{
    m_pSprite = renderer.CreateSprite("Sprites\\Tanks\\tank.png");
    m_pSprite->SetScale(0.2f);

    //initialise player bullet
    PlayerBullet = new Bullet();
    PlayerBullet->Initialise(renderer);

	FMOD_RESULT result = Game::pSoundsystem->createSound("sounds\\largeExplosion.flac", FMOD_DEFAULT, &deadsound);
	if (result != FMOD_OK || deadsound == nullptr) {
		printf("Failed to load largeExplosion.flac: %s\n", FMOD_ErrorString(result));
	}
	else {
		printf("Successfully loaded largeExplosion.flac\n");
	}
	FMOD_RESULT result2 = Game::pSoundsystem->createSound("sounds\\fire.wav", FMOD_DEFAULT, &hitsound1);
	if (result2 != FMOD_OK || hitsound1 == nullptr) {
		printf("Failed to load sounds\\fire.wav: %s\n", FMOD_ErrorString(result2));
	}
	else {
		printf("Successfully loaded sounds\\fire.wav\n");
	}
	FMOD_RESULT result3 = Game::pSoundsystem->createSound("sounds\\engine.wav", FMOD_DEFAULT, &engineSound);
	if (result3 != FMOD_OK || engineSound == nullptr) {
		printf("Failed to load sounds\\engine.wav: %s\n", FMOD_ErrorString(result3));
	}
	else {
		printf("Successfully loaded sounds\\engine.wav\n");
	}


    return true;
}


void Player::Process(float deltaTime, InputSystem& inputSystem)
{
	//reading input
	ButtonState LKeyState = inputSystem.GetKeyState(SDL_SCANCODE_LEFT);
	ButtonState RKeyState = inputSystem.GetKeyState(SDL_SCANCODE_RIGHT);
	ButtonState sKeyState = inputSystem.GetKeyState(SDL_SCANCODE_SPACE);
	ButtonState FKeyState = inputSystem.GetKeyState(SDL_SCANCODE_UP);

	if (LKeyState == BS_HELD)
	{
		if (!channelEngineLeft)
		{
			Game::pSoundsystem->playSound(engineSound, nullptr, false, &channelEngineLeft);
		}
		float currentAngle = m_pSprite->GetAngle();
		float newAngle = NormalizeAngle(currentAngle - 45.0f);
		m_pSprite->SetAngle(newAngle);
	}
	else if ((LKeyState == BS_RELEASED))
	{
		if (channelEngineLeft)
		{
			channelEngineLeft->stop();
			channelEngineLeft = nullptr;
		}
	}

	if (RKeyState == BS_HELD)
	{
		if (!channelEngineRight)
		{
			Game::pSoundsystem->playSound(engineSound, nullptr, false, &channelEngineRight);
		}

		float currentAngle = m_pSprite->GetAngle();
		float newAngle = NormalizeAngle(currentAngle + 45.0f * deltaTime);
		m_pSprite->SetAngle(newAngle);
	}
	else if (RKeyState == BS_RELEASED)
	{
		if (channelEngineRight)
		{
			channelEngineRight->stop();
			channelEngineRight = nullptr;
		}
	}

	if (sKeyState == BS_PRESSED)
	{
		if (!channelFire)
		{
			Game::pSoundsystem->playSound(hitsound1, nullptr, false, &channelFire);
		}
		Vector2 playerPosition = Vector2(m_pSprite->GetX(), m_pSprite->GetY());
		float playerAngle = m_pSprite->GetAngle();
		PlayerBullet->SetPosition(playerPosition, playerAngle);
	}

	if (FKeyState == BS_HELD)
	{
		if (!channelEngineForward)
		{
			Game::pSoundsystem->playSound(engineSound, nullptr, false, &channelEngineForward);
		}
		float playerAngle = m_pSprite->GetAngle();
		float angleInRadians = (playerAngle + 90.0f) * M_PI / 180.0f;

		m_currentSpeed += m_acceleration * deltaTime;
		if (m_currentSpeed > m_maxSpeed)
		{
			m_currentSpeed = m_maxSpeed;
		}

		Vector2 direction(cos(angleInRadians), sin(angleInRadians));
		m_velocity = direction * m_currentSpeed;
	}
	else if (FKeyState == BS_RELEASED)
	{
		if (channelEngineForward)
		{
			channelEngineForward->stop();
			channelEngineForward = nullptr;
		}
		m_velocity = Vector2(0.0f, 0.0f);
	}

    Entity::Process(deltaTime);

	PlayerBullet->Process(deltaTime);
}

void Player::Draw(Renderer& renderer)
{
    Entity::Draw(renderer);
	if (PlayerBullet)
	{
		PlayerBullet->Draw(renderer);
	}
}

int Player::GetLives() const
{
    return m_lives;
}

void Player::TakeDamage(int damage)
{
    if (m_invincibilityRemaining <= 0.0f)  // Check if player is invincible
    {
        m_lives -= damage;
        // Set the invincibility timer
        m_invincibilityRemaining = 2.0f;
    }
}

void Player::SetDead()
{
    m_bAlive = false;
}

float Player::NormalizeAngle(float angle)
{
	while (angle >= 360.0f)
		angle -= 360.0f;
	while (angle < 0.0f)
		angle += 360.0f;
	return angle;
}

Bullet* Player::GetBullet()
{
	return PlayerBullet;
}

void Player::SetPosition(int x, int y)
{
	m_pSprite->SetX(x);
	m_pSprite->SetY(y);
	m_position.x = static_cast<float>(x);
	m_position.y = static_cast<float>(y);
}