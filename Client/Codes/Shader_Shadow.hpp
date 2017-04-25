#include "Shader_Include.hpp"

texture g_baseTexture;

sampler baseSampler = sampler_state
{
	texture = g_baseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

Matrix g_matLightView;
Matrix g_matLightProj;

struct VertexShader_Input
{
	vector vPos : POSITION;
	vector vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
};

struct VertexShader_Output
{
	vector vPos : POSITION;
	vector vShadowDepth : TEXCOORD0;
};


VertexShader_Output VertexShader_WriteShadow(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	Matrix matWV, matWVP;
	matWV = mul(g_matWorld,g_matLightView);
	matWVP = mul(matWV, g_matLightProj);
	vsOut.vPos = mul(vector(vsIn.vPos.xyz,1.f),matWVP);

	vsOut.vShadowDepth = mul(vector(vsIn.vPos.xyz,1.f),matWVP);

	/*플레이어 뷰행렬 광원의 뷰행렬ㅇ로
		그떄 깊이랑 비ㄱ교 

		투패스 렌더링*/

	return vsOut;
}


struct PixelShader_Input
{
	vector vShadowDepth : TEXCOORD0;
};
struct PixelShader_Output
{
	vector vDepth : COLOR0;
};

PixelShader_Output PixelShader_WriteShadowDepth(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	psOut.vDepth.rgb = psIn.vShadowDepth.z / psIn.vShadowDepth.w;
	//psOut.vDepth.rgb = min(psIn.vShadowDepth.z * 0.005f,1.f);
	psOut.vDepth.a = 1.f;

	return psOut;
}


technique Default_Device
{
	pass WriteShadowDepth
	{
		//fillmode = wireframe;
		cullmode = none;
		lighting = false;
		zwriteenable = false;

		VertexShader = compile vs_3_0 VertexShader_WriteShadow();
		PixelShader = compile ps_3_0 PixelShader_WriteShadowDepth();
	}
	
};

//Z write 끄고 구조바꾸기 renderer에서 깊이를 타겟에다가 그리고  플레이어 업데이트 add renderer에서 shadow그룹에 넣어주고 그녀석을 비교하며 그린다;;;;