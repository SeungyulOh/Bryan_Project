matrix g_matWorld;
matrix g_matView;
matrix g_matProj;

texture g_baseTexture;
sampler baseSampler = sampler_state
{
	texture = g_baseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

float g_fTimeAcc = 0.f;


struct VertexShader_Input
{
	vector vPos : POSITION;
	float2 vTexUV : TEXCOORD0;
	float4 W0 : TEXCOORD1;
	float4 W1 : TEXCOORD2;
	float4 W2 : TEXCOORD3;
	float4 W3 : TEXCOORD4;
	float4 vColor : TEXCOORD5;
};

struct VertexShader_Output
{
	vector vPos : POSITION;
	float2 vTexUV : TEXCOORD0;
	float4 vColor : TEXCOORD1;
};


VertexShader_Output VertexShader_Main(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut  = (VertexShader_Output)0.f;

	float4x4 object = float4x4(vsIn.W0,vsIn.W1,vsIn.W2,float4(vsIn.W3.xyz, 1.f));
	float4x4 objectworld = mul(object, g_matWorld);
	float4x4 WVP = mul(objectworld,mul(g_matView,g_matProj));
	vsOut.vPos = mul(float4(vsIn.vPos.xyz,1.f),WVP);
	vsOut.vTexUV = vsIn.vTexUV;
	vsOut.vTexUV.y -= g_fTimeAcc;

	vsOut.vColor = vsIn.vColor;

	return vsOut;
}


struct PixelShader_Input
{
	float2 vTexUV : TEXCOORD0;
	float4 vColor : TEXCOORD1;
};
struct PixelShader_Output
{
	vector vColor : COLOR0;
	vector vColor1 : COLOR1;
	vector vColor2 : COLOR2;
};

PixelShader_Output PixelShader_Main(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	psOut.vColor = tex2D(baseSampler , psIn.vTexUV) * vector(psIn.vColor.xyz,1.f);
	psOut.vColor1 = psOut.vColor;

	psOut.vColor.a *= psIn.vColor.w;
	psOut.vColor2 = psOut.vColor;

	return psOut;
}


technique Default_Device
{
	pass Default_Shading
	{
		lighting = false;
		cullmode = none;
		zwriteenable = false;
		//zenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_Main();
	}

	pass Default_ShadingOne
	{
		lighting = false;
		cullmode = none;
		zwriteenable = false;
		//zenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_Main();
	}
}