#pragma once
#include "Button.h"
class ButtonHelp :  public Button
{
public:
	ButtonHelp(PM3D::CDispositifD3D11* _pDispositif, std::wstring _text, int _x, int _y, int _width, int _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color = Gdiplus::Color(255, 0, 0, 0), const Gdiplus::Color& _bgColor = Gdiplus::Color(0, 0, 0, 0));

	ButtonHelp(PM3D::CDispositifD3D11* _pDispositif, std::wstring _text, int _x, int _y, int _width, int _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color, const std::wstring& _fileTexture, const std::wstring& _fileTextureHovered);

	~ButtonHelp() = default;

	void clicked(Scene * _scene) override;
};
