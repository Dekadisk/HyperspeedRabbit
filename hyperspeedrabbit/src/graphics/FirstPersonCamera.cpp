#include "stdafx.h"
#include "FirstPersonCamera.h"
#include <src\Converter.h>

void PM3D::FirstPersonCamera::updateSpecific(float)
{
	auto dir = m_object->getDirection();
	auto pos = m_object->getPosition();

	m_direction = XMLoadFloat3(&dir);
	m_position = XMLoadFloat3(&pos) + 0.7f * m_direction + 0.3f * caster<DirectX::XMVECTOR>((m_object)->getUpVector());
	m_up = caster<DirectX::XMVECTOR>((m_object)->getUpVector());
}
