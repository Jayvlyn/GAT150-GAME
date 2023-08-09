#pragma once
#include "Framework/Actor.h"
#include "Car.h"

class Player : public Car {

public:

	Player(float enginePower, float acceleration, float brakePower, float turnRate, const kiko::Transform& transform) :
		Car{ enginePower, acceleration, brakePower, turnRate, transform }
	{}

	void Update(float dt) override;
	void OnCollision(std::shared_ptr<Actor> other);

	float GetHealth();

	void AddBonsuTime(float time) { m_bonusTime += time; }
	float GetBonusTime() { return m_bonusTime; }

	bool DidWrap() { return m_wrapped; }

	kiko::vec2 GetLastPosition() { return m_lastPosition; }

private:

	// Drifting
	kiko::vec2 m_driftForce{ 0,0 };
	float m_driftEnginePower = 0;
	float m_driftDrive = 0;

	// Player vars
	float m_health = 100;
	float m_timeScale = 1;
	float m_bonusTime = 0;

	//
	kiko::vec2 m_lastPosition;
	bool m_wrapped = false;

};

