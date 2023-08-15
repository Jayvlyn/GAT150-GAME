#pragma once

namespace kiko
{
	template <typename T>
	class Singleton
	{
	public:
		Singleton(const Singleton& other) = delete; // No copy
		Singleton& operator = (const Singleton& other) = delete; // No assigning to other variable

		static T& Instance()
		{
			static T instance;
			return instance;
		}

	protected:
		Singleton() = default;
	};
}