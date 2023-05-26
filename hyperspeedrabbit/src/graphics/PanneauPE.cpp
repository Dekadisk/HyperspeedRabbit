#include "stdafx.h"
#include "PanneauPE.h"
#include "util.h"
#include "resource.h"
#include "dispositifD3D11.h"
#include <chrono>

namespace PM3D
{
	// Definir l'organisation de notre sommet
	D3D11_INPUT_ELEMENT_DESC CSommetPanneauPE::layout[] =
	{
		{"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,    0,12,D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT CSommetPanneauPE::numElements = ARRAYSIZE(layout);

	CSommetPanneauPE PanneauPE::sommets[6] = {
						CSommetPanneauPE(XMFLOAT3(-1.0f, -1.0f, .0001f), XMFLOAT2(0.0f, 1.0f)),
						CSommetPanneauPE(XMFLOAT3(-1.0f,  1.0f, .0001f), XMFLOAT2(0.0f, 0.0f)),
						CSommetPanneauPE(XMFLOAT3(1.0f,  1.0f, .0001f), XMFLOAT2(1.0f, 0.0f)),
						CSommetPanneauPE(XMFLOAT3(-1.0f, -1.0f, .0001f), XMFLOAT2(0.0f, 1.0f)),
						CSommetPanneauPE(XMFLOAT3(1.0f,  1.0f, .0001f), XMFLOAT2(1.0f, 0.0f)),
						CSommetPanneauPE(XMFLOAT3(1.0f, -1.0f, .0001f), XMFLOAT2(1.0f, 1.0f)) };

	PanneauPE::~PanneauPE()
	{
		DXRelacher(pResourceView);
		DXRelacher(pRenderTargetView);
		DXRelacher(pTextureScene);
		DXRelacher(pDepthStencilView);
		DXRelacher(pDepthTexture);

		DXRelacher(pSampleState);

		DXRelacher(pEffet);
		for (int i = 0; i < NOMBRE_TECHNIQUES; ++i)
		{
			DXRelacher(pVertexLayout[i]);
		}
		DXRelacher(pVertexBuffer);
	}

	//   FONCTION : PanneauPE, constructeur param�tr� 
	//   BUT :	Constructeur d'une classe de PanneauPE
	//   PARAM�TRE:		
	//		pDispositif: pointeur sur notre objet dispositif
	PanneauPE::PanneauPE(CDispositifD3D11* pDispositif_in)
		: pDispositif(pDispositif_in)
		, pVertexBuffer(nullptr)
		, pEffet(nullptr)
		, pTechnique(nullptr)
		, pPasse(nullptr)
		, pVertexLayout{ nullptr, nullptr }
		, pSampleState(nullptr)
		, pTextureScene(nullptr)
		, pRenderTargetView(nullptr)
		, pResourceView(nullptr)
		, pDepthTexture(nullptr)
		, pDepthStencilView(nullptr)
		, pOldRenderTargetView(nullptr)
		, pOldDepthStencilView(nullptr)
		//BLUR
		, m_blurPercent(1.f)
		, m_redPercent(1.f)
		, m_techniqueIndice(1)
	{
		// Cr�ation du vertex buffer et copie des sommets
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(sommets);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = sommets;

		DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer),
			DXE_CREATIONVERTEXBUFFER);

		// Initialisation de l'effet 
		InitEffet();
	}

