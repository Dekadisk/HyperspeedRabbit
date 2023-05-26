#pragma once

#include "stdafx.h"
#include "PxPhysicsAPI.h"
#include "DirectXMath.h"

template<class>
struct getSize;

template<>
struct getSize<DirectX::XMFLOAT3> {
	enum { value = 3 };
};

template<>
struct getSize<DirectX::XMFLOAT4> {
	enum { value = 4 };
};

template<>
struct getSize<physx::PxVec3> {
	enum { value = 3 };
};

template<>
struct getSize<physx::PxQuat> {
	enum { value = 4 };
};

template<>
struct getSize<DirectX::XMVECTOR> {
	enum {value = 0 };
};

class bad_caster {};

template<class D, class S>
D caster(S& s) {
	if constexpr (getSize<S>::value == getSize<D>::value) {
		if constexpr (getSize<S>::value == 3)
			return D{ s.x, s.y, s.z };
		else if constexpr (getSize<S>::value == 4)
			return D{ s.x, s.y, s.z, s.w };
		else if (getSize<S>::value == 0)
			return s;
	}
	else if constexpr (getSize<S>::value == 0)
	{
		if constexpr (getSize<D>::value == 3)
		{
			DirectX::XMFLOAT3 dest;
			DirectX::XMStoreFloat3(&dest, s);
			return D{ dest.x, dest.y, dest.z };
		}
		else {
			DirectX::XMFLOAT4 dest;
			DirectX::XMStoreFloat4(&dest, s);
			return D{ dest.x, dest.y, dest.z, dest.w };
		}
	}
	else if constexpr (getSize<D>::value == 0)
	{
		if constexpr (getSize<S>::value == 3)
			return D{ s.x, s.y, s.z };
		else
			return D{ s.x, s.y, s.z, s.w };
	}
	else throw bad_caster{};
}