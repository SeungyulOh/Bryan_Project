#ifndef Frame_Mgr_h__
#define Frame_Mgr_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Frame.h"

BEGIN(Engine)

class ENGINE_DLL CFrame_Mgr : public CBase
{
	DECLARE_SINGLETON(CFrame_Mgr)
private:
	explicit CFrame_Mgr();
	virtual ~CFrame_Mgr();

public:
	HRESULT Ready_FrameMgr(const _tchar* pTagFrame, const _float& fCallPerSec);
	bool Permit_Call(const _tchar* pTagFrame, const _float& fTimeDelta);
	CFrame* Find_Frame(const _tchar* pTagFrame);

private:
	map<const _tchar*, CFrame*> m_mapFrame;
	typedef map<const _tchar*, CFrame*> MAPFRAME;


private:
	virtual void Free();

};

END


#endif // Frame_Mgr_h__
