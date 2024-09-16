#include "Bullet.h"
#include "renderer.h" 
#include "sprite.h" 
#include <cmath>

Bullet::Bullet()
    : m_pSprite(nullptr)
    , m_isAlive(true)
    , m_lifetime(2.0f)  // lifetime setting
    , m_angle(0.0f)
{

}

Bullet::~Bullet()
{
    if (m_pSprite)
    {
        delete m_pSprite;
        m_pSprite = nullptr;
    }
}

bool Bullet::Initialise(Renderer& renderer)
{
    m_pSprite = renderer.CreateSprite("Sprites\\ball.png");
    m_pSprite->SetScale(0.05);

    return (m_pSprite != nullptr);
}

void Bullet::SetPosition(const Vector2& position, float angle)
{
    m_position = position;
    m_angle = angle;

    const float SPEED = 800;
    float angleInRadians = - m_angle * 3.14159f / 180.0f;
    m_velocity.x = sin(angleInRadians) * SPEED;
    m_velocity.y = cos(angleInRadians) * SPEED;
}

void Bullet::Process(float deltaTime)
{
    //if (m_isAlive)
    //{
    //    m_position += m_velocity * deltaTime;

    //    m_lifetime -= deltaTime;
    //    if (m_lifetime <= 0.0f)
    //    {
    //        m_isAlive = false;
    //    }
    //}

    m_position += m_velocity * deltaTime;

    m_pSprite->SetX(m_position.x);
    m_pSprite->SetY(m_position.y);
}

void Bullet::Draw(Renderer& renderer)
{
    m_pSprite->Draw(renderer);
}

bool Bullet::IsAlive() const
{
    return m_isAlive;
}

Vector2& Bullet::GetPosition()
{
    return m_position;
}

float Bullet::GetRadius()
{
    return m_pSprite->GetWidth() / 2;
}
