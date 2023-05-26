#include "StdAfx.h"
#include "Terrain.h"
#include "SommetTerrain.h"
#include "util.h"
#include "DispositifD3D11.h"

#include "resource.h"
#include "MoteurWindows.h"

#include <fstream>

using namespace DirectX;
using namespace std;

namespace PM3D {
	CTerrain::CTerrain(const char* filenameBMP, XMFLOAT3 scale, CDispositifD3D11* pDispositif_)
		: pDispositif(pDispositif_) // Prendre en note le dispositif
		, matWorld(XMMatrixIdentity())
		, rotation(0.0f)
		, pVertexBuffer(nullptr)
		, pIndexBuffer(nullptr)
		, pVertexShader(nullptr)
		, pPixelShader(nullptr)
		, pVertexLayout(nullptr)
		, pConstantBuffer(nullptr)
		, filename(filenameBMP)
		, pTextureD3D(0)
		, pTextureD3D1(0)
		, pTextureD3D2(0)
		, pSampleState(0)
	{
		this->scale = scale;
		int error;
		unsigned long long count;
		BITMAPFILEHEADER bitmapFileHeader;
		BITMAPINFOHEADER bitmapInfoHeader;

		//Lecture du fichier BMP
		FILE* filePtr;
		error = fopen_s(&filePtr, filename, "rb");
		if (error != 0) return;

		// Lecture du header du fichier Bitmap
		count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
		if (count != 1) return;

		// Lecture du info header du Bitmap
		count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
		if (count != 1) return;

		height = bitmapInfoHeader.biHeight;
		width = bitmapInfoHeader.biWidth;

		nbSommets = height * width;
		nbPolygones = (height - 1) * (width - 1) * 2;

		this->sommets = new CSommetTerrain[nbSommets];
		int imageSize{}, index{};
		bool extrabit = ((nbSommets) % 2 != 0);
		if (!extrabit) imageSize = height * (width * 3);
		else imageSize = height * ((width * 3) + 1);

		// Création du tableau pour stocker les valeurs du BMP
		unsigned char* bitmapImage = new unsigned char[imageSize];
		if (!bitmapImage) return;

		// Lecture des données de l'image BMP
		fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
		count = fread(bitmapImage, 1, imageSize, filePtr);
		if (count != imageSize) return;

		// Fermeture du fichier
		error = fclose(filePtr);
		if (error != 0) return;

		int k{};

		XMFLOAT3 normale(0.0f, 1.0f, 0.0f);

		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				// association des coordonnées en x,z avec les hauteurs récupérées
				index = (width * (height - 1 - j)) + i;

				int heightValue = bitmapImage[k];
				
				// Création des sommmets aux bonnes coordonnées (avec une normale par défaut qui sera calculée plus tard)
				sommets[index] = CSommetTerrain(XMFLOAT3(static_cast<float>((i - width / 2) * scale.x), static_cast<float>(heightValue * scale.y), static_cast<float>((j - height / 2) * scale.z))
					, normale
					, XMFLOAT2(static_cast<float>(i) / 4, static_cast<float>(j) / 4)
					,  XMFLOAT2(static_cast<float>(i) / width, static_cast<float>(height - 1 - j) / height));

				k += 3;
			}
			// Compensation de l'extra byte a la fin de chaque ligne dans les fichiers bitmaps de taille non divible par 2 (eg. 257x257).
			if (extrabit) k++;
		}

		for (int j = 0; j < height ; j++) {
			for (int i = 0; i < width ; i++) {

				// Calcul des normales
				XMVECTOR n1{}, n2{}, n3{}, n4{};
				XMVECTOR v1{}, v2{}, v3{}, v4{};
				int index0{}, index1{}, index2{}, index3{}, index4{};

				n1 = n2 = n3 = n4 = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

				index0 = (width * (height - 1 - j)) + i;
				index1 = (width * (height - 1 - (j+1))) + i;
				index2 = (width * (height - 1 - j)) + (i+1);
				index3 = (width * (height - 1 - (j-1))) + i;
				index4 = (width * (height - 1 - j)) + (i-1);

				if (j < width - 1)	v1 = XMVectorSet(sommets[index1].getPosition().x, sommets[index1].getPosition().y, sommets[index1].getPosition().z, 0.0f) - XMVectorSet(sommets[index0].getPosition().x, sommets[index0].getPosition().y, sommets[index0].getPosition().z, 0.0f);
				if (i < height - 1) v2 = XMVectorSet(sommets[index2].getPosition().x, sommets[index2].getPosition().y, sommets[index2].getPosition().z, 0.0f) - XMVectorSet(sommets[index0].getPosition().x, sommets[index0].getPosition().y, sommets[index0].getPosition().z, 0.0f);
				if (j > 0)			v3 = XMVectorSet(sommets[index3].getPosition().x, sommets[index3].getPosition().y, sommets[index3].getPosition().z, 0.0f) - XMVectorSet(sommets[index0].getPosition().x, sommets[index0].getPosition().y, sommets[index0].getPosition().z, 0.0f);
				if (i > 0)			v4 = XMVectorSet(sommets[index4].getPosition().x, sommets[index4].getPosition().y, sommets[index4].getPosition().z, 0.0f) - XMVectorSet(sommets[index0].getPosition().x, sommets[index0].getPosition().y, sommets[index0].getPosition().z, 0.0f);
			
				if (j < width - 1 && i < height - 1)	n1 = XMVector3Cross(v2,v1);
				if (j > 0 && i < height - 1)			n2 = XMVector3Cross(v3,v2);
				if (j > 0 && i > 0)						n3 = XMVector3Cross(v4,v3);
				if (j < width - 1 && i > 0)				n4 = XMVector3Cross(v1,v4);

				n1 = n1 + n2 + n3 + n4;
				n1 = -XMVector3Normalize(n1);
				XMFLOAT3 resultat;
				XMStoreFloat3(&resultat, n1);

				sommets[index0].setNormal(resultat);

			}
		}

		pIndices = new unsigned int[nbPolygones * 3];

		k = 0;
		for (int j = 0; j < height - 1; j++) {
			for (int i = 0; i < width - 1; i++) {
				// Création des indexs pour rendu
				pIndices[k++] = j* width + i;
				pIndices[k++] = j * width + (i + 1);
				pIndices[k++] = (j + 1) * width + (i + 1);
				pIndices[k++] = j * width + i;
				pIndices[k++] = (j + 1) * width + (i + 1);
				pIndices[k++] = (j + 1) * width + i;
			}
		}

		// Création du vertex buffer et copie des sommets
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(CSommetTerrain)*(nbSommets);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = sommets;

		DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

		// Création de l'index buffer et copie des indices
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(unsigned int)*(nbPolygones*3);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = pIndices;

		DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
			DXE_CREATIONINDEXBUFFER);

		// Inititalisation des shaders
		InitEffect();
	}

	void CTerrain::InitEffect() {  
		// Compilation et chargement du vertex shader
		ID3D11Device* pD3DDevice = pDispositif-> GetD3DDevice(); 

		// Création d’un tampon pour les constantes du VS
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) ); 

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ShadersParams);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		pD3DDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);

		// Pour l’effet
		ID3DBlob* pFXBlob = NULL;
		DXEssayer( D3DCompileFromFile( L"src/graphics/shaders/ShadowMapTerrain.fx", 0, 0, 0, "fx_5_0", 0, 0, &pFXBlob, 0), DXE_ERREURCREATION_FX);
		D3DX11CreateEffectFromMemory(  pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, pD3DDevice, &pEffet);

		pFXBlob->Release();
		pTechnique = pEffet->GetTechniqueByIndex(0);
		pPasse = pTechnique->GetPassByIndex(1);

		// Créer l'organisation des sommets pour les VS de notre effet
		D3DX11_PASS_SHADER_DESC effectVSDesc;
		D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
		const void* vsCodePtr;
		unsigned vsCodeLen;


		// 1 pour le shadowmap
		pTechnique = pEffet->GetTechniqueByName("ShadowMap");
		pPasse = pTechnique->GetPassByIndex(0);
		pPasse->GetVertexShaderDesc(&effectVSDesc);
		effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex,
			&effectVSDesc2);

		vsCodePtr = effectVSDesc2.pBytecode;
		vsCodeLen = effectVSDesc2.BytecodeLength;

		pVertexLayout = nullptr;

		DXEssayer(pD3DDevice->CreateInputLayout(CSommetTerrain::layout,
			CSommetTerrain::numElements,
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

		DXEssayer(pD3DDevice->CreateInputLayout(CSommetTerrain::layout,
			CSommetTerrain::numElements,
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

	/*
		Interpole les hauteurs des points suivants pour approximer la hauteur du CTerrain aux coordonnées envoyées en paramètres
	*/

	float CTerrain::getHeight(float x, float z) const noexcept
	{
		x = ((x) / (scale.x) + (width / 2));  // on remet les coordonnées dans le référenciel de la heightmap
		z = ((z) / (scale.z) + (height / 2));	// on remet les coordonnées dans le référenciel de la heightmap
		int intX = static_cast<int>(x);
		int intZ = static_cast<int>(z);
		float dx = x - intX;
		float dz = z - intZ;
		
		float counter = 1.0f;

		float hxz = sommets[(width * (height - 1 - intZ)) + intX].getPosition().y;
		float y = hxz;

		if (intX < width - 1) {
			y += hxz + (dx * (sommets[(width * (height - 1 - intZ)) + (intX + 1)].getPosition().y - hxz));
			++counter;
		}

		if (intZ < height - 1) {
			y += hxz + (dz * (sommets[(width * (height - 1 - (intZ + 1))) + intX].getPosition().y - hxz));
			++counter;
		}

		if ((intX < width - 1) && (intZ < height - 1)) {
			y += hxz + static_cast<float>((sqrt(pow(dx, 2) + pow(dz, 2)) * (sommets[(width * (height - 1 - (intZ + 1))) + (intX + 1)].getPosition().y - hxz)));
			++counter;
		}
		
		return y / counter;
	}

	void CTerrain::SetTexture(CTexture* pTexture, CTexture* pTexture1, CTexture* pTexture2) noexcept
	{
		pTextureD3D = pTexture->GetD3DTexture();
		pTextureD3D1 = pTexture1->GetD3DTexture();
		pTextureD3D2 = pTexture2->GetD3DTexture();
	}

	void CTerrain::anime(float _tempsEcoule)
	{
		_tempsEcoule;

	}


	CTerrain::~CTerrain()
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

	XMMATRIX CTerrain::getMatWorld() noexcept {
		return matWorld;
	}

	void CTerrain::setMatWorld(XMMATRIX _matWorld) noexcept {
		matWorld = _matWorld;
	}

	void CTerrain::InitBuffers() noexcept{
		// Obtenir le contexte
		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

		// Choisir la topologie des primitives
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Source des index
		pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Source des sommets
		const UINT stride = sizeof(CSommetTerrain);
		const UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	}

	void CTerrain::DrawShadowMap(const XMMATRIX _mVPLight) noexcept {

		InitBuffers();

		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

		// Choix de la technique
		pTechnique = pEffet->GetTechniqueByName("ShadowMap");
		pPasse = pTechnique->GetPassByIndex(0);

		// input layout des sommets
		pImmediateContext->IASetInputLayout(pVertexLayoutShadow);

		// Initialiser et sélectionner les «constantes» de l'effet
		ShadersParams sp;
		sp.matWorldViewProjLight = XMMatrixTranspose(matWorld * _mVPLight);

		// Nous n'avons qu'un seul CBuffer
		ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");
		pCB->SetConstantBuffer(pConstantBuffer);
		pImmediateContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &sp, 0, 0);

		// IMPORTANT pour ajuster les param.
		pPasse->Apply(0, pImmediateContext);

		pImmediateContext->DrawIndexed(nbPolygones * 3, 0, 0);
	}

	void CTerrain::Draw(ID3D11ShaderResourceView* _pShadowMapView, const XMMATRIX _mVPLight, const XMVECTOR _CamDir) noexcept{
	
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


		sp.vAMat = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
		sp.vDMat = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		sp.vSMat = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		sp.puissance = 2.0f;
		sp.matWorldViewProjLight = XMMatrixTranspose(matWorld * _mVPLight);
		sp.remplissage = { static_cast<float>(CMoteurWindows::get().getLights().size()), 0.0f };
		sp.vCamera = _CamDir;

		ID3DX11EffectSamplerVariable* variableSampler;
		variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
		variableSampler->SetSampler(0, pSampleState);

		// input layout des sommets
		pImmediateContext->IASetInputLayout(pVertexLayout);

		ID3DX11EffectShaderResourceVariable* pShadowMap;
		pShadowMap = pEffet->GetVariableByName("ShadowTexture")->AsShaderResource();
		pShadowMap->SetResource(_pShadowMapView);

		pDispositif->SetNormalRSState();

		if (pTextureD3D) {
			ID3DX11EffectShaderResourceVariable* variableTexture;
			variableTexture = pEffet->GetVariableByName("textureEntree1")->AsShaderResource();
			variableTexture->SetResource(pTextureD3D);

			ID3DX11EffectShaderResourceVariable* variableTexture1;
			variableTexture1 = pEffet->GetVariableByName("textureEntree2")->AsShaderResource();
			variableTexture1->SetResource(pTextureD3D1);

			ID3DX11EffectShaderResourceVariable* variableTexture2;
			variableTexture2 = pEffet->GetVariableByName("textureEntreeM")->AsShaderResource();
			variableTexture2->SetResource(pTextureD3D2);
			sp.bTex = 1;
		}
		else
		{
			sp.bTex = 0;
		}

		pPasse->Apply(0, pImmediateContext);

		ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");
		pCB->SetConstantBuffer(pConstantBuffer);
		pImmediateContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &sp, 0, 0);

		pImmediateContext->DrawIndexed(nbPolygones * 3, 0, 0);
	
	}
}