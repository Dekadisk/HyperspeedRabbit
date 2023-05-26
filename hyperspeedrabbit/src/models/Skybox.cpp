#include "stdafx.h"
#include "Skybox.h"
#include "ResourceManager.h"
#include <memory>
#include "DispositifD3D11.h"
Skybox::Skybox(PM3D::CDispositifD3D11* _pDispositif, std::shared_ptr<HyperShip> _hypership, const char* _directory, const char* _filename)
{
	PM3D::CChargeurOBJ& chargeurObj = ResourceManager::get().getChargeur(_directory, _filename,true);
	m_skybox = std::make_unique<PM3D::CMeshSkybox>(chargeurObj, _pDispositif);
	m_hypership = _hypership;
}

void Skybox::update(float)
{
}

void Skybox::anime(float) {
	m_position = caster<XMFLOAT3>(m_hypership->getAnchor());
	XMMATRIX matWorld = XMMatrixIdentity();
	matWorld *= XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	m_skybox->setMatWorld(matWorld);
}

void Skybox::InitBuffers() noexcept {}
void Skybox::DrawShadowMap(const DirectX::XMMATRIX) noexcept {}
void Skybox::Draw(ID3D11ShaderResourceView* _pShadowMapView, const DirectX::XMMATRIX, const DirectX::XMVECTOR) noexcept { m_skybox->Draw(); }
