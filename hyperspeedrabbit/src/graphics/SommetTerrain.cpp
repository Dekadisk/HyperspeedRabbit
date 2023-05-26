
#include "StdAfx.h"
#include "sommetTerrain.h"

namespace PM3D
{

	// Definir l'organisation de notre sommet
	D3D11_INPUT_ELEMENT_DESC CSommetTerrain::layout[] =
	{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORDM", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT CSommetTerrain::numElements = ARRAYSIZE(CSommetTerrain::layout);

	CSommetTerrain::CSommetTerrain(const XMFLOAT3& _position, const XMFLOAT3& _normal,const XMFLOAT2& coordTexM, const XMFLOAT2& _coordTex)
		: m_position(_position)
		, m_normal(_normal)
		, m_coordTex(_coordTex),
		m_coordTexM(coordTexM)
	{
	}
} // namespace PM3D
