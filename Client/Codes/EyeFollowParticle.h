#ifndef EyeFollowParticle_h__
#define EyeFollowParticle_h__

#include "Defines.h"
#include "ParticleSystem.h"
#include "BossDevil.h"

namespace Engine
{
	class CRenderer;
	class CTexture;
	class CShader;
	class CTransform;
}

class CEyeFollowParticle : public Engine::CParticleSystem
{
public:
	enum MYTYPE{BASE,RIGHT,LEFT,MY_END};
private:
	explicit CEyeFollowParticle(LPDIRECT3DDEVICE9 pGraphicDev,_vec3* pOriginPos, _int iNumParticles ,Engine::CTransform* pParentTransform,_matrix* pEyeMat,_int* iImgIdx,MYTYPE eType);
	virtual ~CEyeFollowParticle();
public:
	inline void SetAlpha(_float fAlpha){m_fAlpha = fAlpha;}
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
	_matrix* m_matEyeBone;

	_bool m_isOnce;
	_float m_fTimeAcc;
	_float m_fAlpha;
	_int* m_iImgIdx;

	_bool* m_pBossVisible;

	MYTYPE m_eType;

public:
	static CEyeFollowParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pOriginPos, _int iNumParticles,Engine::CTransform* pParentTransform,_matrix* pEyeMat, _int* iImgIdx ,MYTYPE eType = BASE);

private:
	virtual void Free();
};


#endif // EyeFollowParticle_h__
