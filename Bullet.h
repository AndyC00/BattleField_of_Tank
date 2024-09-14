#ifndef BULLET_H
#define BULLET_H

#include "Vector2.h"
#include "sprite.h"
#include "texture.h"

class Bullet
{
public:
    Bullet();
    ~Bullet();

    bool Initialise(Renderer& renderer);
    void Process(float deltaTime);
    void Draw(Renderer& renderer);
    bool IsAlive() const;
    void SetPosition(const Vector2& position, float angle);
    Vector2& GetPosition();
    float GetRadius();

private:
    Vector2 m_position;
    Vector2 m_velocity;
    Sprite* m_pSprite;
    bool m_isAlive;
    float m_lifetime;
    float m_angle;
};

#endif
