texture g_baseTexture;
sampler baseSampler = sampler_state
{
	texture = g_baseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	/*addressu = clamp;
	addressv = clamp;*/
};

texture g_shadowTexture;
sampler shadowSampler = sampler_state
{
	texture = g_shadowTexture;
	/*minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	addressu = clamp;
	addressv = clamp;*/
};

struct PixelShader_Input
{
	float2 vTexUV : TEXCOORD0;
};
struct PixelShader_Output
{
	vector vColor : COLOR0;
};
PixelShader_Output PixelShader_Shadow(PixelShader_Input psIn)
{
	PixelShader_Output psOut = (PixelShader_Output)0.f;

	psOut.vColor = tex2D(baseSampler,psIn.vTexUV);//tex2D(shadowSampler,psIn.vTexUV);

	return psOut;
}

technique Default_Device
{
	pass Default_Shadow
	{
		lighting = false;
		cullmode = none;
		//fillmode = wireframe;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PixelShader_Shadow();
	}
}