#include "stdafx.h"
#include "Obstacle.h"
#include <string>

#include "ResourceManager.h"
using namespace physx;
Obstacle::Obstacle(PM3D::CDispositifD3D11* _pDispositif,
	physx::PxPhysics* _physics,
	physx::PxScene* _scene,
	physx::PxMaterial* _material,
	std::string _directory,
	std::string _filename,
	DirectX::XMFLOAT3 _position,
	DirectX::XMFLOAT4 _rotation,
	DirectX::XMFLOAT3 _scale,
	float _speed,
	bool _isDestructible
)
	: Objet3DPhysique(_position, _rotation,_scale)
{
	//PHYSIQYE
	//et dans l'attente je pleure
	//la pluie coule sur mes larmes
	//j'aime la vie
	//j'aime la mort
	//bonjour

	m_isDestructible = _isDestructible;
	PxShape* shipBox = _physics->createShape(PxBoxGeometry(m_scale.x/2, m_scale.y/2, m_scale.z/2), *_material, true);

	m_actor = PxCreateDynamic(*_physics, PxTransform(_position.x, _position.y, _position.z), *shipBox, 10.0f);
	m_actor->setMass(0.f);
	m_actor->setMassSpaceInertiaTensor(PxVec3(0.f));
	m_actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	_scene->addActor(*m_actor);

	//PM3D::CChargeurOBJ& chargeurObj = ResourceManager::get().getChargeur("assets/modeles/Obstacles/Mine/", "Mine.obj");
	//m_meshs.emplace_back(std::make_unique<PM3D::CObjetMesh>(chargeurObj, _pDispositif, "assets/modeles/Obstacles/Mine/", "Mine"));


	PM3D::CChargeurOBJ& chargeurObj = ResourceManager::get().getChargeur(_directory, _filename + ".obj");
	m_meshs.emplace_back(std::make_unique<PM3D::CObjetMesh>(chargeurObj, _pDispositif, _directory, _filename));

}

Obstacle::Obstacle(DirectX::XMFLOAT3 _position, DirectX::XMFLOAT4 _rotation, DirectX::XMFLOAT3 _scale) : Objet3DPhysique(_position, _rotation, _scale)
{
}

//void Obstacle::update([[maybe_unused]] float _tempEcoule) {
//	Objet3DPhysique::update();
//	m_actor->setAngularVelocity({0.f, 3.f, 0.f});
//}