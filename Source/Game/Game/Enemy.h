#pragma once
#include "Framework/Actor.h"
#include "Renderer/Model.h"
#include "Player.h"
#include "Framework/Scene.h"

class Enemy : public kiko::Actor {

public:
	enum ePlayerLocation {
		Front = 0,
		FrontRight = 1,
		FrontLeft = 2,
		BackRight = 3,
		BackLeft = 4
	};

	Enemy(float enginePower, float acceleration, float brakePower, float turnRate, const kiko::Transform& transform, std::shared_ptr<kiko::Model> model) :
		kiko::Actor{ transform, model },
		m_enginePower{ enginePower },
		m_acceleration{ acceleration },
		m_brakePower{ brakePower },
		m_turnRate{ turnRate }
	{
		m_damping = 0.95;	
	}

	void Update(float dt) override;
	void OnCollision(Actor* other) override;
	ePlayerLocation FindPlayer(kiko::vec2 forward);
	ePlayerLocation FindPlayer();

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
		if (m_drive < 0.001 && m_drive > -0.001) m_drive = 0;
		else if (m_drive > 0) m_drive -= 0.01;
		else if (m_drive < 0) m_drive += 0.01;
	}
	
	float m_currentSpeed = 0;

private:
	float m_enginePower = 0;
	float m_acceleration = 0;
	float m_brakePower = 0;
	float m_turnRate = 0;
	float m_drive = 0;
	float m_maxSpeed = 200;

	float m_rotate = 0;

	float m_health = 100;

	Player* m_player = nullptr;
};



