#include "Objet3d.h"

Objet3D::Objet3D(DirectX::XMFLOAT3 _position, DirectX::XMFLOAT4 _rotation, DirectX::XMFLOAT3 _scale) 
	: m_scale{ _scale }, Objet(_position, _rotation)
{
}

void Objet3D::InitBuffers() noexcept {
	for (auto& mesh : m_meshs)
	{
		mesh->InitBuffers();
	}
	for (auto& terrain : m_terrains)
	{
		terrain->InitBuffers();
	}
}
void Objet3D::DrawShadowMap(const XMMATRIX _mVPLight) noexcept {
	for (auto& mesh : m_meshs)
	{
		mesh->DrawShadowMap(_mVPLight);
	}
	for (auto& terrain : m_terrains)
	{
		terrain->DrawShadowMap(_mVPLight);
	}
}
void Objet3D::Draw(ID3D11ShaderResourceView* _pShadowMapView, const DirectX::XMMATRIX _mVPLight, const XMVECTOR _CamDir) noexcept {
	for (auto& mesh : m_meshs)
	{
		mesh->Draw(_pShadowMapView, _mVPLight, _CamDir, m_position);
	}
	for (auto& terrain : m_terrains)
	{
		terrain->Draw(_pShadowMapView,_mVPLight, _CamDir);
	}
}


void Objet3D::update(float) {
	for (auto& mesh : m_meshs)
	{
		XMMATRIX matWorld =  XMMatrixIdentity();
		XMVECTOR _q = XMVECTOR{ m_rotation.x, m_rotation.y, m_rotation.z, m_rotation.w };
		matWorld *= XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		matWorld *= XMMatrixRotationQuaternion(_q);
		matWorld *= XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		mesh->setMatWorld(matWorld);
	}
	for (auto& terrain : m_terrains)
	{
		XMMATRIX matWorld = XMMatrixIdentity();
		XMVECTOR _q = XMVECTOR{ m_rotation.x, m_rotation.y, m_rotation.z, m_rotation.w };
		matWorld *= XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		matWorld *= XMMatrixRotationQuaternion(_q);
		matWorld *= XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		terrain->setMatWorld(matWorld);
	}
}

bool Objet3D::nearPoint(const physx::PxVec3& _pt1, const physx::PxVec3& _pt2, const float _epsilon) const
{
	physx::PxVec3 v = { physx::PxAbs(_pt1.x - _pt2.x), physx::PxAbs(_pt1.y - _pt2.y), physx::PxAbs(_pt1.z - _pt2.z) };
	return v.maxElement() < _epsilon;
}

void Objet3D::addMaterial(std::shared_ptr<PM3D::Material> _material) noexcept{
	m_meshs[0]->addMaterial(_material);
}

bool Objet3D::isFar() const noexcept
{
	for (auto& mesh : m_meshs)
	{
		if (!mesh->isFar())
			return false;
	}
	return true;
}
