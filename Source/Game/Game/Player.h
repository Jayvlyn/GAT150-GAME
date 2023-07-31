#pragma once
#include "Framework/Actor.h"

class Player : public kiko::Actor {

public:

	Player(float enginePower, float acceleration, float brakePower, float turnRate, const kiko::Transform& transform, std::shared_ptr<kiko::Model> model) :
		kiko::Actor{ transform, model },
		m_enginePower{ enginePower },
		m_acceleration{ acceleration },
		m_brakePower{ brakePower},
		m_turnRate{ turnRate }
	{}

	void Update(float dt) override;
	void OnCollision(Actor* other) override;

	float GetHealth();

	void AddBonsuTime(int time) { m_bonusTime = time; }
	float GetBonusTime() { return m_bonusTime; }

	float m_currentSpeed = 0;
private:

	// Drifting
	kiko::vec2 m_driftForce{ 0,0 };
	float m_driftEnginePower = 0;
	float m_driftDrive = 0;
	
	// Driving
	float m_enginePower = 0;
	float m_acceleration = 0; 
	float m_brakePower = 0; 
	float m_turnRate = 0;
	float m_drive = 0;
	
	// Turning
	float m_rotate = 0;

	float m_health = 100;
	float m_timeScale = 1;
	float m_bonusTime = 0;

};

