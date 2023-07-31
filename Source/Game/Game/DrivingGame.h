#pragma once
#include "Framework/Game.h"
#include "Renderer/Text.h"
#include "Core/Time.h"

class DrivingGame : public kiko::Game
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

	

private:
	eState m_state = eState::Title;
	float m_spawnTimer = 15;
	float m_spawnTime = 15;
	float m_clockSpawnTimer = 0;
	float m_clockSpawnTime = 20;

	float m_stateTimer = 0;

	float m_gameTime = 0;

	std::shared_ptr<kiko::Font> m_font;
	std::unique_ptr<kiko::Text> m_healthText;
	std::unique_ptr<kiko::Text> m_titleText;
	std::unique_ptr<kiko::Text> m_startPromptText;
	std::unique_ptr<kiko::Text> m_deathText;
	std::unique_ptr<kiko::Text> m_timerText;
	std::unique_ptr<kiko::Text> m_bonusTimeText;

};
