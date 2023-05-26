#include "stdafx.h"
#include "Material.h"
#include "moteurWindows.h" 

namespace PM3D {

	Material::Material(CDispositifD3D11* _pDispositif, 
		const std::string& _nomFichierTexture, const std::string& _nomMateriau , const XMFLOAT4& _valAmbient ,
		const XMFLOAT4& _valDiffuse ,
		const XMFLOAT4& _valSpecular ,
		const float _puissance,
		const bool _transparent) :
		m_nomFichierTexture(_nomFichierTexture),
		m_nomMateriau(_nomMateriau),
		m_pTextureD3D(nullptr),
		m_valAmbient(_valAmbient),
		m_valDiffuse(_valDiffuse),
		m_valSpecular(_valSpecular),
		m_puissance(_puissance),
		m_transparent(_transparent)
	{
		if (m_nomFichierTexture.length() > 0 && _pDispositif != nullptr) {
			CGestionnaireDeTextures& TexturesManager = CMoteurWindows::get().GetTextureManager();
			const std::wstring ws(m_nomFichierTexture.begin(), m_nomFichierTexture.end());
			m_pTextureD3D = TexturesManager.GetNewTexture(ws.c_str(), _pDispositif)->GetD3DTexture();
		}

	}
}