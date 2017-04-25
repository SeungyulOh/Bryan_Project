#ifndef BossWeapon_h__
#define BossWeapon_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CStaticMesh;
	class CBoxCollider;
	class CShader;
}

class CBossWeapon : public Engine::CGameObject
{
private:
	explicit CBossWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossWeapon(void);

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

	inline void SetBossMat(_matrix* pBossMat){m_pBossMat = pBossMat;}
	inline void SetBoolOnce(void){m_isOnce = false;}
	inline void SetWeaponMad(void){m_isMad = true;}

	void SetUpOnce();

private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CStaticMesh* m_pStaticMeshCom;
	Engine::CStaticMesh* m_pMadMeshCom;
	Engine::CBoxCollider* m_pBoxColliderCom;
	Engine::CShader* m_pShaderCom;
	//////////////////
	_matrix* m_pBossMat;
	_matrix* m_pRHFrameFromBoss;
	_matrix m_OffSetMat;

	bool m_isOnce;
	_bool* m_pBossVisible;
	_bool m_isMad;

	Engine::CBoxCollider* m_pTargetCollider;

	list<CGameObject*> m_WeaponEffectList;

public:
	static CBossWeapon* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // BossWeapon_h__
