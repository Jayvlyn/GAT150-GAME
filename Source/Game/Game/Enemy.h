#pragma once
#include "Renderer/Model.h"
#include "Player.h"
#include "Car.h"

class Enemy : public Car {

public:
	enum ePlayerLocation {
		Front = 0,
		FrontRight = 1,
		FrontLeft = 2,
		BackRight = 3,
		BackLeft = 4
	};

	Enemy(float enginePower, float acceleration, float brakePower, float turnRate, const kiko::Transform& transform) :
		Car{ enginePower, acceleration, brakePower, turnRate, transform }
	{ }

	void Update(float dt) override;
	void OnCollision(std::shared_ptr<Actor> other);
	ePlayerLocation FindPlayer(kiko::vec2 forward);
	ePlayerLocation FindPlayer();

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
	ePlayerLocation m_playerLocation = ePlayerLocation::Front;
};



