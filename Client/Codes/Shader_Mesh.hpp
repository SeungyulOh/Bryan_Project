#include "Shader_Include.hpp"

//matrix 3  + texture info
texture g_baseTexture;
sampler baseSampler = sampler_state
{
	texture = g_baseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_shadowTexture;
sampler shadowSampler = sampler_state
{
	texture = g_shadowTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	addressu = border;
	addressv = border;
};

vector g_vLightDir;
vector g_vLightDiffuse;
vector g_vLightAmbient;
vector g_vLightSpecular;

vector g_vMtrlDiffuse;
vector g_vMtrlAmbient;
vector g_vMtrlSpecular;
float g_fPower = 10.f;
float g_OffSet = 0.f;
float g_AlphaForPlayer = 1.f;

vector g_vCamPos;
float g_fMagicNum1 = 0.02f;
float g_fMagicNum2 = 0.25f;

vector g_vHitColor;

//For Shadow
matrix g_matLightView;
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
	/*vector vShade : COLOR0;
	vector vSpecular : COLOR1;*/
	float2 vTexUV : TEXCOORD0;
	//vector vClipPosition : TEXCOORD1;
	vector vProjClipPosition : TEXCOORD1;
	vector vProjPosForShadow : TEXCOORD2;
	vector vNormal : NORMAL;
};

VertexShader_Output VertexShader_Main(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	Matrix matWV, matWVP;
	matWV = mul(g_matWorld,g_matView);
	matWVP = mul(matWV,g_matProj);

	vsOut.vPos = mul(vector(vsIn.vPos.xyz,1.f),matWVP);
	vsOut.vTexUV = vsIn.vTexUV;
	vsOut.vNormal = normalize(mul(vector(vsIn.vNormal.xyz,0.f),g_matWorld));
	vsOut.vProjClipPosition = vsOut.vPos;


	return vsOut;
}

VertexShader_Output VertexShader_DrawShadow(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	Matrix matWV, matWVP;
	matWV = mul(g_matWorld,g_matView);
	matWVP = mul(matWV,g_matProj);

	vsOut.vPos = mul(vector(vsIn.vPos.xyz,1.f),matWVP);
	vsOut.vProjClipPosition = vsOut.vPos;

	Matrix matWLV, matWLVP;
	matWLV = mul(g_matWorld,g_matLightView);
	matWLVP = mul(matWLV,g_matLightProj);
	//vsOut.vClipPosition = mul(vector(vsIn.vPos.xyz,1.f),matWLV);
	vsOut.vProjPosForShadow = mul(vector(vsIn.vPos.xyz,1.f),matWLVP);

	//vector vWorldNormal = mul(vector(vsIn.vNormal.xyz,0.f),g_matWorld);
	//vsOut.vShade = max(dot(normalize(vWorldNormal), normalize(g_vLightDir) * -1.f),0.f);
	//vector vLook = mul(vector(vsIn.vPos.xyz,1.f),g_matWorld) - g_vCamPos;
	//vsOut.vSpecular = pow(max(dot(normalize(vLook*-1.f) , normalize(reflect(g_vLightDir,vsIn.vNormal))),0.f),g_fPower);
	vsOut.vTexUV = vsIn.vTexUV;
	vsOut.vNormal = normalize(mul(vector(vsIn.vNormal.xyz,0.f),g_matWorld));


	return vsOut;
}

VertexShader_Output VertexShader_ForWater(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut = (VertexShader_Output)0.f;

	Matrix matWV, matWVP;
	matWV = mul(g_matWorld,g_matView);
	matWVP = mul(matWV,g_matProj);

	vsOut.vPos = mul(vector(vsIn.vPos.xyz,1.f),matWVP);
	
	vsIn.vTexUV.y -= g_OffSet;
	vsOut.vTexUV = vsIn.vTexUV;	

	return vsOut;
}


