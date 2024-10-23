// This include:
#include "entity.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"

// Library includes:
#include <cassert>
#include "imgui/imgui.h"
#include "vector2.h"


Entity::Entity()
    : m_pSprite(nullptr)
    , m_position(0.0f, 0.0f)
    , m_velocity(0.0f, 0.0f)
    , m_bAlive(true)
    , m_angle(0.0f)
{
}

Entity::~Entity()
{
    delete m_pSprite;
    m_pSprite = nullptr;
}

// Initialise the entity
bool Entity::Initialise(Renderer& renderer)
{
    m_pSprite = renderer.CreateSprite("Sprites\\Tanks\\tank.png"); // replace the path with an actual sprite path
    m_pSprite->SetScale(0.2f);
    m_pSprite->SetAngle(0.0f);

    if (!m_pSprite)
    {
        print:"Failed to load sprite!";
        return false;
    }
    return true;
}

void Entity::Process(float deltaTime)
{
    m_position += m_velocity * deltaTime;

    m_pSprite->SetX(static_cast<int>(m_position.x));
    m_pSprite->SetY(static_cast<int>(m_position.y));

}

void Entity::Draw(Renderer& renderer)
{
    if (m_bAlive)
    {
        m_pSprite->Draw(renderer);
    }
}

void Entity::Rotate(float direction)
{
    if (m_pSprite)
    {
        m_pSprite->SetAngle(direction);
    }
}

bool Entity::IsAlive() const
{
    return m_bAlive;
}

void Entity::SetDead()
{
    m_bAlive = false;
}

// Get the radius of the entity (based on the sprite's size)
float Entity::GetRadius()
{
    return (m_pSprite->GetWidth() / 2.0f) * m_pSprite->GetScale();
}

Vector2 Entity::GetFacingDirection()
{
    // Assuming the sprite's angle represents the facing direction
    float angle = m_pSprite->GetAngle();
    return Vector2(cos(angle), sin(angle));
}

Vector2& Entity::GetPosition()
{
    return m_position;
}

Vector2& Entity::GetVelocity()
{
    return m_velocity;
}

float Entity::GetAngle() const
{
    return m_angle;
}

void Entity::SetAngle(float angle)
{
    m_angle = angle;
    if (m_pSprite)
    {
        m_pSprite->SetAngle(m_angle);
    }
}

bool Entity::IsCollidingWith(Entity& toCheck)
{
    float distanceSquared = (m_position - toCheck.GetPosition()).LengthSquared();
    float combinedRadius = GetRadius() + toCheck.GetRadius();
    return distanceSquared <= (combinedRadius * combinedRadius);
}

bool Entity::IsCollidingWithBullet(Bullet* bullet) 
{
    float distanceSquared = (m_position - bullet->GetPosition()).LengthSquared();
    float combinedRadius = GetRadius() + bullet->GetRadius();
    return distanceSquared <= (combinedRadius * combinedRadius);
}