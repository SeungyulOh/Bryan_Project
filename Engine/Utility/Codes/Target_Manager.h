#ifndef Target_Manager_h__
#define Target_Manager_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CTarget;
class ENGINE_DLL CTarget_Manager : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
private:
	explicit CTarget_Manager(void);
	virtual ~CTarget_Manager(void);
public:
	HRESULT Ready_Target(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTargetTag, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color);
	HRESULT Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
	HRESULT  Begin_MRT(const _tchar* pMRTTag);
	HRESULT  End_MRT(const _tchar* pMRTTag);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const _tchar* pTargetTag, const char* pConstantName);

	HRESULT Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	void Render_DebugBuffer(const _tchar* pTargetTag);
private:
	map<const _tchar*, CTarget*>				m_mapTarget;
	typedef map<const _tchar*, CTarget*>		MAPTARGET;
private:
	map<const _tchar*, list<CTarget*>>			m_mapMRT;
	typedef map<const _tchar*, list<CTarget*>>	MAPMRT;

public:
	CTarget* Find_Target(const _tchar* pTargetTag);
	list<CTarget*>* Find_MRT(const _tchar* pMRTTag);
private:
	virtual void Free(void);	
};

END

#endif // Target_Manager_h__
