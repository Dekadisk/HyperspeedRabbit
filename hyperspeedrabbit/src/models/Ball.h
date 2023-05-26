#pragma once
#include "Objet3DPhysique.h"
#include "DispositifD3D11.h"

class Ball : public Objet3D {

public:
	Ball(PM3D::CDispositifD3D11* _pDispositif,
		DirectX::XMFLOAT3 _position = {},
		DirectX::XMFLOAT4 _rotation = { 0.f, 0.f, 0.f, 1.f });

	virtual ~Ball() = default;

	virtual void update(float) {};

};

