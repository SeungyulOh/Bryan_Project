#include "Shader_Include.hpp"

texture g_baseTexture;

sampler baseSampler = sampler_state
{
	texture = g_baseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_copyTexture;
sampler copySampler = sampler_state
{
	texture = g_copyTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_normalTexture;
sampler normalSampler = sampler_state
{
	texture = g_normalTexture;
};

texture g_shadowTexture;
sampler shadowSampler = sampler_state
{
	texture = g_shadowTexture;
};

vector g_vLightDir;
vector g_vLightDiffuse;
vector g_vLightAmbient;
vector g_vLightSpecular;

vector g_vMtrlDiffuse;
vector g_vMtrlAmbient;
vector g_vMtrlSpecular;

float g_fPower = 50.f;

vector g_vCamPos;
Matrix g_vLightView;

struct VertexShader_Input
{
	vector vPos : POSITION;
	vector vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	/*float3 mTangent: TANGENT;
	float3 mBinormal : BINORMAL;*/
};

struct VertexShader_Output
{
	vector vPos : POSITION;
	vector vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	vector vWorldPos : TEXCOORD1;
	//float3 mTangent: TEXCOORD2;
	//float3 mBinormal : TEXCOORD3;
	//float3 mNormal : TEXCOORD4;
};

VertexShader_Output VertexShader_Main(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	Matrix matWV,matWVP;
	matWV = mul(g_matWorld,g_matView);
	matWVP = mul(matWV, g_matProj);

	vsOut.vPos = mul(vector(vsIn.vPos.xyz,1.f), matWVP);
	vsOut.vNormal = normalize(mul(vector(vsIn.vNormal.xyz,0.f), g_matWorld));
	vsOut.vTexUV = vsIn.vTexUV;
	vsOut.vWorldPos = mul(vector(vsIn.vPos.xyz,1.f), g_matWorld);

	return vsOut;
}

VertexShader_Output VertexShader_DrawShadow(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	Matrix matWV,matWVP;
	matWV = mul(g_matWorld,g_matView);
	matWVP = mul(matWV, g_matProj);

	Matrix matWLV,matWLVP;
	matWLV = mul(g_matWorld, g_vLightView);
	matWLVP = mul(matWLV, g_matProj);

	vsOut.vPos = mul(vector(vsIn.vPos.xyz,1.f), matWVP);
	vsOut.vNormal = normalize(mul(vector(vsIn.vNormal.xyz,0.f), g_matWorld));
	vsOut.vTexUV = vsIn.vTexUV;
	vsOut.vWorldPos = mul(vector(vsIn.vPos.xyz,1.f), g_matWorld);
	return vsOut;
}


struct PixelShader_Input
{
	vector vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	vector vWorldPos : TEXCOORD1;
	//float3 mTangent: TEXCOORD2;
	//float3 mBinormal : TEXCOORD3;
	//float3 mNormal : TEXCOORD4;
};
struct PixelShader_Output
{
	vector vColor : COLOR0;
	vector vColor1 : COLOR1;
};

PixelShader_Output PixelShader_Main(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	vector vShade = max(dot(normalize(g_vLightDir) * -1.f , psIn.vNormal),0.f);
	
	vector vLook = psIn.vWorldPos - g_vCamPos;
	vector vReflect = reflect(g_vLightDir,psIn.vNormal);

	vector vSpecular = pow(max(dot(normalize(vLook)*-1.f , normalize(vReflect)),0.f),g_fPower);

	vector vTexColor = tex2D(baseSampler,psIn.vTexUV);

	/*float fRim = dot(normalize(vLook)*-1.f , psIn.vNormal);
	if(abs(fRim) <= 0.25f)
		psOut.vColor = vector(1.f,1.f,1.f,1.f);
	else*/
		psOut.vColor = (vTexColor * g_vLightDiffuse * g_vMtrlDiffuse + vSpecular * g_vLightSpecular * g_vMtrlSpecular) * (vShade + g_vLightAmbient * g_vMtrlAmbient);

	/////
		psOut.vColor1 = psOut.vColor;
	return psOut;
}

PixelShader_Output PixelShader_DrawShadow(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	vector vShade = max(dot(normalize(g_vLightDir) * -1.f , psIn.vNormal),0.f);
	vector vLook = psIn.vWorldPos - g_vCamPos;
	vector vReflect = reflect(g_vLightDir,psIn.vNormal);
	vector vSpecular = pow(max(dot(normalize(vLook)*-1.f , normalize(vReflect)),0.f),g_fPower);
	vector vTexColor = tex2D(baseSampler,psIn.vTexUV);

	/*float fRim = dot(normalize(vLook)*-1.f , psIn.vNormal);
	if(abs(fRim) <= 0.1f)
		psOut.vColor = vector(1.f,1.f,1.f,1.f);
	else*/
		psOut.vColor = (vTexColor * g_vLightDiffuse * g_vMtrlDiffuse + vSpecular * g_vLightSpecular * g_vMtrlSpecular) * (vShade + g_vLightAmbient * g_vMtrlAmbient);

	/////
		psOut.vColor1 = psOut.vColor;
	return psOut;
}

PixelShader_Output PixelShader_NormalMapping(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	/*psOut.vColor = vector(psIn.mTangent,1.f);
	return psOut;*/

	//float3 tangentNormal = tex2D(normalSampler,psIn.vTexUV).xyz;
	////tangentNormal.y = 1 - tangentNormal.y;
	//tangentNormal = normalize(tangentNormal * 2.f - 1.f);
	//tangentNormal = mul(tangentNormal,(float3x3)g_matWorld);
	//float3x3 TBN = float3x3(normalize(psIn.mTangent), normalize(psIn.mBinormal), normalize(tangentNormal));
	//TBN = transpose(TBN);

	//vector WorldNormal = normalize(vector(mul(TBN, tangentNormal),0.f));

	//vector vShade = max(dot(normalize(g_vLightDir) * -1.f , WorldNormal),0.f);

	//vector vLook = psIn.vWorldPos - g_vCamPos;
	//vector vReflect = reflect(g_vLightDir,WorldNormal);

	//vector vSpecular = pow(max(dot(normalize(vLook)*-1.f , normalize(vReflect)),0.f),g_fPower);

	//vector vTexColor = tex2D(baseSampler,psIn.vTexUV);

	//vSpecular = 0.f;

	////float fRim = dot(normalize(vLook)*-1.f , WorldNormal);
	////if(abs(fRim) <= 0.1f)
	//	//psOut.vColor = vector(1.f,1.f,1.f,1.f);
	////else
	//	psOut.vColor = (vTexColor * g_vLightDiffuse * g_vMtrlDiffuse + vSpecular * g_vLightSpecular * g_vMtrlSpecular) * (vShade /*+ g_vLightAmbient * g_vMtrlAmbient*/);

	//psOut.vColor1 = psOut.vColor;

	return psOut;
}

technique Default_Device
{
	pass PhongShading
	{
		cullmode = ccw;
		lighting = true;

		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_Main();
	}
	pass PhongShadingNormalMapping
	{
		//fillmode = wireframe;
		cullmode = ccw;
		lighting = true;

		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_NormalMapping();
	}
	pass DrawShadow
	{
		cullmode = ccw;
		lighting = true;

		VertexShader = compile vs_3_0 VertexShader_DrawShadow();
		PixelShader = compile ps_3_0 PixelShader_DrawShadow();

	}
};