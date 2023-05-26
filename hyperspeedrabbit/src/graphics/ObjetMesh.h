#pragma once 
#include "stdafx.h" 
#include "d3dx11effect.h" 
#include "chargeur.h" 
#include "DispositifD3D11.h"
#include <vector> 
#include "Material.h"

namespace PM3D
{
	class CObjetMesh
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
		CObjetMesh(const IChargeur& chargeur, CDispositifD3D11* pDispositif, const std::string& objFolder = {}, const std::string& nameObj = {}, float range_1 = 500.0f, float range_2 = 1000.0f, float range_3 = 2000.0f);
		virtual ~CObjetMesh(void);

		XMMATRIX getMatWorld() const noexcept { return matWorld; };
		void setMatWorld(const XMMATRIX& _matWorld) noexcept { matWorld = _matWorld; };

		void InitBuffers() noexcept;
		void DrawShadowMap(const XMMATRIX) noexcept;
		void Draw(ID3D11ShaderResourceView* _pShadowMapView, const XMMATRIX, const XMVECTOR, const DirectX::XMFLOAT3& objPosition) noexcept;
		
		void addMaterial(std::shared_ptr<PM3D::Material> _material) noexcept { m_materialList.emplace_back(_material); }

		bool isFar() const noexcept { return oldShade == 3; }

	private:
		CObjetMesh();
		void TransfertObjet(const IChargeur& chargeur);
		void InitEffet();

	private:
		// **** Données membres 
		XMMATRIX matWorld; // Matrice de transformation dans le monde 

		// Pour le dessin 
		CDispositifD3D11* pDispositif; // On prend en note le dispositif 
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
		// Les sous-objets 
		int NombreSubmesh; // Nombre de sous-objets dans le mesh 
		std::vector<int> SubmeshMaterialIndex; // Index des matériaux 
		std::vector<int> SubmeshIndex; // Index des sous-objets 
		std::vector<std::shared_ptr<Material>> m_materialList; // liste des matériaux pour notre objet
		// Pour les effets et shaders 
		ID3D11SamplerState* pSampleState;
		ID3D11Buffer* pConstantBuffer;
		//ID3D11Buffer* pConstantBuffer1;
		ID3DX11Effect* pEffet;
		ID3DX11EffectTechnique* pTechnique;
		ID3DX11EffectPass* pPasse;
		ID3D11InputLayout* pVertexLayout;

		std::string m_objFolder;
		std::string m_nameObj;

		int oldShade;
		float m_range_1;
		float m_range_2;
		float m_range_3;

		//dimension of the shadow map
		static const int SHADOWMAP_DIM = 512;

		//Shadow
		ID3D11InputLayout* pVertexLayoutShadow; //layout for the effect

	};
}
