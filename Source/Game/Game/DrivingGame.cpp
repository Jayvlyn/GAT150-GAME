#include "DrivingGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Clock.h"

#include "Framework/Scene.h"
#include "Framework/Emitter.h"
#include "Framework/Resource/ResourceManager.h"
#include "Framework/Components/SpriteComponent.h"
#include "Framework/Components/EnginePhysicsComponent.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"

#include "Renderer/Renderer.h"
#include "Renderer/Text.h"
#include "Renderer/ModelManager.h"

#include "Core/Time.h"
#include "Core/Transform.h"

#include <memory>



bool DrivingGame::Init()
{
	// Create font / text objects
	m_font = kiko::g_resources.Get<kiko::Font>("Coalition_v2..ttf", 24);

	m_healthText = std::make_unique<kiko::Text>(m_font);
	m_healthText->Create(kiko::g_renderer, "(Your health will be here, DONT CRASH!)", kiko::Color{ 1, 1, 1, 1});

	m_titleText = std::make_unique<kiko::Text>(kiko::g_resources.Get<kiko::Font>("Coalition_v2..ttf", 48));
	m_titleText->Create(kiko::g_renderer, "TIME DRIFTER", kiko::Color{ 1, 1, 1, 1});

	m_startPromptText = std::make_unique<kiko::Text>(m_font);
	m_startPromptText->Create(kiko::g_renderer, "Press [Space] to Start", kiko::Color{ 1, 1, 1, 1});

	m_deathText = std::make_unique<kiko::Text>(m_font);
	m_deathText->Create(kiko::g_renderer, "YOU ARE DEAD", kiko::Color{ 1, 0, 0, 1});

	m_timerText = std::make_unique<kiko::Text>(m_font);
	m_timerText->Create(kiko::g_renderer, "Time Survived: 0", kiko::Color{ 1, 1, 1, 1});

	m_bonusTimeText = std::make_unique<kiko::Text>(m_font);
	m_bonusTimeText->Create(kiko::g_renderer, "+ 0", kiko::Color{ 1, 1, 1, 1});

	// Load audio
	kiko::g_audioSystem.AddAudio("music", "TDOST1.wav");
	kiko::g_audioSystem.AddAudio("hit1", "Hit1.wav");
	kiko::g_audioSystem.AddAudio("hit2", "Hit2.wav");
	kiko::g_audioSystem.AddAudio("hit3", "Hit3.wav");
	kiko::g_audioSystem.PlayOneShot("music", true);

	// Scene
	m_scene = std::make_unique<kiko::Scene>();

	return true;
}

void DrivingGame::Shutdown(){}

