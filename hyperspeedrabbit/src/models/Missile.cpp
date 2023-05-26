#include "stdafx.h"
#include "Missile.h"
#include "ChargeurOBJ.h"
#include "ResourceManager.h"
#include <src\Converter.h>

using namespace physx;

Missile::Missile(PM3D::CDispositifD3D11* _pDispositif, physx::PxPhysics* _physics, physx::PxScene* _scene, physx::PxMaterial* _material, DirectX::XMFLOAT3 _startPosition, DirectX::XMFLOAT3 _direction, DirectX::XMFLOAT4 _rotation)
	: Objet3DPhysique(_startPosition, { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, 200.f) 
				 /* Position */	  /*      Rotation        */   /*      Scale       */  /*Speed*/
{
	//     PHYSIQUE     //
	PxShape* shipBox = _physics->createShape(PxBoxGeometry(m_scale.x / 2, m_scale.y / 2, m_scale.z / 2), *_material, true);
	m_actor = PxCreateDynamic(*_physics, PxTransform(m_position.x, m_position.y, m_position.z), *shipBox, 8000.0f);

	_scene->addActor(*m_actor);
	m_actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	// TODO : Checker si la normalisation doit être faite ici
	PxVec3 directionVisee = caster<PxVec3>(_direction).getNormalized();
	m_actor->setLinearVelocity(m_speed*directionVisee);

	PxVec3 directionInitiale = m_actor->getGlobalPose().q.getBasisVector0();
	PxQuat q = PxQuat(acos(directionInitiale.dot(directionVisee)), directionInitiale.cross(directionVisee)).getNormalized();

	m_actor->setGlobalPose(PxTransform(m_actor->getGlobalPose().p, m_actor->getGlobalPose().q * std::move(q)));

	//     GRAPHIQUE       //
	PM3D::CChargeurOBJ& chargeurObj = ResourceManager::get().getChargeur("assets/modeles/Ships/", "PointyShip.obj");
	m_meshs.emplace_back(std::make_unique<PM3D::CObjetMesh>(chargeurObj, _pDispositif, "assets/modeles/Ships/", "PointyShip"));
}

