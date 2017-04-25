#ifndef BossDevil_h__
#define BossDevil_h__

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

class CPlayer;
class CBossDevil : public Engine::CGameObject
{
public:
	enum STATE {IDLE,SWING,DASHUPPER_L,DASHUPPER_R,TELEPORTATK1,TELEPORTATK3,BOLT,DAMAGEDSTRONG,DOWNROLLBACK,SAFE_FALL,DAMAGEDFRONT,TELEPORTBEGIN,MAD,SWORDWAVE,SENDPLAYER,EXPLOSION,STATE_END};
public:
	void GetState(_uint* pState);
	inline _uint GetState(){return m_eMyState;}
	inline _bool* GetVisible(){return &m_isVisible;}
	_matrix GetWorldMatrix(void);
	void SetStateWithAnimation(STATE eState);
	inline void DecreaseHp(_uint iAtk){m_iHp -= iAtk;}
	inline Engine::CTransform* GetTransform(){return m_pTransformCom;}

private:
	explicit CBossDevil(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossDevil(void);

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
	virtual void Render_ShadowDepth();

	void MovePattern(const _float& fTimeDelta);
	void AttackPattern(const _float& fTimeDelta);
	void NormalMoving(const _float& fTimeDelta);
	void SetState(STATE eState);
	void Calculate_AngleToPlayer(void);
	void Make_Effect(void);
	void SetUpOnce(void);
	void Make_Sound();


private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CDynamicMesh* m_pDynamicMeshCom;
	Engine::CDynamicMesh* m_pRageModeMesh;
	Engine::CBoxCollider* m_pBoxColliderCom;
	Engine::CShader* m_pShaderCom;
	Engine::CShader* m_pShaderForShadow;
	Engine::CShader* m_pShaderForRenderTarget;

private:
	Engine::CGameObject* m_pPlayer;
	
	_float m_fAngleAcc;
	_ulong m_CurIndex;
	_float m_fTimeDelta;
	_float m_fTimeAcc;
	_uint m_eMyState;
	_uint m_eMyPreState;
	_uint m_ePreStateForSound;
	_float m_fBreakTime;
	
	_float m_fHitColorTime;
	_matrix* m_pDashMoveMat;
	_float m_fDamagedStrongAcc;

	//Sound
	_uint m_iSoundCtrl;
	_uint m_iSkillCntCtrl;


	//Animation Moving Matrix
	_bool m_isOnce;
	_bool m_isMad;
	_matrix* m_LocalMatForMove;
	_vec3 m_vOffSetForAtkAnim;
	_vec3 m_vFirstPosBeforeAction;
	_bool m_isInitSetBeforeStartAnim;
	_vec3* m_pPlayerPos;
	_float m_fDistToPlayer;
	_uint m_iCheckForSwing;
	_bool m_isVisible;
	_float m_fTeleportAtkDelay;
	
	_matrix* m_pWeaponWorldMat;
	_matrix* m_pLeftHandMat;
	_matrix m_LeftHandWorldMat;


	_uint m_iHp;
	_uint m_iMaxHp;
	_bool m_isSendPlayer;
	_bool m_isMadOnce;

public:
	static CBossDevil* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};



#endif // BossDevil_h__
