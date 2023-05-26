#include "stdafx.h"
#include "Camera.h"

#include "MoteurWindows.h"

using namespace DirectX;

namespace PM3D {
	Camera::Camera(const XMVECTOR& position_in, const XMVECTOR& direction_in, const XMVECTOR& up_in, XMMATRIX* pMatView_in, XMMATRIX* pMatProj_in, XMMATRIX* pMatViewProj_in) {
		init(position_in, direction_in, up_in, pMatView_in, pMatProj_in, pMatViewProj_in);
	}

	void Camera::init(const XMVECTOR& position_in, const XMVECTOR& direction_in, const XMVECTOR& up_in, XMMATRIX* pMatView_in, XMMATRIX* pMatProj_in, XMMATRIX* pMatViewProj_in) {

		waitForSwap = false;

		m_position = position_in;
		m_direction = XMVector3Normalize(direction_in);
		m_up = XMVector3Normalize(up_in);

		updateQuat();

		m_pMatView = pMatView_in;
		m_pMatProj = pMatProj_in;
		m_pMatViewProj = pMatViewProj_in;
	}

	void Camera::updateQuat()
	{

		m_direction = XMVector3Normalize(m_direction);
		m_up = XMVector3Normalize(m_up);

		XMFLOAT3 l_direction;
		XMFLOAT3 l_up;
		XMFLOAT3 l_right;

		// TODO : check if Normalize is needed
		XMVECTOR right = XMVector3Normalize(XMVector3Cross(m_up, m_direction));
		XMVECTOR trueUp = XMVector3Normalize(XMVector3Cross(right, m_direction));
		XMStoreFloat3(&l_direction, m_direction);
		XMStoreFloat3(&l_up, trueUp);
		XMStoreFloat3(&l_right, right);

		XMMATRIX tmp = XMMATRIX{ right, trueUp, m_direction , XMVECTOR{ 0,0,0,1 } };
		XMVECTOR quat = XMQuaternionRotationMatrix(tmp);
		XMStoreFloat4(&m_quat, quat);
	}

	void Camera::setPosition(const XMVECTOR& position_in) { m_position = position_in; }

	void Camera::setDirection(const XMVECTOR& direction_in) { m_direction = XMVector3Normalize(direction_in); }

	void Camera::setUp(const XMVECTOR& up_in) { m_up = XMVector3Normalize(up_in); }

	DirectX::XMVECTOR Camera::getRotation() const noexcept
	{
		return XMQuaternionRotationMatrix(m_rotation);
	}

	void Camera::update(float tempsEcoule)
	{
		updateSpecific(tempsEcoule);
		updateMatrices();
		updateQuat();
	}

	void Camera::updateMatrices()
	{
		// Matrice de la vision
		*m_pMatView = XMMatrixLookAtLH(m_position, m_position + m_direction, m_up);

		// Recalculer matViewProj
		*m_pMatViewProj = (*m_pMatView) * (*m_pMatProj);
	}
}