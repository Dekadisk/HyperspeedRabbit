#pragma once
#include "Button.h"
class ButtonSettings : public Button
{
public:
	ButtonSettings(PM3D::CDispositifD3D11* _pDispositif, std::wstring _text, int _x, int _y, int _width, int _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color = Gdiplus::Color(255, 0, 0, 0), const Gdiplus::Color& _bgColor = Gdiplus::Color(0, 0, 0, 0));

	ButtonSettings(PM3D::CDispositifD3D11* _pDispositif, std::wstring _text, int _x, int _y, int _width, int _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color, const std::wstring& _fileTexture, const std::wstring& _fileTextureHovered);

	~ButtonSettings() = default;

	void clicked(Scene * _scene) override;
};