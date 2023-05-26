const static int MAX_LIGHTS = 25;

cbuffer param
{
	float4x4 matWorldViewProj;			// la matrice totale 
	float4x4 matWorldViewProjLight;		// Matrice WVP pour lumière
	float4x4 matWorld;					// matrice de transformation dans le monde 
	float4 vLumiere[MAX_LIGHTS];					// la position de la source d'éclairage (Point)
	float4 vCamera; 					// la position de la caméra
	float4 vAEcl[MAX_LIGHTS]; 						// la valeur ambiante de l'éclairage
	float4 vAMat; 						// la valeur ambiante du matériau
	float4 vDEcl[MAX_LIGHTS]; 						// la valeur diffuse de l'éclairage 
	float4 vDMat; 						// la valeur diffuse du matériau 
	float4 vSEcl[MAX_LIGHTS]; 						// la valeur spéculaire de l'éclairage 
	float4 vSMat; 						// la valeur spéculaire du matériau 
	float puissance;
	int bTex;		    				// Booléen pour la présence de texture
	float2 remplissage;

	float4 cone_range[MAX_LIGHTS];
}


struct ShadowMapVS_SORTIE
{
	float4 Pos : SV_POSITION;
	float3 Profondeur:	TEXCOORD0;
};

//-------------------------------------------------------------------
// Vertex Shader pour construire le shadow map
//-------------------------------------------------------------------
ShadowMapVS_SORTIE ShadowMapVS(float4 Pos : POSITION)
{
	ShadowMapVS_SORTIE Out = (ShadowMapVS_SORTIE)0;

	// Calcul des coordonnées
	Out.Pos = mul(Pos, matWorldViewProjLight);  // WVP de la lumiere

	// Obtenir la profondeur et normaliser avec w
	//for auto shadow change the value here 
	// + ... the shadow got deep
	// - ... the shadow is  nearer
	Out.Profondeur.x = (Out.Pos.z + 2.f) / Out.Pos.w;

	return Out;
}

//-------------------------------------------------------------------
// Pixel Shader pour le shadow map
//-------------------------------------------------------------------
float4 ShadowMapPS(ShadowMapVS_SORTIE entree) : SV_Target
{
	// retourner la profondeur normalisée [0..1]
	return entree.Profondeur.x;
}

RasterizerState rsCullFront
{
	CullMode = Back;
};

//-------------------------------------------------------------------
// Technique pour le shadow map
//-------------------------------------------------------------------
technique11 ShadowMap
{
	pass p0
	{
		VertexShader = compile vs_5_0 ShadowMapVS();
		SetRasterizerState(rsCullFront);

		PixelShader = compile ps_5_0 ShadowMapPS();
		SetGeometryShader(NULL);
	}
}



Texture2D textureEntree1; // la texture 
Texture2D textureEntree2; // la texture 
Texture2D textureEntreeM; // la texture 

SamplerState SampleState;   // l'état de sampling

Texture2D ShadowTexture;	// La texture du shadow map

SamplerState ShadowMapSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};



struct VS_Sortie
{
	float4 Pos : SV_Position;
	float3 Norm :    TEXCOORD0;
	float3 vDirLum : TEXCOORD1;
	float3 vDirCam : TEXCOORD2;
	float2 coordTex : TEXCOORD3;
	float4 PosInMap : TEXCOORD4;
	float2 coordTexM : TEXCOORD5;
	float3 PosWorld : TEXCOORD6;
};

//-----------------------------------------------------
// Vertex Shader miniphong avec shadow map
//-----------------------------------------------------
VS_Sortie MiniPhongVS(float4 Pos : POSITION,
	float3 Normale : NORMAL, float2 coordTexM : TEXCOORDM,float2 coordTex : TEXCOORD)
{
	VS_Sortie sortie = (VS_Sortie)0;

	sortie.Pos = mul(Pos, matWorldViewProj);
	sortie.Norm = mul(float4(Normale, 0.0f), matWorld).xyz;

	float3 PosWorld = mul(Pos, matWorld).xyz;

	sortie.PosWorld = PosWorld;
	sortie.vDirLum = vLumiere[0].xyz - PosWorld;
	sortie.vDirCam = vCamera.xyz - PosWorld;

	// Coordonnées d’application de texture 
	sortie.coordTex = coordTex;
	sortie.coordTexM = coordTexM;

	// Valeurs pour shadow map
	// Coordonnées
	sortie.PosInMap = mul(Pos, matWorldViewProjLight);

	return sortie;
}

#define DIMTEX 1024

