#include "stdafx.h"
#include "BuffSelector.h"
#include "Export_Engine.h"
#include "ChargeLight.h"
#include "ShapeFire.h"
#include "TrailParticle.h"
#include "SoundMgr.h"

CBuffSelector::CBuffSelector(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTransformCom(NULL)
,m_pShader(NULL)
,m_fUV(0.f)
,m_isOnce(false)
{
	m_fTimeAcc = 0.f;
	m_fAlpha = 1.f;
	m_fSpeed = (_float)(rand()% 5) + 15.f;
	m_fDir = ((_float)(rand()%2) - 0.5f) * 2.f;
}

CBuffSelector::~CBuffSelector()
{

}

HRESULT CBuffSelector::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);
		
	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);
	
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_RectTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_RectTexture",m_pBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_Buff");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Buff",m_pTextureCom);

	m_pShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Default");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Default",m_pShader);
	
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(3.f,3.f,3.f));
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(0.f,D3DX_PI/2.f,0.f));

	m_iImgIdx = rand()%3;
	return S_OK;
}

_int CBuffSelector::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	//if(m_fTimeAcc > 1.f)
	//	m_fAlpha -= fTimeDelta;

	if(m_fTimeAcc > 0.5f && (Engine::Get_DIKeyState(DIK_Q) & 0x80))
	{
		((CPlayer*)Engine::GetList(L"Layer_Player")->front())->SetAtkState(*(CPlayer::AttackState*)&m_iImgIdx);

		Engine::CGameObject* pGameObject = CChargeLight::Create(m_pGraphicDev,CChargeLight::RIGHTHAND,&_vec3(0.f,0.f,0.f),10,0.3f,m_iImgIdx);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		pGameObject = CChargeLight::Create(m_pGraphicDev,CChargeLight::LEFTHAND,&_vec3(0.f,0.f,0.f),10,0.3f,m_iImgIdx);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		pGameObject = CShapeFire::Create(m_pGraphicDev,((CPlayer*)Engine::GetList(L"Layer_Player")->front())->GetAtkState());
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		Engine::CDynamicMesh* pPlayerMesh = (Engine::CDynamicMesh*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_Mesh_Hagie");
		pPlayerMesh->Set_AnimationSet(CPlayer::BUFF,false,0.8f);

		pGameObject = CTrailParticle::Create(m_pGraphicDev,50,m_iImgIdx);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		m_isDestroy = true;

		CSoundMgr::GetInstance()->PlaySound(L"hagie_skill_buff_vampric.wav",CSoundMgr::BUFF);
	}

	if(m_isOnce == false)
	{
		m_isOnce = true;
		m_pPlayerTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(Engine::MyGetTransMatFromWorldMat(*m_pPlayerTransform->GetWorldMatrix())._41,Engine::MyGetTransMatFromWorldMat(*m_pPlayerTransform->GetWorldMatrix())._42 + 8.f,Engine::MyGetTransMatFromWorldMat(*m_pPlayerTransform->GetWorldMatrix())._43));
	}

	if(m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->y > D3DX_PI*2 + D3DX_PI/2.f)
	{
		m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(0.f,D3DX_PI/2.f,0.f));

		if(m_iImgIdx >= 2)
			m_iImgIdx = 0;
		else
			++m_iImgIdx;
	}

	//m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->x+ fTimeDelta*m_fSpeed,m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->y + fTimeDelta*m_fSpeed,m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->z+ fTimeDelta*m_fSpeed));

	_vec3 vNewPos = *(_vec3*)&m_pPlayerTransform->GetWorldMatrix()->m[3][0] + _vec3(0.f,8.f,0.f);
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vNewPos);
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->x,m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->y + fTimeDelta*5.f,m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->z));
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	//ComputeViewZ(m_pTransformCom->GetWorldMatrix());
	m_pRendererCom->Add_RenderObject(this, Engine::CRenderer::RT_ALPHA);

	return 0;
}

void CBuffSelector::Render_GameObject(void)
{

	m_pShader->SetMatrixOnShader(*m_pTransformCom->GetWorldMatrix());
	m_pShader->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(m_iImgIdx));
	m_pShader->GetEffect()->SetFloat("g_fAlpha",m_fAlpha);

	_uint iNumPass = 0;

	m_pShader->GetEffect()->Begin(&iNumPass,0);
	m_pShader->GetEffect()->BeginPass(2);
	
	m_pBufferCom->Render_Buffer();

	m_pShader->GetEffect()->EndPass();
	m_pShader->GetEffect()->End();
}

CBuffSelector* CBuffSelector::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBuffSelector* pInst = new CBuffSelector(pGraphicDev);
	
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CBuffSelector::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShader);
	
}

