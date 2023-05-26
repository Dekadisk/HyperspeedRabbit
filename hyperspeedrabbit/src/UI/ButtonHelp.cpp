#include "stdafx.h"
#include "ButtonHelp.h"
#include "Scene.h"

ButtonHelp::ButtonHelp(PM3D::CDispositifD3D11* _pDispositif, std::wstring _text, int _x, int _y, int _width, int _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color, const Gdiplus::Color& _bgColor) : Button(_pDispositif, _text, _x, _y, _width, _height, _pPolice, _color, _bgColor)
{
}

ButtonHelp::ButtonHelp(PM3D::CDispositifD3D11* _pDispositif, std::wstring _text, int _x, int _y, int _width, int _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color, const std::wstring& _fileTexture, const std::wstring& _fileTextureHovered) : Button(_pDispositif, _text, _x, _y, _width, _height, _pPolice, _color, _fileTexture, _fileTextureHovered)
{
}

void ButtonHelp::clicked(Scene* _scene)
{
	_scene->goToHelpMenu();
}