void DrivingGame::Update(float dt)
{
	switch (m_state)
	{
	case eState::Title:
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
		{
			m_state = eState::StartGame;
		}
		break;
	case eState::StartGame:
		m_state = eState::StartLevel;
		break;
	case eState::StartLevel:
		m_scene->RemoveAll();
	{
		// Create player
		auto player = std::make_unique<Player>(1000.0f, 0.01f, 0.005f, kiko::DegToRad(25.0f), kiko::Transform{ {kiko::g_renderer.GetWidth() / 2, kiko::g_renderer.GetHeight() / 2}, 0, 10 }, kiko::g_modelManager.Get("Car.txt"));
		player->m_tag = "Player";
		player->m_game = this;

		// Create Components
		auto spriteComponent = std::make_unique<kiko::SpriteComponent>();
		spriteComponent->m_texture = kiko::g_resources.Get<kiko::Texture>("Car1.png", kiko::g_renderer);
		player->AddComponent(std::move(spriteComponent));

		auto physicsComponent = std::make_unique<kiko::EnginePhysicsComponent>();
		physicsComponent->m_damping = 0.9;
		player->AddComponent(std::move(physicsComponent));



		m_scene->Add(std::move(player));
	}
		m_state = eState::Game;
		break;
		
	case eState::Game:
		{
			m_spawnTimer += dt;
			if (m_spawnTimer >= m_spawnTime && m_scene->GetEnemyCount() < 20) {
				m_spawnTimer = 0;
				std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(kiko::randomf(800.0f, 1200.0f), 0.01f, 0.01f, kiko::DegToRad(kiko::randomf(15.0f, 25.0f)), kiko::Transform { { kiko::randomf(0, (float)kiko::g_renderer.GetWidth()), kiko::randomf(0, (float)kiko::g_renderer.GetHeight()) }, 0, kiko::randomf(8, 11) }, kiko::g_modelManager.Get("EnemyCar.txt"));
				enemy->m_tag = "Enemy";
				enemy->m_game = this;
				m_scene->IncrementEnemyCount();

				// Create Components

				auto component = std::make_unique<kiko::SpriteComponent>();
				int spriteNum = kiko::random(1, 3);
				switch (spriteNum) {
				case 1:
					component->m_texture = kiko::g_resources.Get<kiko::Texture>("Car2.png", kiko::g_renderer);
					break;
				case 2:
					component->m_texture = kiko::g_resources.Get<kiko::Texture>("Car3.png", kiko::g_renderer);
					break;
				case 3:
					component->m_texture = kiko::g_resources.Get<kiko::Texture>("Car4.png", kiko::g_renderer);
					break;
				}
				
				enemy->AddComponent(std::move(component));

				auto physicsComponent = std::make_unique<kiko::EnginePhysicsComponent>();
				physicsComponent->m_damping = 0.9;
				enemy->AddComponent(std::move(physicsComponent));

				m_scene->Add(std::move(enemy));
			}

			m_clockSpawnTimer += dt;
			if (m_clockSpawnTimer >= m_clockSpawnTime && m_scene->GetEnemyCount() < 20) {
				m_clockSpawnTimer = 0;
				float clockWorth = (m_scene->GetEnemyCount() + m_gameTime/4) * kiko::g_time.GetTimeScale();
				std::unique_ptr<Clock> clock = std::make_unique<Clock>(clockWorth, kiko::Transform {{kiko::random(0,kiko::g_renderer.GetWidth()), kiko::random(0,kiko::g_renderer.GetHeight())}, 0, 10}, kiko::g_modelManager.Get("Clock.txt"));
				clock->m_tag = "Clock";
				clock->m_game = this;
				m_scene->Add(std::move(clock));
			}

		}

		break;
	case eState::PlayerDeadStart:
		m_stateTimer = 3.0f;
		m_state = eState::PlayerDead;

		m_gameTime = 0.0f;
		
		break;
	case eState::PlayerDead:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0) {
			m_state = eState::StartLevel;
		}
		
		break;
	case eState::GameOver:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0) {
			m_scene->RemoveAll();
			m_state = eState::Title;
		}
		
		break;
	default:
		break;
	}

	Player* player = m_scene->GetActor<Player>();
	if (player) {
		m_healthText->Create(kiko::g_renderer, std::to_string((int)player->GetHealth()), { 1,1,1,1 });
		m_timerText->Create(kiko::g_renderer, "Time Survived: " + std::to_string(m_gameTime), {1,1,1,1});
		m_bonusTimeText->Create(kiko::g_renderer, "+ " + std::to_string(player->GetBonusTime()), {1,1,1,1});
		m_gameTime += dt;
	}
	
	m_scene->Update(dt);
}

void DrivingGame::Draw(kiko::Renderer& renderer)
{
	if (m_state == eState::Title) {
		m_titleText->Draw(renderer, 40, renderer.GetHeight() / 2);
		m_startPromptText->Draw(renderer, 40, renderer.GetHeight() / 2 + 40);
	}
	else if (m_state == eState::PlayerDead) {
		m_deathText->Draw(renderer, renderer.GetWidth()/2, renderer.GetHeight()/2);
	}
	
	if (m_state != eState::Title) {
		m_timerText->Draw(renderer, 40, renderer.GetHeight()-100);

		m_bonusTimeText->Draw(renderer, 40, renderer.GetHeight()-50);
	}

	m_healthText->Draw(renderer, 40, 40);
	m_scene->Draw(renderer);
}
