#include "Shader_Include.hpp"

float2x2 g_matRot;
float g_fProceed = 1.f;
float g_fDistortionPow = 0.05f;

//-------------Blur---------------//
float g_fWidth;
float g_fHeight;
float g_fWeight[13];
float g_fTotalWeight;
float g_fCoeff;


float g_fRadialWeight[6];
float g_TotalRadialWeight;
int g_iMaxBlurCnt = 6;
float2 g_fFocusPoint = {0.5f,0.5f};
float g_fBlurPower = 0.05f;
////////////////////////////////////
//-------------FadeOut------------//
float g_fFade;


texture g_baseTexture;
sampler baseSampler = sampler_state
{
	texture = g_baseTexture;
	/*minfilter = linear;
	magfilter = linear;
	mipfilter = linear;*/
	/*addressu = clamp;
	addressv = clamp;*/
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


struct VertexShader_Input
{
	vector vPos : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct VertexShader_Output
{
	vector vPos : POSITION;
	float2 vTexUV : TEXCOORD0;
};

VertexShader_Output VertexShader_Main(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut  = (VertexShader_Output)0.f;

	matrix matWV,matWVP;
	matWV = mul(g_matWorld,g_matView);
	matWVP = mul(matWV,g_matProj);

	vsOut.vPos = mul(vsIn.vPos,matWVP);
	vsOut.vTexUV = vsIn.vTexUV;

	return vsOut;
}

struct PixelShader_Input
{
	float2 vTexUV : TEXCOORD0;
};
struct PixelShader_Output
{
	vector vColor : COLOR0;
	vector vColor1 : COLOR1;
};

PixelShader_Output PixelShader_Main(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	float fInit = 5.f - g_fProceed;
	float fScale = max(fInit , 0.5f);

	float2 vNewUV = (psIn.vTexUV-0.5f) * fScale;
	vNewUV = mul(vNewUV,g_matRot)+0.5;

	vector vBrushColor = tex2D(secondSampler,vNewUV);
	psOut.vColor = vBrushColor;

	float x = psOut.vColor.x * 2 - 0.9;
	float y = psOut.vColor.y * 2 - 0.9;

	x = psIn.vTexUV.x + x * 0.15;
	y = psIn.vTexUV.y + y * 0.15;

	psOut.vColor = tex2D(baseSampler,float2(x,y));
	psOut.vColor1 = psOut.vColor;

	return psOut;
}

PixelShader_Output PixelShader_NoRot(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	/*float fInit = 5.f - g_fProceed;
	float fScale = max(fInit , 0.5f);*/

	vector vBrushColor = tex2D(secondSampler,psIn.vTexUV/**fScale*/);
	psOut.vColor = vBrushColor;

	float x = psOut.vColor.x;// * 2 - 0.9;
	float y = psOut.vColor.y;// * 2 - 0.9;

	x = psIn.vTexUV.x + x * g_fDistortionPow;
	y = psIn.vTexUV.y + y * g_fDistortionPow;

	psOut.vColor = tex2D(baseSampler,float2(x,y));
	psOut.vColor1 = psOut.vColor;

	//psOut.vColor = tex2D(baseSampler,psIn.vTexUV);

	return psOut;
}

PixelShader_Output PixelShader_Blur(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	/*if(psIn.vTexUV.x > 0.5)
	{
		psOut.vColor = tex2D(baseSampler,psIn.vTexUV);
		return psOut;
	}*/

	float2 vNewUV = psIn.vTexUV;
	vector vNewColor = vector(0.f,0.f,0.f,0.f);
	for(int i=-6; i<7; ++i)
	{
		vector vTempColor = vector(0.f,0.f,0.f,0.f);
		for(int j=-6; j<7; ++j)
		{
			vNewUV.x = psIn.vTexUV.x + g_fWidth * i;
			vNewUV.y = psIn.vTexUV.y + g_fHeight * j;
			vTempColor += g_fWeight[j+6] * tex2D(baseSampler,vNewUV);
		}
		vTempColor /= g_fTotalWeight;
		vTempColor *= g_fWeight[i+6];
		vNewColor += vTempColor;
	}
	vNewColor /= g_fTotalWeight;
	
	psOut.vColor = vNewColor;
	psOut.vColor1 = psOut.vColor;

	return psOut;
}

PixelShader_Output PixelShader_RadialBlur(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;
	/*psOut.vColor = tex2D(baseSampler,psIn.vTexUV);
	return psOut;*/

	vector vNewColor = vector(0.f,0.f,0.f,0.f);
	float2 vNewUV = g_fFocusPoint;

	float3 vDir = float3(psIn.vTexUV.x,psIn.vTexUV.y,0.f) - float3(vNewUV.x,vNewUV.y,0.f);
	float fLength = length(vDir);

	//if(fLength < 0.1)
	//{
	//	psOut.vColor = tex2D(baseSampler , psIn.vTexUV);
	//	return psOut;
	//}

	for(int i=0; i<g_iMaxBlurCnt; ++i)
	{
		vNewUV = psIn.vTexUV - i * float2(vDir.x,vDir.y) * g_fBlurPower;
		vNewColor += g_fRadialWeight[g_iMaxBlurCnt-1-i] * tex2D(baseSampler, vNewUV); 
	}
	vNewColor /= g_TotalRadialWeight;

	psOut.vColor = vNewColor;
	psOut.vColor1 = psOut.vColor;

	return psOut;
}

PixelShader_Output PixelShader_FadeOut(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	vector vTexColor = tex2D(baseSampler,psIn.vTexUV);
	vTexColor *= g_fFade;
	psOut.vColor = vTexColor;

	return psOut;
}

PixelShader_Output PixelShader_RedScreen(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	vector vTexColor = tex2D(baseSampler,psIn.vTexUV);
	//vTexColor *= g_fFade;
	//vTexColor.r += g_fFade;
	vTexColor.gb *= g_fFade;
	psOut.vColor = vTexColor;

	return psOut;
}

PixelShader_Output PixelShader_WhiteScreen(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	vector vTexColor = tex2D(baseSampler,psIn.vTexUV);
	//vTexColor *= g_fFade;
	//vTexColor.r += g_fFade;
	vTexColor.rgb += g_fFade;
	psOut.vColor = vTexColor;

	return psOut;
}

technique Default_Device
{
	pass Default_Shading
	{
		lighting = false;
		cullmode = none;
		//fillmode = wireframe;
		//zwriteenable = false;
		zenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PixelShader_Main();
	}
	pass Default_ShadingNoRot
	{
		lighting = false;
		cullmode = none;
		//zwriteenable = false;
		zenable = false;
		//fillmode = wireframe;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PixelShader_NoRot();
	}


	pass Default_ShadingBlur
	{
		lighting = false;
		cullmode = none;
		zwriteenable = false;
		//fillmode = wireframe;


		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_Blur();
	}

	pass Default_ShadingRadialBlur
	{
		lighting = false;
		cullmode = none;
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		//fillmode = wireframe;

		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_RadialBlur();
	}
	
	pass Default_FadeOut
	{
		lighting = false;
		cullmode = none;
		zenable = false;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PixelShader_FadeOut();
	}

	pass Default_RedScreen
	{
		lighting = false;
		cullmode = none;
		zenable = false;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PixelShader_RedScreen();
	}
	pass Default_WhiteScreen
	{
		lighting = false;
		cullmode = none;
		zenable = false;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PixelShader_WhiteScreen();
	}
}