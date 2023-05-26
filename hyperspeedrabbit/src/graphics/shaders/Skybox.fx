
Texture2D textureEntree; // la texture 
SamplerState SampleState; // l’état de sampling
const static int MAX_LIGHTS = 4;

cbuffer param
{
	float4x4 matWorldViewProj;			// la matrice totale   
	float4x4 matWorld;					// matrice de transformation dans le monde   
	float4 vLumierePos[MAX_LIGHTS];		// la position de la source d’éclairage (Point très loin donc linéaire)  
	float4 vCamera;						// la position de la caméra  
	float4 vAEcl;						// la valeur ambiante de l’éclairage  
	float4 vAMat;						// la valeur ambiante du matériau  
	float4 vDEcl[MAX_LIGHTS];			// la valeur diffuse de l’éclairage   
	float4 vDMat;						// la valeur diffuse du matériau  
	float4 vSEcl[MAX_LIGHTS];			// la valeur spéculaire de l’éclairage 
	float4 vSMat;						// la valeur spéculaire du matériau	
	float puissance;					// la puissance de spécularité 
	int bTex;							// Booléen pour la présence de texture 
	float2 remplissage;
}

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float3 PosWorld : TEXCOORD0;
	float3 Norm : TEXCOORD1;
	float3 vDirCam : TEXCOORD2;
	float2 coordTex : TEXCOORD3;
};

BlendState SrcAlphaBlendingAdd
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_COLOR;
	DestBlend = SRC_COLOR;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

VS_Sortie MiniPhongVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 coordTex : TEXCOORD) {
	VS_Sortie sortie = (VS_Sortie)0;

	sortie.Pos = mul(Pos, matWorldViewProj);
	sortie.Norm = mul(float4(Normale, 0.0f), matWorld).xyz;

	float3 PosWorld = mul(Pos, matWorld).xyz;

	sortie.PosWorld = mul(Pos, matWorld).xyz;
	sortie.vDirCam = vCamera.xyz - PosWorld;

	// Coordonnées d’application de texture 
	sortie.coordTex = coordTex;

	return sortie;
}


float4 MiniPhongPS1(VS_Sortie vs) : SV_Target
{

	float3 couleur;
	float3 couleurTexture;

	float3 N = normalize(vs.Norm);
	float3 V = normalize(vs.vDirCam);
	float3 L;
	float3 R;
	float3 diff;
	float3 S;
	float3 vDirL;
	float3 vDTotal = 0;
	float3 vSTotal = 0;

	if (bTex > 0) { // Échantillonner la couleur du pixel à partir de la texture 
		couleurTexture = textureEntree.Sample(SampleState, vs.coordTex).rgb;

	}
 else {
  couleurTexture = vDMat.rgb;
}

	//couleurTexture = textureEntree.Sample(SampleState, vs.coordTex).rgb;

	for (int i = 0; i < MAX_LIGHTS; i++) {

		vDirL = vLumierePos[i].xyz - vs.PosWorld;
		L = normalize(vDirL);
		diff = saturate(dot(N, L));
		R = normalize(2 * diff * N - L);
		S = pow(saturate(dot(R, V)), puissance);


		vDTotal = vDTotal + vDEcl[i].rgb * couleurTexture * diff;
		vSTotal = vSTotal + vSEcl[i].rgb * vSMat.rgb * S;

	}

	couleur = vAEcl.rgb * couleurTexture + vDTotal + vSTotal;

	return float4(couleurTexture, 1.0f);
}

technique11 MiniPhong
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_5_0, MiniPhongVS()));
		SetPixelShader(CompileShader(ps_5_0, MiniPhongPS1()));
		SetBlendState(SrcAlphaBlendingAdd, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetGeometryShader(NULL);
	}

	pass pass1 {
		SetVertexShader(CompileShader(vs_5_0, MiniPhongVS()));
		SetPixelShader(CompileShader(ps_5_0, MiniPhongPS1()));
		//SetBlendState(SrcAlphaBlendingAdd, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetGeometryShader(NULL);
	}
}