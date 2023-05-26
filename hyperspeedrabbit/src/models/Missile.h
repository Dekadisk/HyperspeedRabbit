#pragma once
#include "Objet3DPhysique.h"

class Missile :
	public Objet3DPhysique
{
public:
	Missile(PM3D::CDispositifD3D11* _pDispositif,
		physx::PxPhysics* _physics,
		physx::PxScene* _scene,
		physx::PxMaterial* _material,
		DirectX::XMFLOAT3 _startPosition = {},
		DirectX::XMFLOAT3 _direction = {},
		DirectX::XMFLOAT4 _rotation = { 0.f,0.f,0.f,1.f });

	~Missile() = default;
	bool getIsBlownUp() { return m_isBlownUp; }
	void setIsBlownUp(bool _val) { m_isBlownUp = _val; }

private:
	
	float m_minSpeed{ 0.5f };
	float m_maxSpeed{ 20.0f };
	float m_coeffSpeed{ 1.f };
	bool m_isBlownUp{ false };
};

