#include "StdAfx.h" 
#include "strsafe.h" 
#include "DispositifD3D11.h" 
#include "Texture.h" 
#include "resource.h" 
#include "util.h" 
#include "DDSTextureLoader11.h" 
//using namespace UtilitairesDX;
using namespace DirectX;
namespace PM3D
{
	CTexture::CTexture()
	{
	}
	CTexture::~CTexture()
	{
		DXRelacher(m_texture);
	}
	CTexture::CTexture(const std::wstring& _filename, CDispositifD3D11* _pDispositif)
		: m_filename(_filename)
		, m_texture(nullptr)
	{
		ID3D11Device* pDevice = _pDispositif->GetD3DDevice();
		// Charger la texture en ressource 
		DXEssayer(CreateDDSTextureFromFile(pDevice,
			m_filename.c_str(),
			nullptr,
			&m_texture), DXE_FICHIERTEXTUREINTROUVABLE);
	}

}
