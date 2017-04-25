#include "Renderer.h"
#include "GameObject.h"
#include "Shader.h"
#include "Rect_Texture.h"
#include "Target_Manager.h"
#include "Target.h"
#include "LightMgr.h"

using namespace Engine;

Engine::CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphicDev)
:m_pGraphicDev(pGraphicDev)
,m_pShaderForShadow(NULL)
,m_pPlayerWorldMat(NULL)
,m_pShaderForShade(NULL)
,m_pShaderForBlend(NULL)
,m_isBlurOn(false)
,m_isDistortionOn(false)
,m_isGaussianBlurOn(false)
,m_pBlurEffect(NULL)
{
	m_pGraphicDev->AddRef();

}

Engine::CRenderer::~CRenderer(void)
{

}

HRESULT Engine::CRenderer::Ready_Renderer(void)
{
	m_pShaderForShadow = CShader::Create(m_pGraphicDev,L"../../Reference/Headers/Shader_Shadow.hpp");
	m_pShaderForShade = CShader::Create(m_pGraphicDev,L"../../Reference/Headers/Shader_Shade.hpp");
	m_pShaderForBlend = CShader::Create(m_pGraphicDev,L"../../Reference/Headers/Shader_Blend.hpp");

	m_fLightDist = 120.f;

	D3DVIEWPORT9 tViewPort;
	m_pGraphicDev->GetViewport(&tViewPort);
	D3DXMatrixPerspectiveFovLH(&m_matLightProj,(D3DX_PI)/8.f,(_float)tViewPort.Width/tViewPort.Height,1.f,1000.f);

	CTarget_Manager::GetInstance()->Ready_Target(m_pGraphicDev,L"Target_Shadow",tViewPort.Width,tViewPort.Height,D3DFMT_A8R8G8B8,D3DXCOLOR(0.f,0.f,0.f,0.f));
	CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Shadow",L"Target_Shadow");
	CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Shadow",0.f,150.f,200.f,150.f);

	CTarget_Manager::GetInstance()->Ready_Target(m_pGraphicDev,L"Target_Distortion",tViewPort.Width,tViewPort.Height,D3DFMT_A8R8G8B8,D3DXCOLOR(0.f,0.f,0.f,1.f));
	CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Distortion",L"Target_Distortion");
	CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Distortion",0.f,300.f,200.f,150.f);

	CTarget_Manager::GetInstance()->Ready_Target(m_pGraphicDev,L"Target_Albedo",tViewPort.Width,tViewPort.Height,D3DFMT_A8R8G8B8,D3DXCOLOR(0.f,0.f,0.f,0.f));
	CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred",L"Target_Albedo");
	CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Albedo",200.f,0.f,200.f,150.f);

	CTarget_Manager::GetInstance()->Ready_Target(m_pGraphicDev,L"Target_Normal",tViewPort.Width,tViewPort.Height,D3DFMT_A32B32G32R32F,D3DXCOLOR(0.f,0.f,0.f,1.f));
	CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred",L"Target_Normal");
	CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Normal",200.f,150.f,200.f,150.f);

	CTarget_Manager::GetInstance()->Ready_Target(m_pGraphicDev,L"Target_Depth",tViewPort.Width,tViewPort.Height,D3DFMT_A32B32G32R32F,D3DXCOLOR(1.f,1.f,1.f,1.f));
	CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred",L"Target_Depth");
	CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Depth",200.f,300.f,200.f,150.f);

	CTarget_Manager::GetInstance()->Ready_Target(m_pGraphicDev,L"Target_Shade",tViewPort.Width,tViewPort.Height,D3DFMT_A32B32G32R32F,D3DXCOLOR(0.f,0.f,0.f,1.f));
	CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Light",L"Target_Shade");
	CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Shade",200.f,450.f,200.f,150.f);

	CTarget_Manager::GetInstance()->Ready_Target(m_pGraphicDev,L"Target_Specular",tViewPort.Width,tViewPort.Height,D3DFMT_A32B32G32R32F,D3DXCOLOR(0.f,0.f,0.f,0.f));
	CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Light",L"Target_Specular");
	CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Specular",400.f,0.f,200.f,150.f);

	CTarget_Manager::GetInstance()->Ready_Target(m_pGraphicDev,L"Target_Copy",tViewPort.Width,tViewPort.Height,D3DFMT_A8R8G8B8,D3DXCOLOR(0.f,0.f,0.f,0.f));
	CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Copy",L"Target_Copy");
	CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Copy",0.f,0.f,200.f,150.f);

	m_pGraphicDev->CreateVertexBuffer(sizeof(Engine::BRUSH)*4,0,Engine::D3DFVF_BRUSH,D3DPOOL_MANAGED,&m_pVB,NULL);

	Engine::BRUSH* pBrush= NULL;
	m_pVB->Lock(0,0,(void**)&pBrush,0);

	pBrush[0].vPos = _vec4(0.f-0.5f,0.f-0.5f,0.f,1.f);
	pBrush[0].vTexUV = _vec2(0.f,0.f);

	pBrush[1].vPos = _vec4((_float)tViewPort.Width-0.5f,0.f-0.5f,0.f,1.f);
	pBrush[1].vTexUV = _vec2(1.f,0.f);

	pBrush[2].vPos = _vec4((_float)tViewPort.Width-0.5f,(_float)tViewPort.Height-0.5f,0.f,1.f);
	pBrush[2].vTexUV = _vec2(1.f,1.f);

	pBrush[3].vPos = _vec4(0.f-0.5f,(_float)tViewPort.Height-0.5f,0.f,1.f);
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

	//CTarget_Manager::GetInstance()->Find_Target(L"Target_Copy")->SetUp_OnGraphicDev(1);

	return S_OK;
}


