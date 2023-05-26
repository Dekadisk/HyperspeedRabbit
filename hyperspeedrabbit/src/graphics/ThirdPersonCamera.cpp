#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include <InputManager.h>
#include <src\Converter.h>

void PM3D::ThirdPersonCamera::updateSpecific(float tempsEcoule)
{
	using namespace DirectX;

	XMFLOAT3 dir = m_hyperShip->getDirection();
	XMFLOAT3 pos = m_hyperShip->getPosition();
	XMFLOAT3 base = caster<DirectX::XMFLOAT3>(m_hyperShip->getBaseY());
	pos = { pos.x + base.x*1.f
		, pos.y + base.y* 1.f
		, pos.z + base.z* 1.f };

	m_position = XMLoadFloat3(&pos);
	m_direction = XMLoadFloat3(&dir);
	m_position = m_position - m_direction * 35.8;
	m_up = XMLoadFloat3(&base);
	//m_up = { 0.f, 1.f, 0.f };

	XMVECTOR relativeZ = XMVector3Normalize(XMVector3Cross(m_direction, m_up));

	// Might be expensive to compute every time.
	RECT rectFrame;
	GetWindowRect(m_handler, &rectFrame);
	POINT m_centerFrame = { (rectFrame.right - rectFrame.left) / 2,(rectFrame.bottom - rectFrame.top) / 2 };

	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(m_handler, &cursorPos);

	// Adding to the pile how much the mouse's moved so far
	int m_xMouse = cursorPos.x - m_centerFrame.x;
	int m_yMouse = cursorPos.y - m_centerFrame.y;

	//Vérifier si déplacement vers la gauche
	m_position -= static_cast<float>(m_xMouse) / 2000.f * relativeZ;
	m_position -= static_cast<float>(m_yMouse) / 2000.f * m_up;
}
