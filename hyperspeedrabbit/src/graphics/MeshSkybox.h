#pragma once 
#include "stdafx.h" 
#include "d3dx11effect.h" 
#include "chargeur.h" 
#include "DispositifD3D11.h"
#include <vector> 

namespace PM3D
{
	class CMeshSkybox
	{
	private:
		class CSommetMesh
		{
		public:
			CSommetMesh() = default;
			CSommetMesh(XMFLOAT3 _position, XMFLOAT3 _normal,
				XMFLOAT2 _coordTex = XMFLOAT2(0, 0));
		public:
			static UINT numElements;
			static D3D11_INPUT_ELEMENT_DESC layout[];
			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 coordTex;
		};

		struct ShadersParams
		{
			XMMATRIX matWorldViewProj;			// la matrice totale 
			XMMATRIX matWorld;					// matrice de transformation dans le monde 
			XMVECTOR vCamera;					// la position de la caméra 
			XMVECTOR vAMat;						// la valeur ambiante du matériau 
			XMVECTOR vDMat;						// la valeur diffuse du matériau 
			XMVECTOR vSMat;						// la valeur spéculaire du matériau 
			float puissance;					// la puissance de spécularité 
			int bTex;							// Texture ou materiau 
			XMFLOAT2 remplissage;

			XMVECTOR vLumierePos;
			XMVECTOR vAEcl;			// la valeur ambiante de l’éclairage 
			XMVECTOR vDEcl;			// la valeur diffuse de l’éclairage 
			XMVECTOR vSEcl;
		};

		class CMaterial
		{
		public:
			std::string NomFichierTexture;
			std::string NomMateriau;
			ID3D11ShaderResourceView* pTextureD3D;
			XMFLOAT4 Ambient;
			XMFLOAT4 Diffuse;
			XMFLOAT4 Specular;
			float Puissance;
			bool transparent;
			CMaterial() noexcept
			{
				Ambient = XMFLOAT4(1.f, 1.f, 1.0f, 1.0f);
				Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
				Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
				transparent = false;
				Puissance = 9;
				pTextureD3D = NULL;
				NomFichierTexture = "";
			}
		};

	public:
		CMeshSkybox(const IChargeur& chargeur, CDispositifD3D11* pDispositif);
		virtual ~CMeshSkybox(void);

		XMMATRIX getMatWorld() const noexcept { return matWorld; };
		void setMatWorld(XMMATRIX& _matWorld) noexcept { matWorld = _matWorld; };

		void Draw();
	private:
		CMeshSkybox();
		void TransfertObjet(const IChargeur& chargeur);
		void InitEffet();
		void Anime(float tempsEcoule);

	private:
		// **** Données membres 
		XMMATRIX matWorld; // Matrice de transformation dans le monde 
		float rotation;
		// Pour le dessin 
		CDispositifD3D11* pDispositif; // On prend en note le dispositif 
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
		// Les sous-objets 
		int NombreSubmesh; // Nombre de sous-objets dans le mesh 
		std::vector<int> SubmeshMaterialIndex; // Index des matériaux 
		std::vector<int> SubmeshIndex; // Index des sous-objets 
		std::vector<CMaterial> Material; // Vecteur des matériaux 
		// Pour les effets et shaders 
		ID3D11SamplerState* pSampleState;
		ID3D11Buffer* pConstantBuffer;
		ID3DX11Effect* pEffet;
		ID3DX11EffectTechnique* pTechnique;
		ID3DX11EffectPass* pPasse;
		ID3D11InputLayout* pVertexLayout;

	};
}
