#include "GameObject.h"

using namespace Engine;

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
:m_pGraphicDev(pGraphicDev)
,m_isCol(false)
,m_isDestroy(false)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::~CGameObject(void)
{

}

HRESULT Engine::CGameObject::Ready_GameObject(void)
{
	return S_OK;
}

_int Engine::CGameObject::Update_GameObject(const _float& fTimeDelta)
{
	MAPCOMPONENT::iterator iter = m_MapComponent[UPDATE].begin();
	MAPCOMPONENT::iterator iter_end = m_MapComponent[UPDATE].end();

	for( ; iter != iter_end ; ++iter)
	{
		iter->second->Update_Component(fTimeDelta);
	}

	return 0;
}

void Engine::CGameObject::Render_GameObject(void)
{

}


void Engine::CGameObject::Render_ShadowDepth(void)
{

}



void Engine::CGameObject::Push_CloneComponent(COMTYPE eType, const _tchar* pTag, CComponent* pComponent)
{

	CComponent* pComponent_Find = FindComponent(eType, pTag);
	if(pComponent_Find != NULL)
		return;

	m_MapComponent[eType].insert(MAPCOMPONENT::value_type(pTag,pComponent));
	pComponent->AddRefCnt();
}


void Engine::CGameObject::Free(void)
{
	for(_ulong i = 0; i<TYPE_END; ++i)
	{
		for_each(m_MapComponent[i].begin(),m_MapComponent[i].end(),CRelease_Pair());
		m_MapComponent[i].clear();
	}
	
	Engine::Safe_Release(m_pGraphicDev);
}

Engine::CComponent* Engine::CGameObject::FindComponent(COMTYPE eType, const _tchar* pTag)
{
	MAPCOMPONENT::iterator iter = find_if(m_MapComponent[eType].begin(), m_MapComponent[eType].end(), CTagFinder(pTag));
	if(iter == m_MapComponent[eType].end())
		return NULL;

	return iter->second;
}

void Engine::CGameObject::ComputeViewZ(_matrix* pWorldMat)
{
	_vec3 vWorldPos = *(_vec3*)&pWorldMat->m[3][0];
	_vec3 vCamPos;
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_WORLD,&matView);
	D3DXMatrixInverse(&matView,0,&matView);
	memcpy(&vCamPos,(_vec3*)&matView.m[3][0],sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - vWorldPos));
}

