#pragma once
#include "Objet3D.h"
#include <PxRigidActor.h>

class Objet3DPhysique : public Objet3D
{
public:
	Objet3DPhysique(DirectX::XMFLOAT3 _position = {}, DirectX::XMFLOAT4 _rotation = { 0.0f, 0.0f, 0.0f, 1.0f }, DirectX::XMFLOAT3 _scale = {1.f,1.f,1.f}, float _speed = 1.0f);
	Objet3DPhysique(DirectX::XMFLOAT3 _position = {}, DirectX::XMFLOAT4 _rotation = { 0.0f, 0.0f, 0.0f, 1.0f }, float _speed = 1.0f);
	Objet3DPhysique(DirectX::XMFLOAT3 _scale, float _speed = 1.0f);
	Objet3DPhysique(float _speed = 1.0f);

	physx::PxRigidBody* getActor() noexcept { return m_actor; }

	void anime(float tempsEcoule);
	physx::PxVec3 getUpVector() { return m_actor->getGlobalPose().q.getBasisVector1(); }
	physx::PxVec3 getHorizontalVector() { return m_actor->getGlobalPose().q.getBasisVector2(); }
	physx::PxShape* getShape() {
		std::unique_ptr<physx::PxShape*> shapes = std::make_unique<physx::PxShape*>();
		m_actor->getShapes(shapes.get(), 1);
		return *(shapes.get());
	}
	void reset() noexcept override;

protected:
	physx::PxRigidBody* m_actor;
	float m_speed;
};