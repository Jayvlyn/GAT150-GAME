#include "Clock.h"
#include <iostream>
#include "Player.h"

void Clock::Update(float dt) {
    Actor::Update(dt);
}

void Clock::OnCollision(Actor* other)
{
    if (other->tag == "Player") {
        dynamic_cast<Player*>(other)->AddBonsuTime(m_time);
        destroyed = true;
    }
}

