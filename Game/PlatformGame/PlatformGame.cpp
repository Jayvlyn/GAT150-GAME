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
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE)) {
			m_scene->GetActorByName("Title")->active = false;
			m_state = eState::StartGame;
		}
		break;
	case eState::StartGame:
		m_scene->Load("Scenes/tilemap.json");
		m_scene->Load("Scenes/Player.json");
		m_scene->Initialize();
		m_state = eState::Game;
		break;
	case eState::StartLevel:
		break;
	case eState::Game:
		break;
	case eState::PlayerDeadStart:
		break;
	case eState::PlayerDead:
		break;
	case eState::GameOver:

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
	m_state = eState::PlayerDeadStart;
}
