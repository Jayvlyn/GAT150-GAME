#pragma once
#include "Framework/Actor.h"

namespace kiko
{
	class Enemy : public Actor
	{
	public:
		CLASS_DECLARATION(Enemy)

		bool Initialize() override;
		void Update(float dt) override;

		void OnCollisionEnter(Actor* other) override;
		void OnCollisionExit(Actor* other) override;

		void Die();

	private:
		float speed = 0;
		float jump = 0;
		int groundCount = 0;

		class PhysicsComponent* m_physicsComponent = nullptr;
		class SpriteAnimRenderComponent* m_spriteAnimComponent = nullptr;
	};
}