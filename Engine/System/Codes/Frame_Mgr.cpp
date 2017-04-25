#include "Frame_Mgr.h"

using namespace Engine;

IMPLEMENT_SINGLETON(CFrame_Mgr)


Engine::CFrame_Mgr::CFrame_Mgr()
{

}

Engine::CFrame_Mgr::~CFrame_Mgr()
{

}

HRESULT Engine::CFrame_Mgr::Ready_FrameMgr(const _tchar* pTagFrame, const _float& fCallPerSec)
{
	CFrame* pFrame = Find_Frame(pTagFrame);

	if(pFrame != NULL)
		return E_FAIL;

	pFrame = CFrame::Create(fCallPerSec);
	m_mapFrame.insert(MAPFRAME::value_type(pTagFrame,pFrame));
	return S_OK;
}

bool Engine::CFrame_Mgr::Permit_Call(const _tchar* pTagFrame, const _float& fTimeDelta)
{
	CFrame* pFrame = Find_Frame(pTagFrame);

	if(pFrame == NULL)
		return false;

	return pFrame->Permit_Call(fTimeDelta);
}

Engine::CFrame* Engine::CFrame_Mgr::Find_Frame(const _tchar* pTagFrame)
{
	MAPFRAME::iterator iter = find_if(m_mapFrame.begin(),m_mapFrame.end(),CTagFinder(pTagFrame));

	if(iter == m_mapFrame.end())
		return NULL;

	return iter->second;
}

void Engine::CFrame_Mgr::Free()
{
	for_each(m_mapFrame.begin(),m_mapFrame.end(),CRelease_Pair());
	m_mapFrame.clear();
}

