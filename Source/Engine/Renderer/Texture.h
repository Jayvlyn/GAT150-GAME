#pragma once
#include "Core/Vector2.h"

struct SDL_Texture;
namespace kiko
{
	class Texture
	{
	public:
		Texture() = default;
		~Texture();
		bool Create(class Renderer& renderer, const std::string& filename);
		vec2 GetSize();

		friend class Renderer;
	private:
		SDL_Texture* m_texture = nullptr;
	};
}