#pragma once 
#include "stdafx.h"
#include <string> 
#include "DispositifD3D11.h" 
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "gdiplusstringformat.h"

#pragma comment (lib,"Gdiplus.lib")
namespace PM3D
{
	class CAfficheurTexte
	{
	public:
		CAfficheurTexte(CDispositifD3D11* pDispositif, std::wstring _text, int _x, int _y, UINT _width, UINT _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color = Gdiplus::Color(255, 0, 0, 0), const Gdiplus::Color& _bgColor = Gdiplus::Color(0, 0, 0, 0));
		CAfficheurTexte(CDispositifD3D11* pDispositif, std::wstring _text, int _x, int _y, UINT _width, UINT _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color, const std::wstring& _fileTexture, const std::wstring& _fileTextureHovered = L"");

		void init();

		virtual ~CAfficheurTexte();
		void Ecrire(const std::wstring& s);
		ID3D11ShaderResourceView* GetTextureView() { return pTextureView; }
		int getWidth() const noexcept { return m_width; }
		int getHeight() const noexcept { return m_height; }
		int getX() const noexcept { return m_x; }
		int getY() const noexcept { return m_y; }

		std::wstring getCurrentText() const noexcept { return m_text; }

		static void Init();
		static void Close();
	private:
		ID3D11Texture2D* pTexture;
		IDXGISurface1* pSurface;
		ID3D11ShaderResourceView* pTextureView;
		CDispositifD3D11* pDispo;
		Gdiplus::Font* pFont;
		std::unique_ptr<Gdiplus::Bitmap> pCharBitmap;
		std::unique_ptr<Gdiplus::Graphics> pCharGraphics;
		std::unique_ptr<Gdiplus::SolidBrush> pBrush;
		std::unique_ptr<Gdiplus::TextureBrush> pTextureBrush;
		std::unique_ptr<Gdiplus::TextureBrush> pTextureHoveredBrush;
		std::unique_ptr<Gdiplus::Image> pImage;
		std::unique_ptr<Gdiplus::StringFormat> pStringFormat;
		// Variables statiques pour GDI+ 
		static ULONG_PTR token;

		std::wstring m_text;

		// Colors
		Gdiplus::Color m_color;
		Gdiplus::Color m_bgColor;

	protected:
		UINT m_width;
		UINT m_height;
		int m_x, m_y;
		bool m_hovered;
	};
} // namespace PM3D
