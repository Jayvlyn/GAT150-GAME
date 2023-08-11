#pragma once
#include "Component.h"

namespace kiko
{
	class RenderComponent : public Component
	{
	public:
		virtual void Draw(class Renderer& rednerer) = 0;
		virtual float GetRadius() { return 0; }
	};
}