void Engine::CRenderer::Add_RenderObject(CGameObject* pGameObject, RENDERTYPE eType)
{
	pGameObject->AddRefCnt();
	m_RenderList[eType].push_back(pGameObject);
}

void Engine::CRenderer::Render_Object()
{
	//m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 1.f, 0);

	if(m_pPlayerWorldMat != NULL)
	{
		_vec3 vLightPos = *(_vec3*)&m_pPlayerWorldMat->m[3][0] + _vec3(0.f,m_fLightDist,70.f);
		D3DXMatrixLookAtLH(&m_matLightView,&vLightPos,(_vec3*)&m_pPlayerWorldMat->m[3][0],&_vec3(1.f,0.f,0.f));
	}
	
	/*if(GetAsyncKeyState('1'))
	{
		m_fLightDist += 0.5f;
		_vec3 vLightPos = *(_vec3*)&m_pPlayerWorldMat->m[3][0] + _vec3(0.f,m_fLightDist,50.f);
		D3DXMatrixLookAtLH(&m_matLightView,&vLightPos,(_vec3*)&m_pPlayerWorldMat->m[3][0],&_vec3(1.f,0.f,0.f));
	}
	if(GetAsyncKeyState('2'))
	{
		m_fLightDist -= 0.5f;
		_vec3 vLightPos = *(_vec3*)&m_pPlayerWorldMat->m[3][0] + _vec3(0.f,m_fLightDist,50.f);
		D3DXMatrixLookAtLH(&m_matLightView,&vLightPos,(_vec3*)&m_pPlayerWorldMat->m[3][0],&_vec3(1.f,0.f,0.f));
	}*/

	Render_Shadow();

	Render_Priority();
		
	Render_Deferred();
	Render_Shade();
	
	Render_Blend();

	if(m_isBlurOn == true)
		Render_WithRadialBlur(m_pBlurEffect,m_iBlurPassIdx,m_iMaxBlurCnt);
	if(m_isDistortionOn == true)
		Render_WithDistortion(m_pDistortionEffect,m_iDistortionPassIdx,NULL,NULL);
	if(m_isGaussianBlurOn == true)
		Render_WithGaussianBlur(m_pBlurEffect,m_fCoeff,m_iMaxGaussianBlurCnt);
	
	//CTarget_Manager::GetInstance()->Find_Target(L"Target_Copy")->SetUp_OnGraphicDev(1);
	Render_Alpha();

	if(m_isFadeOut == true)
		Render_FadeOut(m_fFadeCoeff);
	if(m_isFadeIn == true)
		Render_FadeOut(m_fFadeCoeff);
	if(m_isRedScreen == true)
		Render_RedScreen(m_fFadeCoeff);
	if(m_isWhiteScreen == true)
		Render_WhiteScreen(m_fFadeCoeff);


	m_pGraphicDev->SetRenderTarget(1,NULL);

	Render_UI();

	

	if(GetAsyncKeyState(VK_TAB))
	{
		CTarget_Manager::GetInstance()->Render_DebugBuffer(L"Target_Copy");
		CTarget_Manager::GetInstance()->Render_DebugBuffer(L"Target_Shadow");
		CTarget_Manager::GetInstance()->Render_DebugBuffer(L"Target_Distortion");
		CTarget_Manager::GetInstance()->Render_DebugBuffer(L"Target_Albedo");
		CTarget_Manager::GetInstance()->Render_DebugBuffer(L"Target_Normal");
		CTarget_Manager::GetInstance()->Render_DebugBuffer(L"Target_Shade");
		CTarget_Manager::GetInstance()->Render_DebugBuffer(L"Target_Depth");
		CTarget_Manager::GetInstance()->Render_DebugBuffer(L"Target_Specular");
	}

	for(_ulong i = 0; i<RT_END ; ++i)
	{
		for_each(m_RenderList[i].begin(), m_RenderList[i].end(), CRelease_Single());
		m_RenderList[i].clear();
	}
}

