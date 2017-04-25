#ifndef Engine_LightMgr_h__
#define Engine_LightMgr_h__

#include "Engine_Defines.h"
#include "Light.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLightMgr
	:public CBase
{
	DECLARE_SINGLETON(CLightMgr)

private:
	explicit CLightMgr();
	virtual ~CLightMgr();

public:
	void AddLight(LPDIRECT3DDEVICE9 pDevice ,D3DLIGHT9& tLightInfo);
	void LightEnable(LPDIRECT3DDEVICE9 pDevice, const _ushort& wIdx);
	void LightDisable(LPDIRECT3DDEVICE9 pDevice, const _ushort& wIdx);
	void DeleteLight(const _ushort& wIdx);
	void Render_Light(LPD3DXEFFECT pEffect);
	void SetisRender(_uint iIndex,_bool isRender);

	void Change_LightInfo(D3DLIGHT9& tLightInfo, _uint iIndex);
	D3DLIGHT9 GetLightInfo(_uint iIndex);

private:
	virtual void Free(void);
	
private:
	vector<CLight*> m_vecLight;



};

END


#endif
