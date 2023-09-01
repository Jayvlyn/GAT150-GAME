#pragma once
#include "Framework/Game.h"
#include "Framework/Event/EventManager.h"
#include "Renderer/Text.h"
#include "Core/Time.h"
#include "Enemy.h"

class PlatformGame : public kiko::Game, kiko::IEventListener
{
public:
	enum class eState
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		PlayerDeadStart,
		PlayerDead,
		GameOver,
	};

public:
	// Inherited via Game
	virtual bool Init() override;

	virtual void Shutdown() override;

	virtual void Update(float dt) override;

	virtual void Draw(kiko::Renderer& renderer) override;

	void SetState(eState state) { m_state = state; }

	void OnPlayerDead(const kiko::Event& event);

private:
	eState m_state = eState::Title;
	float m_spawnTimer = 15;
	float m_spawnTime = 15;
	float m_clockSpawnTimer = 0;
	float m_clockSpawnTime = 20;

	float m_stateTimer = 0;

	float m_gameTime = 0;
};
