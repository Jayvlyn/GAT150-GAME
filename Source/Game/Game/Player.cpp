#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "DrivingGame.h"
#include "Input/InputSystem.h"
#include "Core/MathUtils.h"

#include "Framework/Scene.h"
#include "Framework/Emitter.h"
#include "Framework/Components/PhysicsComponent.h"

#include "Audio/AudioSystem.h"
#include <iostream>

void Player::Update(float dt) {
    Actor::Update(dt);

    m_currentSpeed = kiko::Mag(m_velocity.x, m_velocity.y);
    
    // BRAKE
    if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_S)) {
        if (m_drive > -1) m_drive -= m_brakePower;
    }
    // GAS
    else if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) {
        if (m_drive < 1) m_drive += m_acceleration;
    }
    // Cruising
    else {
        if (m_drive < 0.001 && m_drive > -0.001) m_drive = 0.0f;
        else if (m_drive > 0) m_drive -= 0.005f;
        else if (m_drive < 0) m_drive += 0.005f;
    }

    if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A) && m_rotate > -1 ) m_rotate -= 0.05f;
    else if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D) && m_rotate < 1) m_rotate += 0.05f;
    else if (m_currentSpeed) {
        // Bring rotate back to zero
        if (m_rotate < 0.001 && m_rotate > -0.001) m_rotate = 0.0f;
        else if (m_rotate > 0) m_rotate -= 0.01f;
        else if (m_rotate < 0) m_rotate += 0.01f;
        
    }

    // Steer Rotations
    if (m_currentSpeed != 0) m_transform.rotation += (m_rotate * ((m_currentSpeed/100)* m_drive)) * m_turnRate * kiko::g_time.GetDeltaTime();

    // Move player accordingly
    kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);
    if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_LSHIFT) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_LSHIFT)) {
        m_driftDrive = m_drive;
        m_driftEnginePower = m_enginePower;
        m_driftForce = forward * m_driftEnginePower * m_driftDrive;
    }
    else if (!kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_LSHIFT) && (m_driftForce.x != 0 || m_driftForce.y != 0)) {
        m_driftForce = { 0,0 };
        m_driftDrive = 0;
        m_driftEnginePower = 0;
    }
    else {
        m_driftForce = kiko::Lerp(m_driftForce, forward * m_driftDrive * m_driftEnginePower, kiko::g_time.GetDeltaTime());
    }
    ///
    auto physicsComponent = GetComponent<kiko::PhysicsComponent>();

    if (!kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_LSHIFT)) {
        physicsComponent->ApplyForce((forward)*m_enginePower * m_drive * kiko::g_time.GetDeltaTime());
    }
    else {
        physicsComponent->ApplyForce(m_driftForce * kiko::g_time.GetDeltaTime());
    }

    // Wrapping
    m_wrapped = false; // info for enemies
    m_lastPosition = m_transform.position;

    m_transform.position.x = kiko::Wrap(m_transform.position.x, kiko::g_renderer.GetWidth());
    m_transform.position.y = kiko::Wrap(m_transform.position.y, kiko::g_renderer.GetHeight());

    if (kiko::Mag(m_transform.position - m_lastPosition) > 100.0f) {
        m_wrapped = true;
    }

    // Time Control
    float timeChangeSpeed = 0.08f;
    if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_0)) m_timeScale = 0.0f;
    if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_1)) m_timeScale = 0.2f;
    if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_2)) m_timeScale = 0.4f;
    if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_3)) m_timeScale = 0.6f;
    if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_4)) m_timeScale = 0.8f;
    if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_5)) m_timeScale = 1.0f;
    if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_6)) m_timeScale = 1.2f;
    if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_7)) m_timeScale = 1.4f;
    if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_8)) m_timeScale = 1.6f;
    if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_9)) m_timeScale = 1.8f;
    if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_0) && kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_1)) m_timeScale = 2.0f;

    kiko::g_time.SetTimeScale(kiko::Lerp(kiko::g_time.GetTimeScale(), m_timeScale, timeChangeSpeed));
}

void Player::OnCollision(Actor* other) 
{
    if (!m_collision) {
        m_collision = true; // TURNS OFF COLLISION, MAKE TINY SHORT TIMER TO SET IT BACK TO FALSE
        m_collisionTimer = 1.0f;

        if (other->m_tag == "Clock") {
            other->m_destroyed = true;
        }

        if (other->m_tag == "Enemy")
        {
            // Collision force
            if (kiko::Mag(m_velocity.x, m_velocity.y) < kiko::Mag(other->GetVelocity().x, other->GetVelocity().y)) {
                AddForce(other->GetVelocity());
            }

            int hitSelection = kiko::random(1, 3);
            switch (hitSelection) {
            case 1:
                kiko::g_audioSystem.PlayOneShot("hit1");
                break;
            case 2:
                kiko::g_audioSystem.PlayOneShot("hit2");
                break;
            case 3:
                kiko::g_audioSystem.PlayOneShot("hit3");
                break;
            }


            // Crash Particles
            kiko::EmitterData data;
            data.burst = true;
            data.burstCount = 100;
            data.spawnRate = 10000;
            data.angle = kiko::DegToRad(kiko::randomf(0.0f, 360.0f));
            data.angleRange = kiko::DegToRad(kiko::randomf(30.0f, 180.0f));
            data.lifetimeMin = 0.5f;
            data.lifetimeMax = 1.0f;
            data.speedMin = 300;
            data.speedMax = 600;
            data.damping = 0.2f;
            data.color = kiko::Color{ 1, 0.5, 0, 1 };
            kiko::Transform transform{ { kiko::Lerp(m_transform.position, other->m_transform.position, 0.5)}, m_transform.rotation, 1 };
            auto emitter = std::make_unique<kiko::Emitter>(transform, data);
            emitter->m_lifespan = 0.1f;
            m_scene->Add(std::move(emitter));

            m_health--;
            if (m_health <= 0) {
                // Death Particles
                kiko::EmitterData data;
                data.burst = true;
                data.burstCount = 300;
                data.spawnRate = 250;
                data.angle = 0;
                data.angleRange = kiko::Pi;
                data.lifetimeMin = 0.5f;
                data.lifetimeMax = 1.0f;
                data.speedMin = 300;
                data.speedMax = 600;
                data.damping = 0.2f;
                data.color = kiko::Color{ 1, 0.5, 0, 1 };
                kiko::Transform transform{ { m_transform.position }, m_transform.rotation, 1 };
                auto emitter = std::make_unique<kiko::Emitter>(transform, data);
                emitter->m_lifespan = 0.5f;
                m_scene->Add(std::move(emitter));

                m_destroyed = true;
                dynamic_cast<DrivingGame*>(m_game)->SetState(DrivingGame::eState::PlayerDeadStart);

            }
        }
    }
}

float Player::GetHealth()
{
    return m_health;
}
