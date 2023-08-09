#include "Car.h"
#include "Framework/Components/EnginePhysicsComponent.h"
#include <iostream>

void Car::Update(float dt)
{
    Actor::Update(dt);
    m_currentSpeed = kiko::Mag(m_physics->m_velocity.x, m_physics->m_velocity.y);
}

void Car::Start()
{
    m_physics = GetComponent<kiko::EnginePhysicsComponent>();
}

void Car::Steer(float steerAmount)
{
    if (steerAmount) { // Steer amount not zero
        m_rotate += steerAmount;
        if (m_currentSpeed != 0) m_transform.rotation += (m_rotate * ((m_currentSpeed / 100) * m_drive)) * m_turnRate * kiko::g_time.GetDeltaTime();
    }
    else { // Steer amount zero
        if (m_currentSpeed)
        { // Car is moving, correct steering wheel because steer is 0
            if (m_rotate < 0.01 && m_rotate > -0.01) m_rotate = 0;
            else if (m_rotate > 0) m_rotate -= 0.05f;
            else if (m_rotate < 0) m_rotate += 0.05f;
        }

    }
}

