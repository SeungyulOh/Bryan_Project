#ifndef Loading_h__
#define Loading_h__

#include "Defines.h"
#include "Base.h"

namespace Engine
{
	class CLayer;
}

class CLoading : public CBase
{
public:
	typedef struct tagSaveFile
	{
		TCHAR szName[128];
		_vec3 vScale;
		_vec3 vAngle;
		_vec3 vPos;
	}SaveMeshObj;

public:
	enum LOADINGID {LOADING_STAGE, LOADING_DRAGON, LOADING_BOSS, LOADING_END};
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading(void);
public:
	LOADINGID Get_LoadingID(void) const {
		return m_LoadingID;}
	CRITICAL_SECTION* Get_CriticalSection(void) {
		return &m_Crt;}
	_bool Get_Finish(void) const {
		return m_isFinish;}
	const _tchar* Get_String(void) const {
		return m_szloadingString;}
	HANDLE Get_Handle(void) {return m_hThread;}
public:
	HRESULT Ready_Loading(LOADINGID eLoadingID);
	_uint Loading_ForStage(void);
	_uint Loading_ForDragon(void);
	void LoadNavi(void);
	void LoadSubObject(void);
	Engine::CLayer* Export_LoadedLayer(void);
public:
	static unsigned int APIENTRY Thread_Main(void* pArg);


	
private:
	HANDLE						m_hThread;
	CRITICAL_SECTION			m_Crt;
	LOADINGID					m_LoadingID;
private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
private:
	_tchar						m_szloadingString[128];
	_bool						m_isFinish;
	Engine::CLayer*				m_pLayer;
public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoadingID);
private:
	virtual void Free(void);

};


#endif // Loading_h__
