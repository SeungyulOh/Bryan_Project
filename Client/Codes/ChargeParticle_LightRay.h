#ifndef ChargeParticle_LightRay_h__
#define ChargeParticle_LightRay_h__

#include "Defines.h"
#include "ParticleSystem.h"
#include "Player.h"

namespace Engine
{
	class CRenderer;
	class CTexture;
	class CShader;
	class CTransform;
}

class CChargeParticle_LightRay : public Engine::CParticleSystem
{
public:
	enum TARGET{SPIRIT,FOOTL,FOOTR};
private:
	explicit CChargeParticle_LightRay(LPDIRECT3DDEVICE9 pGraphicDev,_vec3* pOriginPos, _int iNumParticles, TARGET eTarget);
	virtual ~CChargeParticle_LightRay();

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
	_matrix* m_matPlayerRH;
	_matrix* m_matSpiritWorld;
	_matrix m_FootWorld;
	_matrix* m_PlayerFootL;
	_matrix* m_PlayerFootR;

	_bool m_isOnce;
	CPlayer::AttackState* m_pPlayerState;
	_float m_fTimeAcc;
	_float m_fAlpha;

	TARGET m_eTarget;

public:
	static CChargeParticle_LightRay* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pOriginPos, _int iNumParticles, TARGET eTarget = SPIRIT);

private:
	virtual void Free();
};


#endif // ChargeParticle_LightRay_h__
