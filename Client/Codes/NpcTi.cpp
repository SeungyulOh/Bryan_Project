#include "stdafx.h"
#include "NpcTi.h"
#include "Boss.h"
#include "Export_Engine.h"
#include "Stage_Scene.h"


CNpcTi::CNpcTi(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_eMyState(0)
,m_pShaderCom(NULL)
,m_pBoss(NULL)
,m_isOnce(false)
{

}

CNpcTi::~CNpcTi(void)
{

}

HRESULT CNpcTi::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pDynamicMeshCom = (Engine::CDynamicMesh*)Engine::Clone(COM_STAGE,L"Component_Mesh_NpcTi");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_NpcTi",m_pDynamicMeshCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Mesh");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Mesh",m_pShaderCom);

	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(0.1f,0.1f,0.1f));
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(0.f,0.f,0.f));
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(0.f,0.5f,0.f));
	m_pTransformCom->Update_Component(0.f);

	m_pDynamicMeshCom->Set_AnimationSet(EVENT2);

	return S_OK;
}

_int CNpcTi::Update_GameObject(const _float& fTimeDelta)
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

	if(!(m_pBoss->GetState() == 1 || m_pBoss->GetState() == 2 || m_pBoss->GetState() == 4 || m_pBoss->GetState() == 6))
		return 0;

	
	m_fTimeDelta = fTimeDelta;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if(((CStage_Scene*)Engine::GetCurrentScene())->GetDragonSpawn() == false)
		m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA);
	
	return 0;
}

_int CNpcTi::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if(m_pBoss == NULL || m_eMyState == STATE_END || m_eMyState < m_pBoss->GetState() || ((CStage_Scene*)Engine::GetCurrentScene())->GetDragonSpawn() == true)
		return 0;;
	if(m_pBoss->GetState() == 1 || m_pBoss->GetState() == 2 || m_pBoss->GetState() == 4 || m_pBoss->GetState() == 6)
	{
		m_eMyState = m_pBoss->GetState();
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}

	return 0;
}


void CNpcTi::Render_GameObject(void)
{

	m_pTransformCom->SetUp_OnGraphicDev();
	m_pDynamicMeshCom->Play_Animation(m_fTimeDelta);
	
	m_pDynamicMeshCom->SetUp_ConstantTable(m_pShaderCom->GetEffect());
	m_pDynamicMeshCom->Render_MeshWithShader(m_pShaderCom->GetEffect());

	D3DXTRACK_DESC tInfo;
	ZeroMemory(&tInfo,sizeof(D3DXTRACK_DESC));
	LPD3DXANIMATIONCONTROLLER pAnimationControl = *m_pDynamicMeshCom->GetAnimationCtrl()->Get_AnimationCtrl();
	pAnimationControl->GetTrackDesc(m_pDynamicMeshCom->GetAnimationCtrl()->GetCurrentTrack(),&tInfo);

	/*TCHAR szTemp[256] = L"%d";
	wsprintf(szTemp,L"TI : %d",(_uint)tInfo.Position);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,40.f),D3DXCOLOR(1.f,1.f,1.f,1.f));*/
	

}


CNpcTi* CNpcTi::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpcTi* pInst = new CNpcTi(pGraphicDev);

	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CNpcTi::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pDynamicMeshCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CGameObject::Free();
}

void CNpcTi::SetState(STATE eState)
{
	m_eMyState = eState;
}

void CNpcTi::GetState(_uint* pState)
{
	*pState = m_eMyState;
}
