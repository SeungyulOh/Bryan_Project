#ifndef Engine_Camera_h__
#define Engine_Camera_h__

#include "GameObject.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CCamera
	:public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera();
public:
	inline void SetCamDir(_vec3 vCamDir){m_vCamDir = vCamDir;}
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);

protected:
	_matrix matView, matProj;
	_vec3 m_vCamPos, m_vCamDir, m_vTarget, m_vUp;
	_float m_fAspect,m_fNear,m_fFar,m_fFovY;

protected:
	virtual void Free(void);
};

END

#endif // Camera_h__
