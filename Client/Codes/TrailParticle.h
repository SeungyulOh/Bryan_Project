#ifndef TrailParticle_h__
#define TrailParticle_h__

#include "Defines.h"
#include "ParticleSystem.h"

namespace Engine
{
	class CRenderer;
	class CTexture;
	class CShader;
	class CTransform;
	class CDynamicMesh;
}

class CTrailParticle : public Engine::CParticleSystem
{
private:
	explicit CTrailParticle(LPDIRECT3DDEVICE9 pGraphicDev, _int iNumParticles, _int iImgIdx);
	virtual ~CTrailParticle();

public:
	void ResetParticle(Engine::PARICLEATT* pAttribute,_uint iIndex);
	virtual HRESULT Ready_GameObject(); 
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	inline _vec3* GetOriginVector(void){return &m_vOrigin;} 

private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CShader* m_pShaderCom;
	Engine::CTransform* m_pParentTransform;

	_bool m_isOnce;
	_float m_fTimeAcc;
	_float m_fSpeed;
	_float m_fAlpha;
	_int m_iImgIdx;

public:
	static CTrailParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iNumParticles, _int iImgIdx);

private:
	virtual void Free();
};


#endif // TrailParticle_h__
