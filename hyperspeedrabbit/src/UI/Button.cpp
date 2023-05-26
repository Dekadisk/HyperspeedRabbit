#include "stdafx.h"
#include "Button.h"
#include "Scene.h"

Button::Button(PM3D::CDispositifD3D11* _pDispositif, std::wstring _text, int _x, int _y, int _width, int _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color, const Gdiplus::Color& _bgColor) : CAfficheurTexte(_pDispositif, _text, _x, _y, _width, _height, _pPolice, _color, _bgColor)
{
}

Button::Button(PM3D::CDispositifD3D11* _pDispositif, std::wstring _text, int _x, int _y, int _width, int _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color, const std::wstring& _fileTexture, const std::wstring& _fileTextureHovered) : CAfficheurTexte(_pDispositif, _text, _x, _y, _width, _height, _pPolice, _color, _fileTexture, _fileTextureHovered)
{
}

bool Button::containsMouse(int _x, int _y) const noexcept
{
	return _x >= 0 && _x <= static_cast<int>(m_width) && _y >= 0 && _y <= static_cast<int>(m_height);
}

int Button::localPosX(int _x) const noexcept
{
	return _x - m_x;
}

int Button::localPosY(int _y) const noexcept
{
	return _y - m_y;
}