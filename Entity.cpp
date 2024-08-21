// This include:
#include "entity.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "vector2.h"

// Library includes:
#include <cassert>

// Constructor
Entity::Entity()
    : m_pSprite(nullptr)
    , m_position(0.0f, 0.0f)
    , m_velocity(0.0f, 0.0f)
    , m_bAlive(true)
{
}

// Destructor
Entity::~Entity()
{
    delete m_pSprite;
    m_pSprite = nullptr;
}

// Initialise the entity
bool Entity::Initialise(Renderer& renderer)
{
    // Assume there is a default sprite for the entity
    m_pSprite = renderer.CreateSprite("Sprites\\default.png"); // replace the path with an actual sprite path
    if (!m_pSprite)
    {
        return false;
    }
    return true;
}

// Process the entity's logic
void Entity::Process(float deltaTime)
{
    // Update position based on velocity
    m_position += m_velocity * deltaTime;

    // Update sprite position
    m_pSprite->SetX(static_cast<int>(m_position.x));
    m_pSprite->SetY(static_cast<int>(m_position.y));

    // If you want to do additional processing, add here


}

// Draw the entity
void Entity::Draw(Renderer& renderer)
{
    if (m_bAlive)
    {
        m_pSprite->Draw(renderer);
    }
}

// Rotate the entity
void Entity::Rotate(float direction)
{
    if (m_pSprite)
    {
        m_pSprite->SetAngle(direction);
    }
}

// Check if the entity is alive
bool Entity::IsAlive() const
{
    return m_bAlive;
}

// Set the entity as dead
void Entity::SetDead()
{
    m_bAlive = false;
}

// Get the radius of the entity (based on the sprite's size)
float Entity::GetRadius()
{
    return (m_pSprite->GetWidth() / 2.0f) * m_pSprite->GetScale();
}

// Get the direction the entity is facing
Vector2 Entity::GetFacingDirection()
{
    // Assuming the sprite's angle represents the facing direction
    float angle = m_pSprite->GetAngle();
    return Vector2(cos(angle), sin(angle));
}

// Get the position of the entity
Vector2& Entity::GetPosition()
{
    return m_position;
}

// Get the velocity of the entity
Vector2& Entity::GetVelocity()
{
    return m_velocity;
}

// Check if this entity is colliding with another entity
bool Entity::IsCollidingWith(Entity& toCheck)
{
    float distanceSquared = (m_position - toCheck.GetPosition()).LengthSquared();
    float combinedRadius = GetRadius() + toCheck.GetRadius();
    return distanceSquared <= (combinedRadius * combinedRadius);
}
