#pragma once

#include "Vector2.h"
#include "Matrix22.h"

namespace kiko {

	class Transform {

	public:

		vec2 position;
		float rotation = 0;
		float scale = 1;


	public:

		Transform() = default;
		Transform(const vec2& position, float rotation, float scale) :
			position{ position },
			rotation{ rotation },
			scale{ scale }
		{}

		inline mat2 GetMatrix() const
		{
			mat2 ms = mat2::CreateScale(scale);
			mat2 mr = mat2::CreateRotation(rotation);

			return ms * mr;
		}

	};

}