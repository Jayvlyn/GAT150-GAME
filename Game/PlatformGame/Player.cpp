#include "Player.h"

#include "Input/InputSystem.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"

namespace kiko
{
	CLASS_DEFINITION(Player);

	bool Player::Initialize()
	{
		Actor::Initialize();

		m_physicsComponent = GetComponent<PhysicsComponent>();
		m_spriteAnimComponent = GetComponent<SpriteAnimRenderComponent>();
		return true;
	}

	void Player::Update(float dt)
	{
		Actor::Update(dt);

		bool onGround = (groundCount > 0) && m_physicsComponent->velocity.y == 0;

		// Move
		float dir = 0;
		if (g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) dir = -1;
		if (g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) dir = 1;


		vec2 forward = vec2{ 1, 0 };
		m_physicsComponent->ApplyForce(forward * speed * dir * ((onGround) ? 1 : 0.25f));

		// Jump
		if (onGround && g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE))
		{
			vec2 up = vec2{ 0, -1 };
			//m_physicsComponent->SetVelocity(up * jump);
			m_physicsComponent->ApplyForce(up * jump);
		}

		// Animation
		if (onGround)
		{
			vec2 velocity = m_physicsComponent->velocity;
			if (std::fabs(velocity.x) > 0.1f)
			{
				if(dir != 0) m_spriteAnimComponent->flipH = dir < 0;
				m_spriteAnimComponent->SetSequence("run");
			}
			else
			{
				m_spriteAnimComponent->SetSequence("idle");
			}
		}
		else
		{
			if (m_physicsComponent->velocity.y > 0)
			{
				m_spriteAnimComponent->SetSequence("fall");
			}
		}
	}

	void Player::OnCollisionEnter(Actor* other)
	{
		if(other->tag == "Ground")
		{
			groundCount++;
		}
	}

	void Player::OnCollisionExit(Actor* other)
	{
		if (other->tag == "Ground")
		{
			groundCount--;
		}
	}

	void Player::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_DATA(value, speed);
		READ_DATA(value, jump);
	}



}