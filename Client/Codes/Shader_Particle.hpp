#include "Shader_Include.hpp"



float g_fViewPortY;
vector g_vCamWorldPos;
float g_fInputSize;
float g_fRandValue;
float g_fAlphaFunc;
float g_fAlpha = 1.f;
float g_fMaxSize = 100.f;
bool g_isZbufferEnable = false;

texture g_baseTexture;
sampler baseSampler = sampler_state
{
	texture = g_baseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	addressu = clamp;
	addressv = clamp;
};

texture g_secondTexture;
sampler secondSampler = sampler_state
{
	texture = g_secondTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	addressu = clamp;
	addressv = clamp;
};

texture g_thirdTexture;
sampler thirdSampler = sampler_state
{
	texture = g_thirdTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	addressu = clamp;
	addressv = clamp;
};

struct VertexShader_Input
{
	vector vPos : POSITION;
	vector vColor : COLOR0;
};

struct VertexShader_Output
{
	vector vPos : POSITION;
	vector vColor : COLOR0;
	float fSize : PSIZE;
	int iRand : BLENDINDICES0;
};

VertexShader_Output VertexShader_Main(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	Matrix matWV = mul(g_matWorld,g_matView);
	Matrix matWVP = mul(matWV,g_matProj);

	vsOut.vPos = mul(vector(vsIn.vPos.xyz,1.f),matWVP);
	vsOut.vColor = vsIn.vColor;

	vector vWorldPos = mul(vector(vsIn.vPos.xyz,1.f) , g_matWorld);
	float fDist = length(vWorldPos - g_vCamWorldPos);

	vsOut.fSize = (3000.f * vsIn.vColor.a) / fDist;
	return vsOut;
}

VertexShader_Output VertexShader_FromWorld(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	Matrix matVP = mul(g_matView,g_matProj);

	vsOut.vPos = mul(vector(vsIn.vPos.xyz,1.f),matVP);
	vsOut.vColor = vsIn.vColor;

	float fDist = length(vsIn.vPos - g_vCamWorldPos);

	vsOut.fSize = (3000.f * vsIn.vColor.a) / fDist;
	return vsOut;
}

VertexShader_Output VertexShader_FromWorldRandImg(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	Matrix matVP = mul(g_matView,g_matProj);

	vsOut.vPos = mul(vector(vsIn.vPos.xyz,1.f),matVP);
	vsOut.vColor = vsIn.vColor;

	float fDist = length(vsIn.vPos - g_vCamWorldPos);

	vsOut.fSize = (3000.f * vsIn.vColor.a) / fDist;
	
	if(vsIn.vColor.a < 0.33f)
		vsOut.iRand = 0;
	else if(vsIn.vColor.a < 0.66f)
		vsOut.iRand = 1;
	else 
		vsOut.iRand = 2;

	return vsOut;
}


struct PixelShader_Input
{
	float2 vTexUV : TEXCOORD0;
	vector vColor : COLOR0;
	int iRand : BLENDINDICES0;
};
struct PixelShader_Output
{
	vector vColor : COLOR0;
	vector vColor1 : COLOR1;
};

PixelShader_Output PixelShader_Main(PixelShader_Input psIn)
{
	PixelShader_Output psOut;

	float2 fNewUV = psIn.vTexUV;
	psOut.vColor = tex2D(baseSampler , fNewUV) * psIn.vColor;
	psOut.vColor.r = min(psOut.vColor.r+ g_fRandValue,1.f);
	psOut.vColor.g = min(psOut.vColor.g+ g_fRandValue,1.f);
	psOut.vColor.b = min(psOut.vColor.b+ g_fRandValue,1.f);
	//psOut.vColor = vector(1.f,1.f,1.f,1.f);
	//psOut.vColor = psIn.vColor;
	psOut.vColor1 = psOut.vColor;
	
	return psOut;
}

