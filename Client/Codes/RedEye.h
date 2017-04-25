#ifndef RedEye_h__
#define RedEye_h__

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

class CREdEye : public Engine::CParticleSystem
{
private:
	explicit CREdEye(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CREdEye();

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

	_matrix* m_matEyeFrame;
	_bool m_isOnce;
	_float m_fRand;
	_float m_fTimeAcc;
	_int m_iImgIdx;


	_bool* m_pBossVisible;
public:
	static CREdEye* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};


#endif // RedEye_h__
