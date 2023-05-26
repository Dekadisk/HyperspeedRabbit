#pragma once 
#include "stdafx.h"
#include "DispositifD3D11.h"
#include <string>
namespace PM3D 
{ 

class CTexture { 

public: 
	CTexture(); 
	~CTexture(); 

	CTexture(const std::wstring& _filename, CDispositifD3D11* _pDispositif);
	const std::wstring& GetFilename() const noexcept { return m_filename; }
	ID3D11ShaderResourceView* GetD3DTexture() const noexcept { return m_texture; }
private: 
	std::wstring m_filename;
	ID3D11ShaderResourceView* m_texture;
};

}