PixelShader_Output PixelShader_AlphaDecrease(PixelShader_Input psIn)
{
	PixelShader_Output psOut;

	psOut.vColor = tex2D(baseSampler , psIn.vTexUV) * psIn.vColor;
	//psOut.vColor.r = min(psOut.vColor.r+ g_fRandValue,1.f);
	//psOut.vColor.g = min(psOut.vColor.g+ g_fRandValue,1.f);
	//psOut.vColor.b = min(psOut.vColor.b+ g_fRandValue,1.f);
	psOut.vColor.a *= g_fAlpha;
	psOut.vColor1 = psOut.vColor;

	return psOut;
}

PixelShader_Output PixelShader_AlphaDandRandImg(PixelShader_Input psIn)
{
	PixelShader_Output psOut;

	vector vNewColor = psIn.vColor;
	vNewColor.a = 1.f;

	if(psIn.iRand == 0)
		psOut.vColor = tex2D(baseSampler , psIn.vTexUV) * vNewColor;
	else if(psIn.iRand == 1)
		psOut.vColor = tex2D(secondSampler , psIn.vTexUV) * vNewColor;
	else if(psIn.iRand == 2)
		psOut.vColor = tex2D(thirdSampler , psIn.vTexUV) * vNewColor;

	
	psOut.vColor.a *= g_fAlpha;
	psOut.vColor1 = psOut.vColor;

	return psOut;
}

technique Default_Device
{
	pass Particle_Shading
	{
		//fillmode = wireframe;
		//zenable = false;
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		/*alphatestenable = true;
		alpharef = 0x02;
		alphafunc = greater;*/

		pointspriteenable = true;
		pointscaleenable = true;

		pointsize_min = 10.f;
		pointsize = g_fMaxSize;
		pointsize_max = g_fMaxSize;
	
		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_AlphaDecrease();
	}

	pass Particle_ShadingAlphaDecrease
	{
		//fillmode = wireframe;
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		/*alphatestenable = true;
		alpharef = 0x6f;
		alphafunc = greater;*/

		pointspriteenable = true;
		pointscaleenable = true;

		pointsize_min = 10.f;
		pointsize = g_fMaxSize;
		pointsize_max = g_fMaxSize;
	
		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_AlphaDecrease();
	}

	pass Particle_ShadingFromWorld
	{
		//fillmode = wireframe;
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		/*alphatestenable = true;
		alpharef = 0x6f;
		alphafunc = greater;*/

		pointspriteenable = true;
		pointscaleenable = true;

		pointsize_min = 10.f;
		pointsize = g_fMaxSize;
		pointsize_max = g_fMaxSize;
	
		VertexShader = compile vs_3_0 VertexShader_FromWorld();
		PixelShader = compile ps_3_0 PixelShader_AlphaDecrease();
	}

	pass Particle_ShadingFromWorldandRandImg
	{
		//fillmode = wireframe;
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		/*alphatestenable = true;
		alpharef = 0x6f;
		alphafunc = greater;*/

		pointspriteenable = true;
		pointscaleenable = true;

		pointsize_min = 10.f;
		pointsize = g_fMaxSize;
		pointsize_max = g_fMaxSize;
	
		VertexShader = compile vs_3_0 VertexShader_FromWorldRandImg();
		PixelShader = compile ps_3_0 PixelShader_AlphaDandRandImg();
	}
	pass Particle_ShadingFromWorldandRandImgInvSrcAlpha
	{
		//fillmode = wireframe;
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		/*alphatestenable = true;
		alpharef = 0x6f;
		alphafunc = greater;*/

		pointspriteenable = true;
		pointscaleenable = true;

		pointsize_min = 10.f;
		pointsize = g_fMaxSize;
		pointsize_max = g_fMaxSize;
	
		VertexShader = compile vs_3_0 VertexShader_FromWorldRandImg();
		PixelShader = compile ps_3_0 PixelShader_AlphaDandRandImg();
	}
};