#include "PlatformGame.h"
#include "Framework/Framework.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"

#include "Renderer/Renderer.h"

#include "Core/Time.h"
#include "Core/Transform.h"

#include "Player.h"
#include "Enemy.h"

#include "Core/Vector2.h"

#include <memory>
#include <iostream>



bool PlatformGame::Init()
{
	// Load audio
	kiko::g_audioSystem.AddAudio("music", "Audio/GothicGuitar.wav");
	kiko::g_audioSystem.AddAudio("swing", "Audio/swing.wav");
	kiko::g_audioSystem.AddAudio("jump", "Audio/jumpVoice.wav");
	kiko::g_audioSystem.AddAudio("death", "Audio/death.wav");
	kiko::g_audioSystem.AddAudio("coin", "Audio/coin.wav");
	kiko::g_audioSystem.AddAudio("coinLoss", "Audio/coinLoss.wav");
	kiko::g_audioSystem.PlayOneShot("music", true);

	// Scene
	m_scene = std::make_unique<kiko::Scene>();
	m_scene->Load("Scenes/PlatformScene.json");
	m_scene->Load("Scenes/Player.json");
	m_scene->Initialize();

	return true;
}

void PlatformGame::Shutdown() {}

void PlatformGame::Update(float dt)
{
	switch (m_state)
	{
	case eState::Title:
	{
		if (m_scene->GetActorByName("CoinCount")->active) m_scene->GetActorByName("CoinCount")->active = false;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE)) {
			m_scene->GetActorByName("Title")->active = false;
			SetState(eState::StartGame);
		}
		break;
	}
	case eState::StartGame:
		m_scene->Load("Scenes/tilemap.json");
		m_scene->Initialize(false);
		SetState(eState::StartLevel);
		break;
	case eState::StartLevel:
	{
		auto player = INSTANTIATE(Player, "Player");
		player->Initialize();
		player->m_game = this;
		m_scene->Add(std::move(player));

		auto bat = INSTANTIATE(Enemy, "Bat");
		bat->Initialize();
		m_scene->Add(std::move(bat));
		m_scene->IncrementEnemyCount();

		auto hell = INSTANTIATE(Enemy, "HellBeast");
		hell->Initialize();
		m_scene->Add(std::move(hell));
		m_scene->IncrementEnemyCount();

		auto ghost = INSTANTIATE(Enemy, "Ghost");
		ghost->Initialize();
		m_scene->Add(std::move(ghost));
		m_scene->IncrementEnemyCount();
	}
		m_spawnTimer = 1;
		SetState(eState::Game);
		break;
	case eState::Game:
		if (!m_scene->GetActorByName("CoinCount")->active) m_scene->GetActorByName("CoinCount")->active = true;

		if (m_spawnTimer <= 0)
		{
			// spawn coin
			auto coin = INSTANTIATE(Actor, "Coin");
			coin->transform.position = kiko::vec2{ kiko::randomf(50, 1500) , 200.0f };
			coin->Initialize();
			m_scene->Add(std::move(coin));
			// reset timer
			m_spawnTimer = 1;
		}
		else
		{
			// subtract from timer
			m_spawnTimer -= dt;
		}

		break;
	case eState::PlayerDeadStart:
		m_stateTimer = 3;
		SetState(eState::PlayerDead);
		break;
	case eState::PlayerDead:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0) {
			if (m_scene->GetActorByName("HellBeast"))m_scene->GetActorByName("HellBeast")->destroyed = true;
			if (m_scene->GetActorByName("Ghost"))m_scene->GetActorByName("Ghost")->destroyed = true;
			if (m_scene->GetActorByName("Bat"))m_scene->GetActorByName("Bat")->destroyed = true;
			if (m_scene->GetActorByName("Player"))m_scene->GetActorByName("Player")->destroyed = true;
			SetState(eState::GameOver);
		}
		break;
	case eState::GameOver:
		SetState(eState::StartLevel);
		break;
	default:
		break;
	}

	m_scene->Update(dt);
}

void PlatformGame::Draw(kiko::Renderer& renderer)
{
	m_scene->Draw(renderer);
}

void PlatformGame::OnPlayerDead(const kiko::Event& event)
{
	SetState(eState::PlayerDeadStart);
}
