#include "Enemy.h"
#include "Player.h"
#include "DrivingGame.h"
#include "Renderer/Renderer.h"
#include "Framework/Framework.h"
#include <iostream>

namespace kiko
{
    CLASS_DEFINITION(Enemy);

    bool Enemy::Initialize()
    {
        return true;
    }

    void Enemy::Read(const json_t& value)
    {
        Car::Read(value);
    }

    void Enemy::OnDestroy()
    {
        //
    }

    void Enemy::Update(float dt) {
        Car::Update(dt);

        player = m_scene->GetActor<Player>();

        if (player != nullptr) 
        {
            kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);

            float playerDistance = transform.position.Distance(player->transform.position);
        
            // Only update player location if they didnt wrap
        
            //if (!m_player->DidWrap() && m_transform.position.Distance(m_player->GetLastPosition()) > 500) {
            //    m_playerLocation = FindPlayer();
            //}
            //if (!m_player->DidWrap()) 
        
            playerLocation = FindPlayer();
            switch (playerLocation) {
                case ePlayerLocation::Front:
                    Steer(0.0f);
                    if (currentSpeed < maxSpeed) Drive();
                    else Coast();
                    break;
                case ePlayerLocation::FrontLeft:
                    Steer(-0.03f);
                    if (currentSpeed < maxSpeed) Drive();
                    else Coast();
                    break;
                case ePlayerLocation::FrontRight:
                    Steer(0.03f);
                    if (currentSpeed < maxSpeed) Drive();
                    else Coast();
                    break;
                case ePlayerLocation::BackLeft:
                    if (playerDistance < 250) Steer(-0.015f);
                    else Steer(0.015f);
                    if (currentSpeed < maxSpeed) Reverse();
                    else Coast();
                    break;
                case ePlayerLocation::BackRight:
                    if (playerDistance < 250) Steer(0.015f);
                    else Steer(-0.015f);
                    if (currentSpeed < maxSpeed) Reverse();
                    else Coast();
                    break;
            }

            
            auto physicsComponent = GetComponent<kiko::PhysicsComponent>();
        
            // Move accordingly
            if(physicsComponent) physicsComponent->ApplyForce(forward * enginePower * drive * kiko::g_time.GetDeltaTime());

            // Wrapping
            transform.position.x = kiko::Wrap(transform.position.x, kiko::g_renderer.GetWidth());
            transform.position.y = kiko::Wrap(transform.position.y, kiko::g_renderer.GetHeight());
        }
    }

    void Enemy::OnCollisionEnter(Actor* other)
    {
        if (!collision) {
            collision = true; // TURNS OFF COLLISION, MAKE TINY SHORT TIMER TO SET IT BACK TO FALSE
            collisionTimer = 0.8f;

            if (other->tag == "Player") {
            
                Player* player = (Player*)other;

                // Collision force
                if (currentSpeed < kiko::Mag(player->physics->velocity.x, player->physics->velocity.y)) {
                    physics->ApplyForce(player->physics->velocity * 1.5);
                }

                health--;
                if (health <= 0) {
                    // Death Particles
                    kiko::EmitterData data;
                    data.burst = true;
                    data.burstCount = 300;
                    data.spawnRate = 10000;
                    data.angle = 0;
                    data.angleRange = kiko::Pi;
                    data.lifetimeMin = 0.5f;
                    data.lifetimeMax = 1.0f;
                    data.speedMin = 300;
                    data.speedMax = 600;
                    data.damping = 0.2f;
                    data.color = kiko::Color{ 1, 0, 0, 1 };
                    kiko::Transform t{ { transform.position }, transform.rotation, 1 };
                    auto emitter = std::make_unique<kiko::Emitter>(t, data);
                    emitter->lifespan = 0.5f;
                    m_scene->Add(std::move(emitter));

                    m_scene->DecrementEnemyCount();

                    destroyed = true;

                }
            }
        }
    }

    Enemy::ePlayerLocation Enemy::FindPlayer(kiko::vec2 forward)
    {
        float angle = kiko::RadToDeg(kiko::Vector2::SignedAngle(forward.Normalized(), (player->transform.position - transform.position).Normalized()));

        if (angle < -5 && angle > -110)
        {
            return ePlayerLocation::FrontLeft;
        }
        else if (angle < -110 && angle > -180)
        {
            return ePlayerLocation::BackLeft;
        }
        else if (angle > 5 && angle < 110)
        { 
            return ePlayerLocation::FrontRight;
        }
        else if (angle > 110 && angle < 180)
        {
            return ePlayerLocation::BackRight;
        }
        else
        {
            return ePlayerLocation::Front;
        }
    }

    Enemy::ePlayerLocation Enemy::FindPlayer()
    {
        kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
        return FindPlayer(forward);
    }
}


