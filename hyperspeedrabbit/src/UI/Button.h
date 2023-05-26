#pragma once
#include "AfficheurTexte.h"

class Scene;
class Button : public PM3D::CAfficheurTexte
{
public:
	Button(PM3D::CDispositifD3D11* _pDispositif, std::wstring _text, int _x, int _y, int _width, int _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color = Gdiplus::Color(255, 0, 0, 0), const Gdiplus::Color& _bgColor = Gdiplus::Color(0, 0, 0, 0));

	Button(PM3D::CDispositifD3D11* _pDispositif, std::wstring _text, int _x, int _y, int _width, int _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color, const std::wstring& _fileTexture, const std::wstring& _fileTextureHovered = L"");

	Button(const Button&) = delete;
	Button& operator=(const Button&) = delete;
	virtual ~Button() = default;

	void setHovered(bool _hovered) noexcept { m_hovered = _hovered; }

	bool containsMouse(int _x, int _y) const noexcept;
	int localPosX(int _x) const noexcept;
	int localPosY(int _y) const noexcept;
	virtual void clicked(Scene* _scene) = 0;
};