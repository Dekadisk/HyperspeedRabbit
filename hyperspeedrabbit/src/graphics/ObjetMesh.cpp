#include "stdafx.h"
#include "ObjetMesh.h"

#include <string> 
#include "ObjetMesh.h" 
#include "moteurWindows.h" 
#include "util.h" 
#include "resource.h" 
#include <DirectXMath.h>
#include "ResourceManager.h"

namespace PM3D
{
	// Definir l’organisation de notre sommet 
	D3D11_INPUT_ELEMENT_DESC CObjetMesh::CSommetMesh::layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT CObjetMesh::CSommetMesh::numElements;

	CObjetMesh::CObjetMesh(const IChargeur& chargeur, CDispositifD3D11* _pDispositif,const std::string& objFolder,const std::string& nameObj, float range_1, float range_2,float range_3)
		: pDispositif(_pDispositif), m_nameObj(nameObj), m_objFolder(objFolder), oldShade(0),
		m_range_1(range_1), m_range_2(range_2), m_range_3(range_3)
		// prendre en note le dispositif 
	{
		// Placer l'objet sur la carte graphique 
		TransfertObjet(chargeur);
		// Initialisation de l'effet 
		InitEffet();
		matWorld = XMMatrixIdentity();
	}
	
	CObjetMesh::~CObjetMesh()
	{
		DXRelacher(pConstantBuffer);
		//DXRelacher(pConstantBuffer1);
		DXRelacher(pSampleState);
		DXRelacher(pEffet);
		DXRelacher(pVertexLayout);
		DXRelacher(pIndexBuffer);
		DXRelacher(pVertexBuffer);

		DXRelacher(pVertexLayoutShadow);

	}

