#ifndef Dagger_h__
#define Dagger_h__

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

class CDagger : public Engine::CGameObject
{
private:
	explicit CDagger(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDagger(void);

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

	inline void SetPlayerMat(_matrix* pPlayerMat){m_pPlayerMat = pPlayerMat;}

private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CDynamicMesh* m_pDynamicMeshCom;
	Engine::CBoxCollider* m_pBoxColliderCom;
	Engine::CShader* m_pShaderCom;
	//////////////////
	_matrix* m_pPlayerMat;
	_matrix* m_pRHFrameFromPlayer;
	_matrix m_OffSetMat;
	bool m_isOnce;

	_matrix* m_pRHFrameFromDagger;
	Engine::CBoxCollider* m_pTargetCollider;

public:
	static CDagger* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // Dagger_h__
