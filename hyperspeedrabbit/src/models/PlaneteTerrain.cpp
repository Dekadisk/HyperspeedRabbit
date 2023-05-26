#include "stdafx.h"
#include "PlaneteTerrain.h"
#include <memory>
#include "DispositifD3D11.h"
#include "MoteurWindows.h"
#include "Texture.h"
PlaneteTerrain::PlaneteTerrain(const char* filename, XMFLOAT3 scale, PM3D::CDispositifD3D11* pDispositif, XMFLOAT3 _position, XMFLOAT4 _rotation) 
{
	//     GRAPHIQUE       //
	m_position = _position;
	m_rotation = _rotation;
	m_terrains.emplace_back(std::make_unique<PM3D::CTerrain>(filename,scale,pDispositif));
	PM3D::CTexture* texture1 = PM3D::CMoteurWindows::get().GetTextureManager().GetNewTexture(L"assets/textures/DirtTexture.dds", pDispositif);
	PM3D::CTexture* texture2 = PM3D::CMoteurWindows::get().GetTextureManager().GetNewTexture(L"assets/textures/Snow.dds", pDispositif);
	PM3D::CTexture* texture3 = PM3D::CMoteurWindows::get().GetTextureManager().GetNewTexture(L"assets/textures/Terrain32p.dds", pDispositif);
	m_terrains.back()->SetTexture(texture1, texture2, texture3);
}

void PlaneteTerrain::anime(float)
{
}

