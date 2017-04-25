texture g_albedoTexture;
sampler albedoSampler = sampler_state
{
	texture = g_albedoTexture;
};

texture g_shadeTexture;
sampler shadeSampler = sampler_state
{
	texture = g_shadeTexture;
};

texture g_specularTexture;
sampler specularSampler = sampler_state
{
	texture = g_specularTexture;
};

struct PixelShader_Input
{
	float2 vTexUV : TEXCOORD0;
};
struct PixelShader_Output
{
	vector vColor : COLOR0;
	vector vColor1 : COLOR1;
};
PixelShader_Output PixelShader_Blend(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	vector vShade = tex2D(shadeSampler,psIn.vTexUV);
	vector vTexColor = tex2D(albedoSampler,psIn.vTexUV);
	vector vSpecular = tex2D(specularSampler,psIn.vTexUV);

	psOut.vColor = vTexColor * vShade + vTexColor*vSpecular;
	psOut.vColor1 = psOut.vColor;

	return psOut;
}

technique Default_Device
{
	pass Default_Blend
	{
		lighting = false;
		cullmode = none;
		//fillmode = wireframe;
		zwriteenable = false;
		zenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PixelShader_Blend();
	}
}