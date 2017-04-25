#ifndef Player_h__
#define Player_h__

#include "Engine_Defines.h"
#include "GameObject.h"
#include "Phantom_Trail_Tool.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CDynamicMesh;
	class CShader;
}

class CBoss;
class CPlayer_Tool : public Engine::CGameObject
{
public:
	enum STATE {IDLE,RUN,ATK1,ATK2,ATK3,ATK4,DASH_E,DASH_W,DASH_S,DASH_N,DAMAGED_FRONT,DAMAGED_BACK,DAMAGED_LEFT,DAMAGED_RIGHT,ATK3STRONG,ATK3STRONG_AFTER,ATK3STRONG_DURING,ATK3STRONG_FINAL,ATK4STRONG,ATK4STRONG_AFTER,ATK4STRONG_DURING,ATK4STRONG_FINAL,REQ_BEGIN,REQ_END,STATE_END};
private:
	explicit CPlayer_Tool(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer_Tool(void);

public:
	inline Engine::CTransform* GetTransform(){return m_pTransformCom;}
	inline void SetBoolOnce(void){m_isOnce = false;}
	inline _byte* GetState(void){return &m_eMyState;}
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);


	//
	void SetUpOnce(void);

	
public:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CDynamicMesh* m_pDynamicMeshCom;
	Engine::CShader* m_pShaderCom;

	_float m_fTrailSpawnTimeAcc;

	list<CPhantom_Trail_Tool*> m_EffectList;

public:
	_float m_fScale;
	_float m_fAngleAcc;
	_ulong m_CurIndex;
	_float m_fTimeDelta;
	_bool m_isNextCombo;
	_byte m_eMyState;
	_bool m_isOnce;
	//For. Dash Moving
	_matrix* m_pDashMoveMat;
	_ulong m_dwDashEffectCnt;

	//For. Change Player's Look When starting to attack
	_vec3* m_pCamPos;
	_vec3* m_pCamTarget;

	//For. Collision
	_matrix* m_pBossWorld;



public:
	static CPlayer_Tool* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // Player_h__
