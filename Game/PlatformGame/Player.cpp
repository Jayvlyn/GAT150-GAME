#include "Player.h"

#include "Input/InputSystem.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "PlatformGame.h"
#include "Audio/AudioSystem.h"

namespace kiko
{
	CLASS_DEFINITION(Player);

	bool Player::Initialize()
	{
		Actor::Initialize();

		m_physicsComponent = GetComponent<PhysicsComponent>();
		m_spriteAnimComponent = GetComponent<SpriteAnimRenderComponent>();

		m_spriteAnimComponent->SetSequence("fall"); // Player starts in air

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
			m_physicsComponent->ApplyForce(up * jump);
			g_audioSystem.PlayOneShot("jump", false);
		}

		// Attack
		if (g_inputSystem.GetKeyDown(SDL_SCANCODE_J) && onGround && m_spriteAnimComponent->m_sequence->name != "attack")
		{
			m_spriteAnimComponent->SetSequence("attack");
			m_spriteAnimComponent->m_sequence->loop = false;
			g_audioSystem.PlayOneShot("swing", false);
		}

		// Animation
		if (m_spriteAnimComponent->m_sequence->name != "attack")
		{
			if (onGround)
			{
				vec2 velocity = m_physicsComponent->velocity;
				if (std::fabs(velocity.x) > 0.1f)
				{
					if(dir != 0) m_spriteAnimComponent->flipH = dir < 0;
					if(m_spriteAnimComponent->m_sequence->name != "run") m_spriteAnimComponent->SetSequence("run");

				}
				else
				{
					if(m_spriteAnimComponent->m_sequence->name != "idle")m_spriteAnimComponent->SetSequence("idle");
				}
				if (!m_spriteAnimComponent->m_sequence->loop) m_spriteAnimComponent->m_sequence->loop = true;
			}
			else
			{
				if (m_physicsComponent->velocity.y > 0)
				{
					m_spriteAnimComponent->SetSequence("fall");
				}
				else if (m_spriteAnimComponent->m_sequence->name != "jump") 
				{
					m_spriteAnimComponent->SetSequence("jump");
					m_spriteAnimComponent->m_sequence->loop = false;
				}
			}
		}
		else // attacking
		{
			if(m_spriteAnimComponent->frame == m_spriteAnimComponent->m_sequence->endFrame) m_spriteAnimComponent->SetSequence("idle");
		}

		if (transform.position.y > 900) Die();
	}

	void Player::OnCollisionEnter(Actor* other)
	{
		if(other->tag == "Ground")
		{
			groundCount++;
		}

		else if (other->tag == "Enemy" && m_spriteAnimComponent->m_sequence->name != "attack")
		{
			Die();
		}

		else if (other->tag == "Coin")
		{
			other->destroyed = true;
			g_audioSystem.PlayOneShot("coin");
			coinCount++;
			auto coinText = m_scene->GetActorByName("CoinCount")->GetComponent<TextRenderComponent>();
			if (coinText) coinText->SetText(std::to_string(coinCount));
		}
	}

	void Player::OnCollisionExit(Actor* other)
	{
		if (other->tag == "Ground")
		{
			groundCount--;
		}
	}

	void Player::Die()
	{
		kiko::g_audioSystem.PlayOneShot("death");
		destroyed = true;
		dynamic_cast<PlatformGame*>(m_game)->SetState(PlatformGame::eState::PlayerDeadStart);
	}

	void Player::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_DATA(value, speed);
		READ_DATA(value, jump);
	}



}