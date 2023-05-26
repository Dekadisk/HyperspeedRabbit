#include "stdafx.h"
#include "InputManager.h"
#include "util.h"
#include "resource.h" 

using namespace PM3D;

bool InputManager::m_initialized = false;

InputManager::~InputManager()
{
	if (m_pKeys) {
		m_pKeys->Unacquire();
		m_pKeys->Release();
		m_pKeys = nullptr;
	}

	if (m_pMouse) {
		m_pMouse->Unacquire();
		m_pMouse->Release();
		m_pMouse = nullptr;
	}

	if (m_pJoystick) {
		m_pJoystick->Unacquire();
		m_pJoystick->Release();
		m_pJoystick = nullptr;
	}

	if (m_pDirectInput) {
		m_pDirectInput->Release();
		m_pKeys = nullptr;
	}
}

bool InputManager::init(HINSTANCE hInstance, HWND hWnd)
{
	if (!m_initialized) {
		m_pDirectInput = nullptr;
		m_pKeys = nullptr;
		m_pMouse = nullptr; 
		m_pJoystick = nullptr;

		// DirectInput
		DXEssayer(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& m_pDirectInput, NULL), ERREUR_CREATION_DIRECTINPUT);
		
		// Keyboard
		DXEssayer(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeys, NULL), ERREUR_CREATION_CLAVIER);
		DXEssayer(m_pKeys->SetDataFormat(&c_dfDIKeyboard), ERREUR_CREATION_FORMAT_CLAVIER);
		m_pKeys->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		m_pKeys->Acquire();

		// Mouse
		DXEssayer(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL), ERREUR_CREATION_SOURIS);
		DXEssayer(m_pMouse->SetDataFormat(&c_dfDIMouse), ERREUR_CREATION_FORMAT_SOURIS);
		m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		m_pMouse->Acquire();

		// Joystick 
		m_initialized = true;
	}
	return true;
}

void PM3D::InputManager::updateKeyStates() noexcept
{
	m_pKeys->GetDeviceState(sizeof(m_keysBuffer), (void*)&m_keysBuffer);
}

void InputManager::updateMouseState() noexcept {
	m_pMouse->GetDeviceState(sizeof(m_mouseState), (void*)&m_mouseState);
}

bool PM3D::InputManager::isPressed(UINT key) const noexcept
{
	return (m_keysBuffer[key] & 0x80);
}

const DIMOUSESTATE& InputManager::getMouseState() const noexcept{
	return m_mouseState;
}

void InputManager::acquireMouse() noexcept {
	m_pMouse->Acquire();
}

void InputManager::acquireKeys() noexcept {
	m_pKeys->Acquire();
}

DirectX::XMFLOAT2 InputManager::getMousePosition() const noexcept
{
	RECT rectFrame;
	GetWindowRect(m_handler, &rectFrame);
	POINT m_centerFrame = { (rectFrame.right - rectFrame.left) / 2,(rectFrame.bottom - rectFrame.top) / 2 };

	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(m_handler, &cursorPos);
	return { static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y) };
}