	void PanneauPE::InitEffet()
	{
		// Compilation et chargement du vertex shader
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

		// Pour l'effet
		ID3DBlob* pFXBlob = nullptr;
		ID3DBlob* pErrBlob = nullptr;

		DXEssayer(
			D3DCompileFromFile(L"src/graphics/shaders/PostEffect.fx", 0, 0, 0,
				"fx_5_0", 0, 0,
				&pFXBlob, &pErrBlob),
			DXE_ERREURCREATION_FX);

		D3DX11CreateEffectFromMemory(
			pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, pD3DDevice, &pEffet);

		pFXBlob->Release();

		pTechnique = pEffet->GetTechniqueByIndex(0);
		pPasse = pTechnique->GetPassByIndex(0);

		// Cr�er l'organisation des sommets pour le VS de notre effet
		D3DX11_PASS_SHADER_DESC effectVSDesc;
		D3DX11_EFFECT_SHADER_DESC effectVSDesc2;

		pPasse->GetVertexShaderDesc(&effectVSDesc);

		effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

		const void* vsCodePtr = effectVSDesc2.pBytecode;
		unsigned vsCodeLen = effectVSDesc2.BytecodeLength;

		DXEssayer(pD3DDevice->CreateInputLayout(CSommetPanneauPE::layout,
			CSommetPanneauPE::numElements,
			vsCodePtr,
			vsCodeLen,
			&pVertexLayout[0]),
			DXE_CREATIONLAYOUT);

		pTechnique = pEffet->GetTechniqueByIndex(1);
		pPasse = pTechnique->GetPassByIndex(0);

		pPasse->GetVertexShaderDesc(&effectVSDesc);
		effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

		const void* vsCodePtr2 = effectVSDesc2.pBytecode;
		vsCodeLen = effectVSDesc2.BytecodeLength;

		DXEssayer(pD3DDevice->CreateInputLayout(CSommetPanneauPE::layout,
			CSommetPanneauPE::numElements,
			vsCodePtr2,
			vsCodeLen,
			&pVertexLayout[1]),
			DXE_CREATIONLAYOUT);

		// Initialisation des param�tres de sampling de la texture
		// Pas n�cessaire d'�tre compliqu� puisque l'affichage sera
		// en 1 pour 1 et � plat
		D3D11_SAMPLER_DESC samplerDesc;

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Cr�ation de l'�tat de sampling
		pD3DDevice->CreateSamplerState(&samplerDesc, &pSampleState);

		// *********************** POUR LE POST EFFECT **************************
		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		// Description de la texture
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Cette texture sera utilis�e comme cible de rendu et 
		// comme ressource de shader
		textureDesc.Width = pDispositif->GetLargeur();
		textureDesc.Height = pDispositif->GetHauteur();;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Cr�ation de la texture
		pD3DDevice->CreateTexture2D(&textureDesc, nullptr, &pTextureScene);

		// VUE - Cible de rendu
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Cr�ation de la vue.
		pD3DDevice->CreateRenderTargetView(pTextureScene,
			&renderTargetViewDesc,
			&pRenderTargetView);

		// VUE � Ressource de shader
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Cr�ation de la vue.
		pD3DDevice->CreateShaderResourceView(pTextureScene,
			&shaderResourceViewDesc,
			&pResourceView);

		// Au tour du tampon de profondeur
		D3D11_TEXTURE2D_DESC depthTextureDesc;
		ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
		depthTextureDesc.Width = pDispositif->GetLargeur();
		depthTextureDesc.Height = pDispositif->GetHauteur();
		depthTextureDesc.MipLevels = 1;
		depthTextureDesc.ArraySize = 1;
		depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTextureDesc.SampleDesc.Count = 1;
		depthTextureDesc.SampleDesc.Quality = 0;
		depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTextureDesc.CPUAccessFlags = 0;
		depthTextureDesc.MiscFlags = 0;

		DXEssayer(pD3DDevice->CreateTexture2D(&depthTextureDesc, nullptr, &pDepthTexture),
			DXE_ERREURCREATIONTEXTURE);

		// Cr�ation de la vue du tampon de profondeur (ou de stencil)
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSView;
		ZeroMemory(&descDSView, sizeof(descDSView));
		descDSView.Format = depthTextureDesc.Format;
		descDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSView.Texture2D.MipSlice = 0;
		DXEssayer(pD3DDevice->CreateDepthStencilView(pDepthTexture, &descDSView,
			&pDepthStencilView),
			DXE_ERREURCREATIONDEPTHSTENCILTARGET);
	}

	void PanneauPE::DebutPostEffect()
	{
		// Prendre en note l'ancienne surface de rendu
		pOldRenderTargetView = pDispositif->GetRenderTargetView();

		// Prendre en note l'ancienne surface de tampon Z
		pOldDepthStencilView = pDispositif->GetDepthStencilView();

		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
		float Couleur[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
		pImmediateContext->ClearRenderTargetView(pRenderTargetView, Couleur);
		pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


		// Utiliser la texture comme surface de rendu et le tampon de profondeur
		// associ�
		pDispositif->SetRenderTargetView(pRenderTargetView, pDepthStencilView);
	}

	void PanneauPE::FinPostEffect()
	{
		// Restaurer l'ancienne surface de rendu et le tampon de profondeur
		// associ�
		pDispositif->SetRenderTargetView(pOldRenderTargetView,
			pOldDepthStencilView);
	}

	void PanneauPE::Draw(ID3D11ShaderResourceView* _pShadowMapView, const DirectX::XMMATRIX, const DirectX::XMVECTOR) noexcept
	{
		// Obtenir le contexte
		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

		// Choisir la topologie des primitives
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Source des sommets
		UINT stride = sizeof(CSommetPanneauPE);
		const UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		// Choix de la technique
		pTechnique = pEffet->GetTechniqueByIndex(m_techniqueIndice);
		//pTechnique = pEffet->GetTechniqueByIndex(0);
		pPasse = pTechnique->GetPassByIndex(0);

		// input layout des sommets
		pImmediateContext->IASetInputLayout(pVertexLayout[1]);

		// Le sampler state
		ID3DX11EffectSamplerVariable* variableSampler;
		variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
		variableSampler->SetSampler(0, pSampleState);

		// La �constante� distance
		ID3DX11EffectScalarVariable* distance;
		distance = pEffet->GetVariableByName("distance")->AsScalar();
		distance->SetFloat((float)0.10f);

		// La �percent� distance -> c'est un taux d'activation pour le blur
		ID3DX11EffectScalarVariable* percent;
		percent = pEffet->GetVariableByName("percent")->AsScalar();
		percent->SetFloat((float)m_blurPercent);

		// La �redPercent� distance -> c'est un taux d'activation pour le blur
		ID3DX11EffectScalarVariable* redPercent;
		redPercent = pEffet->GetVariableByName("redPercent")->AsScalar();
		redPercent->SetFloat((float)m_redPercent);

		// La �percent� distance -> c'est un taux d'activation
		ID3DX11EffectScalarVariable* time;
		time = pEffet->GetVariableByName("time")->AsScalar();
		using namespace std::chrono;
		time->SetFloat((int)duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()
			).count());

		ID3DX11EffectShaderResourceVariable* variableTexture;
		variableTexture = pEffet->GetVariableByName("textureEntree")->AsShaderResource();

		// Activation de la texture
		variableTexture->SetResource(pResourceView);

		pPasse->Apply(0, pImmediateContext);

		// **** Rendu de l'objet
		pImmediateContext->Draw(6, 0);
	}

} // namespace PM3D
