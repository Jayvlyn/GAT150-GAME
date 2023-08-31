#include "PlatformGame.h"
#include "Framework/Framework.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"

#include "Renderer/Renderer.h"

#include "Core/Time.h"
#include "Core/Transform.h"

#include <memory>



bool PlatformGame::Init()
{
	// Load audio
	kiko::g_audioSystem.AddAudio("music", "Audio/GothicGuitar.wav");
	kiko::g_audioSystem.PlayOneShot("music", true);

	// Scene
	m_scene = std::make_unique<kiko::Scene>();
	m_scene->Load("Scenes/PlatformScene.json");
	m_scene->Initialize();

	// add events

	return true;
}

void PlatformGame::Shutdown() {}

void PlatformGame::Update(float dt)
{
	
	switch (m_state)
	{
	case eState::Title:
	{
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE)) {
			m_scene->GetActorByName("Title")->active = false;
			SetState(eState::StartGame);
		}
		break;
	}
	case eState::StartGame:
		m_scene->Load("Scenes/tilemap.json");
		m_scene->Load("Scenes/Player.json");
		m_scene->GetActorByName("Player")->m_game = this;
		m_scene->Initialize();
		SetState(eState::Game);
		break;
	case eState::StartLevel:
		break;
	case eState::Game:
		break;
	case eState::PlayerDeadStart:
		m_stateTimer = 3;
		SetState(eState::PlayerDead);
		break;
	case eState::PlayerDead:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0) {
			SetState(eState::GameOver);
		}
		break;
	case eState::GameOver:
		m_scene->RemoveAll();
		SetState(eState::StartGame);
		break;
	default:
		SetState(eState::StartGame);
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
