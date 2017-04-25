#include "stdafx.h"
#include "LogoBack.h"
#include "Export_Engine.h"

CLogoBack::CLogoBack(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTransformCom(NULL)
,m_pShader(NULL)
,m_fUV(0.f)
,m_pTexture(NULL)
,m_pSurface(NULL)
,m_fTimeAcc(0.f)
{

}

CLogoBack::~CLogoBack()
{

}

HRESULT CLogoBack::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);
		
	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);
	
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_RectTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_RectTexture",m_pBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_LOGO,L"Component_Texture_Logo");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Logo",m_pTextureCom);

	m_pTextureDistortion = (Engine::CTexture*)Engine::Clone(COM_STATIC,L"Component_Texture_efx_ripple1_normal");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_efx_ripple1_normal",m_pTextureCom);

	m_pShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Default");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Default",m_pShader);

	m_pShaderForRenderTarget = Engine::CShader::Create(m_pGraphicDev,L"../Codes/Shader_RenderTarget.hpp");

	
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(2.f,2.f,2.f));

	m_pGraphicDev->CreateVertexBuffer(sizeof(Engine::BRUSH)*4,0,Engine::D3DFVF_BRUSH,D3DPOOL_MANAGED,&m_pVB,NULL);

	Engine::BRUSH* pBrush= NULL;
	m_pVB->Lock(0,0,(void**)&pBrush,0);

	pBrush[0].vPos = _vec4(0.f,0.f,0.f,1.f);
	pBrush[0].vTexUV = _vec2(0.f,0.f);

	pBrush[1].vPos = _vec4((_float)WINCX,0.f,0.f,1.f);
	pBrush[1].vTexUV = _vec2(1.f,0.f);

	pBrush[2].vPos = _vec4((_float)WINCX,(_float)WINCY,0.f,1.f);
	pBrush[2].vTexUV = _vec2(1.f,1.f);

	pBrush[3].vPos = _vec4(0.f,(_float)WINCY,0.f,1.f);
	pBrush[3].vTexUV = _vec2(0.f,1.f);

	m_pVB->Unlock();

	m_pGraphicDev->CreateIndexBuffer(sizeof(Engine::INDEX16)*2,0,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pIB,NULL);

	Engine::INDEX16* pIndex = NULL;

	m_pIB->Lock(0,0,(void**)&pIndex,0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	D3DXCreateTexture(m_pGraphicDev,WINCX,WINCY,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&m_pTexture);
	m_pTexture->GetSurfaceLevel(0,&m_pSurface);

	return S_OK;
}

_int CLogoBack::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderObject(this, Engine::CRenderer::RT_PRIORITY);
	return 0;
}

void CLogoBack::Render_GameObject(void)
{
	m_pShader->GetEffect()->SetMatrix("g_matWorld",m_pTransformCom->GetWorldMatrix());
	_matrix matView,matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION,&matProj);
	m_pShader->GetEffect()->SetMatrix("g_matView",&matView);
	m_pShader->GetEffect()->SetMatrix("g_matProj",&matProj);
	
	m_fUV += 0.01f;
	m_pShader->GetEffect()->SetFloat("g_fValueForUV",m_fUV);
	m_pShader->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(0));
	m_pShader->GetEffect()->Begin(NULL,0);
	m_pShader->GetEffect()->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	m_pShader->GetEffect()->EndPass();
	m_pShader->GetEffect()->End();
	
	//m_pGraphicDev->Clear(0,0,D3DCLEAR_TARGET,D3DXCOLOR(0.5f,0.5f,0.f,1.f),1.f,0);


	//return;
	//Render_Blur();

	return; //////////////////////////////////////////////return
	//Surface Change
	m_pGraphicDev->SetRenderTarget(1,m_pSurface);
	m_pShader->GetEffect()->Begin(NULL,0);
	m_pShader->GetEffect()->BeginPass(5);

	m_pBufferCom->Render_Buffer();

	m_pShader->GetEffect()->EndPass();
	m_pShader->GetEffect()->End();
	
	return; /////////////////////////////////////////////return
	//////////////////////////////////

	m_fRotMat[0] = cosf(m_fTimeAcc);
	m_fRotMat[1] = -sinf(m_fTimeAcc);
	m_fRotMat[2] = sinf(m_fTimeAcc);
	m_fRotMat[3] = cosf(m_fTimeAcc);
	m_pShaderForRenderTarget->GetEffect()->SetFloatArray("g_matRot",m_fRotMat,4);

	m_pShaderForRenderTarget->GetEffect()->SetTexture("g_baseTexture",m_pTexture);
	m_pShaderForRenderTarget->GetEffect()->SetTexture("g_secondTexture",m_pTextureDistortion->GetTexture());

	m_pShaderForRenderTarget->GetEffect()->Begin(NULL,0);
	m_pShaderForRenderTarget->GetEffect()->BeginPass(0);

	m_pGraphicDev->SetStreamSource(0,m_pVB,0,sizeof(Engine::BRUSH));
	m_pGraphicDev->SetFVF(Engine::D3DFVF_BRUSH);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

	m_pShaderForRenderTarget->GetEffect()->EndPass();
	m_pShaderForRenderTarget->GetEffect()->End();

}

