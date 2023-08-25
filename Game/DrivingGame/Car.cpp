#include "Car.h"
#include "Framework/Framework.h"
#include <iostream>

namespace kiko
{
    CLASS_DEFINITION(Car);


    bool Car::Initialize()
    {
        Actor::Initialize();

        // cache off
        //physics = GetComponent<kiko::EnginePhysicsComponent>(); // does not get component player has Box2D physics component
        
        auto collisionComponent = GetComponent<kiko::CollisionComponent>();
        if (collisionComponent)
        {
        
        }
        
        return true;
    }

    void Car::OnDestroy()
    {
    }

    void Car::Read(const json_t& value)
    {
        Actor::Read(value);

        READ_DATA(value, enginePower);
        READ_DATA(value, acceleration);
        READ_DATA(value, brakePower);
        READ_DATA(value, turnRate);
        READ_DATA(value, maxSpeed);
        READ_DATA(value, health);
    }

    void Car::Update(float dt)
    {
        Actor::Update(dt);
        //currentSpeed = kiko::Mag(physics->velocity.x, physics->velocity.y);
    }

    void Car::OnCollisionEnter(Actor* other)
    {
        //
    }

    void Car::Steer(float steerAmount)
    {
        if (steerAmount) { // Steer amount not zero
            rotate += steerAmount;
            if (currentSpeed != 0) transform.rotation += (rotate * ((currentSpeed / 1001) * drive)) * turnRate * kiko::g_time.GetDeltaTime();
        }
        else { // Steer amount zero
            if (currentSpeed)
            { // Car is moving, correct steering wheel because steer is 0
                if (rotate < 0.01 && rotate > -0.01) rotate = 0;
                else if (rotate > 0) rotate -= 0.05f;
                else if (rotate < 0) rotate += 0.05f;
            }

        }
    }
}

