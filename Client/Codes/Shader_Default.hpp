matrix g_matWorld;
matrix g_matView;
matrix g_matProj;

float g_fValueForUV;
float g_fAlpha = 1.f;
int g_iImgIdx = 0;
int g_iMaxImgX = 4;
float g_fProcess = 0.f;
float g_fScale = 1.f;
float2x2 g_RotMat;
vector g_vOffSetColor;

texture g_baseTexture;
sampler baseSampler = sampler_state
{
	texture = g_baseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_secondTexture;
sampler secondSampler = sampler_state
{
	texture = g_secondTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct VertexShader_Input
{
	vector vPos : POSITION;
	//vector vNormal : NORMAL;
	//vector vTexUV : TEXCOORD0;
	float2 vTexUV : TEXCOORD0;
	float2 vSecondUV : TEXCOORD1;
};

struct VertexShader_Output
{
	vector vPos : POSITION;
	//vector vNormal : NORMAL;
	//vector vTexUV : TEXCOORD0;
	float2 vTexUV : TEXCOORD0;
	float2 vSecondUV : TEXCOORD1;
};

VertexShader_Output VertexShader_Main(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut  = (VertexShader_Output)0.f;

	matrix matWV,matWVP;
	matWV = mul(g_matWorld,g_matView);
	matWVP = mul(matWV,g_matProj);

	vsOut.vPos = mul(vsIn.vPos,matWVP);
	vsOut.vTexUV = vsIn.vTexUV;
	//vsOut.vTexUV.y += g_fProcess;

	vsOut.vSecondUV = vsIn.vSecondUV;

	return vsOut;
}

VertexShader_Output VertexShader_ToProjDirect(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	vsOut.vPos = vector(vsIn.vPos.xyz,1.f);
	vsOut.vPos.xy = mul(vsIn.vPos.xy,g_RotMat);
	vsOut.vPos.xy *= g_fScale;
	vsOut.vTexUV = vsIn.vTexUV;
	//vsOut.vTexUV.y += g_fProcess;

	return vsOut;
}

VertexShader_Output VertexShader_Sprite(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	matrix matWV,matWVP;
	matWV = mul(g_matWorld,g_matView);
	matWVP = mul(matWV,g_matProj);

	vsOut.vPos = mul(vsIn.vPos,matWVP);

	int iX = g_iImgIdx % g_iMaxImgX;
	int iY = g_iImgIdx / g_iMaxImgX;

	vsOut.vTexUV.x = vsIn.vTexUV.x*(1.f/g_iMaxImgX) * iX;
	vsOut.vTexUV.y = vsIn.vTexUV.y*(1.f/g_iMaxImgX) * iY;

	vsOut.vTexUV.x = (vsIn.vTexUV.x / g_iMaxImgX) + (1.f/g_iMaxImgX) * iX;
	vsOut.vTexUV.y = (vsIn.vTexUV.y / g_iMaxImgX) + (1.f/g_iMaxImgX) * iY;


	return vsOut;
}

struct PixelShader_Input
{
	float2 vTexUV : TEXCOORD0;
	float2 vSecondUV : TEXCOORD1;
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

	psOut.vColor = tex2D(baseSampler , psIn.vTexUV);
	psOut.vColor1 = psOut.vColor;
	
	return psOut;
}

PixelShader_Output PixelShader_NoDrawIn1Surface(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	psOut.vColor = tex2D(baseSampler , psIn.vTexUV);
	psOut.vColor2 = psOut.vColor;
	

	return psOut;
}

PixelShader_Output PixelShader_AlphaDecrease(PixelShader_Input psIn)
{
	PixelShader_Output psOut= (PixelShader_Output)0.f;

	psOut.vColor = tex2D(baseSampler , psIn.vTexUV);
	psOut.vColor1 = psOut.vColor;
	psOut.vColor.a *= g_fAlpha;


	return psOut;
}

PixelShader_Output PixelShader_AlphaDecreaseChangeColor(PixelShader_Input psIn)
{
	PixelShader_Output psOut= (PixelShader_Output)0.f;

	psOut.vColor = tex2D(baseSampler , psIn.vTexUV) * g_vOffSetColor;
	psOut.vColor1 = psOut.vColor;
	psOut.vColor.a *= g_fAlpha;

	//For Distortion
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

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_Main();
	}

	pass Default_ShadingSprite
	{
		lighting = false;
		cullmode = none;
		zwriteenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		alphatestenable = true;
		alpharef = 0x1f;
		alphafunc = greater;

		VertexShader = compile vs_3_0 VertexShader_Sprite();
		PixelShader = compile ps_3_0 PixelShader_AlphaDecrease();
	}

	pass Default_ShadingAlphaDecrease
	{
		zwriteenable = false;
		lighting = false;
		cullmode = none;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_AlphaDecrease();
	}

	pass Default_ForSpiral
	{
		zenable = true;
		lighting = false;
		cullmode = none;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		VertexShader = compile vs_3_0 VertexShader_ToProjDirect();
		PixelShader = compile ps_3_0 PixelShader_AlphaDecrease();
	}

	pass Default_ShadingNotInSurface1
	{
		lighting = false;
		cullmode = none;
		zwriteenable = false;
		//zenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_NoDrawIn1Surface();
	}

	pass Default_ShadingAlphaDecreaseEff
	{
		zwriteenable = false;
		lighting = false;
		cullmode = none;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_AlphaDecreaseChangeColor();
	}

	pass Default_ForRedScreen
	{
		zenable = false;
		lighting = false;
		cullmode = none;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VertexShader_ToProjDirect();
		PixelShader = compile ps_3_0 PixelShader_AlphaDecrease();
	}
};