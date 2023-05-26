#pragma once
#include "stdafx.h"
#include <DirectXMath.h>
using namespace DirectX;

namespace PM3D {

	class CSommetTerrain
		{
		public:
			CSommetTerrain() = default;
			CSommetTerrain(const XMFLOAT3& _position, const XMFLOAT3& _normal,const XMFLOAT2& _coordTexM,const XMFLOAT2& _coordTex);

		public:
			static UINT numElements;
			static D3D11_INPUT_ELEMENT_DESC layout[];
			XMFLOAT3 getPosition() const noexcept { return m_position; };
			XMFLOAT3 getNormal() const noexcept { return m_normal; };
			XMFLOAT2 getCoordTex() const noexcept { return m_coordTex; };
			XMFLOAT2 getCoordTexM() const noexcept { return m_coordTexM; };
			void setPosition(XMFLOAT3 _v) noexcept { m_position = _v; };
			void setNormal(XMFLOAT3 _v) noexcept { m_normal = _v; };
			void setCoordTex(XMFLOAT2 _v) noexcept { m_coordTex = _v; };

		protected:
			XMFLOAT3 m_position;
			XMFLOAT3 m_normal;
			XMFLOAT2 m_coordTex;
			XMFLOAT2 m_coordTexM;
		};
}