#include "stdafx.h"
#include "FreeCamera.h"
#include <InputManager.h>

using namespace DirectX;

void PM3D::FreeCamera::updateSpecific(float tempsEcoule)
{
	InputManager& inputManager = InputManager::get();

	float coeffMove = 1000.0f;
	XMVECTOR relativeZ = XMVector3Normalize(XMVector3Cross(m_direction, m_up));

	// Vérifier l’état de la key gauche
	if (inputManager.isPressed(DIK_LEFTARROW)) {
		m_position += (coeffMove * relativeZ * tempsEcoule);
	}

	// Vérifier l’état de la key droite
	if (inputManager.isPressed(DIK_RIGHTARROW)) {
		m_position -= (coeffMove * relativeZ * tempsEcoule);
	}

	// Vérifier l'état de la key forward
	if (inputManager.isPressed(DIK_UPARROW)) {
		m_position += (coeffMove * m_direction * tempsEcoule);
	}

	// Vérifier l’état de la key backward
	if (inputManager.isPressed(DIK_DOWNARROW)) {
		m_position -= (coeffMove * m_direction * tempsEcoule);
	}

	// Go down
	if (inputManager.isPressed(DIK_Q)) {
		m_position += (coeffMove * XMVECTOR{0.f, 1.0f, 0.0f} *tempsEcoule);
	}

	// Go down
	if (inputManager.isPressed(DIK_E)) {
		m_position -= (coeffMove* XMVECTOR{ 0.f, 1.0f, 0.0f } * tempsEcoule);
	}

	// ******** POUR LA SOURIS ************  
	//Vérifier si déplacement vers la gauche
	if ((inputManager.getMouseState().rgbButtons[2] & 0x80) && (inputManager.getMouseState().lX < 0)) {
		m_direction = XMVector3Transform(m_direction, XMMatrixRotationY(-XM_PI / (5000.0f * tempsEcoule)));
	}

	// Vérifier si déplacement vers la droite
	if ((inputManager.getMouseState().rgbButtons[2] & 0x80) && (inputManager.getMouseState().lX > 0)) {
		m_direction = XMVector3Transform(m_direction, XMMatrixRotationY(XM_PI / (5000.0f * tempsEcoule)));
	}

	//Vérifier si déplacement vers le haut
	if ((inputManager.getMouseState().rgbButtons[2] & 0x80) && (inputManager.getMouseState().lY < 0)) {
		m_direction = XMVector3Transform(m_direction, XMMatrixRotationAxis(relativeZ, XM_PI / (5000.0f * tempsEcoule)));
	}

	// Vérifier si déplacement vers le bas
	if ((inputManager.getMouseState().rgbButtons[2] & 0x80) && (inputManager.getMouseState().lY > 0)) {
		m_direction = XMVector3Transform(m_direction, XMMatrixRotationAxis(relativeZ, -XM_PI / (5000.0f * tempsEcoule)));
	}
}
