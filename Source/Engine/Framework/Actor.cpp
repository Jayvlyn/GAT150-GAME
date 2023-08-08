#include "Actor.h"
#include "Components/RenderComponent.h"

namespace kiko {
	void Actor::Update(float dt)
	{
		if (m_lifespan != -1.0f && m_destroyed != true) {
			m_lifespan -= dt;
			m_destroyed = (m_lifespan <= 0);
		}

		for (auto& component : m_components)
		{
			component->Update(dt);
		}

		// Collision timer
		if (m_collision && m_collisionTimer > 0) {
			m_collisionTimer -= dt;
			if (m_collisionTimer <= 0) m_collision = false;
		}
	}

	void Actor::Draw(kiko::Renderer& renderer) 
	{
		//if (m_model) m_model->Draw(renderer, m_transform);
		for (auto& component : m_components)
		{
			RenderComponent* renderComponent = dynamic_cast<RenderComponent*>(component.get());
			if (renderComponent) // is the component a rendercomponent
			{
				renderComponent->Draw(renderer);
			}
		}
	}

	void Actor::AddComponent(std::unique_ptr<Component> component)
	{
		component->m_owner = this;
		m_components.push_back(std::move(component));
		
	}
}
