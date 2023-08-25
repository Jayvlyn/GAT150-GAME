#pragma once
#include "Car.h"

namespace kiko
{

	class Player : public kiko::Car {

	public:
		CLASS_DECLARATION(Player)

		bool Initialize() override;
		void OnDestroy() override;
		void Update(float dt) override;

		void OnCollisionEnter(Actor* other);

		float GetHealth();

		void AddBonsuTime(float time) { bonusTime += time; }
		float GetBonusTime() { return bonusTime; }

		bool DidWrap() { return wrapped; }

		kiko::vec2 GetLastPosition() { return lastPosition; }

	private:

		// Drifting
		kiko::vec2 driftForce{ 0,0 };
		float driftEnginePower = 0;
		float driftDrive = 0;

		float timeScale = 1;
		float bonusTime = 0;

		//
		kiko::vec2 lastPosition;
		bool wrapped = false;


	};

}

