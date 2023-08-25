#pragma once
#include "Renderer/Model.h"
#include "Player.h"
#include "Car.h"

namespace kiko
{
	class Enemy : public kiko::Car {

	public:
		enum ePlayerLocation {
			Front = 0,
			FrontRight = 1,
			FrontLeft = 2,
			BackRight = 3,
			BackLeft = 4
		};

		CLASS_DECLARATION(Enemy)

		bool Initialize() override;
		void OnDestroy() override;
		void Update(float dt) override;

		void OnCollisionEnter(Actor* other) override;


		ePlayerLocation FindPlayer(kiko::vec2 forward);
		ePlayerLocation FindPlayer();

	private:
		Player* player = nullptr;
		ePlayerLocation playerLocation = ePlayerLocation::Front;
	};
}


