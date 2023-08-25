#include "Player.h"
#include "Enemy.h"
#include "DrivingGame.h"
#include "Input/InputSystem.h"
#include "Core/MathUtils.h"
#include "Renderer/Renderer.h"
#include "Audio/AudioSystem.h"
#include "Framework/Framework.h"

#include <iostream>
#include <memory>

namespace kiko
{
    CLASS_DEFINITION(Player);

    bool Player::Initialize()
    {
        Car::Initialize();
        return true;
    }

    void Player::OnDestroy()
    {
        //
    }

    void Player::Read(const json_t& value)
    {
        Car::Read(value);
    }

    void Player::Update(float dt) {
        Car::Update(dt);
        //std::cout << m_currentSpeed << "\n";
        //std::cout << m_rotate << "\n";
    
        // BRAKE
        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_S)) {
            Reverse();
        }
        // GAS
        else if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) {
            Drive();
        }
        // Cruising
        else {
            Coast();
        }

        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A) && rotate > -1)        Steer(-0.05f);
        else if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D) && rotate < 1)    Steer(0.05f);
        else if (currentSpeed)                                                    Steer(0);

        // Steer Rotations
        if (currentSpeed != 0) transform.rotation += (rotate * ((currentSpeed/100)* drive)) * turnRate * kiko::g_time.GetDeltaTime();

        // Drifting
        kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_LSHIFT) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_LSHIFT)) {
            driftDrive = drive;
            driftEnginePower = enginePower;
            driftForce = forward * driftEnginePower * driftDrive;
        }
        else if (!kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_LSHIFT) && (driftForce.x != 0 || driftForce.y != 0)) {
            driftForce = { 0,0 };
            driftDrive = 0;
            driftEnginePower = 0;
        }
        else {
            driftForce = Lerp(driftForce, forward * driftDrive * driftEnginePower, kiko::g_time.GetDeltaTime());
        }
   
        // Apply force
        if (physics) {
            if (!kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_LSHIFT)) {
                physics->ApplyForce((forward)*enginePower * drive * kiko::g_time.GetDeltaTime());
            }
            else {
                physics->ApplyForce(driftForce * kiko::g_time.GetDeltaTime());
            }

        }

        // Wrapping
        wrapped = false; // info for enemies
        lastPosition = transform.position;

        transform.position.x = kiko::Wrap(transform.position.x, kiko::g_renderer.GetWidth());
        transform.position.y = kiko::Wrap(transform.position.y, kiko::g_renderer.GetHeight());

        if (kiko::Mag(transform.position - lastPosition) > 100.0f) {
            wrapped = true;
        }

        // Time Control
        float timeChangeSpeed = 0.08f;
        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_0)) timeScale = 0.0f;
        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_1)) timeScale = 0.2f;
        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_2)) timeScale = 0.4f;
        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_3)) timeScale = 0.6f;
        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_4)) timeScale = 0.8f;
        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_5)) timeScale = 1.0f;
        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_6)) timeScale = 1.2f;
        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_7)) timeScale = 1.4f;
        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_8)) timeScale = 1.6f;
        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_9)) timeScale = 1.8f;
        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_0) && kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_1)) timeScale = 2.0f;

        kiko::g_time.SetTimeScale(Lerp(kiko::g_time.GetTimeScale(), timeScale, timeChangeSpeed));
    }

    void Player::OnCollisionEnter(Actor* other) 
    {
        if (!collision) {
            collision = true; 
            collisionTimer = 0.8f;

            if (other->tag == "Clock") {
                other->destroyed = true;
            }

            if (other->tag == "Enemy")
            {
                Enemy* enemy = (Enemy*)other;

                // Collision force
                if (Mag(physics->velocity.x, physics->velocity.y) < Mag(enemy->physics->velocity.x, enemy->physics->velocity.y)) {
                    physics->ApplyForce(enemy->physics->velocity);
                }

                // Hit sound
                int hitSelection = random(1, 3);
                switch (hitSelection) {
                case 1:
                    g_audioSystem.PlayOneShot("hit1");
                    break;
                case 2:
                    g_audioSystem.PlayOneShot("hit2");
                    break;
                case 3:
                    g_audioSystem.PlayOneShot("hit3");
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
                kiko::Transform transform{ { kiko::Lerp(transform.position, other->transform.position, 0.5)}, transform.rotation, 1 };
                auto emitter = std::make_unique<kiko::Emitter>(transform, data);
                emitter->lifespan = 0.1f;
                m_scene->Add(std::move(emitter));

                // Health + Death
                health--;
                if (health <= 0) {
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
                    kiko::Transform t{ { transform.position }, transform.rotation, 1 };
                    auto emitter = std::make_unique<kiko::Emitter>(t, data);
                    emitter->lifespan = 0.5f;
                    m_scene->Add(std::move(emitter));

                    destroyed = true;
                    dynamic_cast<DrivingGame*>(m_game)->SetState(DrivingGame::eState::PlayerDeadStart);

                }
            }
        }
    }

    float Player::GetHealth()
    {
        return health;
    }

}