CLogoBack* CLogoBack::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogoBack* pInst = new CLogoBack(pGraphicDev);
	
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CLogoBack::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShader);
	Engine::Safe_Release(m_pShaderForRenderTarget);
	Engine::Safe_Release(m_pTextureDistortion);


	Engine::Safe_Release(m_pTexture);
	Engine::Safe_Release(m_pSurface);
	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pIB);
}

void CLogoBack::Render_Blur(void)
{
	m_pShaderForRenderTarget->GetEffect()->SetMatrix("g_matWorld",m_pTransformCom->GetWorldMatrix());
	_matrix matView,matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION,&matProj);
	m_pShaderForRenderTarget->GetEffect()->SetMatrix("g_matView",&matView);
	m_pShaderForRenderTarget->GetEffect()->SetMatrix("g_matProj",&matProj);

	m_pShaderForRenderTarget->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(0));

	//For Blur
	D3DVIEWPORT9 tViewPort;
	m_pGraphicDev->GetViewport(&tViewPort);
	Engine::BLURDATA tBlur;
	tBlur.fWidth = (_float)tViewPort.Width;
	tBlur.fHeight = (_float)tViewPort.Height;
	tBlur.fCoeff = -0.05f;
	tBlur.iMaxCnt = 13;
	tBlur.fTotalWeight = 0.f;
 
	for(_int i= 0 ; i< tBlur.iMaxCnt/2 + 1 ; ++i)
	{
		tBlur.fWeight[tBlur.iMaxCnt/2 + i] = exp(i * i * tBlur.fCoeff);
		tBlur.fWeight[tBlur.iMaxCnt/2 - i] = exp(i * i * tBlur.fCoeff);
	}

	for(_int i= 0 ; i< tBlur.iMaxCnt ; ++i)
	{
		tBlur.fTotalWeight += tBlur.fWeight[i];
	}


	//----------------Radial Blur-------------------
	Engine::tagBlur tRadialBlur;
	tRadialBlur.fCoeff = -0.05f;
	tRadialBlur.fTotalWeight = 0.f;
	for(_int i= 0 ; i< 6 ; ++i)
	{
		tRadialBlur.fWeight[i] = exp(i*i*tRadialBlur.fCoeff);
		tRadialBlur.fTotalWeight += tRadialBlur.fWeight[i];
	}

	m_pShaderForRenderTarget->GetEffect()->SetFloat("g_TotalRadialWeight",tRadialBlur.fTotalWeight);
	m_pShaderForRenderTarget->GetEffect()->SetFloatArray("g_fRadialWeight",tRadialBlur.fWeight,6);

	//////////////////////////////////////////////////
	m_pShaderForRenderTarget->GetEffect()->SetFloat("g_fWidth",1.f / tBlur.fWidth);
	m_pShaderForRenderTarget->GetEffect()->SetFloat("g_fHeight",1.f / tBlur.fHeight);
	m_pShaderForRenderTarget->GetEffect()->SetFloat("g_fTotalWeight",tBlur.fTotalWeight);
	m_pShaderForRenderTarget->GetEffect()->SetFloat("g_fCoeff",tBlur.fCoeff);
	m_pShaderForRenderTarget->GetEffect()->SetFloatArray("g_fWeight",tBlur.fWeight,tBlur.iMaxCnt);

	m_pShaderForRenderTarget->GetEffect()->Begin(NULL,0);
	m_pShaderForRenderTarget->GetEffect()->BeginPass(3);

	m_pBufferCom->Render_Buffer();

	m_pShaderForRenderTarget->GetEffect()->EndPass();
	m_pShaderForRenderTarget->GetEffect()->End();



	//////////////////
}

