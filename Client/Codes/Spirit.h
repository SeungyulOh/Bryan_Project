#ifndef Spirit_h__
#define Spirit_h__

#include "Defines.h"
#include "ParticleSystem.h"
#include "Particle_Trail.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CTexture;
	class CShader;
	class CTransform;
	class CLayer;
}

class CSpirit : public Engine::CParticleSystem
{
private:
	explicit CSpirit(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpirit();

public:
	void ResetParticle(Engine::PARICLEATT* pAttribute, _uint iIndex);
	virtual HRESULT Ready_GameObject(); 
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

private:
	Engine::CTransform* m_pTransformCom;
	Engine::CRenderer* m_pRendererCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CShader* m_pShaderCom;
	Engine::CTransform* m_pParentTransform;

	Engine::CLayer* m_pEffectLayer;

	_matrix* m_matPlayerFrame;
	_bool m_isOnce;
	_float m_fRand;
	_float m_fTimeAcc;

public:
	static CSpirit* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};


#endif // Spirit_h__
