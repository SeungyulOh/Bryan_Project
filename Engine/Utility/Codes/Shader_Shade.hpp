texture g_normalTexture;
sampler normalSampler = sampler_state
{
	texture = g_normalTexture;
	addressu = clamp;
	addressv = clamp;
};

texture g_depthTexture;
sampler depthSampler = sampler_state
{
	texture = g_depthTexture;
};

vector g_vLightWorldDirection;
vector g_vLightWorldPos;
vector g_vCamWorldPos;

//Matrix g_matInvView;
Matrix g_matInvProjView;
Matrix g_matInvView;
Matrix g_matInvProj;

vector g_vLightDiffuse;
vector g_vLightAmbient;
vector g_vLightSpecular;

vector g_vMtrlDiffuse = (vector)1.f;
vector g_vMtrlAmbient = (vector)1.f;
vector g_vMtrlSpecular = (vector)1.f;

float g_fPower = 20.f;
float g_fRange;

struct PixelShader_Input
{
	float2 vTexUV : TEXCOORD0;
};
struct PixelShader_Output
{
	vector vColor : COLOR0;
	vector vColor1 : COLOR1;
	vector vSpecular : COLOR2;
};
PixelShader_Output PixelShader_DrawShade(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	vector vWorldNormal = tex2D(normalSampler,psIn.vTexUV);
	vWorldNormal = vector(vWorldNormal.xyz * 2.f - 1.f,0.f);

	vector vShade = max(dot(normalize(g_vLightWorldDirection) * -1.f , vWorldNormal),0.f);
	psOut.vColor = (vShade + g_vLightAmbient * g_vMtrlAmbient) * g_vLightDiffuse * g_vMtrlDiffuse;

	vector vDepth = tex2D(depthSampler,psIn.vTexUV);
	//W * V * P //Making Proj's Pos before dividing W
	float fViewZ = vDepth.x * 1500.f;

	float fProjX = (psIn.vTexUV.x - 0.5f) * 2.f * fViewZ;
	float fProjY = (-psIn.vTexUV.y + 0.5f) * 2.f * fViewZ;
	float fProjZ = vDepth.y * fViewZ;
	vector vProjPos = vector(fProjX,fProjY,fProjZ,fViewZ);

	//vector vMyWorldPos = mul(vProjPos,g_matInvProjView);
	vector vMyWorldPos = mul(vProjPos,g_matInvProj);
	vMyWorldPos = mul(vMyWorldPos,g_matInvView);
	
	vector vLook = g_vCamWorldPos - vMyWorldPos;
	vector vReflect = reflect(normalize(g_vLightWorldDirection),normalize(vWorldNormal));
	float fSpecular = pow(max(dot(normalize(vLook),normalize(vReflect)),0.f),g_fPower)* (g_vLightSpecular * g_vMtrlSpecular);
	psOut.vSpecular = fSpecular;//vector(fSpecular,fSpecular,fSpecular,0.f);

	psOut.vColor1 = fSpecular;
	return psOut;
}

PixelShader_Output PixelShader_PointLight(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	

	vector vWorldNormal = tex2D(normalSampler,psIn.vTexUV);
	vWorldNormal = vector(vWorldNormal.xyz * 2.f - 1.f,0.f);

	vector vDepth = tex2D(depthSampler,psIn.vTexUV);
	//W * V * P //Making Proj's Pos before dividing W
	float fViewZ = vDepth.x * 1500.f;

	float fProjX = (psIn.vTexUV.x*2.f - 1.f) * fViewZ;
	float fProjY = (psIn.vTexUV.y*-2.f + 1.f) * fViewZ;
	float fProjZ = vDepth.y * fViewZ;
	vector vProjPos = vector(fProjX,fProjY,fProjZ,fViewZ);

	//vector vMyWorldPos = mul(vProjPos,g_matInvProjView);
	vector vMyWorldPos = mul(vProjPos,g_matInvProj);
	vMyWorldPos = mul(vMyWorldPos,g_matInvView);

	vector vLightDir = vector(vMyWorldPos.xyz,1.f) - g_vLightWorldPos;

	float fDistance = length(vLightDir);
	float fAtt = max((g_fRange - fDistance)/g_fRange,0.f);
	
	vector vShade = max(dot(normalize(vLightDir) * -1.f , vWorldNormal),0.f)*fAtt;
	psOut.vColor = (vShade + g_vLightAmbient * g_vMtrlAmbient) * g_vLightDiffuse * g_vMtrlDiffuse;

	vector vReflect = reflect(normalize(vLightDir),normalize(vWorldNormal));
	vector vLook = vMyWorldPos - g_vCamWorldPos;
	
	psOut.vSpecular  = pow(max(dot(normalize(vLook) * -1.f,normalize(vReflect)),0.f),g_fPower) * g_vLightSpecular * g_vMtrlSpecular * fAtt;

	//psOut.vSpecular  = pow(max(dot(normalize(vLook) * -1.f,normalize(vReflect)),0.f),g_fPower) * vector(1.f,0.f,0.f,1.f) * g_vMtrlSpecular * fAtt;
	
	psOut.vColor1 = psOut.vSpecular;
	
	return psOut;
}

technique Default_Device
{
	pass Default_DrawShade
	{
		AlphaBlendEnable = true;
		srcblend = one;
		destblend = one;
		//cullmode = none;
		//fillmode = wireframe;
		zwriteenable = false;
		zenable = false;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PixelShader_DrawShade();
	}
	pass Default_PointLight
	{
		AlphaBlendEnable = true;
		srcblend = one;
		destblend = one;
		//cullmode = none;
		//fillmode = wireframe;
		zwriteenable = false;
		zenable = false;

	
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PixelShader_PointLight();
	}
}