#pragma once
#include <string>
#include "d3dx11effect.h"
#include "sommetTerrain.h"
#include "Texture.h"

namespace PM3D
{
	class CDispositifD3D11;

	class CTerrain {
	private:

		struct ShadersParams // toujours un multiple de 16 pour les constantes
		{
			XMMATRIX matWorldViewProj;	// la matrice totale 
			XMMATRIX matWorldViewProjLight;	// WVP pour lumiere 
			XMMATRIX matWorld;			// matrice de transformation dans le monde 
			XMVECTOR vLumiere[MAX_LIGHTS]; 			// la position de la source d'éclairage (Point)
			XMVECTOR vCamera; 			// la position de la caméra
			XMVECTOR vAEcl[MAX_LIGHTS]; 			// la valeur ambiante de l'éclairage
			XMVECTOR vAMat; 			// la valeur ambiante du matériau
			XMVECTOR vDEcl[MAX_LIGHTS]; 			// la valeur diffuse de l'éclairage 
			XMVECTOR vDMat; 			// la valeur diffuse du matériau 
			XMVECTOR vSEcl[MAX_LIGHTS]; 			// la valeur spéculaire de l'éclairage 
			XMVECTOR vSMat; 			// la valeur spéculaire du matériau 
			float puissance;
			int bTex;					// Texture ou materiau 
			XMFLOAT2 remplissage;

			XMVECTOR cone_range[MAX_LIGHTS];
		};

	public:

		CTerrain(const char* filename, XMFLOAT3 scale, CDispositifD3D11* pDispositif);

		// Destructeur
		virtual ~CTerrain();

		void anime(float tempsEcoule);


		float getHeight(float x, float z) const noexcept;
		void SetTexture(CTexture* pTexture, CTexture* pTexture1, CTexture* pTexture2) noexcept;
		XMMATRIX getMatWorld() noexcept;
		void setMatWorld(XMMATRIX) noexcept;

		void InitBuffers() noexcept;
		void DrawShadowMap(const XMMATRIX) noexcept;
		void Draw(ID3D11ShaderResourceView* _pShadowMapView, const XMMATRIX, const XMVECTOR _CamDir) noexcept;



		int height;
		int width;

		XMFLOAT3 scale;

	private:
		CSommetTerrain* sommets;

		const char* filename;
		CDispositifD3D11* pDispositif;
		void InitEffect();
	

		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;

		ID3D11VertexShader* pVertexShader;
		ID3D11PixelShader* pPixelShader;
		ID3D11InputLayout* pVertexLayout;

		unsigned int* pIndices;
		int nbSommets;
		int nbPolygones;

		// Définitions des valeurs d'animation
		ID3D11Buffer* pConstantBuffer;
		XMMATRIX matWorld;
		float rotation;

		// Pour les effets
		ID3DX11Effect* pEffet;
		ID3DX11EffectTechnique* pTechnique;
		ID3DX11EffectPass* pPasse;

		
		ID3D11ShaderResourceView* pTextureD3D;
		ID3D11ShaderResourceView* pTextureD3D1;
		ID3D11ShaderResourceView* pTextureD3D2;
		ID3D11SamplerState* pSampleState;

		static const int SHADOWMAP_DIM = 512;

		ID3D11InputLayout* pVertexLayoutShadow;

	};
}