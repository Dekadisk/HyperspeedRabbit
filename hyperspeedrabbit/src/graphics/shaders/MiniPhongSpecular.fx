Texture2D textureEntree; // la texture 
SamplerState SampleState; // l’état de sampling
const static int MAX_LIGHTS = 25;

cbuffer param
{
	float4x4 matWorldViewProj;			// la matrice totale   
	float4x4 matWorld;					// matrice de transformation dans le monde   
	float4 vCamera;						// la position de la caméra  
	float4 vAMat;						// la valeur ambiante du matériau  
	float4 vDMat;						// la valeur diffuse du matériau  
	float4 vSMat;						// la valeur spéculaire du matériau	
	float puissance;					// la puissance de spécularité 
	int bTex;							// Booléen pour la présence de texture 
	float2 remplissage;


	float4 vLumierePos;
	float4 vAEcl;
	float4 vDEcl;
	float4 vSEcl;
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
	float3 couleur = float3(0.f,0.f,0.f);
	float3 couleurTexture;
	// Normaliser les paramètres 
	float3 N = normalize(vs.Norm);
	float3 V = normalize(vs.vDirCam);
	float3 VA = float3(0.0f,0.0f,0.0f);
	float3 VD = float3(0.0f,0.0f,0.0f);
	float3 VS = float3(0.0f,0.0f,0.0f);

	float3 L = normalize(vLumierePos);
	float3 diff = saturate(dot(N, L));
	float3 R = normalize(2 * diff * N - L);
	float3 S = pow(saturate(dot(R, V)), puissance);
	float3 lightToPixelVec = vLumierePos - vs.PosWorld;
	float d = length(lightToPixelVec);

	VA += vAEcl.rgb;
	VD += vDEcl.rgb * diff;
	VS += vSEcl.rgb * S;

	if (bTex > 0)
	{
		// Échantillonner la couleur du pixel à partir de la texture 
		couleurTexture = textureEntree.Sample(SampleState, vs.coordTex).rgb;
		// I = A + D * N.L + (R.V)n 
		couleur = couleurTexture * VA +
			couleurTexture * VS +
			VS * vSMat.rgb;
	}
	else
	{
		couleur = VA * vAMat.rgb + VD * vDMat.rgb +
			VS * vSMat.rgb;
	}

	return float4(saturate(couleur), 1.0f);

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