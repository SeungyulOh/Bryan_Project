#include "Timer.h"

using namespace Engine;

Engine::CTimer::CTimer(void)
:m_fTimeDelta(0.f)
{
	ZeroMemory(&m_FrameTime,sizeof(LARGE_INTEGER));
	ZeroMemory(&m_FixTime,sizeof(LARGE_INTEGER));
	ZeroMemory(&m_LastTime,sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick,sizeof(LARGE_INTEGER));
}

Engine::CTimer::~CTimer(void)
{

}

HRESULT Engine::CTimer::Ready_Timer(void)
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

void Engine::CTimer::SetUp_TimeDelta(void)
{
	QueryPerformanceCounter(&m_FrameTime);

	if((m_FrameTime.QuadPart - m_LastTime.QuadPart) > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_LastTime = m_FrameTime;
	}
	
	m_fTimeDelta = _float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / (_float)m_CpuTick.QuadPart;
	m_FixTime = m_FrameTime;
}

Engine::CTimer* Engine::CTimer::Create(void)
{
	CTimer* pInst = new CTimer;
	pInst->Ready_Timer();
	return pInst;
}

void Engine::CTimer::Free(void)
{

}