struct PixelShader_Input
{
	//vector vShade : COLOR0;
	//vector vSpecular : COLOR1;
	float2 vTexUV : TEXCOORD0;
	//vector vClipPosition : TEXCOORD1;
	vector vProjClipPosition : TEXCOORD1;
	vector vProjPosForShadow : TEXCOORD2;
	vector vNormal : NORMAL;
};

struct PixelShader_Output
{
	vector vColor : COLOR0;
	vector vColor1 : COLOR1;
	vector vColor2 : COLOR2; // For normal;
	vector vColor3 : COLOR3; // For Depth;
};

PixelShader_Output PixelShader_Main(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	vector vTexColor = tex2D(baseSampler,psIn.vTexUV); 
	psOut.vColor = vTexColor;
	psOut.vColor1 = psOut.vColor;
	psOut.vColor.a = 1;

	/*float fRim = dot(normalize(vLook)*-1.f , psIn.vNormal);
	if(abs(fRim) <= 0.25f)
		psOut.vColor = vector(1.f,1.f,1.f,1.f);
	else*/

	//psOut.vColor = (vTexColor * g_vLightDiffuse * g_vMtrlDiffuse + psIn.vSpecular * g_vLightSpecular * g_vMtrlSpecular) * (psIn.vShade + g_vLightAmbient * g_vMtrlAmbient);
	
	//psOut.vColor2 = (psIn.vNormal + 1) * 0.5f;
	psOut.vColor2 = vector(psIn.vNormal.xyz * 0.5f + 0.5f, 0.f);
	psOut.vColor3 = vector(psIn.vProjClipPosition.w / 1500.f, psIn.vProjClipPosition.z / psIn.vProjClipPosition.w , 0.f,0.f);
	return psOut;
}



PixelShader_Output PixelShader_GetHit(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	vector vTexColor = tex2D(baseSampler,psIn.vTexUV); 
	psOut.vColor = vTexColor * g_vHitColor;
	psOut.vColor1 = psOut.vColor;
	psOut.vColor.a = 1;

	//psOut.vColor = (vTexColor * g_vLightDiffuse * g_vMtrlDiffuse + psIn.vSpecular * g_vLightSpecular * g_vMtrlSpecular) * (psIn.vShade + g_vLightAmbient * g_vMtrlAmbient);

	//psOut.vColor2 = (psIn.vNormal + 1) * 0.5f;
	psOut.vColor2 = vector(psIn.vNormal.xyz * 0.5f + 0.5f, 0.f);
	psOut.vColor3 = vector(psIn.vProjClipPosition.w / 1500.f, psIn.vProjClipPosition.z / psIn.vProjClipPosition.w , 0.f,0.f);
	return psOut;
}

PixelShader_Output PixelShader_AlphaTest(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	vector vTexColor = tex2D(baseSampler,psIn.vTexUV); 
	psOut.vColor = vTexColor;
	psOut.vColor1 = psOut.vColor;
	//psOut.vColor = (vTexColor * g_vLightDiffuse * g_vMtrlDiffuse + psIn.vSpecular * g_vLightSpecular * g_vMtrlSpecular) * (psIn.vShade + g_vLightAmbient * g_vMtrlAmbient);
	//psOut.vColor2 = (psIn.vNormal + 1) * 0.5f;
	psOut.vColor2 = vector(psIn.vNormal.xyz * 0.5f + 0.5f, 0.f);
	psOut.vColor3 = vector(psIn.vProjClipPosition.w / 1500.f, psIn.vProjClipPosition.z / psIn.vProjClipPosition.w , 0.f,0.f);
	return psOut;
}


PixelShader_Output PixelShader_PlayerDash(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	vector vTexColor = tex2D(baseSampler,psIn.vTexUV); 
	psOut.vColor = vTexColor;//(vTexColor * g_vLightDiffuse * g_vMtrlDiffuse + psIn.vSpecular * g_vLightSpecular * g_vMtrlSpecular) * (psIn.vShade + g_vLightAmbient * g_vMtrlAmbient);
	
	psOut.vColor.a = g_AlphaForPlayer;
	psOut.vColor1 = psOut.vColor;


	return psOut;
}

