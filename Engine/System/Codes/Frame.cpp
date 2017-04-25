#include "Frame.h"

using namespace Engine;

Engine::CFrame::CFrame(void)
:m_fOneOverCall(0.f)
,m_fTimeAcc(0.f)
{
}

Engine::CFrame::~CFrame(void)
{

}

HRESULT Engine::CFrame::Ready_Frame(const _float& fCallPerSec)
{
	m_fOneOverCall = 1 / fCallPerSec;
	return S_OK;
}

bool Engine::CFrame::Permit_Call(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if(m_fTimeAcc > m_fOneOverCall)
	{
		m_fTimeAcc = 0.f;
		return true;
	}

	return false;
}


Engine::CFrame* Engine::CFrame::Create(const _float& fCallPerSec)
{
	CFrame* pInst = new CFrame;
	pInst->Ready_Frame(fCallPerSec);
	return pInst;
}

void Engine::CFrame::Free(void)
{

}


