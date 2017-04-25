#ifndef BlueEye_h__
#define BlueEye_h__

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

class CEyeFollowParticle;
class CBlueEye : public Engine::CParticleSystem
{
public:
	enum EYETYPE{R_EYE,L_EYE};
private:
	explicit CBlueEye(LPDIRECT3DDEVICE9 pGraphicDev, EYETYPE eType);
	virtual ~CBlueEye();
public:
	void SetParentTransform(Engine::CTransform* pParent);
	void SetParentMesh(Engine::CDynamicMesh* pParentMesh);
	inline void SetEyeColorChange(_int iIndex){m_iImgIdx = iIndex;}
	inline _int* GetEyeColor(void){return &m_iImgIdx;}
	inline void SetAlpha(_float falpha){m_fAlpha = falpha;}
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
	Engine::CDynamicMesh* m_pParentMesh;

	Engine::CLayer* m_pEffectLayer;

	_matrix* m_matHeadFrame;
	_bool m_isOnce;
	_float m_fRand;
	_float m_fTimeAcc;
	_float m_fAlpha;

	EYETYPE m_eType;

	_int m_iImgIdx;

	CEyeFollowParticle* m_pRightParticle;
	CEyeFollowParticle* m_pLeftParticle;

public:
	static CBlueEye* Create(LPDIRECT3DDEVICE9 pGraphicDev, EYETYPE eType);

private:
	virtual void Free();
};


#endif // BlueEye_h__
