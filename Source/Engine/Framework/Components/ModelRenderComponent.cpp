#include "ModelRenderComponent.h"
#include "Framework/Actor.h"
#include "Framework/Resource/ResourceManager.h"

namespace kiko
{
	CLASS_DEFINITION(ModelRenderComponent)

	bool ModelRenderComponent::Initialize()
	{
		if(!modelName.empty()) m_model = GET_RESOURCE(Model, modelName);

		return false;
	}

	void ModelRenderComponent::Update(float dt)
	{
	}

	void ModelRenderComponent::Draw(Renderer& renderer)
	{
		m_model->Draw(renderer, m_owner->transform);
	}

	void ModelRenderComponent::Read(const json_t& value)
	{
		READ_DATA(value, modelName);
	}
}


