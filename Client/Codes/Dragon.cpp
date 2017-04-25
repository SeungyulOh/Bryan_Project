#include "stdafx.h"
#include "Dragon.h"
#include "Export_Engine.h"
#include "Boss.h"
#include "Stage_Scene.h"

CDragon::CDragon(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_eMyState(EVENT1)
,m_pShaderCom(NULL)
,m_pBoss(NULL)
,m_isOnce(false)
{

}

CDragon::~CDragon(void)
{

}

HRESULT CDragon::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pDynamicMeshCom = (Engine::CDynamicMesh*)Engine::Clone(COM_STAGE,L"Component_Mesh_Dragon");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_Dragon",m_pDynamicMeshCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Mesh");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Mesh",m_pShaderCom);

	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(0.1f,0.1f,0.1f));
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(0.f,0.f,0.f));
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(0.f,0.5f,0.f));
	m_pTransformCom->Update_Component(0.f);

	m_pDynamicMeshCom->Set_AnimationSet(EVENT1);

	return S_OK;
}

_int CDragon::Update_GameObject(const _float& fTimeDelta)
{
	if(m_isOnce == false)
	{
		m_isOnce = true;
		//m_pBoss = (CBoss*)Engine::GetList(L"Layer_Boss")->front();

		list<CGameObject*>* pList = Engine::GetList(L"Layer_Boss");
		if(NULL == pList)
			return 0;

		m_pBoss = dynamic_cast<CBoss*>(pList->front());
		if(NULL == m_pBoss)
			return 0;

	}

	m_fTimeDelta = fTimeDelta;
	//MovePattern(fTimeDelta);

	//FirstEventCam();

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if(((CStage_Scene*)Engine::GetCurrentScene())->GetDragonSpawn() == false)
		m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA);
	
	return 0;
}

_int CDragon::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if(m_pBoss == NULL || m_eMyState == EVENT9 || ((CStage_Scene*)Engine::GetCurrentScene())->GetDragonSpawn() == true)
		return 0;
	if(m_eMyState < 8 && m_eMyState != m_pBoss->GetState() && m_pBoss->GetState() < 9)
	{
		m_eMyState = m_pBoss->GetState();
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}

	m_pDynamicMeshCom->Play_Animation(fTimeDelta);

	return 0;
}


void CDragon::Render_GameObject(void)
{

	m_pTransformCom->SetUp_OnGraphicDev();
	m_pDynamicMeshCom->Play_Animation(m_fTimeDelta);
	
	m_pDynamicMeshCom->SetUp_ConstantTable(m_pShaderCom->GetEffect());
	m_pDynamicMeshCom->Render_MeshWithShader(m_pShaderCom->GetEffect());

	

}
//
//void CDragon::FirstEventCam(void)
//{
//	if(m_eMyState <= EVENT8 && false == m_pDynamicMeshCom->Get_Occupied())
//	{
//		++m_eMyState;
//		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
//	}
//	else if(m_eMyState == EVENT9 && false == m_pDynamicMeshCom->Get_Occupied())
//	{
//		++m_eMyState;
//		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
//
//		CPlayer* pPlayer = (CPlayer*)Engine::GetList(L"Layer_Player")->front();
//		pPlayer->AddDefaultCamAfterEvent();
//	}
//
//}


CDragon* CDragon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDragon* pInst = new CDragon(pGraphicDev);

	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CDragon::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pDynamicMeshCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CGameObject::Free();
}

void CDragon::SetState(STATE eState)
{
	m_eMyState = eState;
}

void CDragon::GetState(_uint* pState)
{
	*pState = m_eMyState;
}
