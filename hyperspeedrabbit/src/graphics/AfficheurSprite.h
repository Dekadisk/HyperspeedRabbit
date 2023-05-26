#pragma once
#include "d3dx11effect.h"
#include "Objet3D.h"

namespace PM3D
{

	class CDispositifD3D11;

	class CSommetSprite
	{
	public:
		CSommetSprite() = default;
		CSommetSprite(const XMFLOAT3& position, const XMFLOAT2& coordTex)
			: m_Position(position)
			, m_CoordTex(coordTex)
		{
		}

	public:
		static UINT numElements;
		static D3D11_INPUT_ELEMENT_DESC layout[];

		XMFLOAT3 m_Position;
		XMFLOAT2 m_CoordTex;
	};

	class CAfficheurSprite
	{
	public:
		CAfficheurSprite(CDispositifD3D11* _pDispositif, bool _activateBlender = true);
		virtual ~CAfficheurSprite();
		virtual void Draw();

		void AjouterSprite(const std::string& NomTexture, int _x = 0, int _y = 0, int _width = 0, int _height = 0);
		void AjouterPanneau(const std::string& NomTexture, const XMFLOAT3& _position, float _dx = 0.0f, float _dy = 0.0f);
		void AjouterSpriteTexte(ID3D11ShaderResourceView* pTexture, int _x, int _y);

		void ClearSprites();

	private:
		bool m_activateBlender = true;

		class CSprite
		{
		public:
			ID3D11ShaderResourceView* pTextureD3D;

			XMMATRIX matPosDim;
			bool bPanneau;
			bool m_visible;

			CSprite()
				: bPanneau(false)
				, pTextureD3D(nullptr), matPosDim{}
			{
			}
		};

		class CPanneau : public CSprite
		{
		public:
			XMFLOAT3 position;
			XMFLOAT2 dimension;

			CPanneau()
			{
				bPanneau = true;
			}
		};

		static CSommetSprite sommets[6];
		ID3D11Buffer* pVertexBuffer;
		CDispositifD3D11* pDispositif;

		ID3D11Buffer* pConstantBuffer;
		ID3DX11Effect* pEffet;
		ID3DX11EffectTechnique* pTechnique;
		ID3DX11EffectPass* pPasse;
		ID3D11InputLayout* pVertexLayout;

		ID3D11SamplerState* pSampleState;

		// Tous nos sprites
		std::vector<std::unique_ptr<CSprite>> tabSprites;

		void InitEffet();
	};

} // namespace PM3D
