#pragma once
#include <memory>

namespace kiko
{
	class Scene;
	class Renderer;

	class Game
	{
	public:
		Game() = default;
		virtual ~Game() = default;

		virtual bool Init() = 0;
		virtual void Shutdown() = 0;

		virtual void Update(float dt) = 0;
		virtual void Draw(Renderer& renderer) = 0;

	protected:
		std::unique_ptr<Scene> m_scene;
	};
}