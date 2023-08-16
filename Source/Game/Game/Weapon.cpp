#include "Weapon.h"
#include "Renderer/Renderer.h"

void Weapon::Update(float dt) {
    Actor::Update(dt);

    kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
    transform.position += forward * m_speed * kiko::g_time.GetDeltaTime();
    transform.position.x = kiko::Wrap(transform.position.x, kiko::g_renderer.GetWidth());
    transform.position.y = kiko::Wrap(transform.position.y, kiko::g_renderer.GetHeight());
}

void Weapon::OnCollision(Actor* other)
{   
    m_destroyed = true;  
}
