#ifndef Firework_h__
#define Firework_h__

#include "Defines.h"
#include "ParticleSystem.h"
#include "Particle_Trail.h"
#include "Player.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CTexture;
	class CShader;
	class CTransform;
	class CLayer;
}

class CChargeParticle_LightGlow : public Engine::CParticleSystem
{
public:
	enum eType {LEFTHAND,RIGHTHAND};
private:
	explicit CChargeParticle_LightGlow(LPDIRECT3DDEVICE9 pGraphicDev, eType MyType,_vec3* pOriginPos, _int iNumParticles, _float fDeadTime);
	virtual ~CChargeParticle_LightGlow();

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

	CPlayer::AttackState* m_pPlayerState;

public:
	static CChargeParticle_LightGlow* Create(LPDIRECT3DDEVICE9 pGraphicDev, eType MyType,_vec3* pOriginPos, _int iNumParticles, _float fDeadTime);

private:
	virtual void Free();
};


#endif // Firework_h__
