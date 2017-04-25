#ifndef Default_Camera_h__
#define Default_Camera_h__

#include "Defines.h"
#include "Camera.h"

namespace Engine
{
	class CRenderer;
}

class CDefault_Camera
	:public Engine::CCamera

{
private:
	explicit CDefault_Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDefault_Camera(const CDefault_Camera& rhs);
	virtual ~CDefault_Camera();

public:
	virtual HRESULT Ready_GameObject(_vec3 vCamPos, _vec3 vTarget, _vec3 vUp, _float fFovY, _float fAspect, _float fNear, _float fFar);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

public:
	void KeyInput(const _float& fTimeDelta);
	void ShakeCam(_float fShakeTime);
	inline void SetTarget(_vec3* pTarget) {m_pTarget = pTarget;}
	inline _vec3* GetCamPos(){return &m_vCamPos;}
	inline _vec3* GetCamTarget(){return &m_vTarget;}
	inline _float* GetDist(){return &m_vDist;}

private:
	Engine::CRenderer* m_pRendererCom;
	_vec3* m_pTarget;
	_float m_vDist;
	_bool m_isOnce;
	_matrix* m_pPlayerWorldMat;
	_matrix m_matScale;

	//Cam Collision with Terrain
	LPD3DXMESH m_pTerrainMesh;
	BOOL m_isHit;

	//For.Player's Requiem
	_matrix* m_pMatCam;
	_matrix* m_pMatTarget;
	_byte*	m_pPlayerState;
	//

	_bool m_isShake;
	_float m_fShakeTime;
	
public:
	static CDefault_Camera* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCamPos, _vec3 vTarget, _vec3 vUp = _vec3(0.f,1.f,0.f), _float fFovY = D3DX_PI/4.f, _float fAspect = (_float)WINCX/WINCY, _float fNear = 0.1f, _float fFar = 1000.f);
private:
	virtual void Free(void);
};

#endif // Default_Camera_h__
