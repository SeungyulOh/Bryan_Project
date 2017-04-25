#include "Shader_Include.hpp"

texture				g_BaseTexture;

sampler	BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct VS_IN // ���ؽ� ���ۿ��� ������ ������ ����
{
	vector	vPosition : POSITION;
	float3	vTexUV : TEXCOORD0;
};

struct VS_OUT // ���ؽ� ���̴��� �����ϰ� �� ������ ��������
{
	vector	vPosition : POSITION;
	float3	vTexUV : TEXCOORD0;

};

// ���ؽ� ���̴� : ������ ��ȯ + ������ ����
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out;	

	// ������ ��ȯ
	// mul:��İ� ����� ���� �����ϰ� ����� �����Ѵ�.
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	// ������ı��� ���ߴ�.	
	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;


	return Out;
}


// �����Ͷ���� �����Ѵ�. (VS_OUT ���������� ������� �ȼ��� �����)

struct PS_IN
{		
	float3	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
	vector	vColor1 : COLOR1;
};

// �ȼ����̴� : �ȼ��� ������ ���������Ͽ� �ȼ��� �������� ���� �����Ѵ�.
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = texCUBE(BaseSampler, In.vTexUV);	
	Out.vColor1 = Out.vColor;

	return Out;
}


technique Default_Device
{
	pass Default_Rendering
	{
		lighting = false;
		CullMode = cw;
		ZEnable = false;
		ZwriteEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 PS_MAIN();
	}


};