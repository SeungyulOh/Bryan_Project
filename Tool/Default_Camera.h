#ifndef Default_Camera_h__
#define Default_Camera_h__

#include "Engine_Defines.h"
#include "Camera.h"

class CDefault_Camera
	:public Engine::CCamera

{
private:
	explicit CDefault_Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDefault_Camera();

public:
	virtual HRESULT Ready_GameObject(_vec3 vCamPos, _vec3 vTarget, _vec3 vUp, _float fFovY, _float fAspect, _float fNear, _float fFar);
	virtual _int Update_GameObject(const _float& fTimeDelta,_bool bKeyDown);

public:
	void KeyInput(const _float& fTimeDelta,_bool bKeyDown);
	_uint iMousePointCnt;
	_float m_fDist;
	
public:
	static CDefault_Camera* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCamPos, _vec3 vTarget, _vec3 vUp = _vec3(0.f,1.f,0.f), _float fFovY = D3DX_PI/4.f, _float fAspect = (_float)WINCX/WINCY, _float fNear = 0.1f, _float fFar = 1000.f);
private:
	virtual void Free(void);
};

#endif // Default_Camera_h__