void Engine::CRenderer::Render_Priority()
{
	m_pGraphicDev->SetRenderTarget(1,CTarget_Manager::GetInstance()->Find_Target(L"Target_Copy")->GetTargetSurface());

	if(m_RenderList[RT_PRIORITY].size() == 0)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE,FALSE);
	list<CGameObject*>::iterator iter = m_RenderList[RT_PRIORITY].begin();
	list<CGameObject*>::iterator iter_end = m_RenderList[RT_PRIORITY].end();

	for( ; iter != iter_end ; ++iter)
	{
		(*iter)->Render_GameObject();
	}
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE,TRUE);

	m_pGraphicDev->SetRenderTarget(1,NULL);
}

void Engine::CRenderer::Render_Shadow()
{
	//New
	CTarget_Manager::GetInstance()->Begin_MRT(L"MRT_Shadow");

	list<CGameObject*>::iterator iter = m_RenderList[RT_SHADOW].begin();
	list<CGameObject*>::iterator iter_end = m_RenderList[RT_SHADOW].end();
	for( ; iter != iter_end ; ++iter)
	{
		(*iter)->Render_ShadowDepth();
	}

	CTarget_Manager::GetInstance()->End_MRT(L"MRT_Shadow");

	return;
	//Pre

	/*if(m_RenderList[RT_SHADOW].size() == 0)
		return;

	LPDIRECT3DSURFACE9 pBackBuffer = NULL;
	m_pGraphicDev->GetRenderTarget(0,&pBackBuffer);

	m_pGraphicDev->SetRenderTarget(0,m_pSurface[FOR_SHADOW]);
	m_pGraphicDev->Clear(0,NULL,D3DCLEAR_TARGET,D3DXCOLOR(0.f,0.f,0.f,1.f),1.f,0);

	list<CGameObject*>::iterator iter = m_RenderList[RT_SHADOW].begin();
	list<CGameObject*>::iterator iter_end = m_RenderList[RT_SHADOW].end();
	for( ; iter != iter_end ; ++iter)
	{
		(*iter)->Render_ShadowDepth();
	}

	m_pGraphicDev->SetRenderTarget(0,pBackBuffer);

	return;*/

}


bool Compare(CGameObject* pSrc,CGameObject* pDest)
{
	return pSrc->GetViewZ() > pDest->GetViewZ();
}

void Engine::CRenderer::Render_Alpha()
{
	if(m_RenderList[RT_ALPHA].size() == 0)
		return;

	m_RenderList[RT_ALPHA].sort(Compare);
	list<CGameObject*>::iterator iter = m_RenderList[RT_ALPHA].begin();
	list<CGameObject*>::iterator iter_end = m_RenderList[RT_ALPHA].end();

	for( ; iter != iter_end ; ++iter)
	{
		(*iter)->Render_GameObject();
	}
}
void Engine::CRenderer::Render_UI()
{
	if(m_RenderList[RT_UI].size() == 0)
		return;

	list<CGameObject*>::iterator iter = m_RenderList[RT_UI].begin();
	list<CGameObject*>::iterator iter_end = m_RenderList[RT_UI].end();

	for( ; iter != iter_end ; ++iter)
	{
		(*iter)->Render_GameObject();
	}
}


Engine::CRenderer* Engine::CRenderer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRenderer* pRenderer = new CRenderer(pGraphicDev);
	pRenderer->Ready_Renderer();
	return pRenderer;
}

void Engine::CRenderer::Free(void)
{
	//m_pGraphicDev->Release();

	Engine::CComponent::Free();

	for(_int i=0; i<RT_END; ++i)
	{
		for_each(m_RenderList[i].begin(), m_RenderList[i].end(), CRelease_Single());
		m_RenderList[i].clear();
	}
	

	Engine::Safe_Release(m_pShaderForShadow);
	Engine::Safe_Release(m_pShaderForShade);
	Engine::Safe_Release(m_pShaderForBlend);
	
	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pIB);
	

	Engine::Safe_Release(m_pGraphicDev);
}

