#pragma once
#include "Framework/Actor.h"
#include "Framework/Components/EnginePhysicsComponent.h"

namespace kiko
{

	class Car : public kiko::Actor {

	public:

		CLASS_DECLARATION(Car)

		bool Initialize() override;
		void OnDestroy() override;
		void Update(float dt) override;

		void OnCollisionEnter(Actor* other) override;

		/// <summary>
		/// "m_rotate" gets adjusted to steer left or right
		/// </summary>
		/// <param name="steerAmount">Negative for left, Positive for right</param>
		void Steer(float steerAmount);

		void Reverse()
		{
			if (drive > -1) drive -= brakePower;
		}

		void Drive()
		{
			if (drive < 1) drive += acceleration;
		}

		void Coast() {
			if (drive < 0.001 && drive > -0.001) drive = 0.0f;
			else if (drive > 0) drive -= 0.01f;
			else if (drive < 0) drive += 0.01f;
		}

		float currentSpeed = 0;
		kiko::PhysicsComponent* physics = nullptr;

	protected:
		// Construct Variables
		float enginePower = 0;
		float acceleration = 0;
		float brakePower = 0;
		float turnRate = 0;
		float maxSpeed = 0;
		float health = 0;

		// Active Variables
		float drive = 0;
		float rotate = 0;

	};
}

