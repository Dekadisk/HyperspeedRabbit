#include "stdafx.h"
#include "Objet3DPhysique.h"
#include <src\Converter.h>

Objet3DPhysique::Objet3DPhysique(DirectX::XMFLOAT3 _position, DirectX::XMFLOAT4 _rotation, DirectX::XMFLOAT3 _scale, float _speed)
	: m_speed{_speed}, Objet3D(_position, _rotation, _scale)
{
}

Objet3DPhysique::Objet3DPhysique(DirectX::XMFLOAT3 _position, DirectX::XMFLOAT4 _rotation, float _speed) 
	: m_speed{_speed }, Objet3D(_position, _rotation)
{
}

Objet3DPhysique::Objet3DPhysique(DirectX::XMFLOAT3 _scale, float _speed)
	: m_speed{ _speed }, Objet3D(_scale)
{
}

Objet3DPhysique::Objet3DPhysique(float _speed) 
	: m_speed{ _speed }, Objet3D()
{
}

void Objet3DPhysique::anime([[maybe_unused]] float tempsEcoule) {
	auto newPos = m_actor->getGlobalPose().p;
	auto newRot = m_actor->getGlobalPose().q;
	auto direction = m_actor->getGlobalPose().q.getBasisVector0();

	for (auto& mesh : m_meshs) {
		XMMATRIX matWorld = XMMatrixIdentity();
		matWorld *= XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		matWorld *= XMMatrixRotationQuaternion({ newRot.x,newRot.y,newRot.z,newRot.w });
		matWorld *= XMMatrixTranslationFromVector({ newPos.x, newPos.y , newPos.z });
		mesh->setMatWorld(matWorld);
	}

	m_position = { newPos.x, newPos.y, newPos.z };
	m_rotation = { newRot.x,newRot.y,newRot.z,newRot.w };
	m_direction = { direction.x, direction.y, direction.z };
}

void Objet3DPhysique::reset() noexcept{
	Objet::reset();
	m_actor->setGlobalPose(physx::PxTransform(caster<physx::PxVec3>(m_initPosition), physx::PxQuat(m_initRotation.x, m_initRotation.y, m_initRotation.z, m_initRotation.w)));
	m_actor->setLinearVelocity({ 0.f,0.f,0.f });
	m_actor->setAngularVelocity({ 0.f,0.f,0.f });
}