Engine::CComponent* Engine::CRenderer::Clone()
{
	AddRefCnt();
	return this;
}

void Engine::CRenderer::Render_WithDistortion(LPD3DXEFFECT pEffect, _uint iPassNum, LPDIRECT3DBASETEXTURE9 pDistortionImg, _float* fRotMat)
{
	//Distortion
	if(fRotMat != NULL)
		pEffect->SetFloatArray("g_matRot",fRotMat,4);

	
	//pEffect->SetTexture("g_baseTexture",m_pTexture[FOR_COPY]);
	CTarget_Manager::GetInstance()->SetUp_OnShader(pEffect,L"Target_Copy","g_baseTexture");
	CTarget_Manager::GetInstance()->SetUp_OnShader(pEffect,L"Target_Distortion","g_secondTexture");

	//pEffect->SetTexture("g_secondTexture",m_pTexture[FOR_DISTORTION]);

	pEffect->Begin(NULL,0);
	pEffect->BeginPass(iPassNum);

	/*m_pGraphicDev->SetRenderState(D3DRS_ZENABLE,FALSE);
	list<CGameObject*>::iterator iter = m_RenderList[RT_PRIORITY].begin();
	list<CGameObject*>::iterator iter_end = m_RenderList[RT_PRIORITY].end();

	for( ; iter != iter_end ; ++iter)
	{
		(*iter)->Render_GameObject();
	}
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE,TRUE);*/

	m_pGraphicDev->SetStreamSource(0,m_pVB,0,sizeof(Engine::BRUSH));
	m_pGraphicDev->SetFVF(Engine::D3DFVF_BRUSH);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

	pEffect->EndPass();
	pEffect->End();
}

void Engine::CRenderer::Render_WithDistortionNormalRectBuff(LPD3DXEFFECT pEffect, _uint iPassNum, LPDIRECT3DBASETEXTURE9 pDistortionImg, _float* fRotMat, CRect_Texture* pBuffer)
{
	//Distortion
	pEffect->SetFloatArray("g_matRot",fRotMat,4);

	//pEffect->SetTexture("g_baseTexture",m_pTexture[FOR_COPY]);
	pEffect->SetTexture("g_secondTexture",pDistortionImg);

	pEffect->Begin(NULL,0);
	pEffect->BeginPass(iPassNum);

	pBuffer->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();
}

void Engine::CRenderer::Render_WithRadialBlur(LPD3DXEFFECT pEffect, _uint iPassNum, _int iMaxBlurCnt)
{
	//Radial Blur
	tagBlur tRadialBlur;
	tRadialBlur.fCoeff = -0.05f;
	tRadialBlur.fTotalWeight = 0.f;
	for(_int i= 0 ; i< iMaxBlurCnt ; ++i)
	{
		tRadialBlur.fWeight[i] = exp(i*i*tRadialBlur.fCoeff);
		tRadialBlur.fTotalWeight += tRadialBlur.fWeight[i];
	}

	pEffect->SetInt("g_iMaxBlurCnt",iMaxBlurCnt);
	pEffect->SetFloat("g_TotalRadialWeight",tRadialBlur.fTotalWeight);
	pEffect->SetFloatArray("g_fRadialWeight",tRadialBlur.fWeight,iMaxBlurCnt);
	//pEffect->SetTexture("g_baseTexture",m_pTexture[FOR_COPY]);

	//m_pGraphicDev->Clear(0,0,D3DCLEAR_TARGET,D3DXCOLOR(0.f,0.f,0.f,1.f),1.f,0);

	CTarget_Manager::GetInstance()->SetUp_OnShader(pEffect,L"Target_Copy","g_baseTexture");
	
	pEffect->Begin(NULL,0);
	pEffect->BeginPass(iPassNum);

	m_pGraphicDev->SetStreamSource(0,m_pVB,0,sizeof(Engine::BRUSH));
	m_pGraphicDev->SetFVF(Engine::D3DFVF_BRUSH);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

	pEffect->EndPass();
	pEffect->End();
}

