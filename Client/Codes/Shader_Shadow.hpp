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

	/*�÷��̾� ����� ������ ����Ĥ���
		�׋� ���̶� �񤡱� 

		���н� ������*/

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

//Z write ���� �����ٲٱ� renderer���� ���̸� Ÿ�ٿ��ٰ� �׸���  �÷��̾� ������Ʈ add renderer���� shadow�׷쿡 �־��ְ� �׳༮�� ���ϸ� �׸���;;;;