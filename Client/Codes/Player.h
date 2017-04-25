#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CDynamicMesh;
	class CBoxCollider;
	class CShader;
	class CLayer;
}

class CBoss;
class CPlayer : public Engine::CGameObject
{
public:
	enum AttackState {LIGHTNING,ICE,FIRE};
	enum STATE {IDLE,RUN,ATK1,ATK2,ATK3,ATK4,DASH_E,DASH_W,DASH_S,DASH_N,DAMAGED_FRONT,DAMAGED_BACK,DAMAGED_LEFT,DAMAGED_RIGHT,ATK3STRONG,ATK3STRONG_AFTER,ATK3STRONG_DURING,ATK3STRONG_FINAL,ATK4STRONG,ATK4STRONG_AFTER,ATK4STRONG_DURING,ATK4STRONG_FINAL,REQ_BEGIN,REQ_END,BUFF,ATK2STRONG,ATK2BACK,ATK2FRONT,ATK2LEFT,ATK2LEFTEND,ATK2RIGHT,ATK2RIGHTEND,SNATCH,STATE_END};
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer(void);

public:
	inline Engine::CTransform* GetTransform(){return m_pTransformCom;}
	inline void SetBoolOnce(void){m_isOnce = false;}
	inline _byte* GetState(void){return &m_eMyState;}
	inline AttackState* GetAtkState(void){return &m_eAtkState;}
	inline void SetAtkState(AttackState eState){m_eAtkState = eState;}
	inline void SetHumanBossNULL(void){m_pHumanBoss = NULL;}
	_matrix* GetWorldMat();
	inline _uint GetCollisionPossibility(void){return m_iAtkCollisionCtrl;}
	inline void StopAtkCollision(void){m_iAtkCollisionCtrl = 2;}
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
	virtual void Render_ShadowDepth();

	//
	void SetUpOnce(void);

	void MovePattern(const _float& fTimeDelta);
	void NormalMoving(const _float& fTimeDelta);
	void ForceAction(const _float& fTimeDelta);
	void SetPositonOnDashState(void);
	void SetMyStateWhenGetAttacked(void);
	void MakeTrailEffect(void);
	void MakeSound();
	//void MakeParticleEffect(void);
	
private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CDynamicMesh* m_pDynamicMeshCom;
	Engine::CBoxCollider* m_pBoxColliderCom;
	Engine::CShader* m_pShaderCom;
	Engine::CLayer* m_pEffectLayer;
	Engine::CShader* m_pShaderForRenderTarget;
	Engine::CShader* m_pShaderForShadow;

	//TargetBox
	Engine::CBoxCollider* m_pTargetCollider;

	_float m_fTrailSpawnTimeAcc;

private:
	_float m_fFadeTimeAcc;
	_float m_fScale;
	_float m_fAngleAcc;
	_ulong m_CurIndex;
	_float m_fTimeDelta;
	_bool m_isNextCombo;
	_byte m_eMyState;
	_byte m_ePrevState;
	_bool m_isOnce;
	_int m_iSkillCtrl;
	_int m_iSoundCtrl;

	//For. Dash Moving
	_matrix* m_pDashMoveMat;
	_ulong m_dwDashEffectCnt;

	//For. Change Player's Look When starting to attack
	_vec3* m_pCamPos;
	_vec3* m_pCamTarget;

	//For. Collision
	_matrix* m_pBossWorld;
	AttackState m_eAtkState;
	
	//For. Blur
	_float m_fBlurTimeAcc;
	_float m_fBlurFocus[2];
	_bool m_bBlurOn;
	CBoss* m_pHumanBoss;
	_uint m_iBlurCtrl;

	//For.Shadow
	_matrix m_matLight;
	_matrix m_matLightProj;
	_float m_fDistToLight;

	//For.Collision
	_uint m_iAtkCollisionCtrl;

	//For.Remember Prev Action
	_float m_fRememberTime;
	_byte m_ePrevAtkState;

	_bool m_isSnatch;
	_uint m_iDebug;

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // Player_h__
