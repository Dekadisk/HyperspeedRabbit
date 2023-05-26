#pragma once
#include "Objet3d.h"
class ObjDecoration : public Objet3D
{
public:
	ObjDecoration(DirectX::XMFLOAT3 _position = {}
	, DirectX::XMFLOAT4 _rotation = { 0.0f, 0.0f, 0.0f, 1.0f }
	, DirectX::XMFLOAT3 _scale = {1.f,1.f,1.f});
};