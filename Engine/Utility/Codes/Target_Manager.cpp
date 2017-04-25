#include "Target_Manager.h"
#include "Target.h"

using namespace Engine;
IMPLEMENT_SINGLETON(CTarget_Manager)

Engine::CTarget_Manager::CTarget_Manager(void)
{

}

Engine::CTarget_Manager::~CTarget_Manager(void)
{

}

HRESULT Engine::CTarget_Manager::Ready_Target(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTargetTag, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color)
{
	CTarget* pTarget = Find_Target(pTargetTag);

	if(NULL != pTarget)
		return E_FAIL;

	pTarget = CTarget::Create(pGraphicDev, iSizeX, iSizeY, Format, Color);

	if(NULL == pTarget)
		return E_FAIL;

	m_mapTarget.insert(MAPTARGET::value_type(pTargetTag, pTarget));

	return S_OK;
}

HRESULT Engine::CTarget_Manager::Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag)
{
	CTarget* pTarget = Find_Target(pTargetTag);

	if(NULL == pTarget)
		return E_FAIL;

	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);

	if(NULL == pMRTList)
	{
		list<CTarget*>		MRTList;
		MRTList.push_back(pTarget);
		m_mapMRT.insert(MAPMRT::value_type(pMRTTag, MRTList));
	}
	else
		pMRTList->push_back(pTarget);

	return S_OK;
}

// 특정 엠알티그룹에 있는 타겟들을 장치에 올린다.
HRESULT Engine::CTarget_Manager::Begin_MRT(const _tchar* pMRTTag)
{
	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);

	if(NULL == pMRTList)
		return E_FAIL;

	list<CTarget*>::iterator	iter = pMRTList->begin();
	list<CTarget*>::iterator	iter_end = pMRTList->end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Clear_RenderTarget();		
	}


	_uint		iIndex = 0;

	for (iter = pMRTList->begin(); iter != iter_end; ++iter, ++iIndex)
	{
		if(iIndex == 1)
			++iIndex;

		(*iter)->SetUp_OnGraphicDev(iIndex);
	}

	return S_OK;

}

HRESULT Engine::CTarget_Manager::End_MRT(const _tchar* pMRTTag)
{
	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);

	if(NULL == pMRTList)
		return E_FAIL;

	list<CTarget*>::iterator	iter = pMRTList->begin();
	list<CTarget*>::iterator	iter_end = pMRTList->end();

	_uint		iIndex = 0;

	for (; iter != iter_end; ++iter, ++iIndex)
	{
		if(iIndex == 1)
			++iIndex;

		(*iter)->Release_OnGraphicDev(iIndex);
	}

	return S_OK;
}

HRESULT Engine::CTarget_Manager::SetUp_OnShader(LPD3DXEFFECT pEffect, const _tchar* pTargetTag, const char* pConstantName)
{
	CTarget* pTarget = Find_Target(pTargetTag);

	if(NULL == pTarget)
		return E_FAIL;

	return pTarget->SetUp_OnShader(pEffect, pConstantName);
}

HRESULT Engine::CTarget_Manager::Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	CTarget* pTarget = Find_Target(pTargetTag);

	if(NULL == pTarget)
		return E_FAIL;

	if(FAILED(pTarget->Ready_DebugBuffer(fX, fY, fSizeX, fSizeY)))
		return E_FAIL;

	return S_OK;
}
void Engine::CTarget_Manager::Render_DebugBuffer(const _tchar* pTargetTag)
{
	CTarget* pTarget = Find_Target(pTargetTag);

	if(NULL == pTarget)
		return;

	pTarget->Render_DebugBuffer();


}

CTarget* Engine::CTarget_Manager::Find_Target(const _tchar* pTargetTag)
{
	MAPTARGET::iterator	iter = find_if(m_mapTarget.begin(), m_mapTarget.end(), CTagFinder(pTargetTag));

	if(iter == m_mapTarget.end())
		return NULL;

	return iter->second;
}

list<CTarget*>* Engine::CTarget_Manager::Find_MRT(const _tchar* pMRTTag)
{
	MAPMRT::iterator	iter = find_if(m_mapMRT.begin(), m_mapMRT.end(), CTagFinder(pMRTTag));

	if(iter == m_mapMRT.end())
		return NULL;

	return &iter->second;
}

void Engine::CTarget_Manager::Free(void)
{
	for_each(m_mapTarget.begin(), m_mapTarget.end(), CRelease_Pair());

	m_mapTarget.clear();
}

