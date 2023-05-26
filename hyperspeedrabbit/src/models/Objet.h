#pragma once
#include "stdafx.h"
#include <string>
#include <PxPhysicsAPI.h>

class Objet
{
public:
	Objet(DirectX::XMFLOAT3 _position = {}, DirectX::XMFLOAT4 _rotation = { 0.0f, 0.0f, 0.0f, 1.0f });
	virtual ~Objet() = default;

	virtual void anime(float) = 0 ;
	virtual void InitBuffers() noexcept = 0;
	virtual void DrawShadowMap(const DirectX::XMMATRIX) noexcept = 0;
	virtual void Draw(ID3D11ShaderResourceView* _pShadowMapView, const DirectX::XMMATRIX, const DirectX::XMVECTOR) noexcept = 0;
	virtual void update(float) = 0;

	/*static DirectX::XMFLOAT3 PxVec3ToXMFLOAT3(const physx::PxVec3& _vec3);
	static DirectX::XMFLOAT4 XMVECTORToXMFLOAT4(const DirectX::XMVECTOR& _vec3);
	static DirectX::XMFLOAT3 XMVECTORToXMFLOAT3(const DirectX::XMVECTOR& _vec3);
	static physx::PxQuat XMVECTORToPxQuat(const DirectX::XMVECTOR& _vec3);

	static physx::PxVec3 XMFLOAT3ToPxVec3(const DirectX::XMFLOAT3& _vec3);
	static DirectX::XMVECTOR XMFLOAT4ToXMVECTOR(const DirectX::XMFLOAT4& _vec4);
	static physx::PxQuat XMFLOAT4ToPxQuat(const DirectX::XMFLOAT4& _vec4);

	static DirectX::XMVECTOR PxVec3ToXMVECTOR(const physx::PxVec3& _vec3);
	static physx::PxVec3 XMVECTORToPxVec3(const DirectX::XMVECTOR& _vec);
	static DirectX::XMVECTOR XMFLOAT3ToXMVECTOR(const DirectX::XMFLOAT3& _vec);

	static DirectX::XMFLOAT4 PxQuatToXMFLOAT4(const physx::PxQuat& _vec4);*/

	DirectX::XMFLOAT3 getPosition() const noexcept { return m_position; }
	DirectX::XMFLOAT3 getDirection() const noexcept { return m_direction; }
	DirectX::XMFLOAT4 getRotation() const noexcept { return m_rotation; }

	void setPosition(const DirectX::XMFLOAT3&) noexcept;
	void setDirection(const DirectX::XMFLOAT3&) noexcept;
	void setRotation(const DirectX::XMFLOAT4&) noexcept;

	virtual void reset() noexcept;

	std::string typeTag;
protected:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_initPosition;

	DirectX::XMFLOAT4 m_rotation;
	DirectX::XMFLOAT4 m_initRotation;

	DirectX::XMFLOAT3 m_direction;
	DirectX::XMFLOAT3 m_initDirection;

};