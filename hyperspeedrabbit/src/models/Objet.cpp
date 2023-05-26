#include "Objet.h"

Objet::Objet(DirectX::XMFLOAT3 _position, DirectX::XMFLOAT4 _rotation): m_position{_position}, m_rotation{ _rotation }
{
	DirectX::XMStoreFloat4(&m_rotation,
		DirectX::XMQuaternionNormalize({ _rotation.x,_rotation.y ,_rotation.z ,_rotation.w }));
	DirectX::XMStoreFloat3(&m_direction,
		DirectX::XMVector3Rotate(DirectX::XMVECTOR{ 1.0f,0.0f,0.0f }, { _rotation.x,_rotation.y, _rotation.z, _rotation.w }));
	m_initDirection = m_direction;
	m_initPosition = m_position;
	m_initRotation = m_rotation;

}

//DirectX::XMFLOAT3 Objet::PxVec3ToXMFLOAT3(const physx::PxVec3& _vec3)
//{
//	return { _vec3.x, _vec3.y, _vec3.z };
//}
//
//physx::PxQuat Objet::XMFLOAT4ToPxQuat(const DirectX::XMFLOAT4& _vec4) {
//	return XMVECTORToPxQuat(XMFLOAT4ToXMVECTOR(_vec4));
//}
//
//DirectX::XMFLOAT4 Objet::XMVECTORToXMFLOAT4(const DirectX::XMVECTOR& _vec3)
//{
//	DirectX::XMFLOAT4 xmfloat4;
//	XMStoreFloat4(&xmfloat4, _vec3);
//	return { xmfloat4.x, xmfloat4.y, xmfloat4.z, xmfloat4.w };
//}
//
//DirectX::XMFLOAT3 Objet::XMVECTORToXMFLOAT3(const DirectX::XMVECTOR& _vec3)
//{
//	DirectX::XMFLOAT3 xmfloat4;
//	XMStoreFloat3(&xmfloat4, _vec3);
//	return { xmfloat4.x, xmfloat4.y, xmfloat4.z};
//}
//
//DirectX::XMVECTOR Objet::XMFLOAT4ToXMVECTOR(const DirectX::XMFLOAT4& xmfloat4)
//{
//	return DirectX::XMVECTOR{ xmfloat4.x, xmfloat4.y, xmfloat4.z, xmfloat4.w };
//}
//
//physx::PxQuat Objet::XMVECTORToPxQuat(const DirectX::XMVECTOR& _vec3)
//{
//	DirectX::XMFLOAT4 xmfloat4;
//	XMStoreFloat4(&xmfloat4, _vec3);
//	return { xmfloat4.x, xmfloat4.y, xmfloat4.z, xmfloat4.w };
//}
//
////DirectX::XMFLOAT3 Objet::PxVec3ToXMFLOAT3(physx::PxVec3 _vec3)
////{
////	return { _vec3.x, _vec3.y, _vec3.z };
////}
//
//physx::PxVec3 Objet::XMFLOAT3ToPxVec3(const DirectX::XMFLOAT3& _vec3)
//{
//	return { _vec3.x, _vec3.y, _vec3.z };
//}
//
//DirectX::XMVECTOR Objet::PxVec3ToXMVECTOR(const physx::PxVec3& _vec3)
//{
//	return { _vec3.x, _vec3.y, _vec3.z };
//}
//
//physx::PxVec3 Objet::XMVECTORToPxVec3(const DirectX::XMVECTOR& _vec)
//{
//	DirectX::XMFLOAT3 xmfloat3;
//	XMStoreFloat3(&xmfloat3, _vec);
//	return { xmfloat3.x, xmfloat3.y, xmfloat3.z };
//}
//
//DirectX::XMFLOAT4 Objet::PxQuatToXMFLOAT4(const physx::PxQuat& _vec4)
//{
//	return { _vec4.x,_vec4.y,_vec4.z,_vec4.w };
//}
//
//DirectX::XMVECTOR Objet::XMFLOAT3ToXMVECTOR(const DirectX::XMFLOAT3& _vec)
//{
//	return {_vec.x,_vec.y,_vec.z};
//}
void Objet::setPosition(const DirectX::XMFLOAT3& _position) noexcept
{
	m_position = _position;
}

void Objet::setDirection(const DirectX::XMFLOAT3& _direction) noexcept
{
	m_direction = _direction;
	//PxQuat(acos(currentDir.dot(dirTarget)) / 60.f, currentDir.cross(dirTarget).getNormalized());
	//DirectX::XMVECTOR origin{ 1.f,0.f,0.f };
	//DirectX::XMVECTOR target{ _direction.x,_direction.y,_direction.z };
	//DirectX::XMVECTOR rotation = acos(DirectX::DirectX::XMVector2Dot(origin,target))
}

void Objet::setRotation(const DirectX::XMFLOAT4& _rotation) noexcept
{
	m_rotation = _rotation;
	DirectX::XMStoreFloat3(&m_direction,
		DirectX::XMVector3Rotate(DirectX::XMVECTOR{ 1.0f,0.0f,0.0f }, { _rotation.x,_rotation.y, _rotation.z, _rotation.w }));

}

//physx::PxVec3 Objet::XMFLOAT3ToPxVec3(DirectX::XMFLOAT3 _vec3)
//{
//	return { _vec3.x, _vec3.y, _vec3.z };
//}

void Objet::reset() noexcept {
	m_position = m_initPosition;
	m_rotation = m_initRotation;
	m_direction = m_initDirection;
}