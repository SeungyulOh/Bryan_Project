#include "stdafx.h"
#include "Distortion.h"
#include "Export_Engine.h"

CDistortion::CDistortion(LPDIRECT3DDEVICE9 pGraphicDev,_float m_fScaleSpeed,_float fDistortionPow, _int iImgIdx, _bool isBillBoard, _float fDeadTime,LPDIRECT3DBASETEXTURE9 pTexture)
:Engine::CGameObject(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTransformCom(NULL)
,m_pShader(NULL)
,m_fUV(0.f)
,m_isOnce(false)
//,m_pDistortionTex(NULL)
,m_fDistortionPow(fDistortionPow)
,m_isBillBoard(isBillBoard)
,m_fDeadTime(fDeadTime)
//,m_pDistortionTexture(pTexture)
{
	m_fTimeAcc = 0.f;
	m_fAlpha = 1.f;
	m_fSpeed = m_fScaleSpeed;
	m_fDir = ((_float)(rand()%2) - 0.5f) * 2.f;
	m_iImgIdx = iImgIdx;

	//if(m_pDistortionTexture != NULL)
	//	m_pDistortionTexture->AddRef();
}

CDistortion::~CDistortion()
{

}

HRESULT CDistortion::Ready_GameObject(_vec3 vScale,_vec3 vRot,_vec3 vTrans)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);
		
	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);
	
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_RectTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_RectTexture",m_pBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STATIC,L"Component_Texture_efx_ripple1_normal");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_efx_ripple1_normal",m_pTextureCom);

	m_pShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Default");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Default",m_pShader);

	//m_pDistortionShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_RenderTarget");
	//Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_RenderTarget",m_pDistortionShader);

	
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,vScale);
	//_float fAngleY = (_float)(rand()%360);
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,vRot);
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vTrans);

	/*D3DVIEWPORT9 tViewPort;
	m_pGraphicDev->GetViewport(&tViewPort);
	D3DXCreateTexture(m_pGraphicDev,tViewPort.Width,tViewPort.Height,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&m_pDistortionTex);
	m_pDistortionTex->GetSurfaceLevel(0,&m_pSurface);*/
	//m_pGraphicDev->SetRenderTarget(2,m_pSurface);

	//m_iImgIdx = 1;//rand()%3;
	return S_OK;
}

_int CDistortion::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if(m_fTimeAcc > m_fDeadTime)
		m_isDestroy = true;

	if(m_isOnce == false)
	{
		m_isOnce = true;
		Engine::CTransform* pPlayerTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
		m_pPlayerWorld = pPlayerTransform->GetWorldMatrix();
	}

	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->x+ fTimeDelta*m_fSpeed,m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->y + fTimeDelta*m_fSpeed,m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->z));
	//m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->x,m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->y+ fTimeDelta*10.f ,m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->z) );

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	D3DXMatrixInverse(&matView,0,&matView);
	
	if(m_isBillBoard == true)
		m_pTransformCom->SetBillBoardMat(Engine::MyGetRotMatFromWorldMat(matView));
	
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	//ComputeViewZ(m_pTransformCom->GetWorldMatrix());
	//m_pRendererCom->Add_RenderObject(this, Engine::CRenderer::RT_ALPHA);
	return 0;
}

void CDistortion::Render_GameObject(void)
{
	m_pTransformCom->SetUp_OnGraphicDev();

	m_pShader->SetMatrixOnShader(*m_pTransformCom->GetWorldMatrix());
	m_pShader->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(m_iImgIdx));
	m_pShader->GetEffect()->SetFloat("g_fAlpha",m_fAlpha);

	/*LPDIRECT3DSURFACE9 pBackBuffer, pCopyBuffer;
	m_pGraphicDev->GetRenderTarget(0,&pBackBuffer);
	m_pGraphicDev->GetRenderTarget(1,&pCopyBuffer);

	m_pGraphicDev->SetRenderTarget(0,m_pRendererCom->GetTargetSurface(Engine::CRenderer::FOR_DISTORTION));
	m_pGraphicDev->SetRenderTarget(1,m_pSurface);

	m_pGraphicDev->Clear(0,0,D3DCLEAR_TARGET,D3DXCOLOR(0.f,0.f,0.f,0.f),1.f,0);*/

	m_pShader->GetEffect()->Begin(0,0);
	m_pShader->GetEffect()->BeginPass(4);
	
	m_pBufferCom->Render_Buffer();

	m_pShader->GetEffect()->EndPass();
	m_pShader->GetEffect()->End();

	/*m_pGraphicDev->SetRenderTarget(0,pBackBuffer);
	m_pGraphicDev->SetRenderTarget(1,pCopyBuffer);

	m_pDistortionShader->GetEffect()->SetFloat("g_fDistortionPow",m_fDistortionPow);
	m_pDistortionShader->GetEffect()->SetFloat("g_fProceed",m_fTimeAcc*10.f);
	Engine::GetRenderer()->Render_WithDistortion(m_pDistortionShader->GetEffect(),1,m_pDistortionTex,NULL);

	pBackBuffer->Release();
	pCopyBuffer->Release();*/

}

CDistortion* CDistortion::Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 vScale,_vec3 vRot,_vec3 vTrans,_float m_fScaleSpeed,_float fDistortionPow,_int iImgIdx, _bool isBillBoard, _float fDeadTime,LPDIRECT3DBASETEXTURE9 pTexture)
{
	CDistortion* pInst = new CDistortion(pGraphicDev,m_fScaleSpeed,fDistortionPow,iImgIdx,isBillBoard,fDeadTime,pTexture);
	
	if(FAILED(pInst->Ready_GameObject(vScale,vRot,vTrans)))
		return NULL;

	return pInst;
}

void CDistortion::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShader);


	//m_pGraphicDev->Release();
	//m_pGraphicDev->Release();
	
}

