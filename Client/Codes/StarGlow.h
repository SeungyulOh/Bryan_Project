#ifndef StarGlow_h__
#define StarGlow_h__

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

class CStarGlow : public Engine::CParticleSystem
{
public:
	enum COLORTYPE { RED , GREEN, BLUE };
private:
	explicit CStarGlow(LPDIRECT3DDEVICE9 pGraphicDev,  _matrix* matParent, COLORTYPE eType, Engine::CDynamicMesh* pDynamicMesh);
	virtual ~CStarGlow();

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

	_matrix* m_matFrame;
	_bool m_isOnce;
	_float m_fRand;
	_float m_fTimeAcc;
	_float m_fAlpha;
	_uint m_iImgIdx;
	_matrix* m_matParent;

	COLORTYPE m_eType;

public:
	static CStarGlow* Create(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* matParent, COLORTYPE eType, Engine::CDynamicMesh* pDynamicMesh);

private:
	virtual void Free();
};


#endif // StarGlow_h__
