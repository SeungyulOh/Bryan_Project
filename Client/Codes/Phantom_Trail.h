#ifndef Phantom_Trail_h__
#define Phantom_Trail_h__

#include "Defines.h"
#include "GameObject.h"
#include "Player.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CTrail_Texture;
	class CTexture;
	class CShader;
	class CDynamicMesh;
	class CBoxCollider;
}


class CPhantom_Trail : public Engine::CGameObject
{
private:
	explicit CPhantom_Trail(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CDynamicMesh* pPlayerDynamicMesh);
	virtual ~CPhantom_Trail(void);
public:
	inline list<D3DXVECTOR4>* GetTrailList(void){return &m_TrailList;}
	inline D3DXVECTOR4* GetWorldPos(void){return &m_vWorldPos;}
public:
	void Ready_Trail(EFFECTDATA tData);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject();

private:
	Engine::CTransform* m_pTransformCom;
	Engine::CRenderer* m_pRendererCom;
	Engine::CTrail_Texture* m_pTrailBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CShader* m_pShaderCom;
	Engine::CBoxCollider* m_pBoxColliderCom;

	_bool m_isOnce;
	_matrix* m_RHFrameMatrix;
	_matrix* m_pPlayerWorld;

	_float m_fDestroyTime;
	_vec4 m_vFirstPos[2];

	Engine::CDynamicMesh* m_pPlayerDynamicMesh;
	EFFECTDATA m_tEffectData;

	list<D3DXVECTOR4> m_TrailList;
	D3DXVECTOR4 m_vTrailVector[40];
	_matrix m_matAtkRange;
	D3DXVECTOR4 m_vWorldPos;

	CPlayer::AttackState* m_pPlayerAtkState;

public:
	static CPhantom_Trail* Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CDynamicMesh* pPlayerDynamicMesh,EFFECTDATA tData);

private:
	virtual void Free(void);
};


#endif // Phantom_Trail_h__
