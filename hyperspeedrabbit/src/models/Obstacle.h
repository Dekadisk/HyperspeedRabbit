#pragma once
#include "Objet3DPhysique.h"
#include "DispositifD3D11.h"
#include <string>

class Obstacle : public Objet3DPhysique
{
	bool m_isDestructible{ true };

public:

	Obstacle(PM3D::CDispositifD3D11* _pDispositif,
		physx::PxPhysics* _physics,
		physx::PxScene* _scene,
		physx::PxMaterial* _material,
		std::string _directory,
		std::string _filename,
		DirectX::XMFLOAT3 _position = {},
		DirectX::XMFLOAT4 _rotation = {0.f, 0.f, 0.f, 1.f},
		DirectX::XMFLOAT3 _scale = { 1.f,1.f,1.f },
		float _speed = 1.0f,
		bool _isDestructible = true
		);

	Obstacle(DirectX::XMFLOAT3 _position = {},
		DirectX::XMFLOAT4 _rotation = { 0.f, 0.f, 0.f, 1.f },
		DirectX::XMFLOAT3 _scale = { 1.f,1.f,1.f });

	~Obstacle() = default;

	bool getIsDestructible() noexcept { return m_isDestructible; }
	void setIsDestructible(bool _val) { m_isDestructible = _val; }

	/*void update(float _tempEcoule) override;*/
};