void Engine::CRenderer::Render_WithGaussianBlur(LPD3DXEFFECT pEffect, _float fCoeff, _uint iMaxCnt /*= 6*/)
{
	D3DVIEWPORT9 tViewPort;
	m_pGraphicDev->GetViewport(&tViewPort);
	Engine::BLURDATA tBlur;
	tBlur.fWidth = (_float)tViewPort.Width;
	tBlur.fHeight = (_float)tViewPort.Height;
	tBlur.fCoeff = fCoeff;
	tBlur.iMaxCnt = iMaxCnt;
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

	pEffect->SetFloat("g_fWidth",1.f / tBlur.fWidth);
	pEffect->SetFloat("g_fHeight",1.f / tBlur.fHeight);
	pEffect->SetFloat("g_fTotalWeight",tBlur.fTotalWeight);
	pEffect->SetFloat("g_fCoeff",tBlur.fCoeff);
	pEffect->SetFloatArray("g_fWeight",tBlur.fWeight,tBlur.iMaxCnt);

	//pEffect->SetTexture("g_baseTexture",m_pTexture[FOR_COPY]);

	//m_pGraphicDev->Clear(0,0,D3DCLEAR_TARGET,D3DXCOLOR(0.f,0.f,0.f,1.f),1.f,0);

	CTarget_Manager::GetInstance()->SetUp_OnShader(pEffect,L"Target_Copy","g_baseTexture");

	pEffect->Begin(NULL,0);
	pEffect->BeginPass(2);

	m_pGraphicDev->SetStreamSource(0,m_pVB,0,sizeof(Engine::BRUSH));
	m_pGraphicDev->SetFVF(Engine::D3DFVF_BRUSH);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

	pEffect->EndPass();
	pEffect->End();
}

void Engine::CRenderer::Render_Deferred()
{
	if(m_RenderList[RT_NONALPHA].size() == 0)
		return;

	CTarget_Manager::GetInstance()->Begin_MRT(L"MRT_Deferred");
	list<CGameObject*>::iterator iter = m_RenderList[RT_NONALPHA].begin();
	list<CGameObject*>::iterator iter_end = m_RenderList[RT_NONALPHA].end();

	for( ; iter != iter_end ; ++iter)
	{
		(*iter)->Render_GameObject();
	}

	CTarget_Manager::GetInstance()->End_MRT(L"MRT_Deferred");
}

void Engine::CRenderer::Render_Shade()
{
	CTarget_Manager::GetInstance()->Begin_MRT(L"MRT_Light");

	_matrix matView, matProj , matInvPV;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION,&matProj);
	D3DXMatrixInverse(&matView,0,&matView);
	D3DXMatrixInverse(&matProj,0,&matProj);
	matInvPV = matProj * matView;

	m_pShaderForShade->GetEffect()->SetMatrix("g_matInvProjView",&matInvPV);
	m_pShaderForShade->GetEffect()->SetMatrix("g_matInvView",&matView);
	m_pShaderForShade->GetEffect()->SetMatrix("g_matInvProj",&matProj);
	m_pShaderForShade->GetEffect()->SetVector("g_vCamWorldPos",&_vec4(*(_vec3*)&matView.m[3][0],1.f));
	
	CTarget_Manager::GetInstance()->SetUp_OnShader(m_pShaderForShade->GetEffect(),L"Target_Normal","g_normalTexture");
	CTarget_Manager::GetInstance()->SetUp_OnShader(m_pShaderForShade->GetEffect(),L"Target_Depth","g_depthTexture");
	
	m_pShaderForShade->GetEffect()->Begin(NULL,0);
	//m_pShaderForShade->GetEffect()->BeginPass(0);

	CLightMgr::GetInstance()->Render_Light(m_pShaderForShade->GetEffect());

	//m_pShaderForShade->GetEffect()->EndPass();
	m_pShaderForShade->GetEffect()->End();	

	CTarget_Manager::GetInstance()->End_MRT(L"MRT_Light");
}

void Engine::CRenderer::Render_Blend()
{
	//CTarget_Manager::GetInstance()->Find_Target(L"Target_Copy")->SetUp_OnGraphicDev(1);

	m_pGraphicDev->SetRenderTarget(1,CTarget_Manager::GetInstance()->Find_Target(L"Target_Copy")->GetTargetSurface());

	CTarget_Manager::GetInstance()->SetUp_OnShader(m_pShaderForBlend->GetEffect(), L"Target_Albedo","g_albedoTexture");
	CTarget_Manager::GetInstance()->SetUp_OnShader(m_pShaderForBlend->GetEffect(), L"Target_Shade","g_shadeTexture");
	CTarget_Manager::GetInstance()->SetUp_OnShader(m_pShaderForBlend->GetEffect(), L"Target_Specular","g_specularTexture");

	m_pShaderForBlend->GetEffect()->Begin(NULL,0);
	m_pShaderForBlend->GetEffect()->BeginPass(0);

	m_pGraphicDev->SetStreamSource(0,m_pVB,0,sizeof(Engine::BRUSH));
	m_pGraphicDev->SetFVF(Engine::D3DFVF_BRUSH);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

	m_pShaderForBlend->GetEffect()->EndPass();
	m_pShaderForBlend->GetEffect()->End();

	//CTarget_Manager::GetInstance()->Find_Target(L"Target_Copy")->Release_OnGraphicDev(1);

}

