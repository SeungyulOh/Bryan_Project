#include "stdafx.h"
#include "Player_Tool.h"
#include "Export_Engine.h"
#include "Default_Camera.h"

CPlayer_Tool::CPlayer_Tool(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_fAngleAcc(D3DX_PI/2.f)
,m_CurIndex(10)
,m_eMyState(IDLE)
,m_isNextCombo(false)
,m_pShaderCom(NULL)
,m_isOnce(false)
,m_fScale(0.f)
,m_dwDashEffectCnt(0)
,m_fTrailSpawnTimeAcc(0.f)
{

}

CPlayer_Tool::~CPlayer_Tool(void)
{

}

HRESULT CPlayer_Tool::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(0,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(0,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pDynamicMeshCom = (Engine::CDynamicMesh*)Engine::Clone(0,L"Component_Mesh_Hagie");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_Hagie",m_pDynamicMeshCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(0,L"Component_Shader_Mesh_Phong");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Mesh_Phong",m_pShaderCom);

	m_fScale = 0.09f;

	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(m_fScale,m_fScale,m_fScale));
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(0.f,m_fAngleAcc,0.f));
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(0.f,0.f,0.f));
	//m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(-10.f,1.f,0.f));

	Engine::CGameObject::Update_GameObject(1.f);

	m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);

	
	return S_OK;
}

_int CPlayer_Tool::Update_GameObject(const _float& fTimeDelta)
{
	SetUpOnce();

	if(Engine::Get_KeyMgr()->KeyDown(KEY_T))
	{
		m_eMyState = REQ_BEGIN;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false,0.5f);
	}
	if(m_pDynamicMeshCom->Get_Occupied() == true && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.7 && m_eMyState == REQ_BEGIN)
	{
		m_eMyState = REQ_END;
		m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false,0.5f);
	}

	if((m_eMyState == ATK3) && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() < 0.4)
	{
		_vec3 vLocalDashPoint = *(_vec3*)&m_pDashMoveMat->m[3][0] * m_fScale *1.5f;
		D3DXVec3TransformCoord(&vLocalDashPoint,&vLocalDashPoint,m_pTransformCom->GetWorldMatrix());
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vLocalDashPoint);
	}
	else if((m_eMyState == ATK3STRONG) && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() < 0.3)
	{
		_vec3 vLocalDashPoint = *(_vec3*)&m_pDashMoveMat->m[3][0] * m_fScale;
		D3DXVec3TransformCoord(&vLocalDashPoint,&vLocalDashPoint,m_pTransformCom->GetWorldMatrix());
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vLocalDashPoint);
	}
	else if((m_eMyState == ATK4 || m_eMyState == ATK3STRONG_FINAL) && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() < 0.3)
	{
		_vec3 vLocalDashPoint = *(_vec3*)&m_pDashMoveMat->m[3][0] * m_fScale *2.5f;
		D3DXVec3TransformCoord(&vLocalDashPoint,&vLocalDashPoint,m_pTransformCom->GetWorldMatrix());
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vLocalDashPoint);
	}
	else if((m_eMyState == ATK4STRONG || m_eMyState == ATK4STRONG_FINAL) && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.2 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() < 0.3)
	{
		_vec3 vLocalDashPoint = *(_vec3*)&m_pDashMoveMat->m[3][0] * m_fScale *0.5f;
		D3DXVec3TransformCoord(&vLocalDashPoint,&vLocalDashPoint,m_pTransformCom->GetWorldMatrix());
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vLocalDashPoint);
	}
	
	m_fTimeDelta = fTimeDelta;

	Engine::CGameObject::Update_GameObject(fTimeDelta); //Component Update
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA); //Render enable

	return 0;
}

_int CPlayer_Tool::LastUpdate_GameObject(const _float& fTimeDelta)
{
	
	return 0;
}


void CPlayer_Tool::Render_GameObject(void)
{
	list<CPhantom_Trail_Tool*>::iterator iter = m_EffectList.begin();
	list<CPhantom_Trail_Tool*>::iterator iter_end = m_EffectList.end();

	for(; iter!=iter_end ; ++iter)
	{
		if((*iter)->GetDestroy() == true)
		{
			Engine::Safe_Release(*iter);
			iter = m_EffectList.erase(iter);
		}
		if(iter == m_EffectList.end())
			break;

		(*iter)->Render_GameObject();
	}


	if(m_pDynamicMeshCom->Get_Occupied() == false)
		m_pDynamicMeshCom->Set_AnimationSet(IDLE);

	m_pTransformCom->SetUp_OnGraphicDev();
	m_pDynamicMeshCom->Play_Animation(0.025f);
	
	m_pDynamicMeshCom->SetUp_ConstantTable(m_pShaderCom->GetEffect());
	m_pDynamicMeshCom->Render_MeshWithShader(m_pShaderCom->GetEffect());

}


CPlayer_Tool* CPlayer_Tool::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer_Tool* pInst = new CPlayer_Tool(pGraphicDev);

	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CPlayer_Tool::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pDynamicMeshCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CGameObject::Free();
}



void CPlayer_Tool::SetUpOnce(void)
{
	if(m_isOnce == false)
	{
		m_isOnce = true;

		Engine::D3DXFRAME_DERIVED* pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pDynamicMeshCom->GetRootFrame(),"ValveBiped_Anim_Attachment_CAM_Origin");
		m_pDashMoveMat = &pFrameFound->CombinedMatrix;

	}
}
