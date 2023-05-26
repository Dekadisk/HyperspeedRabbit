struct VS_Sortie
{
	float4 Pos : SV_POSITION;
	float2 CoordTex : TEXCOORD0;
};

Texture2D textureEntree;    // la texture
SamplerState SampleState;   // l'etat de sampling

float distance;
float percent;
float redPercent;
float time;

//-------------------------------------------------------
// Vertex Shader ?Nul?
//-------------------------------------------------------
VS_Sortie NulVS(float4 Pos : POSITION, float2 CoordTex : TEXCOORD)
{
	VS_Sortie sortie = (VS_Sortie)0;

	sortie.Pos = Pos;
	sortie.CoordTex = CoordTex;

	return sortie;
}

//-----------------------------------------------------
// Pixel Shader ?Nul?
//-----------------------------------------------------
float4 NulPS(VS_Sortie vs) : SV_Target
{
	float4 couleur;

	couleur = textureEntree.Sample(SampleState, vs.CoordTex);

	return couleur;
}

float4 RadialBlurPS(VS_Sortie vs) : SV_Target
{
	float4 couleur;
	float4 ct;
	float2 tc = vs.CoordTex;
	float4 sc = vs.Pos;
	float d, dx, dy;

	couleur = 0;
	float x = tc.x * 2 - 1.0;
	float y = tc.y * 2 - 1.0;
	dx = sqrt(x * x)/(10 - percent*6);  // Distance du centre
	dy = sqrt(y * y)/(10 - percent * 6);  // Distance du centre

	dx = x * (distance * dx);  	// Le d?grad? (blur) est en fonction de la
	dy = y * (distance * dy);	 	// distance du centre et de la variable distance.

	x = x - (dx * 10);   // Vous pouvez jouer avec le nombre d'it?rations
	y = y - (dy * 10);

	tc.x = (x + 1.0) / 2.0;
	tc.y = (y + 1.0) / 2.0;

	for (int i = 0; i < 10; i++)   // Vous pouvez jouer avec le nombre d'it?rations
	{
	   ct = textureEntree.Sample(SampleState, tc);
	   couleur = percent * couleur * 0.6 + ct*(1.f- percent*0.6); // Vous pouvez ?jouer? avec les %

	   x = x + dx;
	   y = y + dy;

	   tc.x = (x + 1.0) / 2.0;
	   tc.y = (y + 1.0) / 2.0;
	}

	return couleur;
}

float4 RadialBlurRedPS(VS_Sortie vs) : SV_Target
{
	float4 couleur;
	float4 ct;
	float2 tc = vs.CoordTex;
	float4 sc = vs.Pos;
	float d, dx, dy;
	float4 red;
	float disToCenter;
	float r;
	float2 center;

	couleur = 0;
	center = float2(0.5f, 0.5f);
	r = 0.4f - 0.01f * saturate(fmod(time / 1000.f, 1.f));
	disToCenter = sqrt(pow(center.x - tc.x, 2) + pow(center.y - tc.y, 2));

	red = float4(1.f,0,0,1.f);
	float x = tc.x * 2 - 1.0;
	float y = tc.y * 2 - 1.0;
	dx = sqrt(x * x) / (10 - percent * 6);  // Distance du centre
	dy = sqrt(y * y) / (10 - percent * 6);  // Distance du centre

	dx = x * (distance * dx);  	// Le d?grad? (blur) est en fonction de la
	dy = y * (distance * dy);	 	// distance du centre et de la variable distance.

	x = x - (dx * 10);   // Vous pouvez jouer avec le nombre d'it?rations
	y = y - (dy * 10);

	tc.x = (x + 1.0) / 2.0;
	tc.y = (y + 1.0) / 2.0;

	for (int i = 0; i < 10; i++)   // Vous pouvez jouer avec le nombre d'it?rations
	{
	   ct = textureEntree.Sample(SampleState, tc);
	   couleur = percent * couleur * 0.6 + ct * (1.f - percent * 0.6); // Vous pouvez ?jouer? avec les % BLUR
	   couleur = redPercent * red * saturate(disToCenter - r) + (1.f - saturate(disToCenter - r)*redPercent) * couleur; //RED
	   x = x + dx;
	   y = y + dy;

	   tc.x = (x + 1.0) / 2.0;
	   tc.y = (y + 1.0) / 2.0;
	}

	return couleur;
}

float4 RadialBlurRedBraomPS(VS_Sortie vs) : SV_Target
{
	float4 couleur;
	float4 ct;
	float2 tc = vs.CoordTex;
	float4 sc = vs.Pos;
	float d, dx, dy;
	float4 red;
	float disToCenter;
	float r;
	float2 center;

	couleur = 0;
	center = float2(0.5f, 0.5f);
	r = 0.4f - 0.01f * saturate(fmod(time / 1000.f, 1.f));
	disToCenter = sqrt(pow(center.x - tc.x, 2) + pow(center.y - tc.y, 2));

	red = float4(1.f,0,0,1.f);
	float x = tc.x * 2 - 1.0;
	float y = tc.y * 2 - 1.0;
	dx = sqrt(x * x) / (10 - percent * 6);  // Distance du centre
	dy = sqrt(y * y) / (10 - percent * 6);  // Distance du centre

	dx = x * (distance * dx);  	// Le d?grad? (blur) est en fonction de la
	dy = y * (distance * dy);	 	// distance du centre et de la variable distance.

	x = x - (dx * 10);   // Vous pouvez jouer avec le nombre d'it?rations
	y = y - (dy * 10);

	tc.x = (x + 1.0) / 2.0;
	tc.y = (y + 1.0) / 2.0;

	for (int i = 0; i < 10; i++)   // Vous pouvez jouer avec le nombre d'it?rations
	{
		if (fmod(tc.x * 10000.f, 2) < 1) {
			ct = textureEntree.Sample(SampleState, float2(tc.x - 0.001f+time/10000.f, tc.y));
		}
		else {
			ct = textureEntree.Sample(SampleState, float2(tc.x + 0.001f+time / 10000.f, tc.y));
		}
		couleur = percent * couleur * 0.6 + ct * 0.4; // Vous pouvez ?jouer? avec les % BLUR
	   
		x = x + dx;
		y = y + dy;

		tc.x = (x + 1.0) / 2.0;
		tc.y = (y + 1.0) / 2.0;
	}
	
	couleur = red * saturate(disToCenter - r) + (1.f - saturate(disToCenter - r)) * couleur; //RED

	return couleur;
}

technique11 Nul
{
	pass p0
	{
		VertexShader = compile vs_5_0 NulVS();
		PixelShader = compile ps_5_0 NulPS();
		SetGeometryShader(NULL);
	}
}

technique11 RadialBlur
{
	pass p0
	{
		VertexShader = compile vs_5_0 NulVS();
		PixelShader = compile ps_5_0 RadialBlurPS();
		SetGeometryShader(NULL);
	}
}

technique11 RadialBlurRed
{
	pass p0
	{
		VertexShader = compile vs_5_0 NulVS();
		PixelShader = compile ps_5_0 RadialBlurRedPS();
		SetGeometryShader(NULL);
	}
}

technique11 RadialBlurRedBraom
{
	pass p0
	{
		VertexShader = compile vs_5_0 NulVS();
		PixelShader = compile ps_5_0 RadialBlurRedBraomPS();
		SetGeometryShader(NULL);
	}
}