void Engine::CRenderer::SetBlurOn(LPD3DXEFFECT pEffect, _uint iPassNum, _int iMaxBlurCnt /*= 6*/)
{
	m_isBlurOn = true;
	m_pBlurEffect = pEffect;
	m_iBlurPassIdx = iPassNum;
	m_iMaxBlurCnt = iMaxBlurCnt;
}

void Engine::CRenderer::SetDistortionOn(LPD3DXEFFECT pEffect, _uint iPassNum)
{
	m_isDistortionOn = true;
	m_pDistortionEffect = pEffect;
	m_iDistortionPassIdx = iPassNum;
}

void Engine::CRenderer::SetGaussianBlurOn(LPD3DXEFFECT pEffect, _float fCoeff,_uint iGaussianBlurCnt /*= -0.05f*/)
{
	m_isGaussianBlurOn = true;
	m_pBlurEffect = pEffect;
	m_fCoeff = fCoeff;
	m_iMaxGaussianBlurCnt=iGaussianBlurCnt;
}

void Engine::CRenderer::Render_FadeOut(_float fFadeCoeff)
{
	if(m_pBlurEffect == NULL)
		return;

	CTarget_Manager::GetInstance()->SetUp_OnShader(m_pBlurEffect,L"Target_Copy","g_baseTexture");
	m_pBlurEffect->SetFloat("g_fFade",m_fFadeCoeff);

	m_pBlurEffect->Begin(NULL,0);
	m_pBlurEffect->BeginPass(4);

	m_pGraphicDev->SetStreamSource(0,m_pVB,0,sizeof(Engine::BRUSH));
	m_pGraphicDev->SetFVF(Engine::D3DFVF_BRUSH);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

	m_pBlurEffect->EndPass();
	m_pBlurEffect->End();
}

void Engine::CRenderer::SetFadeOut(_float fFade)
{
	m_isFadeOut = true;
	m_fFadeCoeff = fFade;
}

void Engine::CRenderer::SetRedScreen(_float fFade)
{
	m_isRedScreen = true;
	m_fFadeCoeff = fFade;
}

void Engine::CRenderer::Render_RedScreen(_float fFadeCoeff)
{
	if(m_pBlurEffect == NULL)
		return;

	CTarget_Manager::GetInstance()->SetUp_OnShader(m_pBlurEffect,L"Target_Copy","g_baseTexture");
	m_pBlurEffect->SetFloat("g_fFade",m_fFadeCoeff);

	m_pBlurEffect->Begin(NULL,0);
	m_pBlurEffect->BeginPass(5);

	m_pGraphicDev->SetStreamSource(0,m_pVB,0,sizeof(Engine::BRUSH));
	m_pGraphicDev->SetFVF(Engine::D3DFVF_BRUSH);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

	m_pBlurEffect->EndPass();
	m_pBlurEffect->End();
}

void Engine::CRenderer::SetFadeIn(_float fFade)
{
	m_isFadeIn = true;
	m_fFadeCoeff = fFade;
}

void Engine::CRenderer::Render_WhiteScreen(_float fFadeCoeff)
{
	if(m_pBlurEffect == NULL)
		return;

	CTarget_Manager::GetInstance()->SetUp_OnShader(m_pBlurEffect,L"Target_Copy","g_baseTexture");
	m_pBlurEffect->SetFloat("g_fFade",m_fFadeCoeff);

	m_pBlurEffect->Begin(NULL,0);
	m_pBlurEffect->BeginPass(6);

	m_pGraphicDev->SetStreamSource(0,m_pVB,0,sizeof(Engine::BRUSH));
	m_pGraphicDev->SetFVF(Engine::D3DFVF_BRUSH);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

	m_pBlurEffect->EndPass();
	m_pBlurEffect->End();
}

void Engine::CRenderer::SetWhiteScreen(_float fWhite)
{
	m_isWhiteScreen = true;
	m_fFadeCoeff = fWhite;
}

