#ifndef Particle_Trail_h__
#define Particle_Trail_h__

#include "Defines.h"
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

class CParticle_Trail : public Engine::CGameObject
{
private:
	explicit CParticle_Trail(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* pUpperMat,_vec3* pParentPos, _float fLifeTime);
	virtual ~CParticle_Trail(void);

public:
	void Ready_Trail();
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject();

private:
	Engine::CTransform* m_pTransformCom;
	Engine::CRenderer* m_pRendererCom;
	Engine::CTrail_Texture* m_pTrailBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CShader* m_pShaderCom;

	_bool m_isOnce;
	_vec3* m_pParentWorldPos;
	_matrix* m_pUpperParentMat;

	_float m_fDestroyTime;
	_float m_fLifeTime;

	list<D3DXVECTOR4> m_TrailList;
	D3DXVECTOR4 m_vTrailVector[40];
	

public:
	static CParticle_Trail* Create(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* pUpperMat,_vec3* pParentPos,_float fLifeTime);

private:
	virtual void Free(void);
};


#endif // Particle_Trail_h__
