#include "stdafx.h"
#include "RingWind.h"
#include "Export_Engine.h"
#include "BossDevil.h"

CRingWind::CRingWind(LPDIRECT3DDEVICE9 pGraphicDev,_float fOffSetAngleY)
:Engine::CGameObject(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTransformCom(NULL)
,m_pShader(NULL)
,m_fUV(0.f)
,m_isOnce(false)
{
	m_fTimeAcc = 0.f;
	m_fAlpha = 1.f;
	m_fSpeed = 5.f;
	m_fDir = (rand()%5 + 1) * -2.f;
	m_fXdirection = 1.f;

	D3DXMatrixRotationY(&m_matRotAfterLocal,fOffSetAngleY +D3DX_PI/4.f);
}

CRingWind::~CRingWind()
{

}

HRESULT CRingWind::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);
		
	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);
	
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_RectTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_RectTexture",m_pBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_Particle_ring_outer_wind06");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Particle_ring_outer_wind06",m_pTextureCom);

	m_pShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Default");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Default",m_pShader);
	
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(10.f,10.f,10.f));
	_float fAngleY = (_float)(rand()%360);
	_float fAngleX = D3DXToRadian((_float)(rand()%10));
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(D3DX_PI/2.f+fAngleX,D3DXToRadian(fAngleY),0.f));

	m_vLocalPos = _vec3(0.f,0.f,0.f);
	m_iRadius = 8;


	


	return S_OK;
}

_int CRingWind::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if(m_fTimeAcc > 0.1f)
		m_fAlpha -= fTimeDelta;
	if(m_fAlpha < 0.f)
		m_isDestroy = true;

	if(m_isOnce == false)
	{
		m_isOnce = true;
		Engine::CTransform* pPlayerTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
		m_pPlayerWorldMat = pPlayerTransform->GetWorldMatrix();
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(Engine::MyGetTransMatFromWorldMat(*pPlayerTransform->GetWorldMatrix())._41,Engine::MyGetTransMatFromWorldMat(*pPlayerTransform->GetWorldMatrix())._42,Engine::MyGetTransMatFromWorldMat(*pPlayerTransform->GetWorldMatrix())._43));
	}

	m_vLocalPos.x += fTimeDelta*m_iRadius*m_fXdirection * m_fSpeed;
	if(m_vLocalPos.x > 2*m_iRadius)
		m_fXdirection *= -1.f;
	if(m_vLocalPos.x < 0)
		m_fXdirection *= -1.f;

	if(m_fXdirection == -1.f)
		m_vLocalPos.z = sqrtf(m_iRadius*m_iRadius-(m_vLocalPos.x-m_iRadius) * (m_vLocalPos.x-m_iRadius)) + m_iRadius;
	else
		m_vLocalPos.z = -1.f * sqrtf(m_iRadius*m_iRadius-(m_vLocalPos.x-m_iRadius) * (m_vLocalPos.x-m_iRadius)) + m_iRadius;

	_vec3 vWorldPos;
	D3DXVec3TransformCoord(&vWorldPos,&m_vLocalPos,&(m_matRotAfterLocal * Engine::MyGetTransMatFromWorldMat(*m_pPlayerWorldMat)));


	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vWorldPos);
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->x,m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->y + fTimeDelta*m_fSpeed*m_fDir,m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->z));
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	//ComputeViewZ(m_pTransformCom->GetWorldMatrix());
	m_pRendererCom->Add_RenderObject(this, Engine::CRenderer::RT_ALPHA);

	CBossDevil* pBoss = ((CBossDevil*)Engine::GetList(L"Layer_Boss")->front());
	float fDist = D3DXVec3Length(&(*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] - *(_vec3*)&pBoss->GetWorldMatrix().m[3][0]));
	if(fDist < 20.f)
		pBoss->SetisCol(true);

	return 0;
}

void CRingWind::Render_GameObject(void)
{

	m_pShader->SetMatrixOnShader(*m_pTransformCom->GetWorldMatrix());
	m_pShader->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(0));
	m_pShader->GetEffect()->SetFloat("g_fAlpha",m_fAlpha);

	_uint iNumPass = 0;

	m_pShader->GetEffect()->Begin(&iNumPass,0);
	m_pShader->GetEffect()->BeginPass(2);
	
	m_pBufferCom->Render_Buffer();

	m_pShader->GetEffect()->EndPass();
	m_pShader->GetEffect()->End();
}

CRingWind* CRingWind::Create(LPDIRECT3DDEVICE9 pGraphicDev,_float fOffSetAngleY)
{
	CRingWind* pInst = new CRingWind(pGraphicDev,fOffSetAngleY);
	
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CRingWind::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShader);
	
}

