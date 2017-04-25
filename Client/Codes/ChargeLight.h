#ifndef ChargeLight_h__
#define ChargeLight_h__

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

class CChargeLight : public Engine::CParticleSystem
{
public:
	enum eType {LEFTHAND,RIGHTHAND};
private:
	explicit CChargeLight(LPDIRECT3DDEVICE9 pGraphicDev, eType MyType,_vec3* pOriginPos, _int iNumParticles, _float fDeadTime , _int iImgIdx);
	virtual ~CChargeLight();

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
	eType m_eMyType;
	_float m_fDeadTime;
	_int m_iImgIdx;

public:
	static CChargeLight* Create(LPDIRECT3DDEVICE9 pGraphicDev, eType MyType,_vec3* pOriginPos, _int iNumParticles, _float fDeadTime , _int iImgIdx = 0);

private:
	virtual void Free();
};


#endif // ChargeLight_h__
