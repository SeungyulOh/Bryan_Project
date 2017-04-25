#ifndef Dynamic_Camera_h__
#define Dynamic_Camera_h__

#include "Defines.h"
#include "Camera.h"

class CDynamic_Camera
	:public Engine::CCamera

{
private:
	explicit CDynamic_Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamic_Camera();

public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);

public:
	void KeyInput(const _float& fTimeDelta);

private:
	_float m_vDist;
	_matrix m_FirstView, m_FirstProj;
public:
	static CDynamic_Camera* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // Dynamic_Camera_h__
