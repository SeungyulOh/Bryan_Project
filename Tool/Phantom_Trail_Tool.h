#ifndef Phantom_Trail_h__
#define Phantom_Trail_h__

#include "Engine_Defines.h"
#include "GameObject.h"



namespace Engine
{
	class CTransform;
	class CRenderer;
	class CTrail_Texture;
	class CTexture;
	class CShader;
	class CDynamicMesh;
}

class CPhantom_Trail_Tool : public Engine::CGameObject
{
private:
	explicit CPhantom_Trail_Tool(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CDynamicMesh* pPlayerDynamicMesh);
	virtual ~CPhantom_Trail_Tool(void);

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

	_bool m_isOnce;
	
	_float m_fDestroyTime;
	_vec4 m_vFirstPos[2];

	Engine::CDynamicMesh* m_pPlayerDynamicMesh;
	EFFECTDATA m_tEffectData;

	////For. Frame RH
	_matrix* m_pMatPlayerRH;
	_matrix* m_pPlayerWorld;
	list<D3DXVECTOR4> m_TrailList;
	D3DXVECTOR4 m_vTrailVector[40];

public:
	static CPhantom_Trail_Tool* Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CDynamicMesh* pPlayerDynamicMesh, EFFECTDATA tData);

private:
	virtual void Free(void);
};


#endif // Phantom_Trail_h__
