#pragma once
#include "stdafx.h"
#include <limits>
#include <DirectXMath.h>
using namespace DirectX;
namespace PM3D {
	class CLumierePoint
	{
	public:
		CLumierePoint(XMVECTOR _position, 
			XMVECTOR _valA = { .4f, .4f, .4f, 1.0f },
			XMVECTOR _valD = { 0.2f,0.2f,0.2f,1.f },
			XMVECTOR _valS = { 0.4f,0.4f, 0.4f,1.f },
			float _r1 = std::numeric_limits<float>::infinity(), 
			float _r2 = std::numeric_limits<float>::infinity())
			: m_position(_position)
			, m_ValSpeculaire(_valS)
			, m_ValDiffuse(_valD)
			, m_ValAmbiante(_valA)
			, m_cone(_r1)
			, m_range(_r2) {};

		XMVECTOR m_position;
		XMVECTOR m_ValDiffuse;
		XMVECTOR m_ValSpeculaire;
		XMVECTOR m_ValAmbiante;
		float m_cone;
		float m_range;
	};
}