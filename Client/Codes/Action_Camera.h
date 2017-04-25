#ifndef Action_Camera_h__
#define Action_Camera_h__

#include "Defines.h"
#include "Camera.h"

namespace Engine
{
	class CDynamicMesh;
}

class CBoss;
class CAction_Camera
	:public Engine::CCamera

{
private:
	explicit CAction_Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAction_Camera();

public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);

	void ShakeCam(_float fShakeTime);

private:
	Engine::CDynamicMesh* m_pDynamicMeshCom;
	CBoss* m_pBoss;

	_bool m_isOnce;
	_matrix* m_pCamWorld;
	_matrix* m_pTarget;
	_matrix matScale;

	_uint m_iState;

	_bool m_isShake;
	_float m_fShakeTime;

public:
	static CAction_Camera* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // Action_Camera_h__
