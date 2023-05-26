#include "stdafx.h"
#include "AfficheurTexte.h"
#include "util.h"
#include "gdiplusenums.h"

namespace PM3D {
	ULONG_PTR CAfficheurTexte::token = 0;

	void CAfficheurTexte::Init()
	{
		Gdiplus::GdiplusStartupInput startupInput(0, TRUE, TRUE);
		Gdiplus::GdiplusStartupOutput startupOutput;
		GdiplusStartup(&token, &startupInput, &startupOutput);
	}

	void CAfficheurTexte::Close() {
		Gdiplus::GdiplusShutdown(token);
	}

	CAfficheurTexte::CAfficheurTexte(CDispositifD3D11* pDispositif, std::wstring _text, int _x, int _y, UINT _width, UINT _height, Gdiplus::Font* _pPolice, const Gdiplus::Color &_color, const Gdiplus::Color &_bgColor)
		: pDispo(pDispositif)
		, m_width(_width)
		, m_height(_height)
		, m_x(_x)
		, m_y(_y)
		, m_text{ _text }
		, m_hovered{false}
		, pFont(_pPolice)
		, m_color{ _color}
		, m_bgColor{ _bgColor }
		, pTexture(nullptr)
		, pSurface(nullptr)
		, pTextureView(nullptr)
		, pCharBitmap(nullptr)
		, pCharGraphics(nullptr)
		, pBrush(nullptr)
		, pTextureHoveredBrush(nullptr)
	{
		init();

		// Brushes
		pBrush = std::make_unique<Gdiplus::SolidBrush>(m_color); // ARGB

		Ecrire(std::move(_text));
	}

	CAfficheurTexte::CAfficheurTexte(CDispositifD3D11* pDispositif, std::wstring _text, int _x, int _y, UINT _width, UINT _height, Gdiplus::Font* _pPolice, const Gdiplus::Color& _color, const std::wstring& _fileTexture, const std::wstring& _fileTextureHovered)
		: pDispo(pDispositif)
		, m_width(_width)
		, m_height(_height)
		, m_x(_x)
		, m_y(_y)
		, m_text{ _text }
		, m_hovered{ false }
		, pFont(_pPolice)
		, m_color{ _color }
		, m_bgColor{ Gdiplus::Color(255, 0, 0, 0) }
		, pTexture(nullptr)
		, pSurface(nullptr)
		, pTextureView(nullptr)
		, pCharBitmap(nullptr)
		, pCharGraphics(nullptr)
		, pBrush(nullptr)
		, pTextureHoveredBrush(nullptr)
	{
		init();
		
		// Brushes
		pBrush = std::make_unique<Gdiplus::SolidBrush>(m_color); // ARGB
		pTextureBrush = std::make_unique<Gdiplus::TextureBrush>(Gdiplus::Image::FromFile(_fileTexture.c_str()));
		pTextureHoveredBrush = std::make_unique<Gdiplus::TextureBrush>(Gdiplus::Image::FromFile(_fileTextureHovered.c_str()));

		Ecrire(std::move(_text));
	}

	void CAfficheurTexte::init()
	{
		// Créer le bitmap et un objet GRAPHICS (un dessinateur) 
		pCharBitmap = std::make_unique<Gdiplus::Bitmap>(m_width, m_height, PixelFormat32bppARGB);
		pCharGraphics = std::make_unique<Gdiplus::Graphics>(pCharBitmap.get());
		// Paramètres de l’objet Graphics 
		pCharGraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
		pCharGraphics->SetCompositingQuality(Gdiplus::CompositingQualityHighSpeed);
		pCharGraphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQuality);
		pCharGraphics->SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighSpeed);
		pCharGraphics->SetSmoothingMode(Gdiplus::SmoothingModeNone);
		pCharGraphics->SetPageUnit(Gdiplus::UnitPixel);
		Gdiplus::TextRenderingHint hint = Gdiplus::TextRenderingHintAntiAlias; //TextRenderingHintSystemDefault; 
		pCharGraphics->SetTextRenderingHint(hint);

		pCharGraphics->Clear(m_bgColor); // ARGB Gdiplus::Color(70, 255, 255, 240)
		Gdiplus::BitmapData bmData;
		pCharBitmap->LockBits(&Gdiplus::Rect(0, 0, m_width, m_height), Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmData);

		pStringFormat = std::make_unique<Gdiplus::StringFormat>(0, LANG_NEUTRAL);
		pStringFormat->SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
		pStringFormat->SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);

		// Création d’une texture de même dimension sur la carte graphique 
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = m_width;
		texDesc.Height = m_height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT; texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = bmData.Scan0;
		data.SysMemPitch = m_width * 4;
		data.SysMemSlicePitch = 0;
		// Création de la texture à partir des données du bitmap 
		DXEssayer(pDispo->GetD3DDevice()->CreateTexture2D(&texDesc, &data, &pTexture));
		// Création d’un « resourve view » pour accéder facilement à la texture 
		// (comme pour les sprites) 
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		DXEssayer(pDispo->GetD3DDevice()->CreateShaderResourceView(pTexture, &srvDesc, &pTextureView));
		pCharBitmap->UnlockBits(&bmData);
	}

	CAfficheurTexte::~CAfficheurTexte() 
	{
		DXRelacher(pTexture);
	}

	void CAfficheurTexte::Ecrire(const std::wstring& s)
	{
		// Effacer 
		pCharGraphics->Clear(m_bgColor);

		if (pTextureBrush.get())
		{
			if (m_hovered)
				pCharGraphics->FillRectangle(pTextureHoveredBrush.get(), Gdiplus::Rect(0, 0, m_width, m_height));
			else
				pCharGraphics->FillRectangle(pTextureBrush.get(), Gdiplus::Rect(0, 0, m_width, m_height));
		}

		// Écrire le nouveau texte
		pCharGraphics->DrawString(s.c_str(), static_cast<int>(s.size()), pFont, Gdiplus::PointF(m_width / 2.f, m_height / 2.f), pStringFormat.get(), pBrush.get());
		// Transférer 
		Gdiplus::BitmapData bmData;
		pCharBitmap->LockBits(&Gdiplus::Rect(0, 0, m_width, m_height), Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmData);
		pDispo->GetImmediateContext()->UpdateSubresource(pTexture, 0, 0, bmData.Scan0, m_width * 4, 0);
		pCharBitmap->UnlockBits(&bmData);
	}
}