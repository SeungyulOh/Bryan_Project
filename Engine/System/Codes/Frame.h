#ifndef Engine_Frame_h__
#define Engine_Frame_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFrame : public CBase
{
private:
	explicit CFrame(void);
	virtual ~CFrame(void);

public:
	HRESULT Ready_Frame(const _float& fCallPerSec);
	bool Permit_Call(const _float& fTimeDelta);

private:
	_float m_fOneOverCall;
	_float m_fTimeAcc;

public:
	static CFrame* Create(const _float& fCallPerSec);
private:
	virtual void Free(void);


};

END

#endif // Engine_Frame_h__
