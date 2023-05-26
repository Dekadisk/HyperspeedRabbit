#pragma once

namespace PM3D
{
	template <class T> class Singleton
	{
	public:
        static T& get() noexcept
        {
            static T instance;
            return instance;
        }

		Singleton(Singleton&) = delete;
		void operator=(Singleton&) = delete;

	protected :
		Singleton() = default;
		~Singleton() = default;
	};
}