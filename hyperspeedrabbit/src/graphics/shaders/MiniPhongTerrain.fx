Texture2D textureEntree; // la texture 
SamplerState SampleState; // ltat de sampling
const static int MAX_LIGHTS = 10;

cbuffer param
{
	float4x4 matWorldViewProj;			// la matrice totale   
	float4x4 matWorld;					// matrice de transformation dans le monde   
	float4 vCamera;						// la position de la camra  
	float4 vAMat;						// la valeur ambiante du matriau  
	float4 vDMat;						// la valeur diffuse du matriau  
	float4 vSMat;						// la valeur spculaire du matriau	
	float puissance;					// la puissance de spcularit 
	int bTex;							// Boolen pour la prsence de texture 
	float2 remplissage;


	float4 vLumierePos[MAX_LIGHTS];
	float4 vAEcl[MAX_LIGHTS];
	float4 vDEcl[MAX_LIGHTS];
	float4 vSEcl[MAX_LIGHTS];
	float4 cone_range[MAX_LIGHTS];
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

	// Coordonnes dapplication de texture 
	sortie.coordTex = coordTex;

	return sortie;
}


float4 MiniPhongPS1(VS_Sortie vs) : SV_Target
{
	float3 couleur = float3(0.f,0.f,0.f);
	float3 couleurTexture;
	// Normaliser les paramtres 
	float3 N = normalize(vs.Norm);
	float3 V = normalize(vs.vDirCam);
	float3 VA = float3(0.0f,0.0f,0.0f);
	float3 VD = float3(0.0f,0.0f,0.0f);
	float3 VS = float3(0.0f,0.0f,0.0f);

	for (int i = 0; i < remplissage.x; ++i) {
		float3 L = normalize(vLumierePos[i]);
		float3 diff = saturate(dot(N, L));
		float3 R = normalize(2 * diff * N - L);
		float3 S = pow(saturate(dot(R, V)), puissance);
		float3 lightToPixelVec = vLumierePos[i] - vs.PosWorld;
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

			VA += vAEcl[i].rgb;
			VD += (vDEcl[i].rgb * alpha) * diff;
			VS += (vSEcl[i].rgb * alpha) * S;

			//facteur d'attenuation
			continue;
		}

		VA += vAEcl[i].rgb;
		VD += vDEcl[i].rgb * diff;
		VS += vSEcl[i].rgb * S;
		continue;
	}


	if (bTex > 0)
	{
		// chantillonner la couleur du pixel  partir de la texture 
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
