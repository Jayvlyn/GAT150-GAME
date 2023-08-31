#include "Enemy.h"

#include "Input/InputSystem.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "PlatformGame.h"
#include "Audio/AudioSystem.h"
#include "Player.h"

namespace kiko
{
	CLASS_DEFINITION(Enemy);

	bool Enemy::Initialize()
	{
		Actor::Initialize();

		m_physicsComponent = GetComponent<PhysicsComponent>();
		m_spriteAnimComponent = GetComponent<SpriteAnimRenderComponent>();
		return true;
	}

	void Enemy::Update(float dt)
	{
		Actor::Update(dt);

		vec2 forward = vec2{ 1, 0 };
		Player* player = m_scene->GetActor<Player>();
		if (player)
		{
			vec2 direction = player->transform.position - transform.position;

			if(Mag(m_physicsComponent->velocity) < speed)m_physicsComponent->ApplyForce(direction.Normalized() * speed);
		}

		float dir = 0;
		if (m_physicsComponent->velocity.x < -0.1f) dir = 1;
		else if (m_physicsComponent->velocity.x > 0.1f) dir = -1;

		if (dir != 0) m_spriteAnimComponent->flipH = dir < 0;
	}

	void Enemy::OnCollisionEnter(Actor* other)
	{
	}

	void Enemy::OnCollisionExit(Actor* other)
	{
	}

	void Enemy::Die()
	{
		kiko::g_audioSystem.PlayOneShot("death");
		destroyed = true;
	}

	void Enemy::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_DATA(value, speed);
		READ_DATA(value, jump);
	}



}