#pragma once
#include "Framework/Actor.h"
#include "Framework/Components/EnginePhysicsComponent.h"

class Car : public kiko::Actor {

public:

	Car(float enginePower, float acceleration, float brakePower, float turnRate, const kiko::Transform& transform) :
		kiko::Actor{ transform },
		m_enginePower{ enginePower },
		m_acceleration{ acceleration },
		m_brakePower{ brakePower },
		m_turnRate{ turnRate }
	{}

	void Update(float dt) override;

	void Start();

	/// <summary>
	/// "m_rotate" gets adjusted to steer left or right
	/// </summary>
	/// <param name="steerAmount">Negative for left, Positive for right</param>
	void Steer(float steerAmount);

	void Reverse()
	{
		if (m_drive > -1) m_drive -= m_brakePower;
	}

	void Drive()
	{
		if (m_drive < 1) m_drive += m_acceleration;
	}

	void Coast() {
		if (m_drive < 0.001 && m_drive > -0.001) m_drive = 0.0f;
		else if (m_drive > 0) m_drive -= 0.01f;
		else if (m_drive < 0) m_drive += 0.01f;
	}

	float m_currentSpeed = 0;

	kiko::PhysicsComponent* m_physics = nullptr;
protected:
	// Driving
	float m_enginePower = 0;
	float m_acceleration = 0;
	float m_brakePower = 0;
	float m_turnRate = 0;
	float m_drive = 0;

	// Turning
	float m_rotate = 0;

};
