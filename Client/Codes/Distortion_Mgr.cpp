#include "stdafx.h"
#include "Distortion_Mgr.h"
#include "Distortion.h"
#include "Export_Engine.h"

CDistortion_Mgr::CDistortion_Mgr(LPDIRECT3DDEVICE9 pGraphicDev,_float fDeadTime, _float fDistortionPow)
:Engine::CGameObject(pGraphicDev)
,m_pRendererCom(NULL)
,m_pDistortionShader(NULL)
,m_pDistortionTex(NULL)
,m_pSurface(NULL)
,m_fTimeAcc(0.f)
,m_fDeadTime(fDeadTime)
,m_fDistortionPow(fDistortionPow)
{
}

CDistortion_Mgr::~CDistortion_Mgr(void)
{

}

HRESULT CDistortion_Mgr::Ready_GameObject()
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pDistortionShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_RenderTarget");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_RenderTarget",m_pDistortionShader);

	D3DVIEWPORT9 tViewPort;
	m_pGraphicDev->GetViewport(&tViewPort);
	D3DXCreateTexture(m_pGraphicDev,tViewPort.Width,tViewPort.Height,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&m_pDistortionTex);
	m_pDistortionTex->GetSurfaceLevel(0,&m_pSurface);

	return S_OK;
}

void CDistortion_Mgr::AddDistortion(CDistortion* pDistortion)
{
	m_pDistortionList.push_back(pDistortion);
}

_int CDistortion_Mgr::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if(m_fTimeAcc > m_fDeadTime)
	{
		m_pRendererCom->SetisDistortion(false);
		m_isDestroy = true;
	}

	list<CDistortion*>::iterator iter = m_pDistortionList.begin();
	list<CDistortion*>::iterator iter_end = m_pDistortionList.end();

	for(; iter!=iter_end ; ++iter)
	{
		(*iter)->Update_GameObject(fTimeDelta);
	}

	m_pRendererCom->Add_RenderObject(this, Engine::CRenderer::RT_ALPHA);
	return 0;
}

void CDistortion_Mgr::Render_GameObject()
{
	/*LPDIRECT3DSURFACE9 pBackBuffer, pCopyBuffer;
	m_pGraphicDev->GetRenderTarget(0,&pBackBuffer);
	m_pGraphicDev->GetRenderTarget(1,&pCopyBuffer);

	m_pGraphicDev->SetRenderTarget(0,m_pRendererCom->GetTargetSurface(Engine::CRenderer::FOR_DISTORTION));
	m_pGraphicDev->SetRenderTarget(1,NULL);

	m_pGraphicDev->Clear(0,0,D3DCLEAR_TARGET,D3DXCOLOR(0.f,0.f,0.f,0.f),1.f,0);*/

	Engine::Begin_MRT(L"MRT_Distortion");

	list<CDistortion*>::iterator iter = m_pDistortionList.begin();
	list<CDistortion*>::iterator iter_end = m_pDistortionList.end();

	for(; iter!=iter_end ; ++iter)
	{
		(*iter)->Render_GameObject();
	}

	Engine::End_MRT(L"MRT_Distortion");

	//m_pGraphicDev->SetRenderTarget(0,pBackBuffer);
	//m_pGraphicDev->SetRenderTarget(1,pCopyBuffer);

	m_pDistortionShader->GetEffect()->SetFloat("g_fDistortionPow",m_fDistortionPow);
	m_pDistortionShader->GetEffect()->SetFloat("g_fProceed",m_fTimeAcc*10.f);
	m_pRendererCom->SetDistortionOn(m_pDistortionShader->GetEffect(),1);
	if(m_isDestroy == true)
	{
		m_pRendererCom->SetisDistortion(false);
	}
	//Engine::GetRenderer()->Render_WithDistortion(m_pDistortionShader->GetEffect(),1,m_pDistortionTex,NULL);

	

	//pBackBuffer->Release();
	//pCopyBuffer->Release();
}

CDistortion_Mgr* CDistortion_Mgr::Create(LPDIRECT3DDEVICE9 pGraphicDev,_float fDeadTime, _float fDistortionPow)
{
	CDistortion_Mgr* pInst = new CDistortion_Mgr(pGraphicDev,fDeadTime,fDistortionPow);
	if(pInst == NULL)
		Engine::Safe_Release(pInst);

	pInst->Ready_GameObject();
	return pInst;
}

void CDistortion_Mgr::Free(void)
{
	Engine::CGameObject::Free();

	_ulong dwRefCnt = Engine::Safe_Release(m_pRendererCom);
	dwRefCnt =Engine::Safe_Release(m_pDistortionShader);
	dwRefCnt=Engine::Safe_Release(m_pSurface);
	dwRefCnt=Engine::Safe_Release(m_pDistortionTex);
	//Engine::Safe_Release(m_pGraphicDev);

	for_each(m_pDistortionList.begin(),m_pDistortionList.end(),Engine::CRelease_Single());
	m_pDistortionList.clear();
}
