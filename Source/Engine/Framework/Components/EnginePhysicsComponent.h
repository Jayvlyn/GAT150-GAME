#pragma once
#include "PhysicsComponent.h"

namespace kiko
{
	class EnginePhysicsComponent : public PhysicsComponent
	{
		virtual void Update(float dt)override;
		virtual void ApplyForce(const vec2& force)override;
	};
}