#include "Enemy.h"
#include "Player.h"
#include "DrivingGame.h"
#include "Framework/Scene.h"
#include "Framework/Emitter.h"
#include "Renderer/Renderer.h"
#include <iostream>

void Enemy::Update(float dt) {
    Actor::Update(dt);

    m_player = m_scene->GetActor<Player>();

    if (m_player != nullptr) 
    {
        m_currentSpeed = kiko::Mag(m_velocity.x, m_velocity.y);

        kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);

        float playerDistance = m_transform.position.Distance(m_player->m_transform.position);
        
        // Only update player location if they didnt wrap
        
        //if (!m_player->DidWrap() && m_transform.position.Distance(m_player->GetLastPosition()) > 500) {
        //    m_playerLocation = FindPlayer();
        //}
        //if (!m_player->DidWrap()) 
        
        m_playerLocation = FindPlayer();
        switch (m_playerLocation) {
            case ePlayerLocation::Front:
                Steer(0.0f);
                if (m_currentSpeed < m_maxSpeed) Drive();
                else Coast();
                break;
            case ePlayerLocation::FrontLeft:
                Steer(-0.03f);
                if (m_currentSpeed < m_maxSpeed) Drive();
                else Coast();
                break;
            case ePlayerLocation::FrontRight:
                Steer(0.03f);
                if (m_currentSpeed < m_maxSpeed) Drive();
                else Coast();
                break;
            case ePlayerLocation::BackLeft:
                if (playerDistance < 250) Steer(-0.015f);
                else Steer(0.015f);
                if (m_currentSpeed < m_maxSpeed) Reverse();
                else Coast();
                break;
            case ePlayerLocation::BackRight:
                if (playerDistance < 250) Steer(0.015f);
                else Steer(-0.015f);
                if (m_currentSpeed < m_maxSpeed) Reverse();
                else Coast();
                break;
        }

        
        // Move accordingly
        AddForce(forward * m_enginePower * m_drive * kiko::g_time.GetDeltaTime());

        // Wrapping
        m_transform.position.x = kiko::Wrap(m_transform.position.x, kiko::g_renderer.GetWidth());
        m_transform.position.y = kiko::Wrap(m_transform.position.y, kiko::g_renderer.GetHeight());
    }
}

void Enemy::OnCollision(Actor* other)
{
    if (!m_collision) {
        m_collision = true; // TURNS OFF COLLISION, MAKE TINY SHORT TIMER TO SET IT BACK TO FALSE
        m_collisionTimer = 1.0f;

        if (other->m_tag == "Player") {
            // Collision force
            if (m_currentSpeed < kiko::Mag(other->GetVelocity().x, other->GetVelocity().y)) {
                AddForce(other->GetVelocity() * 1.5);
            }


            m_health--;
            if (m_health <= 0) {
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
                kiko::Transform transform{ { m_transform.position }, m_transform.rotation, 1 };
                auto emitter = std::make_unique<kiko::Emitter>(transform, data);
                emitter->m_lifespan = 0.5f;
                m_scene->Add(std::move(emitter));

                m_scene->DecrementEnemyCount();


                m_destroyed = true;

            }
        }
    }
}

Enemy::ePlayerLocation Enemy::FindPlayer(kiko::vec2 forward)
{
    float angle = kiko::RadToDeg(kiko::Vector2::SignedAngle(forward.Normalized(), (m_player->m_transform.position - m_transform.position).Normalized()));

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
    kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);
    return FindPlayer(forward);
}

void Enemy::Steer(float steerAmount)
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

