#include "Actor.h"
#include "Components/RenderComponent.h"

namespace kiko 
{
	CLASS_DEFINITION(Actor);

	bool kiko::Actor::Initialize()
	{
		for (auto& component : components)
		{
			component->Initialize();
		}

		return true;
	}

	void Actor::OnDestroy()
	{
		for (auto& component : components)
		{
			component->OnDestroy();
		}
	}

	void Actor::Update(float dt)
	{
		if (lifespan != -1.0f && destroyed != true) {
			lifespan -= dt;
			destroyed = (lifespan <= 0);
		}

		for (auto& component : components)
		{
			component->Update(dt);
		}

		// Collision timer
		if (collision && collisionTimer > 0) {
			collisionTimer -= dt;
			if (collisionTimer <= 0) collision = false;
		}
	}

	void Actor::Draw(kiko::Renderer& renderer) 
	{
		//if (m_model) m_model->Draw(renderer, m_transform);
		for (auto& component : components)
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
		components.push_back(std::move(component));
	}

	void Actor::Read(const json_t& value)
	{
		Object::Read(value);

		READ_DATA(value, tag);
		READ_DATA(value, lifespan);

		if(HAS_DATA(value, transform)) transform.Read(value);

		if (HAS_DATA(value, components) && GET_DATA(value, components).IsArray())
		{
			for (auto& componentValue : GET_DATA(value, components).GetArray())
			{
				std::string type;
				READ_DATA(componentValue, type);

				auto component = CREATE_CLASS_BASE(Component, type);
				component->Read(componentValue);
				AddComponent(std::move(component));
			}
		}
	}
}