	void CObjetMesh::TransfertObjet(const IChargeur& chargeur)
	{

		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
		// 1. SOMMETS a) Créations des sommets dans un tableau temporaire 
		{
			const size_t nombreSommets = chargeur.GetNombreSommets();
			std::unique_ptr<CSommetMesh[]> ts(new CSommetMesh[nombreSommets]);
			for (uint32_t i = 0; i < nombreSommets; ++i)
			{
				ts[i].position = chargeur.GetPosition(i);
				ts[i].normal = chargeur.GetNormale(i);
				ts[i].coordTex = chargeur.GetCoordTex(i);
			}
			// 1. SOMMETS b) Création du vertex buffer et copie des sommets 
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_IMMUTABLE;
			bd.ByteWidth = static_cast<uint32_t>(sizeof(CSommetMesh) * nombreSommets);
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory(&InitData, sizeof(InitData));
			InitData.pSysMem = ts.get();
			pVertexBuffer = nullptr;
			DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);
		}
		// 2. INDEX - Création de l’index buffer et copie des indices 
		// Les indices étant habituellement des entiers, j’ai 
		// pris directement ceux du chargeur, mais attention au 
		// format si vous avez autre chose que DXGI_FORMAT_R32_UINT 
		{
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_IMMUTABLE;
			bd.ByteWidth = static_cast<uint32_t>(sizeof(uint32_t) * chargeur.GetNombreIndex());
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory(&InitData, sizeof(InitData));
			InitData.pSysMem = chargeur.GetIndexData();
			pIndexBuffer = nullptr;
			DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
				DXE_CREATIONINDEXBUFFER);
		}
		// 3. Les sous-objets 
		NombreSubmesh = chargeur.GetNombreSubset();
		// Début de chaque sous-objet et un pour la fin 
		SubmeshIndex.reserve(NombreSubmesh);
		chargeur.CopieSubsetIndex(SubmeshIndex);
	}

	void CObjetMesh::InitEffet()
	{
		// Compilation et chargement du vertex shader 
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

		// Création d’un tampon pour les constantes du VS 
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ShadersParams);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		DXEssayer(pD3DDevice->CreateBuffer(&bd, nullptr, &pConstantBuffer));

		// Pour l’effet 

		ID3DBlob* pFXBlob = ResourceManager::get().getEffect(L"src/graphics/shaders/ShadowMap.fx");
		//DXEssayer(D3DCompileFromFile(L"src/graphics/shaders/ShadowMap.fx", 0, 0, 0, "fx_5_0", 0, 0, &pFXBlob, 0), DXE_ERREURCREATION_FX);
		D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, pD3DDevice, &pEffet);

		//pFXBlob->Release();

		pTechnique = pEffet->GetTechniqueByIndex(0);
		pPasse = pTechnique->GetPassByIndex(0);

		// Créer l'organisation des sommets pour les VS de notre effet
		D3DX11_PASS_SHADER_DESC effectVSDesc;
		D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
		const void* vsCodePtr;
		unsigned vsCodeLen;
		CSommetMesh::numElements = ARRAYSIZE(CSommetMesh::layout);


		// 1 pour le shadowmap
		pTechnique = pEffet->GetTechniqueByName("ShadowMap");
		pPasse = pTechnique->GetPassByIndex(0);
		pPasse->GetVertexShaderDesc(&effectVSDesc);
		effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex,
			&effectVSDesc2);

		vsCodePtr = effectVSDesc2.pBytecode;
		vsCodeLen = effectVSDesc2.BytecodeLength;

		pVertexLayout = nullptr;

		DXEssayer(pD3DDevice->CreateInputLayout(CSommetMesh::layout,
			CSommetMesh::numElements,
			vsCodePtr,
			vsCodeLen,
			&pVertexLayoutShadow),
			DXE_CREATIONLAYOUT);



		// 2 pour miniphong
		pTechnique = pEffet->GetTechniqueByName("MiniPhong");
		pPasse = pTechnique->GetPassByIndex(0);
		pPasse->GetVertexShaderDesc(&effectVSDesc);
		effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex,
			&effectVSDesc2);

		vsCodePtr = effectVSDesc2.pBytecode;
		vsCodeLen = effectVSDesc2.BytecodeLength;

		pVertexLayout = nullptr;

		DXEssayer(pD3DDevice->CreateInputLayout(CSommetMesh::layout,
			CSommetMesh::numElements,
			vsCodePtr,
			vsCodeLen,
			&pVertexLayout),
			DXE_CREATIONLAYOUT);


		// Initialisation des paramètres de sampling de la texture
		D3D11_SAMPLER_DESC samplerDesc;

		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 4;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Création de l'état de sampling
		pD3DDevice->CreateSamplerState(&samplerDesc, &pSampleState);

	}

	void CObjetMesh::InitBuffers() noexcept{
		// Get the Immediat Context
		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
				
		// Choose the topology of the Index	
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Set the Index Buffer
		pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// set the Vertex Buffer
		const UINT stride = sizeof(CSommetMesh);
		const UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	}

	void CObjetMesh::DrawShadowMap(const XMMATRIX _mVPLight)noexcept {

		InitBuffers();

		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

		//Choose the technique of the shader
		pTechnique = pEffet->GetTechniqueByName("ShadowMap");
		pPasse = pTechnique->GetPassByIndex(0);

		//Set the Layout Buffer 
		pImmediateContext->IASetInputLayout(pVertexLayoutShadow);

		// create the "constant" for the shader
		ShadersParams sp;
		sp.matWorldViewProjLight = XMMatrixTranspose(matWorld * _mVPLight);

		// Set the "constant" in the shader
		ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");
		pCB->SetConstantBuffer(pConstantBuffer);
		pImmediateContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &sp, 0, 0);

		// Draw the subMesh
		for (int i = 0; i < NombreSubmesh; ++i)
		{
			const int indexStart = SubmeshIndex[i];
			const int indexDrawAmount = SubmeshIndex[i + 1] - SubmeshIndex[i];
			if (indexDrawAmount)
			{
				
				pPasse->Apply(0, pImmediateContext);

				pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
			}
		}
	}

	void CObjetMesh::Draw(ID3D11ShaderResourceView* _pShadowMapView, const XMMATRIX _mVPLight, const XMVECTOR _CamDir, const DirectX::XMFLOAT3& objPosition) noexcept {


		//Calcul de la distance entre la camra et la position de notre objets
		double composante_x = pow(objPosition.x - XMVectorGetByIndex(_CamDir, 0), 2);
		double composante_y = pow(objPosition.y - XMVectorGetByIndex(_CamDir, 1), 2);
		double composante_z = pow(objPosition.z - XMVectorGetByIndex(_CamDir, 2), 2);
		float distanceCamObj = static_cast<float>(sqrt(composante_x + composante_y + composante_z));


		if (distanceCamObj < m_range_1) {
			if (oldShade != 1)
			{
				oldShade = 1;
				PM3D::CChargeurOBJ& chargeurObj = ResourceManager::get().getChargeur(m_objFolder, m_nameObj + ".obj");
				TransfertObjet(chargeurObj);
			}
		}
		else if (distanceCamObj < m_range_2) {
			if (oldShade != 0)
			{
				oldShade = 0;
				PM3D::CChargeurOBJ& chargeurObj = ResourceManager::get().getChargeur(m_objFolder, m_nameObj + "LOD1.obj");
				TransfertObjet(chargeurObj);
			}
		}
		else if (distanceCamObj < m_range_3) {
			if (oldShade != 2)
			{
				oldShade = 2;
				PM3D::CChargeurOBJ& chargeurObj = ResourceManager::get().getChargeur(m_objFolder, m_nameObj + "LOD2.obj");
				TransfertObjet(chargeurObj);
			}
		}
		else {
			oldShade = 3;
			return;
		}

		InitBuffers();

		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
		ShadersParams sp;

		// Choix de la technique
		pTechnique = pEffet->GetTechniqueByName("MiniPhong");
		pPasse = pTechnique->GetPassByIndex(0);

		// Initialiser et sélectionner les «constantes» de l'effet
		XMMATRIX viewProj = CMoteurWindows::get().GetMatViewProj();

		sp.matWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
		sp.matWorld = XMMatrixTranspose(matWorld);

		for (int i = 0; i != CMoteurWindows::get().getLights().size(); ++i) {
			sp.vDEcl[i] = CMoteurWindows::get().getLights()[i]->m_ValDiffuse;
			sp.vSEcl[i] = CMoteurWindows::get().getLights()[i]->m_ValSpeculaire;
			sp.vAEcl[i] = CMoteurWindows::get().getLights()[i]->m_ValAmbiante;
			sp.vLumiere[i] = CMoteurWindows::get().getLights()[i]->m_position;
			sp.cone_range[i] = { CMoteurWindows::get().getLights()[i]->m_cone, CMoteurWindows::get().getLights()[i]->m_range, 0.0f, 0.0f };
		}
		

		sp.remplissage = { static_cast<float>(CMoteurWindows::get().getLights().size()), 0.0f};
		sp.matWorldViewProjLight = XMMatrixTranspose(matWorld * _mVPLight);
		sp.vCamera = _CamDir;

		// Le sampler state
		ID3DX11EffectSamplerVariable* variableSampler;
		variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
		variableSampler->SetSampler(0, pSampleState);

		// input layout des sommets
		pImmediateContext->IASetInputLayout(pVertexLayout);

		ID3DX11EffectShaderResourceVariable* pShadowMap;
		pShadowMap = pEffet->GetVariableByName("ShadowTexture")->AsShaderResource();
		pShadowMap->SetResource(_pShadowMapView);

		pDispositif->SetNormalRSState();

		// Draw the subMesh object
		for (int i = 0; i < NombreSubmesh; ++i)
		{
			const int indexStart = SubmeshIndex[i];
			const int indexDrawAmount = SubmeshIndex[i + 1] - SubmeshIndex[i];
			if (indexDrawAmount)
			{
				sp.vAMat = XMLoadFloat4(&m_materialList[0]->getAmbiant());
				sp.vDMat = XMLoadFloat4(&m_materialList[0]->getDiffuse());
				sp.vSMat = XMLoadFloat4(&m_materialList[0]->getSpecular());
				sp.puissance = 1.0f;

				// Activation de la texture ou non
				if (m_materialList[0]->getTexture() != nullptr)
				{
					ID3DX11EffectShaderResourceVariable* variableTexture;
					variableTexture =
						pEffet->GetVariableByName("textureEntree")->AsShaderResource();
					variableTexture->SetResource(m_materialList[0]->getTexture());
					sp.bTex = 1;
				}
				else
				{
					sp.bTex = 0;
				}

				// IMPORTANT pour ajuster les param.
				pPasse->Apply(0, pImmediateContext);

				ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");
				pCB->SetConstantBuffer(pConstantBuffer);
				pImmediateContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &sp, 0, 0);

				pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
			}
		}
	}
}
