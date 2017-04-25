#include "Timer_Mgr.h"

using namespace Engine;

IMPLEMENT_SINGLETON(CTimer_Mgr)

Engine::CTimer_Mgr::CTimer_Mgr()
{

}

Engine::CTimer_Mgr::~CTimer_Mgr()
{

}

HRESULT Engine::CTimer_Mgr::Ready_TimerMgr(const _tchar* pTagTimer)
{
	CTimer* pTimer = Find_Timer(pTagTimer);

	if(pTimer != NULL)
		return E_FAIL;

	pTimer = CTimer::Create();
	m_mapTimer.insert(MAPTIMER::value_type(pTagTimer,pTimer));

	return S_OK;	


}

void Engine::CTimer_Mgr::SetUp_TimeDelta(const _tchar* pTagTimer)
{
	CTimer* pTimer = Find_Timer(pTagTimer);

	if(pTimer == NULL)
		return;

	pTimer->SetUp_TimeDelta();
}

void Engine::CTimer_Mgr::Free(void)
{
	for_each(m_mapTimer.begin(),m_mapTimer.end(),CRelease_Pair());
	m_mapTimer.clear();

}


Engine::CTimer* Engine::CTimer_Mgr::Find_Timer(const _tchar* pTagTimer)
{
	CTagFinder TagFinder(pTagTimer);

	MAPTIMER::iterator iter = find_if(m_mapTimer.begin(),m_mapTimer.end(),TagFinder);

	if(iter == m_mapTimer.end())
		return NULL;

	return iter->second;
}

_float Engine::CTimer_Mgr::Get_TimeDelta(const _tchar* pTagTimer)
{
	MAPTIMER::iterator iter = find_if(m_mapTimer.begin(),m_mapTimer.end(),CTagFinder(pTagTimer));

	if(iter == m_mapTimer.end())
		return 0.f;

	return iter->second->Get_TimeDelta();
}

