#ifndef KeyMgr_h__
#define KeyMgr_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CKeyMgr: public CBase
{
	DECLARE_SINGLETON(CKeyMgr)
private:
	DWORD		m_dwKey;
	DWORD		m_dwKeyPressed;
	DWORD		m_dwKeyPresseds[3];


	DWORD		m_dwKeyDown;
	DWORD		m_dwKeyDowns[3];


public:
	void		KeyCheck(void);
	bool		KeyPressed(DWORD dwKey);
	bool		KeyPressed(DWORD dwKey, int iIndex);

	bool		KeyDown(DWORD dwKey);
	bool		KeyDown(DWORD _dwKey, BYTE byIndex);

	bool		KeyCombine(DWORD dwFirstKey, DWORD dwSecondKey);

public:
	virtual void Free(void);

private:
	explicit CKeyMgr(void);
	virtual ~CKeyMgr(void);
};

END


#endif // KeyMgr_h__
