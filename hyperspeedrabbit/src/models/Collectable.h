#pragma once
#include "Objet3DPhysique.h"
#include <string>

class Collectable : public Objet3DPhysique
{
	int m_collectableValue;
	physx::PxVec3 m_axisRotation;


public:
	Collectable(PM3D::CDispositifD3D11* _pDispositif,
		physx::PxPhysics* _physics,
		physx::PxScene* _scene,
		physx::PxMaterial* _material,
		std::string _directory,
		std::string _filename,
		DirectX::XMFLOAT3 _position = {},
		DirectX::XMFLOAT4 _rotation = { 0.f, 0.f, 0.f, 1.f },
		DirectX::XMFLOAT3 _scale = { 10.f,10.f,10.f },
		physx::PxVec3 _axisRotation = { 0.f, 3.f, 0.f },
		float _speed = 1.0f,
		bool isBonus = true
	);
	~Collectable() = default;

	void update(float) override;

	int getCollectableValue() const noexcept;
	bool isBonus() const noexcept;

private:
	bool m_isBonus;
};

