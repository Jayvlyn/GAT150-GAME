#include "DrivingGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Framework/Framework.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"

#include "Renderer/Renderer.h"

#include "Core/Time.h"
#include "Core/Transform.h"

#include <memory>



bool DrivingGame::Init()
{
	// Create font / text objects
	m_font = GET_RESOURCE(kiko::Font,"Coalition_v2..ttf", 24);

	m_healthText = std::make_unique<kiko::Text>(m_font);
	m_healthText->Create(kiko::g_renderer, "(Your health will be here, DONT CRASH!)", kiko::Color{ 1, 1, 1, 1});

	m_titleText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Coalition_v2..ttf", 48));
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
	m_scene->Load("Scenes/Scene.json");
	m_scene->Initialize();

	// add events

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
			auto player = INSTANTIATE(Player, "Player");
			player->transform = kiko::Transform{ {400,300}, 0 , 1 };
			player->Initialize();
			m_scene->Add(std::move(player));

			auto enemy = INSTANTIATE(Enemy, "Enemy");
			enemy->transform = kiko::Transform{ {kiko::random(800), kiko::random(600)}, kiko::randomf(kiko::TwoPi), 1 };
			enemy->Initialize();
			m_scene->Add(std::move(enemy));
	}
		m_state = eState::Game;
		break;
		
	case eState::Game:
	{
		m_spawnTimer += dt;
		if (m_spawnTimer >= m_spawnTime && m_scene->GetEnemyCount() < 20) {
			m_spawnTimer = 0;
			m_scene->IncrementEnemyCount();

			// Create Components

			auto spriteComponent = std::make_unique<kiko::SpriteComponent>();
			int spriteNum = kiko::random(1, 3);
			switch (spriteNum) {
			case 1:
				spriteComponent->m_texture = GET_RESOURCE(kiko::Texture, "Car2.png", kiko::g_renderer);
				break;
			case 2:
				spriteComponent->m_texture = GET_RESOURCE(kiko::Texture, "Car3.png", kiko::g_renderer);
				break;
			case 3:
				spriteComponent->m_texture = GET_RESOURCE(kiko::Texture, "Car4.png", kiko::g_renderer);
				break;
			}

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

	kiko::Player* player = m_scene->GetActor<kiko::Player>();
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
	m_scene->Draw(renderer);

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
}

void DrivingGame::OnPlayerDead(const kiko::Event& event)
{
	m_state = eState::PlayerDeadStart;
}
