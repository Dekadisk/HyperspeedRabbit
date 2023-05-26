#include "stdafx.h"
#include "Collectable.h"
#include "ResourceManager.h"

using namespace physx;

Collectable::Collectable(PM3D::CDispositifD3D11* _pDispositif,
	physx::PxPhysics* _physics,
	physx::PxScene* _scene,
	physx::PxMaterial* _material,
	std::string _directory,
	std::string _filename,
	DirectX::XMFLOAT3 _position,
	DirectX::XMFLOAT4 _rotation,
	DirectX::XMFLOAT3 _scale,
	physx::PxVec3 _axisRotation,
	float _speed, bool _isBonus) : Objet3DPhysique(_position, _rotation, _scale)

{
	m_isBonus = _isBonus;

	PxShape* collectableShape = _physics->createShape(PxBoxGeometry(m_scale.x / 2, m_scale.y / 2, m_scale.z / 2), *_material, true);

	m_actor = PxCreateDynamic(*_physics, PxTransform(_position.x, _position.y, _position.z), *collectableShape, 10.0f);
	m_actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	_scene->addActor(*m_actor);

	PM3D::CChargeurOBJ& chargeurObj = ResourceManager::get().getChargeur(_directory, _filename + ".obj");
	m_meshs.emplace_back(std::make_unique<PM3D::CObjetMesh>(chargeurObj, _pDispositif, _directory, _filename));

	m_collectableValue = (_isBonus ? 1000 : -500);

	m_axisRotation = _axisRotation;
}

void Collectable::update(float) {
	m_actor->setAngularVelocity(m_axisRotation);
}

int Collectable::getCollectableValue() const noexcept {
	return m_collectableValue;
}

bool Collectable::isBonus() const noexcept {
	return m_isBonus;
}