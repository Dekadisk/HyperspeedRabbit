#pragma once

#include "stdafx.h" 
#include <string>
#include "GestionnaireDeTextures.h"

using namespace DirectX;
namespace PM3D {
	class Material
	{
	private:
		std::string m_nomFichierTexture;
		std::string m_nomMateriau;
		ID3D11ShaderResourceView* m_pTextureD3D;
		XMFLOAT4 m_valAmbient;
		XMFLOAT4 m_valDiffuse;
		XMFLOAT4 m_valSpecular;
		float m_puissance;
		bool m_transparent;

	public:
		Material(CDispositifD3D11* _pDispositif = nullptr, const std::string& _nomFichierTexture = {}, const std::string& _nomMateriau = "unknown", const XMFLOAT4& _valAmbient = { 0.2f, 0.2f, 0.2f, 1.0f },
			const XMFLOAT4& _valDiffuse = { 0.4f, 0.4f, 0.4f, 1.0f },
			const XMFLOAT4& _valSpecular = { 0.4f, 0.4f, 0.4f, 1.0f },
			const float _puissance = 1.0f,
			const bool _transparent = false
		);

		~Material() {
		}

		std::string getNomMateriau() const noexcept { return m_nomMateriau; }
		bool isTransparent() { return m_transparent; }
		ID3D11ShaderResourceView* getTexture() const noexcept { return m_pTextureD3D; }
		XMFLOAT4 getAmbiant() const noexcept { return m_valAmbient; }
		XMFLOAT4 getDiffuse() const noexcept { return m_valDiffuse; }
		XMFLOAT4 getSpecular() const noexcept { return m_valSpecular; }
		float getPuissance() const noexcept { return m_puissance; }

		void setTransparent(const bool _transparent) noexcept { m_transparent = _transparent; }
		void setTexture(const PM3D::CTexture* _texture) noexcept {
			if (_texture != nullptr)
				m_pTextureD3D = _texture->GetD3DTexture();
		}
		void setPuissance(const float _puissance) noexcept { m_puissance = _puissance; }
	};

}
