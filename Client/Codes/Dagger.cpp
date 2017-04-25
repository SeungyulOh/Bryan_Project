#include "stdafx.h"
#include "Dagger.h"
#include "Export_Engine.h"
#include "Engine_Struct.h"
#include "BoxCollider.h"
#include "Stage_Scene.h"

CDagger::CDagger(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_isOnce(false)
,m_pTargetCollider(NULL)
,m_pShaderCom(NULL)
{

}

CDagger::~CDagger(void)
{

}

HRESULT CDagger::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pDynamicMeshCom = (Engine::CDynamicMesh*)Engine::Clone(COM_STAGE,L"Component_Mesh_Dagger");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_Dagger",m_pDynamicMeshCom);

	m_pBoxColliderCom = (Engine::CBoxCollider*)Engine::Clone(COM_STAGE,L"Component_BoxCollider");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_BoxCollider",m_pBoxColliderCom);
	m_pBoxColliderCom->Compute_MinMax(m_pDynamicMeshCom,*m_pDynamicMeshCom->GetInitMatrix());

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Mesh");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Mesh",m_pShaderCom);
	
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(0.f,0.f,0.f));
	m_pDynamicMeshCom->Set_AnimationSet(1);
	return S_OK;
}

_int CDagger::Update_GameObject(const _float& fTimeDelta)
{
	if(m_isOnce == false)
	{
		Engine::CDynamicMesh* pDynamicMesh = (Engine::CDynamicMesh*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_Mesh_Hagie");
		
		Engine::D3DXFRAME_DERIVED* pFrameFromDagger = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pDynamicMeshCom->GetRootFrame(),"weapon_bone_01");
		Engine::D3DXFRAME_DERIVED* pFrameFromPlayer = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pDynamicMesh->GetRootFrame(),"ValveBiped_Anim_Attachment_RH");
		m_pRHFrameFromPlayer = &pFrameFromPlayer->CombinedMatrix;
		m_pRHFrameFromDagger = &pFrameFromDagger->CombinedMatrix;

		//D3DXMatrixTranslation(&m_OffSetMat,vTemp.x,vTemp.y,vTemp.z);
		
		m_isOnce = true;
	}

	//if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	//	m_pDynamicMeshCom->Set_AnimationSet(1,false);
	//else
	//	m_pDynamicMeshCom->Set_AnimationSet(0);
	
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	
	//if(m_pDynamicMeshCom->Get_Occupied() == false)
	//{
	//	D3DXTRACK_DESC tInfo,tInfo2;
	//	ZeroMemory(&tInfo,sizeof(D3DXTRACK_DESC));
	//	LPD3DXANIMATIONCONTROLLER pAnimationControl = *m_pDynamicMeshCom->GetAnimationCtrl()->Get_AnimationCtrl();
	//	pAnimationControl->GetTrackDesc(0,&tInfo);
	//	pAnimationControl->GetTrackDesc(1,&tInfo2);

	//	if(tInfo.Enable + tInfo2.Enable <= 1)
	//	{

	//	}
	//}


	m_pTransformCom->Adjust_ParentMat(&(*m_pRHFrameFromPlayer * /**m_pRHFrameFromDagger **/ *m_pPlayerMat));

	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA);


	return 0;
}


_int CDagger::LastUpdate_GameObject(const _float& fTimeDelta)
{

	//if(m_pTargetCollider == NULL)
	//	m_pTargetCollider = (Engine::CBoxCollider*)Engine::GetList(L"Layer_Boss")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_BoxCollider");
	//else	
	//	m_isCol = m_pBoxColliderCom->CollisionCheck_OBB(m_pTargetCollider);

	//m_pDynamicMeshCom->Play_Animation(2.5f * fTimeDelta,false);

	return 0;
}


void CDagger::Render_GameObject(void)
{
	m_pTransformCom->SetUp_OnGraphicDev();

	m_pDynamicMeshCom->SetUp_ConstantTable(m_pShaderCom->GetEffect());
	m_pDynamicMeshCom->Render_MeshWithShader(m_pShaderCom->GetEffect());
	//m_pDynamicMeshCom->Render_Mesh();

	if(((CStage_Scene*)Engine::GetCurrentScene())->GetBoolLine() == true)
		m_pBoxColliderCom->Render(Engine::CBoxCollider::BOX_OBB,*m_pTransformCom->GetWorldMatrix(),m_isCol);


}

CDagger* CDagger::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDagger* pInst = new CDagger(pGraphicDev);

	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CDagger::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pDynamicMeshCom);
	Engine::Safe_Release(m_pBoxColliderCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CGameObject::Free();
}
