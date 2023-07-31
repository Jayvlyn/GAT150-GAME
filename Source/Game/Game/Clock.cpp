#include "Clock.h"
#include <iostream>
#include "Player.h"

void Clock::Update(float dt) {
    Actor::Update(dt);
}

void Clock::OnCollision(Actor* other)
{
    if (other->m_tag == "Player") {
        dynamic_cast<Player*>(other)->AddBonsuTime(m_time);
        m_destroyed = true;
    }

}
