#include "stdafx.h"
#include "ButtonHigherSound.h"
#include "Scene.h"

ButtonHigherSound::ButtonHigherSound(PM3D::CDispositifD3D11* _pDispositif, std::wstring _text, int _x, int _y, int _width, int _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color, const Gdiplus::Color& _bgColor) : Button(_pDispositif, _text, _x, _y, _width, _height, _pPolice, _color, _bgColor)
{
}

ButtonHigherSound::ButtonHigherSound(PM3D::CDispositifD3D11* _pDispositif, std::wstring _text, int _x, int _y, int _width, int _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color, const std::wstring& _fileTexture, const std::wstring& _fileTextureHovered) : Button(_pDispositif, _text, _x, _y, _width, _height, _pPolice, _color, _fileTexture, _fileTextureHovered)
{
}

void ButtonHigherSound::clicked(Scene* _scene)
{
	_scene->higherSound();
}
