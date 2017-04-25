#include "Com_Manager.h"

using namespace Engine;

IMPLEMENT_SINGLETON(CCom_Manager)

Engine::CCom_Manager::CCom_Manager(void)
:m_pMapComponent(NULL)
,m_iContainerSize(0)
{

}

Engine::CCom_Manager::~CCom_Manager(void)
{

}

HRESULT Engine::CCom_Manager::Ready_ComManager(_uint iSize)
{
	if(m_pMapComponent != NULL)
		return E_FAIL;

	m_pMapComponent = new MAPCOMPONENT[iSize];

	m_iContainerSize = iSize;

	return S_OK;
}

void Engine::CCom_Manager::Add_Component(_uint iIndex, const _tchar* pComTag, CComponent* pComponent)
{
	CComponent* pComponent_Find = Find_Component(iIndex,pComTag);

	if(pComponent_Find != NULL)
		return;

	m_pMapComponent[iIndex].insert(MAPCOMPONENT::value_type(pComTag,pComponent));
		
}

Engine::CComponent* Engine::CCom_Manager::Clone(_uint iIndex, const _tchar* pComTag)
{
	CComponent* pComponent_Find = Find_Component(iIndex,pComTag);

	if(pComponent_Find == NULL)
		return NULL;

	return pComponent_Find->Clone();
}

void Engine::CCom_Manager::Free(void)
{
	for(_ulong i = 0 ; i < m_iContainerSize; ++i)
	{
		for_each(m_pMapComponent[i].begin(), m_pMapComponent[i].end(), CRelease_Pair());
		m_pMapComponent[i].clear();
	}

	Engine::Safe_Delete_Array(m_pMapComponent);
	
}

Engine::CComponent* Engine::CCom_Manager::Find_Component(_uint iIndex, const _tchar* pComTag)
{
	MAPCOMPONENT::iterator iter = find_if(m_pMapComponent[iIndex].begin(), m_pMapComponent[iIndex].end(), CTagFinder(pComTag));

	if(iter == m_pMapComponent[iIndex].end())
		return NULL;

	return iter->second;
}

void Engine::CCom_Manager::Release_Component(_uint iIndex)
{
	for_each(m_pMapComponent[iIndex].begin(),m_pMapComponent[iIndex].end(),CRelease_Pair());
	m_pMapComponent[iIndex].clear();
}

