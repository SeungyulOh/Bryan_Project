#ifndef Atk2Glow_h__
#define Atk2Glow_h__

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

class CAtk2Glow : public Engine::CParticleSystem
{

private:
	explicit CAtk2Glow(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vOriginPos, _int iNumParticles, _float fDeadTime , _int iImgIdx);
	virtual ~CAtk2Glow();

public:
	void ResetParticle(Engine::PARICLEATT* pAttribute,_uint iIndex);
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

	_matrix* m_matPlayerRH;
	_bool m_isOnce;
	_float m_fGlowSize;
	_float m_fAlpha;
	_float m_fDeadTime;
	_int m_iImgIdx;

public:
	static CAtk2Glow* Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 vOriginPos, _int iNumParticles, _float fDeadTime , _int iImgIdx = 0);

private:
	virtual void Free();
};


#endif // Atk2Glow_h__
