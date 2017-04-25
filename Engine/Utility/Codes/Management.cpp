#include "Management.h"
#include "Layer.h"

using namespace Engine;
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement()
:m_pCurrentScene(NULL)
,m_pRenderer(NULL)
,m_pEventScene(NULL)
{

}

Engine::CManagement::~CManagement()
{

}

HRESULT Engine::CManagement::Ready_Management(void)
{
	return S_OK;
}

_int Engine::CManagement::Update_Management(const _float fTimeDelta)
{
	return m_pCurrentScene->Update_Scene(fTimeDelta);
	
}

void Engine::CManagement::Render_Management(void)
{
	m_pRenderer->Render_Object();
	m_pCurrentScene->Render_Scene();
}
void Engine::CManagement::Set_Renderer(CRenderer* pRenderer)
{
	pRenderer->AddRefCnt();
	m_pRenderer = pRenderer;
}
void Engine::CManagement::Set_CurrentScene(CScene* pScene)
{
	if(m_pCurrentScene != NULL)
	{
		Engine::Safe_Release(m_pCurrentScene);
	}

	m_pCurrentScene = pScene;
}

void Engine::CManagement::Free(void)
{
	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pCurrentScene);
	
	/*if(m_pEventScene != NULL)
		Engine::Safe_Release(m_pEventScene);*/
}

list<CGameObject*>* Engine::CManagement::GetList(const _tchar* pTagName)
{
	return	m_pCurrentScene->GetList(pTagName);
}

CLayer* Engine::CManagement::GetLayer(const _tchar* pTagName)
{
	return m_pCurrentScene->GetLayer(pTagName);
}

CScene* Engine::CManagement::GetCurrentScene(void)
{
	return m_pCurrentScene;
}

void Engine::CManagement::SwapScene(void)
{
	CScene* pTemp = m_pCurrentScene;
	m_pCurrentScene = m_pEventScene;
	m_pEventScene = m_pCurrentScene;
}

void Engine::CManagement::Set_EvenetScene(CScene* pScene)
{
	m_pEventScene = pScene;
}
