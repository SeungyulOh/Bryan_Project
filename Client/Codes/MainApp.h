#ifndef MainApp_h__
#define MainApp_h__

#include "Defines.h"
#include "Base.h"

class CMainApp: public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT Ready_Component(void);

public:
	HRESULT Ready_MainApp();
	_int	Update_MainApp(const _float& fTimeDelta);
	void	Render_MainApp();
private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	_tchar m_szFps[128];
	_ulong m_dwFpsCnt;
	_float m_fTimeAcc;

private:
	HRESULT Ready_DefaultSetting(void);
protected:
private:

public:
	static CMainApp* Create();
	virtual void Free();


};

#endif // MainApp_h__