float4 MiniPhongPS(VS_Sortie vs) : SV_Target
{
	float3 couleur;
	float ValeurOmbre;

	// Normaliser les paramètres
	float3 N = normalize(vs.Norm);
	float3 V = normalize(vs.vDirCam);
	float3 VA = float3(0.0f, 0.0f, 0.0f);
	float3 VD = float3(0.0f, 0.0f, 0.0f);
	float3 VS = float3(0.0f, 0.0f, 0.0f);


	// Calculer la valeur du shadowmapping
	// Calculer les coordonnées de texture (ShadowMap)
	float2 ShadowCoord = 0.5f * vs.PosInMap.xy / vs.PosInMap.w + float2(0.5f, 0.5f);
	ShadowCoord.y = 1.0f - ShadowCoord.y;

	float2 PixelActuel = DIMTEX * ShadowCoord; // Pour une texture de 512 X 512

	// Valeur de l'interpolation linéaire
	float2 lerps = frac(PixelActuel);

	// Lire les valeurs du tableau, avec les vérifications de profondeur
	float Profondeur = vs.PosInMap.z / vs.PosInMap.w;
	float3 kernel[9];

	float echelle = 1.0 / DIMTEX;

	float2 coord[9];
	coord[0] = ShadowCoord + float2(-echelle, -echelle);
	coord[1] = ShadowCoord + float2(-echelle, 0.0);
	coord[2] = ShadowCoord + float2(-echelle, echelle);
	coord[3] = ShadowCoord + float2(0.0, -echelle);
	coord[4] = ShadowCoord + float2(0.0, 0.0);
	coord[5] = ShadowCoord + float2(0.0, echelle);
	coord[6] = ShadowCoord + float2(echelle, -echelle);
	coord[7] = ShadowCoord + float2(echelle, 0.0);
	coord[8] = ShadowCoord + float2(echelle, echelle);

	// Colonne 1
	kernel[0].x = (ShadowTexture.Sample(ShadowMapSampler, coord[0]).r < Profondeur) ? 0.0f : 1.0f;
	kernel[0].y = (ShadowTexture.Sample(ShadowMapSampler, coord[1]).r < Profondeur) ? 0.0f : 1.0f;
	kernel[0].z = (ShadowTexture.Sample(ShadowMapSampler, coord[2]).r < Profondeur) ? 0.0f : 1.0f;
	// Colonne 2
	kernel[1].x = (ShadowTexture.Sample(ShadowMapSampler, coord[3]).r < Profondeur) ? 0.0f : 1.0f;
	kernel[1].y = (ShadowTexture.Sample(ShadowMapSampler, coord[4]).r < Profondeur) ? 0.0f : 1.0f;
	kernel[1].z = (ShadowTexture.Sample(ShadowMapSampler, coord[5]).r < Profondeur) ? 0.0f : 1.0f;
	// Colonne 3
	kernel[2].x = (ShadowTexture.Sample(ShadowMapSampler, coord[6]).r < Profondeur) ? 0.0f : 1.0f;
	kernel[2].y = (ShadowTexture.Sample(ShadowMapSampler, coord[7]).r < Profondeur) ? 0.0f : 1.0f;
	kernel[2].z = (ShadowTexture.Sample(ShadowMapSampler, coord[8]).r < Profondeur) ? 0.0f : 1.0f;

	// Les interpolations linéaires
	// Interpoler colonnes 1 et 2
	float3 col12 = lerp(kernel[0], kernel[1], lerps.x);
	// Interpoler colonnes 2 et 3
	float3 col23 = lerp(kernel[1], kernel[2], lerps.x);
	// Interpoler ligne 1 et colonne 1
	float4 lc;
	lc.x = lerp(col12.x, col12.y, lerps.y);
	// Interpoler ligne 2 et colonne 1
	lc.y = lerp(col12.y, col12.z, lerps.y);
	// Interpoler ligne 1 et colonne 2
	lc.z = lerp(col23.x, col23.y, lerps.y);
	// Interpoler ligne 2 et colonne 1
	lc.w = lerp(col23.y, col23.z, lerps.y);

	// Faire la moyenne
	ValeurOmbre = (lc.x + lc.y + lc.z + lc.w) / 4.0;

	float3 L = normalize(vLumiere[0]);
	float3 diff = saturate(dot(N, L));
	float3 R = normalize(2 * diff * N - L);
	float3 S = pow(saturate(dot(R, V)), puissance);
	VA += vAEcl[0].rgb;
	VD += vDEcl[0].rgb * diff;
	VS += vSEcl[0].rgb * S;

	for (int i = 1; i < remplissage.x; ++i) {
		float3 L = normalize(vLumiere[i]);
		float3 diff = saturate(dot(N, L));
		float3 R = normalize(2 * diff * N - L);
		float3 S = pow(saturate(dot(R, V)), puissance);
		float3 lightToPixelVec = vLumiere[i] - vs.PosWorld;
		float d = length(lightToPixelVec);

		if (d > cone_range[i].y) {
			VA += vAEcl[i].rgb;
			continue;
		}
		if (d > cone_range[i].x) {

			float range_bis = cone_range[i].y - cone_range[i].x;
			float d_bis = cone_range[i].y - d;

			float R1 = range_bis / d_bis;
			float R2 = 100.0f / R1;
			float alpha = R2 / 100.0f;


			if (ValeurOmbre < alpha)
				ValeurOmbre = alpha;

			VA += vAEcl[i].rgb;
			VD += (vDEcl[i].rgb * alpha) * diff;
			VS += (vSEcl[i].rgb * alpha) * S;
			//facteur d'attenuation
			continue;
		}
		else {
			ValeurOmbre = 1.0f;
			VA += vAEcl[i].rgb;
			VD += vDEcl[i].rgb * diff;
			VS += vSEcl[i].rgb * S;
			continue;
		}
	}


	float3 couleurTextureR;

	// I = A + D * N.L + (R.V)n
	if (bTex > 0) {
		float3 couleurTexture = textureEntree1.Sample(SampleState, vs.coordTex).rgb;
		float3 couleurTexture1 = textureEntree2.Sample(SampleState, vs.coordTex).rgb;
		float alpha = (textureEntreeM.Sample(SampleState, vs.coordTexM).rgb).x;

		couleurTextureR = alpha * couleurTexture + (1 - alpha) * couleurTexture1;

		couleur = (couleurTextureR * VA + couleurTextureR * VD + VS * vSMat.rgb) * ValeurOmbre;
	}
	else
		couleur = (vAMat.rgb * VA + vDMat.rgb * VD + vSMat.rgb * VS) * ValeurOmbre;

	return float4(saturate(couleur), 1.0f);
}

technique11 MiniPhong
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_5_0, MiniPhongVS()));
		SetPixelShader(CompileShader(ps_5_0, MiniPhongPS()));
		SetGeometryShader(NULL);
	}
}