#pragma once
#include "Framework/Actor.h"
#include "Core/Time.h"

class Clock : public kiko::Actor {

public:

    Clock(float time, const kiko::Transform& transform, std::shared_ptr<kiko::Model> model) :
        kiko::Actor{ transform, model },
        m_time{ time }
    {
        m_collision = false;
        m_lifespan = 20.0f;
    }

    void Update(float dt) override;
    void OnCollision(Actor* other) override;

private:
    float m_time = 0;

};