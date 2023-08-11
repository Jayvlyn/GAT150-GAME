#include "CircleCollisionComponent.h"
#include "Framework/Actor.h"

namespace kiko
{
    void CircleCollisionComponent::Update(float dt) {}

    bool CircleCollisionComponent::CheckCollision(CollisionComponent* collision)
    {
        float distance = m_owner->m_transform.position.Distance(collision->m_owner->m_transform.position);
        float radius = m_radius + collision->m_radius;

        return (distance <= radius);
    }
}


