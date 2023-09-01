#pragma once
#include "Framework/Actor.h"

namespace kiko
{
	class Player : public Actor
	{
	public:
		CLASS_DECLARATION(Player)

		bool Initialize() override;
		void Update(float dt) override;

		void OnCollisionEnter(Actor* other) override;
		void OnCollisionExit(Actor* other) override;

		void Die();

	private:
		float speed = 0;
		float jump = 0;
		int groundCount = 0;
		int coinCount = 0;

		class PhysicsComponent* m_physicsComponent = nullptr;
		class SpriteAnimRenderComponent* m_spriteAnimComponent = nullptr;
	};
}