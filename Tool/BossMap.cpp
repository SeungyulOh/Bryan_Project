#include "stdafx.h"
#include "BossMap.h"
#include "Export_Engine.h"

CBossMap::CBossMap(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
{

}

CBossMap::~CBossMap(void)
{

}

HRESULT CBossMap::Ready_GameObject(void)
{
	m_pTransformCom = (Engine::CTransform*)Engine::Clone(0,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pStaticMeshCom = (Engine::CStaticMesh*)Engine::Clone(0,L"Component_Mesh_BossMap");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_BossMap",m_pStaticMeshCom);

	m_pGraphicDev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);

	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(1.f,1.f,1.f));
	m_pTransformCom->Update_Component(0.f);
	return S_OK;
}


void CBossMap::Render_GameObject(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pTransformCom->SetUp_OnGraphicDev();
	m_pStaticMeshCom->Render_SubSets();
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

}

CBossMap* CBossMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossMap* pInst = new CBossMap(pGraphicDev);

	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CBossMap::Free(void)
{
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pStaticMeshCom);

	Engine::CGameObject::Free();
}
