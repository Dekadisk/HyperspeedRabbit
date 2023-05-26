#include "objet3d.h"

namespace PM3D
{
	class CDispositifD3D11;

	class CSommetPanneauPE
	{
	public:
		CSommetPanneauPE() = default;
		CSommetPanneauPE(const XMFLOAT3& position, const XMFLOAT2& coordTex)
			: m_position(position)
			, m_coordTex(coordTex)
		{ }

	public:
		static UINT numElements;
		static D3D11_INPUT_ELEMENT_DESC layout[];

		XMFLOAT3 m_position;
		XMFLOAT2 m_coordTex;
	};

	//   Classe : PanneauPE
	//   BUT : 	Classe pour les post-effects
	class PanneauPE : public Objet3D
	{
	public:
		explicit PanneauPE(CDispositifD3D11* pDispositif_in);
		virtual ~PanneauPE();

		virtual void Draw(ID3D11ShaderResourceView* = nullptr, const DirectX::XMMATRIX = {}, const DirectX::XMVECTOR = {}) noexcept override;

		void DebutPostEffect();
		void FinPostEffect();

		void anime(float) override {}

		void setBlurPercent(float _blurPercent) {
			m_blurPercent = min(1.f, max(0.f, _blurPercent));
		}
		void setRedPercent(float _blurPercent) {
			m_redPercent = min(1.f, max(0.f, _blurPercent));
		}
		void setTechniqueIndice(int _indice) noexcept { m_techniqueIndice = _indice; }

	private:

		void InitEffet();

		static CSommetPanneauPE sommets[6];
		ID3D11Buffer* pVertexBuffer;
		CDispositifD3D11* pDispositif;

		// Pour les effets
		ID3DX11Effect* pEffet;
		ID3DX11EffectTechnique* pTechnique;
		ID3DX11EffectPass* pPasse;

		static const int NOMBRE_TECHNIQUES = 2;
		ID3D11InputLayout* pVertexLayout[NOMBRE_TECHNIQUES];

		ID3D11SamplerState* pSampleState;

		// Texture de rendu pour effets
		ID3D11Texture2D* pTextureScene;
		ID3D11RenderTargetView* pRenderTargetView;
		ID3D11ShaderResourceView* pResourceView;
		ID3D11Texture2D* pDepthTexture;
		ID3D11DepthStencilView* pDepthStencilView;

		ID3D11RenderTargetView* pOldRenderTargetView;
		ID3D11DepthStencilView* pOldDepthStencilView;
		// BLUR
		float m_blurPercent;
		float m_redPercent;
		int m_techniqueIndice;
	};

} // namespace PM3D
