#include "Shader_Include.hpp"

//matrix 3  + texture info

vector g_FirstWorldPosLB;
vector g_FirstWorldPosRB;
float g_fSlope = -0.005f;
float g_fProceed;
float g_fScale = 60.f;
float g_fEffectRadius = 2.f;

int g_iVexMaxNum = 40;
vector g_TrailVertex[40];
vector g_SecondTrailVertex[40];
vector g_ThirdTrailVertex[40];

////////////////////////////

texture g_baseTexture;

sampler baseSampler = sampler_state
{
	texture = g_baseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_SecondTexture;
sampler secondSampler = sampler_state
{
	texture = g_SecondTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_ThirdTexture;
sampler thirdSampler = sampler_state
{
	texture = g_ThirdTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct VertexShader_Input
{
	vector vPos : POSITION;
	vector vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
};

struct VertexShader_Output
{
	vector vPos : POSITION;
	float2 vTexUV : TEXCOORD0;
};
VertexShader_Output VertexShader_Main(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	Matrix matWV, matWVP;
	matWV = mul(g_matWorld,g_matView);
	matWVP = mul(matWV,g_matProj);

	vsOut.vPos = mul(vector(vsIn.vPos.xyz,1.f),matWVP);
	vsOut.vTexUV = vsIn.vTexUV;

	return vsOut;
}


VertexShader_Output VertexShader_Trail(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	Matrix matWV, matWVP, matVP;
	matWV = mul(g_matWorld,g_matView);
	matWVP = mul(matWV,g_matProj);
	matVP = mul(g_matView,g_matProj);

	float2 vNewUV = float2(vsIn.vTexUV.y,vsIn.vTexUV.x);
	vsOut.vTexUV = 1.f - vNewUV;
	
	float fNewY = vsIn.vPos.y * g_fScale;
	float fCoeff = g_fScale/2.f;
	
	if(vsIn.vTexUV.x == 1)
	{
		float fNewX = g_fSlope*(fNewY-fCoeff) * (fNewY-fCoeff) + (g_fEffectRadius - g_fSlope * fCoeff*fCoeff);
		vector vModifiedVertex = vector(fNewX,fNewY,vsIn.vPos.z,1.f);
		vsOut.vPos = mul(vModifiedVertex,matWVP);
	}
	else if(vsIn.vTexUV.x == 0)
	{
		float fNewX = g_fSlope*(fNewY-fCoeff) * (fNewY-fCoeff) + (g_fEffectRadius - g_fSlope * fCoeff*fCoeff) - g_fEffectRadius*2.f;
		vector vModifiedVertex = vector(fNewX,fNewY,vsIn.vPos.z,1.f);
		vsOut.vPos = mul(vModifiedVertex,matWVP);
	}
	else
		vsOut.vPos = mul(vector(vsIn.vPos.xyz,1.f),matWVP);
	
	if(g_fProceed < 0.5f)
		vsOut.vTexUV.x = vsOut.vTexUV.x * (1.f-g_fProceed) * 2.f;

	if(vsOut.vTexUV.x > 1.f)
		vsOut.vTexUV.x = 1.f;
	

	return vsOut;
}

VertexShader_Output VertexShader_TrailSecond(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	Matrix matVP;
	matVP = mul(g_matView,g_matProj);


	vector vWorldPos;
	int iIndex;
	if(vsIn.vTexUV.x == 0) // Even Number
		iIndex = 2*vsIn.vTexUV.y * (g_iVexMaxNum/2 - 1);
	else
		iIndex = 2*vsIn.vTexUV.y * (g_iVexMaxNum/2 - 1) + 1;
	
	vWorldPos = g_TrailVertex[iIndex];
	vsOut.vPos = mul(vWorldPos,matVP);
	vsOut.vTexUV = vsIn.vTexUV;

	return vsOut;
}

VertexShader_Output VertexShader_TrailThird(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	Matrix matVP;
	matVP = mul(g_matView,g_matProj);


	vector vWorldPos;
	int iIndex;
	if(vsIn.vTexUV.x == 0) // Even Number
		iIndex = 2*vsIn.vTexUV.y * (g_iVexMaxNum/2 - 1);
	else
		iIndex = 2*vsIn.vTexUV.y * (g_iVexMaxNum/2 - 1) + 1;

	vWorldPos = g_SecondTrailVertex[iIndex];
	vsOut.vPos = mul(vWorldPos,matVP);
	vsOut.vTexUV = vsIn.vTexUV;

	return vsOut;
}
VertexShader_Output VertexShader_TrailKadan(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	Matrix matVP;
	matVP = mul(g_matView,g_matProj);


	vector vWorldPos;
	int iIndex;
	if(vsIn.vTexUV.x == 0) // Even Number
		iIndex = 2*vsIn.vTexUV.y * (g_iVexMaxNum/2 - 1);
	else
		iIndex = 2*vsIn.vTexUV.y * (g_iVexMaxNum/2 - 1) + 1;

	vWorldPos = g_ThirdTrailVertex[iIndex];
	vsOut.vPos = mul(vWorldPos,matVP);
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


PixelShader_Output PixelShader_PlayerDash(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;


	vector vTexColor = tex2D(baseSampler,psIn.vTexUV); 
	psOut.vColor = vTexColor;
	psOut.vColor.a = 0.6f;
	psOut.vColor1 = psOut.vColor;

	return psOut;
}

PixelShader_Output PixelShader_Trail(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	vector vTexColor = tex2D(baseSampler,psIn.vTexUV);
	psOut.vColor = vTexColor;
	psOut.vColor1 = psOut.vColor;


	//psOut.vColor.a = (vTexColor.r + vTexColor.g + vTexColor.b) / 3.f;
	//psOut.vColor.a = vector(1.f,0.f,0.f,1.f);
	
	//psOut.vColor.a *= 1-g_fProceed;

	return psOut;
}

PixelShader_Output PixelShader_TrailSpirit(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	vector vTexColor = tex2D(secondSampler,psIn.vTexUV);
	psOut.vColor = vTexColor;
	psOut.vColor1 = psOut.vColor;

	//psOut.vColor.r = 1.f;
	psOut.vColor.a = min(vTexColor.a + 0.4f,1.f);

	//psOut.vColor.a = (vTexColor.r + vTexColor.g + vTexColor.b) / 3.f;
	//psOut.vColor.a = vector(1.f,0.f,0.f,1.f);

	//psOut.vColor.a *= 1-g_fProceed;

	return psOut;
}

PixelShader_Output PixelShader_KadanTrail(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	vector vTexColor = tex2D(thirdSampler,psIn.vTexUV);
	psOut.vColor = vTexColor;
	psOut.vColor1 = psOut.vColor;

	return psOut;
}



technique Default_Device
{
	pass Mesh_AlphaBlending
	{
		zwriteenable = false;
		lighting = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_PlayerDash();
	}

	pass Mesh_AlphaBlendingNTesting
	{
		//fillmode = wireframe;
		lighting = false;

		cullmode = none;
		zwriteenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		VertexShader = compile vs_3_0 VertexShader_Trail();
		PixelShader = compile ps_3_0 PixelShader_Trail();
	}

	pass Mesh_NewTrail
	{
		//fillmode = wireframe;
		lighting = false;
		cullmode = none;
		zwriteenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		/*alphatestenable = true;
		alpharef = 0x0f;
		alphafunc = greater;*/

		VertexShader = compile vs_3_0 VertexShader_TrailSecond();
		PixelShader = compile ps_3_0 PixelShader_Trail();
	}

	pass Mesh_NewTrail2
	{
		//fillmode = wireframe;
		//zenable = false;
		zwriteenable = false;
		lighting = false;
		cullmode = none;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		alphatestenable = true;
		alpharef = 0x1f;
		alphafunc = greater;

		VertexShader = compile vs_3_0 VertexShader_TrailThird();
		PixelShader = compile ps_3_0 PixelShader_TrailSpirit();
	}

	pass Mesh_KadanTrail
	{
		zwriteenable = false;
		lighting = false;
		cullmode = none;

		//fillmode = wireframe;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VertexShader_TrailKadan();
		PixelShader = compile ps_3_0 PixelShader_KadanTrail();
	}
};