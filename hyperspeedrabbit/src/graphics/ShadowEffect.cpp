#include "stdafx.h"
#include "ShadowEffect.h"
#include "util.h"
#include "Resource.h"

namespace PM3D {

	ShadowEffect::ShadowEffect(CDispositifD3D11* _pDispositif) noexcept : pDispositif{ _pDispositif }
	{

		InitTextureView();
		InitDepthBuffer();
		InitMatricesShadowMap();
	}

	ShadowEffect::~ShadowEffect() {

		DXRelacher(pShadowMapView);
		DXRelacher(pRenderTargetView);
		DXRelacher(pTextureShadowMap);
		DXRelacher(pDepthStencilView);
		DXRelacher(pDepthTexture);
	
	}

	void ShadowEffect::beginShadowEffect(){

		//save the context
		pOldRenderTargetView = pDispositif->GetRenderTargetView();
		pOldDepthStencilView = pDispositif->GetDepthStencilView();
	
		pDispositif->SetRenderTargetView(pRenderTargetView, pDepthStencilView);

		//erase the content of the RenderTarget View & DepthStencilView
		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
		float Couleur[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
		pImmediateContext->ClearRenderTargetView(pRenderTargetView, Couleur);
		pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		//resize the dispositif for the texture.
		pDispositif->SetViewPortDimension(SHADOWMAP_DIM, SHADOWMAP_DIM);

	}

	void ShadowEffect::endShadowEffect() noexcept{
		pDispositif->SetRenderTargetView(pOldRenderTargetView, pOldDepthStencilView);

		//resize the dispositif in the good dimension for rendering
		pDispositif->ResetViewPortDimension();
	}

	void ShadowEffect::InitDepthBuffer() noexcept{
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
		
		//creation of the DepthTexture
		D3D11_TEXTURE2D_DESC depthTextureDesc;
		ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
		depthTextureDesc.Width = SHADOWMAP_DIM;
		depthTextureDesc.Height = SHADOWMAP_DIM;
		depthTextureDesc.MipLevels = 1;
		depthTextureDesc.ArraySize = 1;
		depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTextureDesc.SampleDesc.Count = 1;
		depthTextureDesc.SampleDesc.Quality = 0;
		depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTextureDesc.CPUAccessFlags = 0;
		depthTextureDesc.MiscFlags = 0;

		DXEssayer(
			pD3DDevice->CreateTexture2D(&depthTextureDesc, nullptr, &pDepthTexture),
			DXE_ERREURCREATIONTEXTURE);
	

		// creation of the DepthStencil View
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSView;
		ZeroMemory(&descDSView, sizeof(descDSView));
		descDSView.Format = depthTextureDesc.Format;
		descDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSView.Texture2D.MipSlice = 0;
		
		DXEssayer(
			pD3DDevice->CreateDepthStencilView(pDepthTexture, &descDSView, &pDepthStencilView),
			DXE_ERREURCREATIONDEPTHSTENCILTARGET);
	}

	void ShadowEffect::InitMatricesShadowMap() noexcept{
		//TODO : add the light in the parameter of the method


		// Matrice de la vision vu par la lumière - Le point TO est encore 0,0,0
		mVLight = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(10000.f, 10000.f, 0.f, 1.f),
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		float champDeVision;
		float ratioDAspect;
		float planRapproche;
		float planEloigne;
		champDeVision = DirectX::XM_PI / 4; // 45 degrés 
		ratioDAspect = 2.0f; // 512/512 
		planRapproche = 1000.0; // Pas besoin d’être trop près 
		planEloigne = 100000.0; // Suffisemment pour avoir tous les objets 
		mPLight = DirectX::XMMatrixPerspectiveFovLH(champDeVision,
			ratioDAspect,
			planRapproche,
			planEloigne);
		mVPLight = mVLight * mPLight;
	
	}

	void ShadowEffect::InitTextureView() noexcept {
		
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		// description of the texture
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// this texture will be the render
		textureDesc.Width = SHADOWMAP_DIM;
		textureDesc.Height = SHADOWMAP_DIM;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// cration  of the texture
		pD3DDevice->CreateTexture2D(&textureDesc, nullptr, &pTextureShadowMap);

		// RenderTargetView
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// creation of the RenderTargetView.
		pD3DDevice->CreateRenderTargetView(pTextureShadowMap,
			&renderTargetViewDesc,
			&pRenderTargetView);

		// RessourceView
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Creation of the RessourceView
		pD3DDevice->CreateShaderResourceView(pTextureShadowMap,
			&shaderResourceViewDesc,
			&pShadowMapView);

	}

	ID3D11ShaderResourceView* ShadowEffect::GetShadowMapView() const noexcept {
		return pShadowMapView;
	}

	DirectX::XMMATRIX ShadowEffect::GetMatricVPLight() const noexcept {
		return mVPLight;
	}

}