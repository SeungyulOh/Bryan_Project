#ifndef Boss_h__
#define Boss_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CDynamicMesh;
	class CShader;
	class CTexture;
}

class CBoss : public Engine::CGameObject
{
public:
	enum STATE {EVENT1,EVENT2,EVENT3,EVENT4,EVENT5,EVENT6,EVENT7,EVENT8,EVENT9,IDLE,STATE_END};
public:
	void GetState(_uint* pState);
	inline _uint GetState(){return m_eMyState;}
private:
	explicit CBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss(void);

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

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
	Engine::CTexture* m_pTextureCom;

	Engine::CShader* m_pShaderForRenderTarget;

	list<CGameObject*> m_EvilEyeList;

private:
	_float m_fTimeDelta;
	_uint m_eMyState;
	_uint m_PrevState;

	_float m_fTimeAcc;
	_bool m_isOnce;

	_matrix* m_RightHandMat;
	_matrix m_TotalMatForEffect;
	_matrix* m_LeftHandMat;
	_matrix* m_HeadMat;
	_matrix m_TotalMatForEffectLeft;
	_matrix m_TotalMatForHead;

	//Blur
	_float m_fBlurFocus[2];
	_float m_fBlurTimeAcc;
	_bool m_bBlurOn;
	_uint m_iBlurCtrl;
	_uint m_iStateCtrl;

	
	_float m_fRedScreenTime;
	_bool m_isRedScreen;

	_float m_fFadeOutTime;
	_bool m_isFadeOutIn;

	_float m_fWhiteScreenTime;
	_bool m_isWhiteScreen;

public:
	static CBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // Boss_h__
