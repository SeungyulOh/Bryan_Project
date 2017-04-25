#include "stdafx.h"
#include "WaterFall.h"
#include "Export_Engine.h"
#include "ExpEffect.h"
#include "ChildEffect.h"

CWaterFall::CWaterFall(LPDIRECT3DDEVICE9 pGraphicDev,_matrix matWorld, _uint iImgIdx, _uint iPass)
:Engine::CGameObject(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTransformCom(NULL)
,m_pShader(NULL)
,m_fUV(0.f)
,m_isOnce(false)
,m_iImgIdx(iImgIdx)
,m_matWorld(matWorld)
,m_iPassIdx(iPass)
{
	m_fTimeAcc = 0.f;
	m_fAlpha = 1.f;
	m_fSpeed = (_float)(rand()% 4) + 1.f;
	m_fDir = ((_float)(rand()%2) - 0.5f) * 2.f;
}

CWaterFall::~CWaterFall()
{

}

HRESULT CWaterFall::Ready_GameObject(_float fAngleY)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);
		
	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);
	
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_RectTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_RectTexture",m_pBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_WaterFall");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_WaterFall",m_pTextureCom);

	m_pShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Water");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Water",m_pShader);
	
	_float fScale = 100.f;//rand()%100 + 350.f;
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(fScale,fScale,fScale));
	//_float fAngleY = 0.f;// (_float)(rand()%360);
	//m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(D3DX_PI/2.f,D3DXToRadian(fAngleY),0.f));
//	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,m_vFirstPos);

	_matrix matScale,matTrans,matRot;
	D3DXMatrixScaling(&matScale,25.f,140.f,25.f);
	D3DXMatrixTranslation(&matTrans,650.f,390.f,-25.f);
	D3DXMatrixRotationY(&matRot,fAngleY);
	m_LocalMat = matScale * matRot * matTrans;

	//m_iImgIdx = 0;//rand()%3;
	return S_OK;
}

_int CWaterFall::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	m_fUV = m_fTimeAcc;
	//if(m_fTimeAcc > 1.f)
	//	m_fAlpha -= fTimeDelta;
	//if(m_fAlpha < 0.f)
	//	m_isDestroy = true;

	if(m_isOnce == false)
	{
		m_isOnce = true;

		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN3, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WATERFALL,CExpEffect::WATERFALL0, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN4, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
	}

	//m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->x+ fTimeDelta*m_fSpeed,m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->y + fTimeDelta*m_fSpeed,m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->z+ fTimeDelta*m_fSpeed));
	//m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->x,m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->y + fTimeDelta*m_fDir*0.1f,m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->z));
	//Engine::CGameObject::Update_GameObject(fTimeDelta);
	
	m_pTransformCom->SetWorldmat(m_LocalMat * m_matWorld);

	//ComputeViewZ(m_pTransformCom->GetWorldMatrix());
	m_pRendererCom->Add_RenderObject(this, Engine::CRenderer::RT_ALPHA);
	return 0;
}

void CWaterFall::Render_GameObject(void)
{
	m_pShader->SetMatrixOnShader(*m_pTransformCom->GetWorldMatrix());
	m_pShader->GetEffect()->SetTexture("g_BaseTexture",m_pTextureCom->GetTexture(m_iImgIdx));
	m_pShader->GetEffect()->SetFloat("g_OffSet",m_fUV / m_fSpeed);

	_uint iNumPass = 0;

	m_pShader->GetEffect()->Begin(&iNumPass,0);
	m_pShader->GetEffect()->BeginPass(m_iPassIdx);
	
	m_pBufferCom->Render_Buffer();

	m_pShader->GetEffect()->EndPass();
	m_pShader->GetEffect()->End();
}

CWaterFall* CWaterFall::Create(LPDIRECT3DDEVICE9 pGraphicDev,_matrix matWorld, _uint iImgIdx, _uint iPass,_float fAngleY)
{
	CWaterFall* pInst = new CWaterFall(pGraphicDev,matWorld,iImgIdx,iPass);
	
	if(FAILED(pInst->Ready_GameObject(fAngleY)))
		return NULL;

	return pInst;
}

void CWaterFall::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShader);
	
}

