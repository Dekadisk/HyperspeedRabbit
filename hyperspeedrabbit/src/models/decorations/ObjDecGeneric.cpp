#include "stdafx.h"
#include "ObjDecGeneric.h"
#include "ResourceManager.h"

ObjDecGeneric::ObjDecGeneric(PM3D::CDispositifD3D11* _pDispositif,const std::string & _chemin,const std::string & _fileName,
	const DirectX::XMFLOAT3 _position,
	const DirectX::XMFLOAT4 _rotation,
	const DirectX::XMFLOAT3 _scale) : ObjDecoration(_position, _rotation, _scale)
{
	//PM3D::CChargeurOBJ& chargeurObj = ResourceManager::get().getChargeur(_chemin, _fileName + ".obj");
	m_meshs.emplace_back(std::make_unique<PM3D::CObjetMesh>(ResourceManager::get().getChargeur(_chemin, _fileName + ".obj"), _pDispositif, _chemin, _fileName,  8000, 10000, 20000));
}

void ObjDecGeneric::anime(float)
{
}
