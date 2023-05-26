#include "stdafx.h"
#include "Ball.h"

#include "ResourceManager.h"

using namespace DirectX;

Ball::Ball(PM3D::CDispositifD3D11* _pDispositif, [[maybe_unused]] XMFLOAT3 _position, [[maybe_unused]] XMFLOAT4 _rotation)
{
	//     GRPHIQUE     //

	PM3D::CChargeurOBJ& chargeurObj = ResourceManager::get().getChargeur("assets/modeles/Obstacles/Ball/","Ball.obj");
	m_meshs.emplace_back(std::make_unique<PM3D::CObjetMesh>(chargeurObj, _pDispositif, "assets/modeles/Obstacles/Ball/", "Ball"));

	for (auto& mesh : m_meshs)
	{
		XMMATRIX matWorld = mesh->getMatWorld();
		matWorld *= XMMatrixTranslation(_position.x, _position.y, _position.z);
		mesh->setMatWorld(matWorld);
	}
	m_position = _position;
}
