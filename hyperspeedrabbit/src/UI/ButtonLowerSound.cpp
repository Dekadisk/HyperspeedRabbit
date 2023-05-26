#include "stdafx.h"
#include "ButtonLowerSound.h"
#include "Scene.h"

ButtonLowerSound::ButtonLowerSound(PM3D::CDispositifD3D11* _pDispositif, std::wstring _text, int _x, int _y, int _width, int _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color, const Gdiplus::Color& _bgColor) : Button(_pDispositif, _text, _x, _y, _width, _height, _pPolice, _color, _bgColor)
{
}

ButtonLowerSound::ButtonLowerSound(PM3D::CDispositifD3D11* _pDispositif, std::wstring _text, int _x, int _y, int _width, int _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color, const std::wstring& _fileTexture, const std::wstring& _fileTextureHovered) : Button(_pDispositif, _text, _x, _y, _width, _height, _pPolice, _color, _fileTexture, _fileTextureHovered)
{
}

void ButtonLowerSound::clicked(Scene* _scene)
{
	_scene->lowerSound();
}