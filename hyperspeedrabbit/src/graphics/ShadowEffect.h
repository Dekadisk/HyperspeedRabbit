#pragma once
#include <DirectXMath.h>
#include "DispositifD3D11.h"

namespace PM3D {
	class ShadowEffect
	{
	private:

		//size of the ShadowMap Texture
		static const int SHADOWMAP_DIM = 1024;

		//dispositif
		CDispositifD3D11* pDispositif;

		//Shadow
		ID3D11Texture2D* pTextureShadowMap;         // Texture pour le shadow map
		ID3D11RenderTargetView* pRenderTargetView;  // Vue cible de rendu
		ID3D11ShaderResourceView* pShadowMapView;   // Vue ressource de shader
		ID3D11Texture2D* pDepthTexture;				// texture de profondeur
		ID3D11DepthStencilView* pDepthStencilView;  // Vue tampon de profondeur

		//Matrices for the shadow map
		DirectX::XMMATRIX mVLight;
		DirectX::XMMATRIX mPLight;
		DirectX::XMMATRIX mVPLight;

		//OldView
		ID3D11RenderTargetView* pOldRenderTargetView;
		ID3D11DepthStencilView* pOldDepthStencilView; 

	public:
		
		ShadowEffect(CDispositifD3D11* _pDispositif) noexcept;
		~ShadowEffect();


		//change the context to the shadow context
		void beginShadowEffect();
		//re-change the context to the real one
		void endShadowEffect() noexcept;

		//init the Depth Texture & view
		void InitDepthBuffer() noexcept;

		//init the "view" from the light
		void InitMatricesShadowMap() noexcept;

		//creation of the RenderTargetView & the RessourceView & the ShadowTexture
		void InitTextureView() noexcept;

		ID3D11ShaderResourceView* GetShadowMapView() const noexcept;

		DirectX::XMMATRIX GetMatricVPLight() const noexcept;
	};
}
