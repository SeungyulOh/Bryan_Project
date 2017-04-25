#include "stdafx.h"
#include "PlayerTrace.h"
#include "Export_Engine.h"

CPlayerTrace::CPlayerTrace(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_dAniPos(0.0)
,m_fTimeAcc(0.f)
{

}

CPlayerTrace::~CPlayerTrace(void)
{

}

HRESULT CPlayerTrace::Ready_GameObject(Engine::CTransform* pPlayerTransfom, Engine::CDynamicMesh* pPlayerMesh, _ulong dwAniIdx)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pDynamicMeshCom = (Engine::CDynamicMesh*)Engine::Clone(COM_STAGE,L"Component_Mesh_Hagie");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_Hagie",m_pDynamicMeshCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Effect");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Effect",m_pShaderCom);

	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,*pPlayerTransfom->GetInfomation(Engine::CTransform::SCALE));
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,*pPlayerTransfom->GetInfomation(Engine::CTransform::ANGLE));
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,*pPlayerTransfom->GetInfomation(Engine::CTransform::POSITION));


	//////


	/////

	D3DXTRACK_DESC tTrackDesc;
	ZeroMemory(&tTrackDesc,sizeof(D3DXTRACK_DESC));
	LPD3DXANIMATIONCONTROLLER pAnimationCtrl = *pPlayerMesh->GetAnimationCtrl()->Get_AnimationCtrl();
	pAnimationCtrl->GetTrackDesc(0,&tTrackDesc);

	m_pDynamicMeshCom->Set_AnimationSet(dwAniIdx,true,1.f);
	m_dAniPos = tTrackDesc.Position;
	//tTrackDesc.Position

	return S_OK;
}

_int CPlayerTrace::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if(m_fTimeAcc > 1.5f)
		m_isDestroy = true;
	
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA); //Render enable
	return 0;
}

_int CPlayerTrace::LastUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CPlayerTrace::Render_GameObject(void)
{
	m_pTransformCom->SetUp_OnGraphicDev();

	m_pDynamicMeshCom->SetCurrentTrackPosition(m_dAniPos);
	m_pDynamicMeshCom->Play_Animation(1.f);
	 	
	m_pDynamicMeshCom->SetUp_ConstantTable(m_pShaderCom->GetEffect());
	m_pDynamicMeshCom->Render_MeshWithShader(m_pShaderCom->GetEffect(),0);
}

CPlayerTrace* CPlayerTrace::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform* pPlayerTransfom, Engine::CDynamicMesh* pPlayerMesh, _ulong dwAniIdx)
{
	CPlayerTrace* pInstance = new CPlayerTrace(pGraphicDev);
	if(FAILED(pInstance->Ready_GameObject(pPlayerTransfom,pPlayerMesh,dwAniIdx)))
	{
		Engine::Safe_Release(pInstance);
		return NULL;
	}
	return pInstance;
}

void CPlayerTrace::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pDynamicMeshCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CGameObject::Free();
}

