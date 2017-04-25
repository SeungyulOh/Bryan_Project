#include "stdafx.h"
#include "MeshObj.h"
#include "Export_Engine.h"

CMeshObj::CMeshObj(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(szName,sizeof(_tchar)*256);
}

CMeshObj::~CMeshObj(void)
{

}

HRESULT CMeshObj::Ready_GameObject(const _tchar* pNameTag,_vec3 vFirstPos)
{
	m_pTransformCom = (Engine::CTransform*)Engine::Clone(0,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pStaticMeshCom = (Engine::CStaticMesh*)Engine::Clone(0,pNameTag);
	Push_CloneComponent(CGameObject::NONUPDATE,pNameTag,m_pStaticMeshCom);

	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(1.f,1.f,1.f));
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vFirstPos);
	m_pTransformCom->Update_Component(0.f);
	lstrcpy(szName,pNameTag);
	return S_OK;
}


void CMeshObj::Render_GameObject(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pTransformCom->SetUp_OnGraphicDev();
	m_pStaticMeshCom->Render_SubSets();
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

}

CMeshObj* CMeshObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pNameTag,_vec3 vFirstPos)
{
	CMeshObj* pInst = new CMeshObj(pGraphicDev);

	if(FAILED(pInst->Ready_GameObject(pNameTag,vFirstPos)))
		return NULL;

	return pInst;
}

void CMeshObj::Free(void)
{
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pStaticMeshCom);

	Engine::CGameObject::Free();
}
