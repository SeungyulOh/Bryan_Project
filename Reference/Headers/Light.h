#ifndef Engine_Light_h__
#define Engine_Light_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLight
	:public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLight();

public:
	inline D3DLIGHT9 GetLightInfo(){return m_tLightInfo;}
	inline void SetisRender(_bool isRender){m_isRender = isRender;}
	inline _bool GetisRender(void){return m_isRender;}
public:
	void InitLight(D3DLIGHT9& rLightInfo);
	D3DLIGHT9* GetLight();
	void DeleteLight(void);
	void Render_Light(LPD3DXEFFECT pEffect);

	void ChangeLightInfo(D3DLIGHT9& tLightInfo);

private:
	D3DLIGHT9 m_tLightInfo;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	_bool m_isRender;

public:
	static CLight* Create(LPDIRECT3DDEVICE9 pDevice, D3DLIGHT9& rLightInfo);
private:
	virtual void Free(void);

};

END

#endif
