#include "Scene.h"
#include "GameObject.h"

using namespace Engine;

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
:m_pGraphicDev(pGraphicDev)
,m_isTimeStop(false)
,m_pDynamicCamLayer(NULL)
,m_pDefaultCamLayer(NULL)
,m_isEventOn(false)
{
	m_pGraphicDev->AddRef();
}

Engine::CScene::~CScene()
{

}

HRESULT Engine::CScene::Ready_Scene()
{
	return S_OK;
}

_int Engine::CScene::Update_Scene(const _float& fTimeDelta)
{
	MAPLAYER::iterator iter = m_mapLayer.begin();
	MAPLAYER::iterator iter_end = m_mapLayer.end();

	_int iExitCode = 0;
	for( ; iter != iter_end ; ++iter)
	{
		if(m_isEventOn == true && iter->second->GetisAlive() == false)
			continue;

		if(m_isTimeStop == false && iter->second != m_pDynamicCamLayer)
			iExitCode = iter->second->Update_Layer(fTimeDelta);
		else if(m_isTimeStop == true)
		{
			if(iter->second != m_pDynamicCamLayer && iter->second != m_pDefaultCamLayer)
				iExitCode = iter->second->Update_Layer(0.f);
			else if(iter->second == m_pDynamicCamLayer)
			{
				m_pDynamicCamLayer->GetList()->front()->Ready_GameObject();
				m_pDynamicCamLayer->Update_Layer(fTimeDelta);
			}
		}
		
	
		if(iExitCode & 0x80000000)
			return iExitCode;
	}

	iter = m_mapLayer.begin();
	for( ; iter != iter_end ; ++iter)
	{
		if(m_isEventOn == true && iter->second->GetisAlive() == false)
			continue;

		iExitCode = iter->second->LastUpdate_Layer(fTimeDelta);
		if(iExitCode & 0x80000000)
			return iExitCode;
	}
	

	return 0;
}

void Engine::CScene::Render_Scene()
{

}

void Engine::CScene::Free()
{
	for_each(m_mapLayer.begin(),m_mapLayer.end(),CRelease_Pair());
	m_mapLayer.clear();

	Engine::Safe_Release(m_pGraphicDev);
}

list<CGameObject*>* Engine::CScene::GetList(const _tchar* pTagName)
{
	MAPLAYER::iterator iter = find_if(m_mapLayer.begin(),m_mapLayer.end(),CTagFinder(pTagName));

	if(iter == m_mapLayer.end())
		return NULL;

	return iter->second->GetList();
}

CLayer* Engine::CScene::GetLayer(const _tchar* pTagName)
{
	MAPLAYER::iterator iter = find_if(m_mapLayer.begin(),m_mapLayer.end(),CTagFinder(pTagName));

	if(iter == m_mapLayer.end())
		return NULL;

	return iter->second;
}

void Engine::CScene::DeleteLayer(const _tchar* pLayerTag)
{
	MAPLAYER::iterator iter_found = find_if(m_mapLayer.begin(),m_mapLayer.end(),CTagFinder(pLayerTag));

	if(iter_found == m_mapLayer.end())
		return ;

	list<CGameObject*>::iterator iter = iter_found->second->GetList()->begin();
	list<CGameObject*>::iterator iter_end = iter_found->second->GetList()->end();

	for( ; iter != iter_end ; ++iter)
	{
		Engine::Safe_Release(*iter);
	}
	iter_found->second->GetList()->clear();

	Engine::Safe_Release(iter_found->second);
	m_mapLayer.erase(iter_found);
}

