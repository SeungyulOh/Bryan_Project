#ifndef Reaper_h__
#define Reaper_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CDynamicMesh;
	class CBoxCollider;
	class CShader;
}

class CBlueEye;
class CStarGlow;
class CReaper : public Engine::CGameObject
{
public:
	enum STATE {IDLE,RUN,SLASH,DASHL,DASHR,STATE_END};
	enum EYECOLOR {RED,GREEN,BLUE};
public:
	void GetState(_uint* pState);
	inline _uint GetState(){return m_eMyState;}
	inline void SetMakeMoving(_bool bMove){m_bMakeMoving = bMove;}
private:
	explicit CReaper(LPDIRECT3DDEVICE9 pGraphicDev, EYECOLOR eColor);
	virtual ~CReaper(void);

public:
	static void SelectOnce(CReaper* pReaper){m_pSelectedReaper = pReaper;}
	static CReaper* GetCurrentOne(){return m_pSelectedReaper;}
private:
	static CReaper* m_pSelectedReaper;

public:
	virtual HRESULT Ready_GameObject(_vec3 vFirstPos, _float fOffSetAngle);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
	virtual void Render_ShadowDepth();

	void MovePattern(const _float& fTimeDelta);
	void AttackPattern(void);
	void NormalMoving(const _float& fTimeDelta);
	void FirstEventCam(void);
	void SetState(STATE eState);
	void Calculate_AngleToPlayer(void);


private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CDynamicMesh* m_pDynamicMeshCom;
	Engine::CShader* m_pShaderCom;
	Engine::CBoxCollider* m_pBoxColliderCom;
	Engine::CShader* m_pShaderForShadow;

private:
	_float m_fTimeDelta;
	_float m_fTimeAcc;
	_uint m_eMyState;

	_float m_fChangeColorTimeAcc;

	CBlueEye* m_pRightEye;
	CBlueEye* m_pLeftEye;
	CStarGlow* m_pStarGlow;

	_bool m_isOnce;
	_matrix* m_pPlayerWorldMat;
	_matrix* m_pLampMat;

	EYECOLOR m_eMyColor;
	_bool m_bMakeMoving;
	_vec3 m_vTargetDir;
	_float m_fChangeDirectionTime;
	_float m_fAngleOffSet;

	Engine::CGameObject* m_pPlayer;

	list<_int>* m_pTargetList;
	_bool* m_pChaseFlag;

public:
	static CReaper* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vFirstPos, _float fOffSetAngle, EYECOLOR eColor);
private:
	virtual void Free(void);
};

#endif // Boss_h__
