#ifndef Atk4Swing_h__
#define Atk4Swing_h__

#include "Defines.h"
#include "ParticleSystem.h"

namespace Engine
{
	class CRenderer;
	class CTexture;
	class CShader;
	class CTransform;
}

class CAtk4Swing : public Engine::CParticleSystem
{
private:
	explicit CAtk4Swing(LPDIRECT3DDEVICE9 pGraphicDev,_int iNumParticles);
	virtual ~CAtk4Swing();

public:
	void ResetParticle(Engine::PARICLEATT* pAttribute,_uint iIndex);
	virtual HRESULT Ready_GameObject(); 
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	inline _vec3* GetOriginVector(void){return &m_vOrigin;} 
	void CalculateParticlePos(Engine::PARICLEATT* pAttribute);

private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CShader* m_pShaderCom;
	Engine::CTransform* m_pParentTransform;
	_matrix* m_matPlayerRH;

	_bool m_isOnce;
	_matrix m_matLocal;
	_float m_fTimeAcc;
	_float m_fAlpha;

public:
	static CAtk4Swing* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iNumParticles);

private:
	virtual void Free();
};


#endif // Atk4Swing_h__
