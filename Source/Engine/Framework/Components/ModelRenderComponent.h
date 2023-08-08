#pragma once
#include "RenderComponent.h"
#include "Renderer/Texture.h"

namespace kiko
{
	class ModelRenderComponent : public RenderComponent
	{
	public: 
		void Update(float dt) override;
		void Draw(class Renderer& renderer) override;

	public:


	};
}