PixelShader_Output PixelShader_ForWater(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;
	psOut.vColor = tex2D(baseSampler,psIn.vTexUV);
	psOut.vColor1 = psOut.vColor;

	psOut.vColor2 = vector(psIn.vNormal.xyz * 0.5f + 0.5f, 0.f);
	psOut.vColor3 = vector(psIn.vProjClipPosition.w / 1500.f , psIn.vProjClipPosition.z / psIn.vProjClipPosition.w , 0.f,0.f);
	return psOut;
}
PixelShader_Output PixelShader_DrawShadow(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;
	vector vTexColor = tex2D(baseSampler,psIn.vTexUV); 
	psOut.vColor = vTexColor;//(vTexColor * g_vLightDiffuse * g_vMtrlDiffuse + psIn.vSpecular * g_vLightSpecular * g_vMtrlSpecular) * (psIn.vShade + g_vLightAmbient * g_vMtrlAmbient);
	
	
	//float fCurrentDepth = min(psIn.vProjClipPosition.w * g_fMagicNum1 , 1.f);
	float fCurrentDepth = min(psIn.vProjPosForShadow.z / psIn.vProjPosForShadow.w , 1.f);
	
	float2 RealProjXY = psIn.vProjPosForShadow.xy / psIn.vProjPosForShadow.w;
	float fNewU = ((RealProjXY.x + 1) * 0.5f);
	float fNewV = ((-1.f * RealProjXY.y + 1) * 0.5f);

	float fShadowDepth = tex2D(shadowSampler, float2(fNewU,fNewV)).r;

	if(fCurrentDepth > fShadowDepth + g_fMagicNum2)
	{
		//psOut.vColor = vector(1.f,0.f,0.f,1.f);
	}
	else
	{
		psOut.vColor *= 0.3f;
	}

	psOut.vColor1 = psOut.vColor;
	psOut.vColor2 = vector(psIn.vNormal.xyz * 0.5f + 0.5f, 0.f);
	psOut.vColor3 = vector(psIn.vProjClipPosition.w / 1500.f , psIn.vProjClipPosition.z / psIn.vProjClipPosition.w , 0.f,0.f);

	return psOut;
}

technique Default_Device
{
	pass StaticMesh_DeafultShading
	{
		lighting = true;
		cullmode = none;
		alphatestenable = false;

		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_Main();
	}

	pass StaticMesh_Water
	{
		lighting = false;
		cullmode = ccw;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		alpharef = 0x00000044;
		alphatestenable = true;
		alphafunc = greater;

		VertexShader = compile vs_3_0 VertexShader_ForWater();
		PixelShader = compile ps_3_0 PixelShader_ForWater();
	}

	pass Mesh_AlphaTest
	{
		lighting = true;
		cullmode = ccw;

		alpharef = 0x00000044;
		alphatestenable = true;
		alphafunc = greater;

		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_AlphaTest();
	}

	pass Mesh_CullNone
	{
		lighting = true;
		cullmode = ccw;

		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_Main();
	}


	pass Mesh_AlphaBlending
	{
		lighting = true;
		
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;


		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_PlayerDash();
	}

	pass StaticMesh_EffectMesh
	{
		lighting = true;
		cullmode = ccw;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_Main();
	}
	pass DrawShadow
	{
		cullmode = none;
		lighting = true;

		VertexShader = compile vs_3_0 VertexShader_DrawShadow();
		PixelShader = compile ps_3_0 PixelShader_DrawShadow();
	}

	pass StaticMesh_GetHitMesh
	{
		//lighting = true;
		cullmode = none;
		alphatestenable = false;

		VertexShader = compile vs_3_0 VertexShader_Main();
		PixelShader = compile ps_3_0 PixelShader_GetHit();
	}
};