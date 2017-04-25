#ifndef Timer_Mgr_h__
#define Timer_Mgr_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Timer.h"

BEGIN(Engine)

class ENGINE_DLL CTimer_Mgr : public CBase
{
	DECLARE_SINGLETON(CTimer_Mgr)
private:
	explicit CTimer_Mgr();
	virtual ~CTimer_Mgr();

public:
	HRESULT Ready_TimerMgr(const _tchar* pTagTimer);
	void SetUp_TimeDelta(const _tchar* pTagTimer);
	CTimer* Find_Timer(const _tchar* pTagTimer);
	_float Get_TimeDelta(const _tchar* pTagTimer);

private:
	map<const _tchar*, CTimer*> m_mapTimer;
	
	typedef map<const _tchar*, CTimer*> MAPTIMER;

private:
	virtual void Free(void);

};

END

#endif // Timer_Mgr_h__
