Matrix g_matWorld;
Matrix g_matView;
Matrix g_matProj;

texture g_BaseTexture;
float g_OffSet;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	//addressv = mirror;
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
	vector vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
};

VertexShader_Output VertexShader_Main_Down(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut;

	Matrix matWV,matWVP;
	matWV = mul(g_matWorld,g_matView);
	matWVP = mul(matWV,g_matProj);

	vsOut.vPos = mul(vsIn.vPos,matWVP);
	vsOut.vNormal = vsIn.vNormal;
	
	vsIn.vTexUV.y -= g_OffSet;
	vsOut.vTexUV = vsIn.vTexUV;	

	return vsOut;
}
//
//VertexShader_Output VertexShader_Main_LDown(VertexShader_Input vsIn)
//{
//	VertexShader_Output vsOut;
//
//	Matrix matWV,matWVP;
//	matWV = mul(g_matWorld,g_matView);
//	matWVP = mul(matWV,g_matProj);
//
//	vsOut.vPos = mul(vsIn.vPos,matWVP);
//	vsOut.vNormal = vsIn.vNormal;
//
//	float fNewY = vsIn.vTexUV.y;
//	float fNewX = vsIn.vTexUV.x;
//	fNewY -= g_OffSet;
//	vsOut.vTexUV = float2(fNewX,fNewY);
//
//	//y - vsIn.vTexUV.y = -1.f * (x - vsIn.vTexUV.x)
//
//	return vsOut;
//}

VertexShader_Output VertexShader_Main_LDown(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut;

	Matrix matWV,matWVP;
	matWV = mul(g_matWorld,g_matView);
	matWVP = mul(matWV,g_matProj);

	vsOut.vPos = mul(vsIn.vPos,matWVP);
	vsOut.vNormal = vsIn.vNormal;

	vsOut.vTexUV = vsIn.vTexUV - g_OffSet;	


	return vsOut;
}

VertexShader_Output VertexShader_Main_RDown(VertexShader_Input vsIn)
{
	VertexShader_Output vsOut;

	Matrix matWV,matWVP;
	matWV = mul(g_matWorld,g_matView);
	matWVP = mul(matWV,g_matProj);

	vsOut.vPos = mul(vsIn.vPos,matWVP);
	vsOut.vNormal = vsIn.vNormal;

	vsOut.vTexUV.x = vsIn.vTexUV.x + g_OffSet;	
	vsOut.vTexUV.y = vsIn.vTexUV.y - g_OffSet;	

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
	PixelShader_Output psOut;

	psOut.vColor = tex2D(BaseSampler,psIn.vTexUV);
	psOut.vColor1 = psOut.vColor;


	return psOut;
}

technique Default_Device
{
	pass DOWN
	{
		/*LPDIRECT3DDEVICE9 pDevice;
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,true);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF,0x00000088);
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);*/

		lighting = false;
		cullmode = none;
		
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;
		

		VertexShader = compile vs_3_0 VertexShader_Main_Down();
		PixelShader = compile ps_3_0 PixelShader_Main();
	}
	pass LDOWN
	{
		lighting = false;
		cullmode = none;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		VertexShader = compile vs_3_0 VertexShader_Main_LDown();
		PixelShader = compile ps_3_0 PixelShader_Main();
	}
	pass RDOWN
	{
		lighting = false;
		cullmode = none;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		VertexShader = compile vs_3_0 VertexShader_Main_RDown();
		PixelShader = compile ps_3_0 PixelShader_Main();
	}
};