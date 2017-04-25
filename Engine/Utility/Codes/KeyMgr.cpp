#include "KeyMgr.h"

using namespace Engine;

IMPLEMENT_SINGLETON(CKeyMgr)

Engine::CKeyMgr::CKeyMgr(void)
: m_dwKey(0)
, m_dwKeyPressed(0)
, m_dwKeyDown(0)
{
	ZeroMemory(m_dwKeyPresseds, sizeof(m_dwKeyPresseds));
}

Engine::CKeyMgr::~CKeyMgr(void)
{
}

void Engine::CKeyMgr::KeyCheck(void)
{
	m_dwKey = 0;

	if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;

	if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;

	if(GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwKey |= KEY_ENTER;

	if(GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		m_dwKey |= KEY_ESCAPE;

	if(GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKey |= KEY_SPACE;

	if(GetAsyncKeyState('1') & 0x8000)
		m_dwKey |= KEY_1;

	if(GetAsyncKeyState('2') & 0x8000)
		m_dwKey |= KEY_2;

	if(GetAsyncKeyState('3') & 0x8000)
		m_dwKey |= KEY_3;

	if(GetAsyncKeyState('4') & 0x8000)
		m_dwKey |= KEY_4;

	if(GetAsyncKeyState('5') & 0x8000)
		m_dwKey |= KEY_5;

	if(GetAsyncKeyState('6') & 0x8000)
		m_dwKey |= KEY_6;

	if(GetAsyncKeyState('7') & 0x8000)
		m_dwKey |= KEY_7;

	if(GetAsyncKeyState('8') & 0x8000)
		m_dwKey |= KEY_8;

	if(GetAsyncKeyState('9') & 0x8000)
		m_dwKey |= KEY_9;

	if(GetAsyncKeyState('0') & 0x8000)
		m_dwKey |= KEY_0;

	if(GetAsyncKeyState('Q') & 0x8000)
		m_dwKey |= KEY_Q;

	if(GetAsyncKeyState('W') & 0x8000)
		m_dwKey |= KEY_W;

	if(GetAsyncKeyState('E') & 0x8000)
		m_dwKey |= KEY_E;

	if(GetAsyncKeyState('R') & 0x8000)
		m_dwKey |= KEY_R;

	if(GetAsyncKeyState('A') & 0x8000)
		m_dwKey |= KEY_A;

	if(GetAsyncKeyState('S') & 0x8000)
		m_dwKey |= KEY_S;

	if(GetAsyncKeyState('D') & 0x8000)
		m_dwKey |= KEY_D;

	if(GetAsyncKeyState('F') & 0x8000)
		m_dwKey |= KEY_F;

	if(GetAsyncKeyState('G') & 0x8000)
		m_dwKey |= KEY_G;
	if(GetAsyncKeyState('M') & 0x8000)
		m_dwKey |= KEY_M;
	if(GetAsyncKeyState('T') & 0x8000)
		m_dwKey |= KEY_T;
	if(GetAsyncKeyState('P') & 0x8000)
		m_dwKey |= KEY_P;

	if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
		m_dwKey |= KEY_SHIFT;

	if(GetAsyncKeyState(VK_ADD) & 0x8000)
		m_dwKey |= KEY_NUMPLUS;

	if(GetAsyncKeyState(VK_SUBTRACT) & 0x8000)
		m_dwKey |= KEY_NUMMINUS;

	
	if(GetAsyncKeyState('K') & 0x8000)
		m_dwKey |= KEY_K;

	if(GetAsyncKeyState('L') & 0x8000)
		m_dwKey |= KEY_L;
}
						
bool Engine::CKeyMgr::KeyPressed(DWORD dwKey)
{		
	if(m_dwKey & dwKey)
	{
		m_dwKeyPressed |= dwKey;
		return false;
	}

	else if(m_dwKeyPressed & dwKey)
	{
		m_dwKeyPressed = m_dwKey;
		return true;
	}

	return false;
}

bool Engine::CKeyMgr::KeyPressed(DWORD dwKey, int iIndex)
{
	if(m_dwKey & dwKey)
	{
		m_dwKeyPresseds[iIndex] |= dwKey;
		return false;
	}

	else if(m_dwKeyPresseds[iIndex] & dwKey)
	{
		m_dwKeyPresseds[iIndex] = m_dwKey;
		return true;
	}

	return false;
}						
									
					
bool Engine::CKeyMgr::KeyDown(DWORD dwKey)
{
	if(KeyPressed(dwKey))
		m_dwKeyDown ^= dwKey;	//	0001
			
	if(!(m_dwKeyDown & dwKey))
	{		
		if(m_dwKey & dwKey)
		{					
			m_dwKeyDown |= dwKey;
			return true;
		}
	}

	return false;
}

bool Engine::CKeyMgr::KeyCombine(DWORD dwFirstKey, DWORD dwSecondKey)
{
	if(m_dwKey & dwFirstKey)
	{
		if(KeyDown(dwSecondKey))
			return true;
	}

	return false;
}


bool Engine::CKeyMgr::KeyDown(DWORD _dwKey, BYTE byIndex)
{
	if(KeyPressed(_dwKey))
		m_dwKeyDowns[byIndex] ^= _dwKey;


	if(!(m_dwKeyDowns[byIndex] & _dwKey))
	{
		if(m_dwKey & _dwKey)
		{
			m_dwKeyDowns[byIndex] |= _dwKey;
			return true;
		}
	}

	return false;
}

void Engine::CKeyMgr::Free(void)
{
	
}
