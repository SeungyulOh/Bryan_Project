#ifndef Party_SprayA_h__
#define Party_SprayA_h__

#include "Defines.h"
#include "ParticleSystem.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CTexture;
	class CShader;
	class CLayer;
}

class CParty_SprayA : public Engine::CParticleSystem
{
private:
	explicit CParty_SprayA(LPDIRECT3DDEVICE9 pGraphicDev,_int iNumParticles);
	virtual ~CParty_SprayA();

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

	_matrix* m_matPlayerFrame;
	_bool m_isOnce;
	_float m_fTimeAcc;
	_uint m_iMaxParticles;
	_float m_fAlpha;
	_uint m_iImgIdx;

public:
	static CParty_SprayA* Create(LPDIRECT3DDEVICE9 pGraphicDev,_int iNumParticles);

private:
	virtual void Free();
};


#endif // Party_SprayA_h__
